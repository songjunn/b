#ifndef _PLUGIN_NETWORK_H_
#define _PLUGIN_NETWORK_H_

#include "plugin.h"
#include "Net.h"

class CNetwork : public CPlugin, public CNet
{
public:
	CNetwork();
	~CNetwork();

	bool startup(int type, int port, int connectmax, int sendbuffsize, int recvbuffsize, int packsize);

	virtual bool handlePacket(Packet * pCmd);

private:
	void _closeReturn(SOCKET sock);
	void _acceptReturn(SOCKET sock);
	void _connectReturn(SOCKET sock, int error = 0);

};

#endif //_PLUGIN_NETWORK_H_
