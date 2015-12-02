#ifndef _LOGINSERVER_H_
#define _LOGINSERVER_H_

#include "server.h"
#include "Singleton.h"

class CLoginServer : public CBaseServer, public Singleton<CLoginServer>
{
public:
	CLoginServer();
	~CLoginServer();

	virtual bool onStartup(string logconf, string logfile);
	virtual void onShutdown();
	virtual bool onLogic() { return true; }
	virtual bool onMessage(Packet* pack) { return true; }
	virtual void onPrint(char* output) { return; }

};

#define LoginServer CLoginServer::getSingleton()

#endif //_LOGINSERVER_H_
