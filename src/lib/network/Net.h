#ifndef SHARED_NET_H
#define SHARED_NET_H

#include "NetHeader.h"
#include "Packet.h"
#include "stlmap.h"
#include "Lock.h"
#include "NetObservable.h"

class Packet;
class CSocketNet;
class CNet : public CNetObservable
{
public:
	enum NET_IO_TYPE {
		NET_IO_SELECT = 1,
		NET_IO_IOCP,
		NET_IO_KQUEUE,
		NET_IO_EPOLL,
	};

public:
	CNet();
	virtual ~CNet();

	virtual bool	startup(int type, int port, int connectmax, int sendbuffsize, int recvbuffsize, int packsize);
	virtual void	terminate();
	virtual bool	accept(SOCKET sock, const char * ip);
	virtual bool	recv(SOCKET sock, char * data, int size);
	virtual int		send(SOCKET sock, char * data, int size);
	virtual int		sendMsg(SOCKET sock, Packet* pack);
	virtual void	close(SOCKET sock);
	virtual bool	shutdown(SOCKET sock);
	virtual SOCKET	connect(const char * ip, int port);
	virtual SOCKET	connectAsync(const char * ip, int port);
	virtual bool	connectReturn(SOCKET sock, int error=0);

	virtual bool	handlePacket(Packet * pCmd)		{return true;}

private:
	Packet * _getDataBuff(SOCKET sock);
	Packet * _removeDataBuff(SOCKET sock);
	bool _addDataBuff(SOCKET sock, Packet * pCmd);

	void _startMonitor(const char * path, int port);

	virtual void _closeReturn(SOCKET sock) {}
	virtual void _acceptReturn(SOCKET sock) {}
	virtual void _connectReturn(SOCKET sock, int error = 0) {}

private:
	Mutex m_PacketLock;
	CStlMap<SOCKET, Packet*> m_PacketList;	//没有解析完整的数据包

public:
	CSocketNet * m_Net;

};

#endif //SHARED_NET_H
