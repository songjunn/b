#ifndef SHARED_SOCKER_H
#define SHARED_SOCKER_H

#include <string.h>
#include "NetHeader.h"
#include "RingBuffer.h"
#include "Lock.h"

class CSocker
{
public:
	enum Key_Status {
		Key_Free = 0,
		Key_Listen,
		Key_Connect,
		Key_Work,
		Key_Close,
		Key_CloseWait
	};

public:
	CSocker();
	virtual ~CSocker();

	bool createBuffer(int nSendSize, int nRecvSize);	//初始化
	bool createSocket(SOCKET sock = INVALID_SOCKET);	//创建socket
	void release();
	void clear();

	void setIP(const char *ip) {strncpy(m_szIP, ip, 32);}

public:
	SOCKET			m_socket;		//socket句柄
	Key_Status		m_status;		//工作状态
	RingBuffer*		m_RecvBuffer;	//接收数据缓冲
	RingBuffer*		m_SendBuffer;	//发送数据缓冲
	Mutex			m_SendLock;
	char			m_szIP[32];		//连接端的ip
	int				m_timeout;		//超时时间

};

#endif	//SHARED_SOCKER_H
