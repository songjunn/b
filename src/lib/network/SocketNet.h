#ifndef SHARED_SOCKETNET_H
#define SHARED_SOCKETNET_H

#include "NetHeader.h"

class CSocketNet
{
public:
	virtual bool	Startup(int port, int connectmax, int sendbuffsize, int recvbuffsize) = 0;
	virtual void	Terminate() = 0;
	virtual bool	Shutdown(SOCKET sock) = 0;
	virtual int		Send(SOCKET sock, char * buf, int nSize) = 0;
	virtual bool	Recv(SOCKET sock, char * buf, int nSize) = 0;
	virtual SOCKET	Connect(const char * ip, int port) = 0;
	virtual SOCKET	ConnectAsync(const char * ip, int port) = 0;

};

#endif
