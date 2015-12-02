#ifndef _GATESERVER_H_
#define _GATESERVER_H_

#include "server.h"
#include "Singleton.h"

class CGateServer : public CBaseServer, public Singleton<CGateServer>
{
public:
    CGateServer();
    ~CGateServer();

    virtual bool onStartup(string logconf, string logfile);
    virtual bool onLogic();
    virtual void onShutdown();
    virtual bool onMessage(Packet* pack);
    virtual void onPrint(char* output);

	inline void setAuthAddress(string addr) { m_authAddress = addr; }
	inline string getAuthAddress() { return m_authAddress; }

private:
	string m_authAddress;

};

#define GateServer CGateServer::getSingleton()

#endif //_GATESERVER_H_
