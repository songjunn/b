#include "IOSelect.h"
#include "SocketOps.h"
#include "Net.h"
#include "logger.h"
#include "memory.h"


///////////////////////////////////////////////////////////////////////////////////////////////
//
CIOSelect::CIOSelect(CNet * net) : m_pNet(net)
{
}

CIOSelect::~CIOSelect()
{
	MemFree(m_Sockers);
	SocketOps::NetClose();
}

bool CIOSelect::Startup(int port, int connectmax, int sendbuffsize, int recvbuffsize)
{
	//检查端口合法性
	if( port < 1024 )
	{			
		LOGGER_ERROR("[IOSelect]listen port must larger than 1024, port:%d", port);
		return false;
	}

	//初始化Winsock服务
	if( !SocketOps::NetStartup() )
	{
		LOGGER_ERROR("[IOSelect]WSAStartup init failed：%d", SocketOps::GetLastError());
		return false;
	}

	m_ConnectMax = connectmax;
	m_SockerSendBuffsize = sendbuffsize;
	m_SockerRecvBuffsize = recvbuffsize;

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

	m_RunFlag = true;

	//创建非阻塞连接线程
	m_ConnThreadID = ThreadLib::Create(ConnThread, this);
	LOGGER_NOTICE("[IOSelect]ThreadLib Create:ConnThread %lu", m_ConnThreadID);

	//创建发送线程
	m_SendThreadID = ThreadLib::Create(SendThread, this);
	LOGGER_NOTICE("[IOSelect]ThreadLib Create:SendThread %lu", m_SendThreadID);

	//创建接收线程
	m_RecvThreadID = ThreadLib::Create(RecvThread, this);
	LOGGER_NOTICE("[IOSelect]ThreadLib Create:RecvThread %lu", m_RecvThreadID);

	LOGGER_NOTICE("[IOSelect]Net Startup listen %d", port);

	return true;
}

void CIOSelect::Terminate()
{
	m_RunFlag = false;
	ThreadLib::WaitForFinish(m_RecvThreadID);
	ThreadLib::WaitForFinish(m_SendThreadID);
	ThreadLib::WaitForFinish(m_ConnThreadID);

	LOGGER_NOTICE("[IOSelect]Net Terminate");
}

SOCKET CIOSelect::Connect(const char * ip, int port)
{
	CSocker * s = _GetFreeSocker();
	if( !s )
	{
		LOGGER_ERROR("[IOSelect]Create Socker Failed, ip:%s port:%d", ip, port);
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
		LOGGER_ERROR("[IOSelect]Socket Connect Error:(%d:%s) %d, socket=%d", errno, strerror(errno), SocketOps::GetLastError(), s->m_socket);
		_FreeSocker(s->m_socket);
		return INVALID_SOCKET;
	}

	// 设置非阻塞模式
	if( !SocketOps::Nonblocking(s->m_socket) )
	{
		LOGGER_ERROR("[IOSelect]Nonblocking Error:%d, socket=%d", SocketOps::GetLastError(), s->m_socket);
		_FreeSocker(s->m_socket);
		return INVALID_SOCKET;
	}
	
	_AddSocker(s);

	s->setIP(ip);
	s->m_status = CSocker::Key_Work;

	LOGGER_NOTICE("[IOSelect]Connect Socket %d, %s %d", s->m_socket, s->m_szIP, port);

	return s->m_socket;
}

SOCKET CIOSelect::ConnectAsync(const char * ip, int port)
{
	CSocker * s = _GetFreeSocker();
	if( !s )
	{
		LOGGER_ERROR("[IOSelect]Create Socker Failed in ConnectAsync, ip:%s port:%d", ip, port);
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
			LOGGER_ERROR("[IOSelect]Async connect Error:%d, socket=%d", ret, s->m_socket);
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

int	CIOSelect::Send(SOCKET sock, char * data, int size)
{
	LOGGER_DEBUG("[IOSelect]Send to %d size %d", sock, size);
	CSocker * s = _GetSocker(sock);
	if( !s )
	{
		LOGGER_ERROR("[IOSelect]send faild :no socker %d", sock);
		return 0;
	}

	s->m_SendLock.LOCK();
	if( !s->m_SendBuffer->Write(data, size) )
	{
		LOGGER_ERROR("[IOSelect]send faild :socker %d send buffer is full", sock);
		s->m_SendLock.UNLOCK();
		return 0;
	}
	s->m_SendLock.UNLOCK();

	m_pNet->updateSendPacket(1);

	m_SendEvent.Event();

	return size;
}

bool CIOSelect::Recv(SOCKET sock, char * data, int size)
{
	return m_pNet->recv(sock, data, size);
}

bool CIOSelect::Shutdown(SOCKET sock)
{
	CSocker * s = _GetSocker(sock);
	if( !s )
		return false;
	
	_Shutdown(s);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
bool CIOSelect::_CreateListenSocket(int port)
{
	if (!m_ListenSocker.createBuffer(m_SockerSendBuffsize, m_SockerRecvBuffsize) || !m_ListenSocker.createSocket())
	{
		LOGGER_ERROR("[IOSelect]Init Listen Socker failed, port:%d", port);
		//return false;
		_exit(-1);
	}

	//保证accept立即返回
	if( !SocketOps::Nonblocking(m_ListenSocker.m_socket) )
	{
		LOGGER_ERROR("[IOSelect]SocketOps::Nonblocking error:%d, socket:%d", SocketOps::GetLastError(), m_ListenSocker.m_socket);
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
		LOGGER_ERROR("[IOSelect]bind port:%d failed error:%d" , port, SocketOps::GetLastError());
		//_exit(-1);
		return false;
	}
	LOGGER_NOTICE("[IOSelect]bind port:%d", port);

	ret = listen(m_ListenSocker.m_socket, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		LOGGER_ERROR("[IOSelect]Listen Socket:%d Failed:%d", m_ListenSocker.m_socket, SocketOps::GetLastError());
		return false;
	}

	return true;
}

void CIOSelect::_AcceptAllConnections()
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
				closesocket(sockfd);
				break;
			}

			sockaddr_in * pRemote = (sockaddr_in*)&client_addr;
			char * szIP = inet_ntoa(pRemote->sin_addr);
			s->setIP(szIP);
			s->m_status = CSocker::Key_Work;

			_AddSocker(s);

			if( !m_pNet->accept(s->m_socket, s->m_szIP) )
			{
				_FreeSocker(s->m_socket);
				break;
			}

			LOGGER_ERROR("[IOSelect]Accept Socket %d, %s", s->m_socket, s->m_szIP);
		}
	} 
	while(sockfd != INVALID_SOCKET);
}

void CIOSelect::_Shutdown(CSocker * s)
{
	/*s->m_Lock.LOCK();
	s->m_status = CSocker::Key_Close;
	s->m_Lock.UNLOCK();

	m_CloseEvent.Event();*/

	LOGGER_ERROR("[IOSelect]Shutdown Socket %d", s->m_socket);

	_CloseSocket(s->m_socket);
}

bool CIOSelect::_CloseSocket(SOCKET sock)
{
	m_pNet->close(sock);

	_RemoveSocker(sock);

	_FreeSocker(sock);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////
//
CSocker * CIOSelect::_GetFreeSocker(SOCKET sock)
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
	
	if (!s || !s->createSocket(sock))
	{
		_FreeSocker(sock);
		return NULL;
	}

	return s;
}

void CIOSelect::_FreeSocker(SOCKET sock)
{
	for(int i=0; i<m_ConnectMax; ++i)
	{
		if( m_Sockers[i].m_socket == sock )
		{
			m_Sockers[i].release();
			break;
		}
	}
}

bool CIOSelect::_AddSocker(CSocker * s)
{
	m_SocketsLock.LOCK();
	bool ret = m_Sockets.Insert(s->m_socket, s);
	m_SocketsLock.UNLOCK();
	return ret;
}

CSocker * CIOSelect::_RemoveSocker(SOCKET s)
{
	if( INVALID_SOCKET == s )
		return NULL;
	m_SocketsLock.LOCK();
	CSocker * socker = m_Sockets.Remove(s);
	m_SocketsLock.UNLOCK();
	return socker;
}

CSocker * CIOSelect::_GetSocker(SOCKET s)
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
void CIOSelect::SendThread(void * param)
{
	if( !param )
		return;

	CIOSelect *	pThis = (CIOSelect*)param;  
	fd_set fdwrite;

	while( pThis->m_RunFlag )
	{
		pThis->m_SendEvent.Wait(1000);

		FD_ZERO(&fdwrite);
		SOCKET nfds = 0;
		int fdcount = 0;
		
		for( int i=0; i<pThis->m_ConnectMax; ++i )
		{
			if(CSocker::Key_Work != pThis->m_Sockers[i].m_status )
				continue;

			pThis->m_Sockers[i].m_SendLock.LOCK();
			int size = pThis->m_Sockers[i].m_SendBuffer->GetReadSize();
			pThis->m_Sockers[i].m_SendLock.UNLOCK();
			if( size > 0 )
			{
				FD_SET(pThis->m_Sockers[i].m_socket, &fdwrite);

				if( nfds < pThis->m_Sockers[i].m_socket )
				{
					nfds = pThis->m_Sockers[i].m_socket;
				}
				++fdcount;
			}
		}

		if( fdcount == 0 )
			continue;

		timeval	t = {0,100};
		int ret = select(nfds+1, NULL, &fdwrite, NULL, &t);

		if( ret > 0 )
		{
			for( int index=0; index<pThis->m_ConnectMax; ++index )
			{					
				if(CSocker::Key_Work != pThis->m_Sockers[index].m_status )
					continue;

				if( !FD_ISSET(pThis->m_Sockers[index].m_socket, &fdwrite) )
					continue;

				ULONG sendsize;
				pThis->m_Sockers[index].m_SendLock.LOCK();
				char * szBuff = pThis->m_Sockers[index].m_SendBuffer->GetReadPtr(sendsize);
				pThis->m_Sockers[index].m_SendLock.UNLOCK();

				int size = send(pThis->m_Sockers[index].m_socket, szBuff, sendsize, 0);
				if( size > 0 )
				{
					LOGGER_DEBUG("[IOSelect]SendThread debug: socket=%d send size=%d total=%d", pThis->m_Sockers[index].m_socket, size, sendsize); 
					pThis->m_pNet->updateSendSize(size);
					pThis->m_Sockers[index].m_SendLock.LOCK();
					pThis->m_Sockers[index].m_SendBuffer->Remove(size);
					pThis->m_Sockers[index].m_SendLock.UNLOCK();
				}
				else if( size == SOCKET_ERROR )
				{						
					int err = SocketOps::GetLastError();
					LOGGER_ERROR("[IOSelect]SendThread error:%d, socket=%d", err, pThis->m_Sockers[index].m_socket);
					pThis->m_pNet->updateSendError(err);
#ifdef _WIN
					if( err == WSAENOBUFS )	//tcp/ip缓冲区已满
					{
						Sleep(1);
					}
					else
#endif
					pThis->_Shutdown(&pThis->m_Sockers[index]);
				}
				else
				{	
					LOGGER_ERROR("[IOSelect]send return 0 ?, socket=%d", pThis->m_Sockers[index].m_socket);
				}
			}
		}
	}

	LOGGER_NOTICE("[IOSelect]SendThread Quit...");

	return;
}

void CIOSelect::RecvThread(void * param)
{
	if( !param )
		return;

	CIOSelect *	pThis = (CIOSelect*)param;  
	int	ret	= 0; 
	fd_set fdread;	

	while (pThis->m_RunFlag)
	{	
		FD_ZERO(&fdread);
		SOCKET nfds = 0;

		//添加监听socket的事件
		if( pThis->m_ListenSocker.m_socket != INVALID_SOCKET )
		{
			FD_SET(pThis->m_ListenSocker.m_socket, &fdread);
			nfds = pThis->m_ListenSocker.m_socket;
		}

		for( int index=0; index<pThis->m_ConnectMax; ++index )
		{
			if(CSocker::Key_Work != pThis->m_Sockers[index].m_status )
				continue;	
				
			FD_SET(pThis->m_Sockers[index].m_socket, &fdread);
			if( nfds < pThis->m_Sockers[index].m_socket )
			{
				nfds = pThis->m_Sockers[index].m_socket;
			}
		}

		timeval	t = {1,0};
		ret = select(nfds+1, &fdread, NULL, NULL, &t);

		if( ret > 0 )
		{
			//有新的连接来了
			if( pThis->m_ListenSocker.m_socket != INVALID_SOCKET && FD_ISSET( pThis->m_ListenSocker.m_socket, &fdread) )
				pThis->_AcceptAllConnections();

			for( int index=0; index<pThis->m_ConnectMax; ++index )
			{
				if( CSocker::Key_Work != pThis->m_Sockers[index].m_status )
					continue;

				if( FD_ISSET(pThis->m_Sockers[index].m_socket, &fdread) )
				{
					int len;
					char * buf = pThis->m_Sockers[index].m_RecvBuffer->GetWritePtr(len);
					int size = recv(pThis->m_Sockers[index].m_socket, buf, len, 0);

					if( size > 0 )
					{
						LOGGER_DEBUG("[IOSelect]RecvThread debug: socket=%d recv size=%d", pThis->m_Sockers[index].m_socket, size); 
						pThis->m_pNet->updateRecvSize(size);

						int len;
						char * buf = pThis->m_Sockers[index].m_RecvBuffer->GetReadPtr(len);
						pThis->Recv(pThis->m_Sockers[index].m_socket, buf, len);
					}
					else if( size == 0 )	//远程主机socket正常关闭
					{
						pThis->_Shutdown(&pThis->m_Sockers[index]);
						continue;
					}
					else
					{						
						int err = SocketOps::GetLastError();
						LOGGER_ERROR("[IOSelect]RecvThread error:%d, socket=%d", err, pThis->m_Sockers[index].m_socket);
						pThis->m_pNet->updateRecvError(err);
#ifdef _WIN
						if( err != WSAENOTSOCK && err != WSAEWOULDBLOCK && err != WSAENOBUFS )
#else
						if( err != EAGAIN )
#endif
						{
							pThis->_Shutdown(&pThis->m_Sockers[index]);
						}
						continue;
					}
				}
			}
		}
	}

	LOGGER_NOTICE("[IOSelect]RecvThread Quit...");

	return;
}

void CIOSelect::ConnThread(void * param)
{
	if( !param )
		return;

	CIOSelect *	pThis = (CIOSelect*)param;  

	int	ret	 = 0;  		
	fd_set fdwrite;	

	while (pThis->m_RunFlag)
	{
		pThis->m_ConnEvent.Wait(1000);

		FD_ZERO(&fdwrite);

		SOCKET nfds = 0;
		for(int i=0; i<pThis->m_ConnectMax; ++i)
		{
			if( pThis->m_Sockers[i].m_status != CSocker::Key_Connect )
				continue;

			FD_SET(pThis->m_Sockers[i].m_socket, &fdwrite);
			if( nfds < pThis->m_Sockers[i].m_socket )
			{
				nfds = pThis->m_Sockers[i].m_socket;
			}
		}

		if( nfds <= 0 )
			continue;

		timeval	t = {5,0};
		ret = select(nfds+1, NULL, &fdwrite, NULL, &t);

		if( ret > 0 )  //有事件发生了
		{			
			for(int i=0; i<pThis->m_ConnectMax; ++i)
			{
				if( pThis->m_Sockers[i].m_status != CSocker::Key_Connect )
					continue;

				if( FD_ISSET(pThis->m_Sockers[i].m_socket, &fdwrite) )
				{ 
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

					if( getpeername(pThis->m_Sockers[i].m_socket, (struct sockaddr*)&add, &addr) == 0 )
					{
						pThis->m_Sockers[i].m_status = CSocker::Key_Work;
						LOGGER_NOTICE("[IOSelect]Socket %d async connect success!", pThis->m_Sockers[i].m_socket);
					}
					else
					{
						err = SocketOps::GetLastError();
						LOGGER_ERROR("[IOSelect]Socket %d async connect error %d", pThis->m_Sockers[i].m_socket, err);

						pThis->_Shutdown(&pThis->m_Sockers[i]);
					}

					pThis->m_pNet->connectReturn(pThis->m_Sockers[i].m_socket, err);
				}
			}
		}
		else if( ret == 0 )
		{
			LOGGER_ERROR("[IOSelect]Async Select timeout");

			for(int i=0; i<pThis->m_ConnectMax; ++i)
			{
				if( pThis->m_Sockers[i].m_status == CSocker::Key_Connect )
				{
					pThis->m_pNet->connectReturn( pThis->m_Sockers[i].m_socket, -1 );
					pThis->_Shutdown(&pThis->m_Sockers[i]);
				}
			}
		}
		else
		{
			continue;
		}
	}

	LOGGER_NOTICE("[IOSelect]ConnThread Quit...");

	return;
}
