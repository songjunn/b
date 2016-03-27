#ifndef _PLUGIN_H_
#define _PLUGIN_H_

class CBaseServer;
class CPlugin
{
public:
	CPlugin() {}
	virtual ~CPlugin() {}

	virtual bool startup() { return false; }
	virtual bool exit() { return false; }

	void setServer(CBaseServer* server) {
		m_server = server;
	}

protected:
	CBaseServer* m_server;

};

#endif //_PLUGIN_H_
