#ifndef _PLUGIN_HTTPSERVE_H_
#define _PLUGIN_HTTPSERVE_H_

#include "plugin.h"
#include "httpd.h"

class CHttpServe : public CPlugin
{
public:
    CHttpServe();
    ~CHttpServe();

    bool startup(int port, httpd_handler_t handler);

};

#endif //_PLUGIN_HTTPSERVE_H_
