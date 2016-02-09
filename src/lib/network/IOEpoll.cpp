#include <sys/epoll.h>
#include <stdlib.h>
#include <limits.h>
#include "IOEpoll.h"
#include "SocketOps.h"
#include "Net.h"
#include "logger.h"
#include "memory.h"
#include "NetMonitor.h"
#include "NetObservable.h"

#ifdef DEBUG
inline std::string _getModuleName()
{
	std::string nfmt = __FILE__;
	int i = nfmt.rfind("/");

	if (i >= 0) {
		nfmt = nfmt.substr(i+1 , nfmt.length()-1);
	}
	i = nfmt.find("Module");
	if (i < 0) {
		i = nfmt.find(".");
	}
	nfmt = "[" + nfmt.substr(0, i) + "]" ;

	return nfmt;
}

#define LDEBUG(fmt, ...)	\
{\
	std::string nfmt = _getModuleName() + fmt;\
	LOGGER_DEBUG(nfmt.c_str(), ## __VA_ARGS__ );\
}
#define LDEBUGFUNC(fmt, ...)	\
{\
	std::string nfmt = _getModuleName() + "[%s] ";\
	nfmt = nfmt + fmt;\
	LOGGER_DEBUG(nfmt.c_str(), __func__, ## __VA_ARGS__ );\
}
#define LNOTICE(fmt, ...)	\
{\
	std::string nfmt = _getModuleName() + fmt;\
	LOGGER_NOTICE(nfmt.c_str(), ## __VA_ARGS__ );\
}
#define LERROR(fmt, ...)	\
{\
	std::string nfmt = _getModuleName() + fmt;\
	LOGGER_ERROR(nfmt.c_str(), ## __VA_ARGS__ );\
}
#else
inline std::string _getModuleName()
{
    return "[IOEpoll]";
}
#define LDEBUG(fmt, ...) \
{\
    std::string nfmt = _getModuleName() + fmt;\
    LOGGER_DEBUG(nfmt.c_str(),  ## __VA_ARGS__); \
}
#define LNOTICE(fmt, ...) \
{\
    std::string nfmt = _getModuleName() + fmt;\
    LOGGER_NOTICE(nfmt.c_str(),  ## __VA_ARGS__);\
}
#define LERROR(fmt, ...) \
{\	
    std::string nfmt = _getModuleName() + fmt;\    
    LOGGER_ERROR(nfmt.c_str(),  ## __VA_ARGS__); \
}
#define LDEBUGFUNC(fmt, ...) \
{\
    std::string nfmt = _getModuleName() + fmt;\
    LOGGER_DEBUG(nfmt.c_str(), ## __VA_ARGS__);  \
}
#endif


#define INIT_DEFAULT_MAX_EVENTS	4096
#define INIT_DEFAULT_MAX_TIMEOUT 5000 

///////////////////////////////////////////////////////////////////////////////////////////////
//
CIOEpoll::CIOEpoll(CNet * net)
{
	m_pNet = net;
}

CIOEpoll::~CIOEpoll()
{
	SAFE_DELETE_ARRAY(m_Sockers);
	SocketOps::NetClose();
}

bool CIOEpoll::Startup(int port, int connectmax, int sendbuffsize, int recvbuffsize)
{
	//检查端口合法性
	if( port < 1024 )
	{			
		LERROR("listen port must larger than 1024, port:%d", port);
		return false;
	}

	//初始化Winsock服务
	if( !SocketOps::NetStartup() )
	{
		LERROR("WSAStartup init failed：%d", SocketOps::GetLastError());
		return false;
	}

	m_ConnectMax = connectmax;
	m_SockerSendBuffsize = sendbuffsize;
	m_SockerRecvBuffsize = recvbuffsize;

	read_fd = _epoll_create();
	write_fd = _epoll_create();

	//创建监听端口
	if( !_CreateListenSocket(port) )
	{
		return false;
	}

	//预分配对象池
	//m_Sockers = NEW CSocker[m_ConnectMax];
	m_Sockers = (CSocker*)MemAlloc(sizeof(CSocker)*m_ConnectMax);
	for( int i=0; i<m_ConnectMax; ++i )
	{
	    new(&m_Sockers[i]) CSocker;
		m_Sockers[i].createBuffer(sendbuffsize, recvbuffsize);
	}

	//创建非阻塞连接线程
	m_ConnThreadID = ThreadLib::Create(ConnThread, this);

	//创建发送线程
	m_SendThreadID = ThreadLib::Create(SendThread, this);

	//创建接收线程
	m_RecvThreadID = ThreadLib::Create(RecvThread, this);

	LNOTICE("Net Startup listen %d", port);

	return true;
}

void CIOEpoll::Terminate()
{
	ThreadLib::WaitForFinish(m_RecvThreadID);
	ThreadLib::WaitForFinish(m_SendThreadID);
	ThreadLib::WaitForFinish(m_ConnThreadID);

	LNOTICE("Net Terminate");
}

SOCKET CIOEpoll::Connect(const char * ip, int port)
{
	CSocker * s = _GetFreeSocker();
	if( !s )
	{
		LERROR("Create Socker Failed, ip:%s port:%d", ip, port);
		return INVALID_SOCKET;
	}

	SocketOps::SetSendBufferSize(s->m_socket, 100*1024*1204);
	SocketOps::SetRecvBufferSize(s->m_socket, 100*1024*1204);
	SocketOps::SetGracefulClose(s->m_socket, 120);
	//SocketOps::SetKeepAlive(s->m_socket, 5000, 60000, 1);
	SocketOps::DisableBuffering(s->m_socket);

	sockaddr_in add;
	add.sin_family = AF_INET;
	add.sin_port = htons(port);
	add.sin_addr.s_addr = inet_addr(ip);

	if( connect(s->m_socket, (struct sockaddr*)&add, sizeof(sockaddr)) < 0 )
	{
		LERROR("Socket Connect Error:(%d:%s) %d, socket=%d", errno, strerror(errno), SocketOps::GetLastError(), s->m_socket);
		_FreeSocker(s->m_socket);
		return INVALID_SOCKET;
	}

	// 设置非阻塞模式
	if( !SocketOps::Nonblocking(s->m_socket) )
	{
		LERROR("Nonblocking Error:%d, socket=%d", SocketOps::GetLastError(), s->m_socket);
		_FreeSocker(s->m_socket);
		return INVALID_SOCKET;
	}
	
	_AddSocker(s);

	s->setIP(ip);
	s->m_status = CSocker::Key_Work;

	LNOTICE("Connect Socket %d, %s %d", s->m_socket, s->m_szIP, port);

	return s->m_socket;
}

SOCKET CIOEpoll::ConnectAsync(const char * ip, int port)
{
	CSocker * s = _GetFreeSocker();
	if( !s )
	{
		LERROR("Create Socker Failed in ConnectAsync, ip:%s port:%d", ip, port);
		return INVALID_SOCKET;
	}

	SocketOps::SetSendBufferSize(s->m_socket, 100*1024*1204);
	SocketOps::SetRecvBufferSize(s->m_socket, 100*1024*1204);
	SocketOps::SetGracefulClose(s->m_socket, 120);
	//SocketOps::SetKeepAlive(s->m_socket, 5000, 60000, 1);
	SocketOps::Nonblocking(s->m_socket);
	SocketOps::DisableBuffering(s->m_socket);

	sockaddr_in add;
	add.sin_family = AF_INET;
	add.sin_port = htons(port);
	add.sin_addr.s_addr = inet_addr(ip);

	int ret = connect(s->m_socket, (struct sockaddr*)&add, sizeof(sockaddr));
	if( ret < 0 )
	{
		ret = SocketOps::GetLastError();
#ifdef _WIN
		if( ret != WSAEINPROGRESS && ret != WSAEWOULDBLOCK )
#else
		if( ret != EINPROGRESS )
#endif
		{
			LERROR("Async connect Error:%d, socket=%d", ret, s->m_socket);
			_FreeSocker(s->m_socket);
			return INVALID_SOCKET;
		}

		s->m_status = CSocker::Key_Connect;
		m_ConnEvent.Event();
	}
	/*else if( ret != SOCKET_ERROR )	//客户程序与服务程序在同一主机，有可能立即返回连接成功
	{
	}*/

	_AddSocker(s);

	s->m_status = CSocker::Key_Connect;
	s->setIP(ip);

	m_ConnEvent.Event();

	return s->m_socket;
}

int	CIOEpoll::Send(SOCKET sock, char * data, int size)
{
	LDEBUG("Send to %d size %d", sock, size);
	CSocker * s = _GetSocker(sock);
	if( !s )
	{
		LERROR("send faild :no socker %d", sock);
		return 0;
	}

	s->m_SendLock.LOCK();
	if( !s->m_SendBuffer->Write(data, size) )
	{
		LERROR("send faild :socker %d send buffer is full", sock);
		s->m_SendLock.UNLOCK();
		return 0;
	}
	s->m_SendLock.UNLOCK();

	LDEBUG("_epoll_ctl write, sock:%d", sock);
	_epoll_ctl(write_fd, EPOLL_CTL_MOD, sock, EPOLLOUT );

	m_pNet->updateSendPacket(1);

	m_SendEvent.Event();

	return size;
}

bool CIOEpoll::Recv(SOCKET sock, char * data, int size)
{
	return m_pNet->recv(sock, data, size);
}

bool CIOEpoll::Shutdown(SOCKET sock)
{
	CSocker * s = _GetSocker(sock);
	if( !s )
		return false;

	s->m_status = CSocker::Key_CloseWait;

	_epoll_ctl(write_fd, EPOLL_CTL_MOD, sock, EPOLLOUT );

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
bool CIOEpoll::_CreateListenSocket(int port)
{
	if (!m_ListenSocker.createBuffer(m_SockerSendBuffsize, m_SockerRecvBuffsize) || !m_ListenSocker.createSocket())
	{
		LERROR("Init Listen Socker failed, port:%d", port);
		//return false;
		_exit(-1);
	}

	//保证accept立即返回
	if( !SocketOps::Nonblocking(m_ListenSocker.m_socket) )
	{
		LERROR("SocketOps::Nonblocking error:%d, socket:%d", SocketOps::GetLastError(), m_ListenSocker.m_socket);
		return false;
	}

	SOCKADDR_IN localAddr;
#ifdef _WIN32
	ZeroMemory (&localAddr, sizeof(localAddr));	
#else
	bzero(&localAddr, sizeof(localAddr));
#endif
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(port);
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret = bind(m_ListenSocker.m_socket,(PSOCKADDR)&localAddr, sizeof(localAddr));
	if (ret == SOCKET_ERROR)
	{
		LERROR("bind port:%d failed error:%d" , port, SocketOps::GetLastError());
		return false;
	}
	LDEBUG("bind port:%d " , port);

	ret = listen(m_ListenSocker.m_socket, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		LERROR("Listen Socket:%d Failed:%d", m_ListenSocker.m_socket, SocketOps::GetLastError());
		return false;
	}

	_epoll_ctl(read_fd, EPOLL_CTL_ADD, m_ListenSocker.m_socket, EPOLLIN );//| EPOLLET);

	return true;
}

void CIOEpoll::_AcceptAllConnections()
{
	SOCKET sockfd = INVALID_SOCKET;
	do
	{
		sockaddr client_addr;
#ifdef _WIN32
		int nAddrLen = sizeof(client_addr);
#else
#ifdef __linux__
		socklen_t nAddrLen = sizeof(client_addr);
#endif
#endif
		sockfd = accept(m_ListenSocker.m_socket, &client_addr, &nAddrLen);
		if(sockfd != INVALID_SOCKET)
		{
			CSocker* s = _GetFreeSocker(sockfd);
			if( s )
			{
				SocketOps::SetSendBufferSize(s->m_socket, 100*1024*1204);
				SocketOps::SetRecvBufferSize(s->m_socket, 100*1024*1204);
				SocketOps::SetGracefulClose(s->m_socket, 120);
				//SocketOps::SetKeepAlive(s->m_socket, 5000, 60000, 1);
				SocketOps::DisableBuffering(s->m_socket);
			}
			else
			{
				LNOTICE("_GetFreeSocker(%d) = NULL", sockfd);
				closesocket(sockfd);
				break;
			}

			sockaddr_in * pRemote = (sockaddr_in*)&client_addr;
			char * szIP = inet_ntoa(pRemote->sin_addr);
			s->setIP(szIP);
			s->m_status = CSocker::Key_Work;

			bool ret = _AddSocker(s); if (!ret) LERROR("_AddSocker(%d:%d) failed", s->m_socket, sockfd);

			m_pNet->updateConnectingNum(1);

			if( !m_pNet->accept(s->m_socket, s->m_szIP) )
			{
				LERROR("m_pNet->Accept(%d) failed, ip:%s", s->m_socket, s->m_szIP);
				_FreeSocker(s->m_socket);
				break;
			}

			_epoll_ctl(read_fd, EPOLL_CTL_ADD, s->m_socket, EPOLLIN );
			_epoll_ctl(write_fd, EPOLL_CTL_ADD, s->m_socket, EPOLLOUT );

			LNOTICE("Accept Socket %d, %s", s->m_socket, s->m_szIP);
		} 
		else
		{
			if( errno != EAGAIN )
				LERROR("Accept failed %d", errno);
		}
	} 
	while(sockfd != INVALID_SOCKET);
}

void CIOEpoll::_Shutdown(SOCKET s)
{
	LNOTICE("Shutdown Socket %d", s);
	_CloseSocket(s);
}

bool CIOEpoll::_CloseSocket(SOCKET sock)
{
	// 通知逻辑层关闭socket，释放对应的缓冲区
	_epoll_ctl(write_fd, EPOLL_CTL_DEL, sock, 0);
	_epoll_ctl(read_fd, EPOLL_CTL_DEL, sock, 0);

	m_pNet->close(sock);

	// 从m_Sockets中删除
	_RemoveSocker(sock);

	_FreeSocker(sock);

	m_pNet->updateClosedNum(1);
	m_pNet->updateConnectingNum(-1);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
CSocker * CIOEpoll::_GetFreeSocker(SOCKET sock)
{
	CSocker * s = NULL;

	for(int i=0; i<m_ConnectMax; ++i)
	{
		if( m_Sockers[i].m_status == CSocker::Key_Free )
		{
			s = &m_Sockers[i];
			break;
		}
	}

	if( !s )
		return NULL;
	
	if (!s->createSocket(sock))
	{
		LERROR("_GetFreeSocker failed, socket:%d", s->m_socket);
		_FreeSocker(sock);
		return NULL;
	}

	return s;
}

void CIOEpoll::_FreeSocker(SOCKET sock)
{
	for(int i=0; i<m_ConnectMax; ++i)
	{
		if( m_Sockers[i].m_socket == sock )
		{
			m_Sockers[i].clear();
			break;
		}
	}
}

bool CIOEpoll::_AddSocker(CSocker * s)
{
	m_SocketsLock.LOCK();
	bool ret = m_Sockets.Insert(s->m_socket, s);
	m_SocketsLock.UNLOCK();
	return ret;
}

CSocker * CIOEpoll::_RemoveSocker(SOCKET s)
{
	if( INVALID_SOCKET == s )
		return NULL;
	m_SocketsLock.LOCK();
	CSocker * socker = m_Sockets.Remove(s);
	m_SocketsLock.UNLOCK();
	return socker;
}

CSocker * CIOEpoll::_GetSocker(SOCKET s)
{
	if( INVALID_SOCKET == s )
		return NULL;
	CSocker * socker = NULL;
	m_SocketsLock.LOCK();
	socker = m_Sockets.Find(s);
	m_SocketsLock.UNLOCK();
	return socker;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
void CIOEpoll::SendThread(void * param)
{
	if( !param )
		return;

	CIOEpoll *	pThis = (CIOEpoll*)param;  

	struct epoll_event events[INIT_DEFAULT_MAX_EVENTS];

	int i , nevents;
	size_t len;

	len = sizeof(struct sockaddr_in);

	bool work_flag ;
	while (1) {

		nevents = epoll_wait(pThis->write_fd, events, INIT_DEFAULT_MAX_EVENTS, INIT_DEFAULT_MAX_TIMEOUT); 
		if (nevents < 0) {
			if (errno == EINTR) continue;
			LERROR("SendThread epoll_wait add error: %d ", SocketOps::GetLastError());
			continue;
		} else if (nevents == 0) {
			continue;
		}

		LDEBUG("SendThread : nevents=%d", nevents);
		/* Handle the events*/
		for (i = 0; i < nevents; i++) {

			LDEBUG("SendThread : event=%d fd=%d", events[i].events, events[i].data.fd);
			CSocker *pSocker = pThis->_GetSocker(events[i].data.fd);
			if (pSocker == NULL) {
				LERROR("SendThread error: _GetSocker(%d) failed", events[i].data.fd);
				pThis->_epoll_ctl(pThis->write_fd, EPOLL_CTL_DEL, events[i].data.fd, 0);
				continue;
			}
			/*if (pSocker->m_socket == INVALID_SOCKET) {
				LERROR("SendThread error: pSocker invalid, sock:%d", events[i].data.fd);
				pThis->_epoll_ctl(pThis->write_fd, EPOLL_CTL_DEL, events[i].data.fd, 0);
				continue;
			}*/

			if (CSocker::Key_Work != pSocker->m_status && CSocker::Key_CloseWait != pSocker->m_status) {
				LERROR("SendThread error: fd=%d status=%d", events[i].data.fd, pSocker->m_status);
				pThis->_Shutdown(pSocker->m_socket);
				continue;
			}

			if (events[i].events & EPOLLOUT) {
				pThis->_send(pSocker, pThis->write_fd);
			} else /*if (!(events[i].events & EPOLLIN))*/{
				LERROR("SendThread error: socket=%d, event=%d", pSocker->m_socket, events[i].events); 
				pThis->_Shutdown(pSocker->m_socket);
				continue;
			}
		}
	}

	LNOTICE("SendThread Quit...");

	return;
}

void CIOEpoll::RecvThread(void * param)
{
	if( !param )
		return;

	CIOEpoll *	pThis = (CIOEpoll*)param;  
	
	struct epoll_event events[INIT_DEFAULT_MAX_EVENTS];

	int i , nevents;
	size_t len;

	len = sizeof(struct sockaddr_in);

	while (1) {

		nevents = epoll_wait(pThis->read_fd, events, INIT_DEFAULT_MAX_EVENTS, INIT_DEFAULT_MAX_TIMEOUT); 
		if (nevents < 0) {
			if (errno == EINTR) continue;
			LERROR("RecvThread epoll_wait add error: %d ", SocketOps::GetLastError());
			continue;
		} else if (nevents == 0) {
			continue;
		}

		/* Handle the events*/
		for (i = 0; i < nevents; i++) {
			LDEBUG("RecvTHread epoll events: %d ", events[i].data.fd);
			if (pThis->m_ListenSocker.m_socket == events[i].data.fd ) {
				if( pThis->m_ListenSocker.m_socket != INVALID_SOCKET ) {
					pThis->_AcceptAllConnections();
				}
				continue;
			}

			CSocker *pSocker = pThis->_GetSocker(events[i].data.fd);
			if (pSocker == NULL || CSocker::Key_CloseWait == pSocker->m_status) {
				LERROR("RecvThread : _GetSocker(%d) failed events[i].events=%d", 
						events[i].data.fd, events[i].events);
				pThis->_epoll_ctl(pThis->read_fd, EPOLL_CTL_DEL, events[i].data.fd, 0);
				continue;
			}

			if (CSocker::Key_Work != pSocker->m_status ) {
				LERROR("RecvThread error: fd=%d status=%d", events[i].data.fd, (int)pSocker->m_status);
				pThis->_Shutdown(pSocker->m_socket);
				continue;
			}

			if (events[i].events & EPOLLIN) {
				pThis->_recv(pSocker, pThis->read_fd);
			} else/* if (!(events[i].events & EPOLLOUT))*/{
				LERROR("RecvThread error: socket=%d, event=%d", pSocker->m_socket, events[i].events); 
				pThis->_Shutdown(pSocker->m_socket);
				continue;
			}
		}
	}

	LNOTICE("RecvThread Quit...");

	return;
}

void CIOEpoll::ConnThread(void * param)
{
	if( !param )
		return;

	CIOEpoll *	pThis = (CIOEpoll*)param;  

	struct epoll_event events[INIT_DEFAULT_MAX_EVENTS];

	/* Open an epoll file descriptor.*/
	int epoll_fd = pThis->_epoll_create();

	int i , nevents;
	size_t len;

	len = sizeof(struct sockaddr_in);

	bool work_flag ;
	while (1) {
		pThis->m_ConnEvent.Wait(1000);
		work_flag = false;

		for (i = 0; i < pThis->m_ConnectMax; i++) {
			if (CSocker::Key_Connect != pThis->m_Sockers[i].m_status) {
				continue;
			}
			work_flag = true;
			pThis->_epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pThis->m_Sockers[i].m_socket, EPOLLOUT );//| EPOLLET);
		}
		if (!work_flag ) {
			continue;
		}
		nevents = epoll_wait(epoll_fd, events, INIT_DEFAULT_MAX_EVENTS, INIT_DEFAULT_MAX_TIMEOUT); 
		if (nevents < 0) {
			if (errno == EINTR) continue;
			LERROR("ConnThread epoll_wait add error: %d ", SocketOps::GetLastError());
			continue;
		} else if (nevents == 0) {
			continue;
		}

		/* Handle the events*/
		for (i = 0; i < nevents; i++) {

			CSocker *pSocker = pThis->_GetSocker(events[i].data.fd);
			if (pSocker == NULL) {
				LERROR("ConnThread error: _GetSocker(%d) faile", events[i].data.fd);
				continue;
			}

			if (CSocker::Key_Connect != pSocker->m_status) {
				LERROR("ConnThread error: fd=%d status=%d", events[i].data.fd, pSocker->m_status);
				continue;
			}

			if (events[i].events & EPOLLOUT) {

				//处理非阻塞连接的返回
				int err = 0;
				sockaddr_in add;
#ifdef _WIN32
				int addr = sizeof(sockaddr);
#else
#ifdef __linux__
				socklen_t addr = sizeof(sockaddr);
#endif
#endif

				if( getpeername(pSocker->m_socket, (struct sockaddr*)&add, &addr) == 0 )
				{
					pSocker->m_status = CSocker::Key_Work;
					LNOTICE("Socket %d async connect success", pSocker->m_socket);
				}
				else
				{
					err = SocketOps::GetLastError();
					LERROR("Socket %d async connect error %d", pSocker->m_socket, err);

					pThis->_Shutdown(pSocker->m_socket);
				}

				pThis->m_pNet->connectReturn(pSocker->m_socket, err);
			} else {
				LDEBUG("ConnThread error: socket=%d, event=%d", pSocker->m_socket, events[i].events); 
				continue;
			}
		}
	}

	LNOTICE("ConnThread Quit...");

	return;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
int CIOEpoll::_epoll_create() const
{
	/* Open an epoll file descriptor.*/
	int epoll_fd = epoll_create(INIT_DEFAULT_MAX_EVENTS);
	if (epoll_fd < 0) 
	{
		LERROR("epoll_create error: %d", SocketOps::GetLastError());
		exit(-1);
	}
	LDEBUG("epoll create %d", epoll_fd);
	return epoll_fd;
}

void CIOEpoll::_epoll_ctl(int epollFd, int op, int fd, uint32_t event) const 
{
	struct epoll_event ev;
	bzero(&ev, sizeof(ev));
	ev.events = event;
	ev.data.fd = fd;
	if (epoll_ctl(epollFd, op, fd, &ev) < 0 )
	{
		if (errno == EEXIST) {
			return;
		} else if (errno == ENOENT && op == EPOLL_CTL_MOD) {
			epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &ev);
			return;
	    }
		LDEBUG("epoll_ctl add error: %d, socket=%d op=%d", errno, fd, op);
	}
}

int CIOEpoll::_send(CSocker* pSocker, int epoll_fd)
{
	ULONG sendsize;
	pSocker->m_SendLock.LOCK();
	char *szBuff = pSocker->m_SendBuffer->GetReadPtr(sendsize);
	pSocker->m_SendLock.UNLOCK();
	int size = send(pSocker->m_socket, szBuff, sendsize, 0);

	if (size > 0) {
		m_pNet->updateSendSize(size);
		LDEBUG("SendThread debug: socket=%d send size=%d total=%d", pSocker->m_socket, size, sendsize); 
		pSocker->m_SendLock.LOCK();
		int lsize = pSocker->m_SendBuffer->Remove(size);
		pSocker->m_SendLock.UNLOCK();
		if (lsize <= 0) {
			if (pSocker->m_status == CSocker::Key_CloseWait) {
				LDEBUG("_send: sock=%d lsize=%d status=%d", pSocker->m_socket, lsize, pSocker->m_status);
				this->_Shutdown(pSocker->m_socket);
			} else {
				this->_epoll_ctl(write_fd, EPOLL_CTL_DEL, pSocker->m_socket, 0);
			}
		}
	} 
	else if (size == 0 && pSocker->m_status == CSocker::Key_CloseWait) {
		this->_Shutdown(pSocker->m_socket);
	}
	else if (size == SOCKET_ERROR) {
		m_pNet->updateSendError(errno);
		LERROR("SendThread error:%d, socket=%d", SocketOps::GetLastError(), pSocker->m_socket); 
		if( errno != EAGAIN )
		{
			//this->_epoll_ctl(epoll_fd, EPOLL_CTL_DEL, pSocker->m_socket, 0);
			this->_Shutdown(pSocker->m_socket);
		}
	} else {
	  this->_epoll_ctl(write_fd, EPOLL_CTL_DEL, pSocker->m_socket, 0);
	  LDEBUG("send is unknow fd = %d status = %d size = %d  sendsize = %d", pSocker->m_socket, pSocker->m_status, size, sendsize);
	}

	return size;
}

int CIOEpoll::_recv(CSocker* pSocker, int epoll_fd)
{
	int len;
	char * buf = pSocker->m_RecvBuffer->GetWritePtr(len);
	int size = recv(pSocker->m_socket, buf, len, 0);

	if( size > 0 )
	{
		m_pNet->updateRecvSize(size);
		LDEBUG("RecvThread debug: socket=%d recv size=%d", pSocker->m_socket, size); 
		int len;
		char * buf = pSocker->m_RecvBuffer->GetReadPtr(len);
		if( !this->Recv(pSocker->m_socket, buf, len) )
		{
			//this->_epoll_ctl(epoll_fd, EPOLL_CTL_DEL, pSocker->m_socket, 0);
			this->_Shutdown(pSocker->m_socket);
		}
	}
	else if( size == 0 )	//远程主机socket正常关闭
	{
		//this->_epoll_ctl(epoll_fd, EPOLL_CTL_DEL, pSocker->m_socket, 0);
		this->_Shutdown(pSocker->m_socket);
	}
	else
	{						
		int err = errno;
		LERROR("RecvThread err: %d, sock=%d", err, pSocker->m_socket);
		if( err != EAGAIN )
		{
			//this->_epoll_ctl(epoll_fd, EPOLL_CTL_DEL, pSocker->m_socket, 0);
			this->_Shutdown(pSocker->m_socket);
		}
		m_pNet->updateRecvError(err);
	}

	return size;
}
