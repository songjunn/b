#include "LoginServer.h"
#include "LuaEngine.h"
#include "PathFunc.h"
#include "logger.h"
#include <unistd.h>
#include "gtime.h"
#include "http_message_handler.h"

createFileSingleton(CLuaEngine);
createFileSingleton(CLoginServer);

CLoginServer::CLoginServer()
{
}

CLoginServer::~CLoginServer()
{
}

bool CLoginServer::onStartup(string logconf, string logfile)
{
    CBaseServer::onStartup(logconf, logfile);

    CHttpServe* httpd = (CHttpServe *)this->createPlugin(CBaseServer::Plugin_HttpServe);
    if (!httpd->startup(1313, httpserver_ev_handler)) {
        LOGGER_ERROR("[CLoginServer] create Plugin_HttpServe failed");
        return false;
    }

    return true;
}

void CLoginServer::onShutdown()
{
    CBaseServer::onShutdown();
}
