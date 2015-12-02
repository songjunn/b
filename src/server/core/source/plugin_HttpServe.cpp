#include "plugin_HttpServe.h"
#include "memory.h"
#include "httpd.h"
#include "ThreadLib.h"

CHttpServe::CHttpServe()
{
}

CHttpServe::~CHttpServe()
{
}

bool CHttpServe::startup(int port, httpd_handler_t handler)
{
    struct httpd_server* server = httpd_create_server(port, handler);
    ThreadLib::Create(httpd_start, server);
    return true;
}

