#ifndef SHARED_IOSELECT_H
#define SHARED_IOSELECT_H

#include "NetHeader.h"
#include "SocketNet.h"
#include "Socker.h"
#include "stlmap.h"
#include "Lock.h"
#include "NetObservable.h"
#include <string>
#include "ThreadLib.h"

class CNet;
class CIOSelect : public CSocketNet
{
public:
	CIOSelect(CNet * net);
	virtual ~CIOSelect();

	bool	Startup(int port, int connectmax, int sendbuffsize, int recvbuffsize);
	void	Terminate();

	int		Send(SOCKET sock, char * data, int size);
	bool	Recv(SOCKET sock, char * data, int size);
	bool	Shutdown(SOCKET sock);

	SOCKET	Connect(const char * ip, int port);
	SOCKET	ConnectAsync(const char * ip, int port);

protected:
	bool	_CreateListenSocket(int port);
	void	_AcceptAllConnections();
	void	_Shutdown(CSocker * s);
	bool	_CloseSocket(SOCKET sock);

	void		_FreeSocker(SOCKET sock);
	CSocker *	_GetFreeSocker(SOCKET sock=INVALID_SOCKET);

	bool		_AddSocker(CSocker * s);
	CSocker *	_RemoveSocker(SOCKET s);
	CSocker *	_GetSocker(SOCKET s, const char *file , int len );

	static void ConnThread(void * param);	//非阻塞连接方式的处理线程
	static void RecvThread(void * param);
	static void SendThread(void * param);
	static void	CloseThread(void * param);

public:
	void  _dPrintSocket(const char* file = __FILE__, int  line = __LINE__);

protected:
	bool		m_RunFlag;
	CNet *		m_pNet;
	int			m_ConnectMax;		//最大连接数量
	int			m_ListenPort;		//监听端口
	CSocker		m_ListenSocker;		//监听socket
	int			m_SockerSendBuffsize;	//Socker发送缓冲大小
	int			m_SockerRecvBuffsize;	//Socker接收缓冲大小

	CSocker *	m_Sockers;

	ThreadLib::ThreadID m_ConnThreadID;
	ThreadLib::ThreadID m_SendThreadID;
	ThreadLib::ThreadID m_RecvThreadID;

	Eventer		m_ConnEvent;
	Eventer		m_SendEvent;
	Eventer		m_CloseEvent;

	Mutex		m_SocketsLock;
	CStlMap<SOCKET, CSocker*>	m_Sockets;

};

#endif	//SHARED_IOSELECT_H
