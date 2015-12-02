#ifndef _PLUGIN_MONITOR_H_
#define _PLUGIN_MONITOR_H_

#include "plugin.h"
#include "udsvr.h"

class CMonitor : public CPlugin
{
public:
	typedef void (*data_func)( char *output );

	CMonitor();
	virtual ~CMonitor();

	bool startup(const char *path);

protected:
	static void MonitorThread(void *param);
	
public:
	data_func m_func;
	CUnixDomainServer m_udsvr;

};

#endif	//_PLUGIN_MONITOR_H_
