#include "BIServer.h"
#include "exception.h"
#include "LuaEngine.h"
#include "PathFunc.h"
#ifdef __linux__
#include "linux_time.h"
#endif
#include "AnalysisModule.h"

createFileSingleton(CLog);
createFileSingleton(CLuaEngine);
createFileSingleton(CBIServer);
createFileSingleton(CAnalysisModule);

CBIServer::CBIServer()
{
	setType(CBaseServer::Linker_Server_DataAnalysis);
}

CBIServer::~CBIServer()
{
}

bool CBIServer::onStartup(string logconf, string logfile)
{
	CBaseServer::onStartup(logconf, logfile);

	//初始化启动参数
	char g_szExePath[512] = { 0 };
	GetExePath(g_szExePath, 512);
	char szConfile[256] = { 0 };
	sprintf(szConfile, "%ssconf.lua", g_szExePath);
	if (!LuaEngine.LoadLuaFile(szConfile))
	{
		Log.Error("[CBIServer] Load sconf.lua Error");
		return false;
	}

	//读取启动参数
	const char* centralip = LuaEngine.GetLuaVariableString("CentralServer_ip", "Sconf");
	int centralid = LuaEngine.GetLuaVariableNumber("CentralServer_id", "Sconf");
	int centralport = LuaEngine.GetLuaVariableNumber("CentralServer_port", "Sconf");
	const char* myip = LuaEngine.GetLuaVariableString("BIServer_ip", "Sconf");
	int myid = LuaEngine.GetLuaVariableNumber("BIServer_id", "Sconf");
	int myport = LuaEngine.GetLuaVariableNumber("BIServer_port", "Sconf");
	const char* eventdbip = LuaEngine.GetLuaVariableString("db_event_ip", "Sconf");
	const char* eventdbname = LuaEngine.GetLuaVariableString("db_event_name", "Sconf");
	const char* eventdbport = LuaEngine.GetLuaVariableString("db_event_port", "Sconf");
	int worldID = LuaEngine.GetLuaVariableNumber("WorldID", "Sconf");
	int connmax = LuaEngine.GetLuaVariableNumber("connect_count_max", "BIServer");
	int packsize = LuaEngine.GetLuaVariableNumber("packet_pool_size", "BIServer");
	int recvsize = LuaEngine.GetLuaVariableNumber("recv_buff_size", "BIServer");
	int sendsize = LuaEngine.GetLuaVariableNumber("send_buff_size", "BIServer");
	int dboperator = LuaEngine.GetLuaVariableNumber("db_operator_max", "BIServer");
	const char* udPath = LuaEngine.GetLuaVariableString("MonitorPath", "Key");

	//初始化
	char mpath[1024] = { 0 };
	sprintf(mpath, "%s//FPS_%d.sock", udPath, myid);
	this->initSelf(worldID, CBaseServer::Linker_Server_DataAnalysis, myid, myport, myip, 0, NULL, mpath);

	CNetwork* servernet = (CNetwork *)this->createPlugin(CBaseServer::Plugin_Net4Server);
	if (!servernet->startup(CNet::NET_IO_SELECT, myport, connmax, sendsize, recvsize, packsize)) {
		Log.Error("[CBIServer] create Plugin_Net4Server failed");
		return false;
	}

	CMongoDB* db = (CMongoDB *)this->createPlugin(CBaseServer::Plugin_Mongodb);
	if (!db->startup(eventdbip, eventdbport, eventdbname)) {
		Log.Error("[CBIServer] create plugin CMongoDB failed");
		return false;
	}

	char spath[1024] = { 0 };
	sprintf(spath, "%s//Server_%d.sock", udPath, myid);
	CMonitor* monitor = (CMonitor *)this->createPlugin(CBaseServer::Plugin_Monitor);
	if (!monitor->startup(spath)) {
		Log.Error("[CBIServer] create Plugin_Monitor failed");
		return false;
	}

	if (!this->createLinker(CBaseServer::Linker_Server_Central, centralid, centralport, centralip, 0, NULL, worldID, true)) {
		Log.Error("[CBIServer] create Central Server failed");
		return false;
	}

	return true;
}

bool CBIServer::onMessage(Packet* pack)
{
	VPROF("CBIServer::onMessage");

	if (!pack) {
		return false;
	}

	if (AnalysisModule.OnMsg(pack))
		return true;

	if (CBaseServer::onMessage(pack))
		return true;

	Log.Debug("OnMsg no module %d", pack->Type());

	return false;
}

bool CBIServer::onLogic()
{
	CBaseServer::onLogic();

	return true;
}

void CBIServer::onPrint(char* output)
{
	char szServer[10240] = { 0 };

	CBaseServer::onPrint(szServer);
	sprintf(output,
		" BIServer monitor: Player:%d\n"
		" ======================================================\n"
		" %s",
		AnalysisModule.GetOnlinePlayer(),
		szServer);
}

void CBIServer::onShutdown()
{
	CBaseServer::onShutdown();
}
