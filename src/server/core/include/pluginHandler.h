#ifndef _PLUGINHANDLER_H_
#define _PLUGINHANDLER_H_

#include "plugin_httpServe.h"

class CPluginHandler
{
public:
	CPluginHandler();
	virtual ~CPluginHandler();

	virtual void handle() {}

};

#endif //_PLUGINHANDLER_H_
