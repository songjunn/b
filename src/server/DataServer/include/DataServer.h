#ifndef _DATASERVER_H_
#define _DATASERVER_H_

#include "server.h"
#include "Singleton.h"

class CDataServer : public CBaseServer, public Singleton<CDataServer>
{
public:
	CDataServer();
	~CDataServer();

	virtual bool onStartup(string logconf, string logfile);
	virtual bool onLogic();
	virtual void onShutdown();
	virtual bool onMessage(Packet* pack);
	virtual void onPrint(char* output);
};

#define DataServer CDataServer::getSingleton()

#endif //_DATASERVER_H_
