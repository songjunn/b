#ifndef _PLUGIN_H_
#define _PLUGIN_H_

class CBaseServer;
class CPluginHandler;
class CPlugin
{
public:
	CPlugin() {}
	virtual ~CPlugin() {}

	virtual bool startup() { return false; }
	virtual bool exit() { return false; }

	void setHandler(CPluginHandler* handler) {
		m_handler = handler;
	}

	void setServer(CBaseServer* server) {
		m_server = server;
	}

protected:
	CBaseServer* m_server;
	CPluginHandler* m_handler;

};

#endif //_PLUGIN_H_
