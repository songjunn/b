#include "Net.h"
#include "IOSelect.h"
#include "memorypool.h"
#ifdef _WIN
#include "IOCP.h"
#else
#include "IOEpoll.h"
#include "NetMonitor.h"
#include "limits.h"
#endif

CNet::CNet()
{
}
	
CNet::~CNet()
{
}

bool CNet::startup(int type, int port, int connectmax, int sendbuffsize, int recvbuffsize, int packsize)
{
	make_crc32_table();

	switch(type)
	{
	case NET_IO_SELECT:
		{
			m_Net = new CIOSelect(this);
		}
		break;
#ifdef _WIN
	case NET_IO_IOCP:
		{
			m_Net = new CIOCP(this);
		}
		break;
#elif defined __linux__
	case NET_IO_EPOLL:
		{
			m_Net = new CIOEpoll(this);
		}
		break;
#endif
	default:
		return false;
	}
	
	if( !m_Net || !m_Net->Startup(port, connectmax, sendbuffsize, recvbuffsize) )
		return false;

	_startMonitor("./monitor/", port);

	return true;
}

void CNet::terminate()
{
	m_Net->Terminate();
}

int CNet::send(SOCKET sock, char * data, int size)
{
	if( !m_Net )
		return false;

	return m_Net->Send( sock, data, size );
}

int CNet::sendMsg(SOCKET sock, Packet* pack)
{
	return send(sock, (char*)pack->Data(), pack->Size());
}

bool CNet::recv(SOCKET sock, char * data, int size)
{
	if (size <= 0 || !data) {
		LOGGER_ERROR("[CNet] socket:%d size:%d", sock, size);
		return false;
	}

	int rsize = 0;
	bool cached = false;

	while (size > 0) {
		
		Packet * pCmd = _getDataBuff(sock);
		if (pCmd) {
			cached = true;
		} else {
			pCmd = new Packet;
			pCmd->SetNetID(sock);
			cached = false;
		}
	
		rsize = pCmd->assemble(data+rsize, size-rsize);
		if (rsize <= 0) {
			_removeDataBuff(sock);
			SAFE_DELETE(pCmd);
			return false;
		} 

		if (pCmd->isFull()) {
			//存入链表
			handlePacket(pCmd);
			updateRecvPacket(1);

			//清空缓存
			if (cached) {
				_removeDataBuff(sock);
			}
		} else {
			_addDataBuff(sock, pCmd);
		}
	}
	
	return true;
}

void CNet::close(SOCKET sock)
{
	Packet * pCmd = _removeDataBuff(sock);
	SAFE_DELETE(pCmd);
	_closeReturn(sock);
}

bool CNet::shutdown(SOCKET sock)
{
	if( !m_Net )
		return false;

	return m_Net->Shutdown(sock);
}

bool CNet::accept(SOCKET sock, const char * ip)
{
	_acceptReturn(sock);
	return true;
}

SOCKET CNet::connect(const char * ip, int port)
{
	return m_Net->Connect(ip, port);
}

SOCKET CNet::connectAsync(const char * ip, int port)
{
	return m_Net->ConnectAsync(ip, port);
}

bool CNet::connectReturn(SOCKET sock, int error)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
Packet * CNet::_getDataBuff(SOCKET sock)
{
	Packet * pCmd = NULL;
	m_PacketLock.LOCK();
	pCmd = m_PacketList.Find(sock);
	m_PacketLock.UNLOCK();
	return pCmd;
}

bool CNet::_addDataBuff(SOCKET sock, Packet * pCmd)
{
	if( sock == INVALID_SOCKET || !pCmd )
		return false;

	bool ret = false;
	m_PacketLock.LOCK();
	ret = m_PacketList.Insert(sock, pCmd);
	updatePacketBuffNum(m_PacketList.Count());
	m_PacketLock.UNLOCK();
	return true;
}

Packet * CNet::_removeDataBuff(SOCKET sock)
{
	Packet * pCmd = NULL;
	m_PacketLock.LOCK();
	pCmd = m_PacketList.Remove(sock);
	updatePacketBuffNum(m_PacketList.Count());
	m_PacketLock.UNLOCK();
	return pCmd;
}

void CNet::_startMonitor(const char * path, int port)
{
#ifdef __linux__
	CNetMonitorTData *pMonitor = new CNetMonitorTData();
	char sock_path[PATH_MAX] = {0};

	sprintf(sock_path, "%s/Net_%d.sock", path, port);
	pMonitor->init(sock_path);
	regist(&(pMonitor->monitor));

	ThreadLib::Create(NetMonitorThread, pMonitor);
#endif

}
