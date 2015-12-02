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
#endif
#ifdef __linux__
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
	if( size <= 0 || !data )
	{
		LOGGER_ERROR("[CNet] Error:%s:%d", __FILE__, __LINE__);
		return false;
	}

	bool bOldPacket = false;
	bool bFinish = false;

	Packet * pCmd = getPacketBuff(sock);
	if( pCmd )
		bOldPacket = true;

	int lsize = 0;
	UINT nReadPtr = 0;
	while( (lsize = size - nReadPtr) > 0 )
	{
		if( !pCmd )
		{
			//开始构建一个新的数据包
			pCmd = new Packet;
			if( !pCmd )
			{
				LOGGER_ERROR("[CNet] Error:%s:%d, new PACKET_COMMAN failed", __FILE__, __LINE__);
				return false;
			}

			bFinish = false;
		}
		pCmd->SetNetID(sock);

		int wsize = 0;
		if( pCmd->GetHeadLeftSize() > 0 )	//包头未满，把包头写满
		{
			wsize = pCmd->GetHeadLeftSize() < lsize ? pCmd->GetHeadLeftSize() : lsize;
		}
		else if( pCmd->GetLeftSize() > 0 )	//包数据未满，把整个包写满
		{
			wsize = pCmd->GetLeftSize() < lsize ? pCmd->GetLeftSize() : lsize;
		}

		pCmd->recvData(data + nReadPtr, wsize);
		nReadPtr += wsize;
		
		if( pCmd->GetLeftSize() == 0 )		//已摘出一个完整包
		{
			//数据校验
			if( !pCmd->crcCheck() )
			{
				LOGGER_ERROR("[CNet] crcCheck() sock=%d size=%d lsize=%d nReadPtr=%d failed", sock, size, lsize, nReadPtr);
				removePacketBuff(sock);
				SAFE_DELETE(pCmd);
				return false;
			}
			else
			{
				//设置其它参数
				pCmd->SetNetID(sock);

				//存入链表
				handlePacket(pCmd);
				updateRecvPacket(1);

				//清空缓存
				if( bOldPacket )
				{
					removePacketBuff(sock);
					bOldPacket = false;
				}

				pCmd = NULL;
				bFinish = true;
			}
		} 
		else if (pCmd->GetLeftSize() < 0) 
		{
			LOGGER_ERROR("[CNet] (pCmd->GetLeftSize() < 0 sock=%d", sock);
			removePacketBuff(sock);
			SAFE_DELETE(pCmd);
			return false;
	   }
	}

	//当前数据包没有构建完整，缓存下来，等待下次网络数据
	if( !bFinish )
	{
		insertPacketBuff( sock, pCmd );
	}
	
	return true;
}

void CNet::close(SOCKET sock)
{
	Packet * pCmd = removePacketBuff(sock);
	SAFE_DELETE(pCmd);
	_closeReturn(sock);
}

bool CNet::shutdown(SOCKET sock)
{
	if( !m_Net )
		return false;

	return m_Net->Shutdown(sock);
}

void CNet::printLog()
{
	/*int del = timeGetTime() - time;
	if( del )
		Log.Notice("data:%d, time:%d, avg:%d bytes per second", size, del, size/del);*/
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
Packet * CNet::getPacketBuff(SOCKET sock)
{
	Packet * pCmd = NULL;
	m_PacketLock.LOCK();
	pCmd = m_PacketList.Find(sock);
	m_PacketLock.UNLOCK();
	return pCmd;
}

bool CNet::insertPacketBuff(SOCKET sock, Packet * pCmd)
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

Packet * CNet::removePacketBuff(SOCKET sock)
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
