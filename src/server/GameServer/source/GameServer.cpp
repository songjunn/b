#include "GameServer.h"
#include "LoginModule.h"
#include "DebugModule.h"
#include "LuaEngine.h"
#include "PathFunc.h"
#include "User.h"
#include "attrs.h"
#include "ItemMgr.h"
#include "HeroMgr.h"
#include "PlayerMgr.h"
#include "RoleTemplate.h"
#include "ItemTemplate.h"
#include "PlayerMgr.h"
#include "UserMgr.h"
#include "NameText.h"
#include "DataModule.h"
#include "TimerModule.h"
#include "NoticeModule.h"
#include "LuaFuncDefine.h"
#include "MessageTypeDefine.pb.h"
#include "MessageServer.pb.h"
#ifdef __linux__
#include "types.h"
#include "gtime.h"
#endif

createFileSingleton(CLuaEngine);
createFileSingleton(CGameServer);
createFileSingleton(CLoginModule);
createFileSingleton(CEventPool);
createFileSingleton(CPlayerMgr);
createFileSingleton(CUserMgr);
createFileSingleton(CHeroMgr);
createFileSingleton(CItemMgr);
createFileSingleton(CRoleTemplateMgr);
createFileSingleton(CItemTemplateMgr);
createFileSingleton(CDebugModule);
createFileSingleton(CNameTextMgr);
createFileSingleton(CTimerModule);
createFileSingleton(CNoticeModule);
createFileSingleton(CDataModule);

char g_szExePath[512];

CGameServer::CGameServer()
{
	setType(CBaseServer::Linker_Server_Game);
}

CGameServer::~CGameServer()
{
}

bool CGameServer::onStartup(string logconf, string logfile)
{
	CBaseServer::onStartup(logconf, logfile);

	//初始化启动参数
	GetExePath(g_szExePath, 512);
	char szConfile[256] = { 0 };
	sprintf(szConfile, "%ssconf.lua", g_szExePath);
	if (!LuaEngine.LoadLuaFile(szConfile))
	{
		LOGGER_ERROR("[CGameServer] Load sconf.lua Error");
		return false;
	}

	//读取启动参数
	const char* centralip = LuaEngine.GetLuaVariableString("CentralServer_ip", "Sconf");
	int centralid = LuaEngine.GetLuaVariableNumber("CentralServer_id", "Sconf");
	int centralport = LuaEngine.GetLuaVariableNumber("CentralServer_port", "Sconf");
	const char* myip = LuaEngine.GetLuaVariableString("GameServer_ip", "Sconf");
	int myid = LuaEngine.GetLuaVariableNumber("GameServer_id", "Sconf");
	int myport = LuaEngine.GetLuaVariableNumber("GameServer_port", "Sconf");
	int worldID = LuaEngine.GetLuaVariableNumber("WorldID", "Sconf");
	int connmax = LuaEngine.GetLuaVariableNumber("connect_count_max", "GameServer");
	int packsize = LuaEngine.GetLuaVariableNumber("packet_pool_size", "GameServer");
	int recvsize = LuaEngine.GetLuaVariableNumber("recv_buff_size", "GameServer");
	int sendsize = LuaEngine.GetLuaVariableNumber("send_buff_size", "GameServer");
	int usercnt = LuaEngine.GetLuaVariableNumber("user_count_max", "GameServer");
	int playercnt = LuaEngine.GetLuaVariableNumber("player_count_max", "GameServer");
	int itemcnt = LuaEngine.GetLuaVariableNumber("item_count_max", "GameServer");
	int herocnt = LuaEngine.GetLuaVariableNumber("hero_count_max", "GameServer");
	const char* udPath = LuaEngine.GetLuaVariableString("MonitorPath", "Key");

	//开服时间
	const char* strOpenTime = LuaEngine.GetLuaVariableString("ServerOpenTime", "Key");
	//g_firstServerTime = StringToDatetime(strOpenTime);

	//加载lua脚本
	if (!onLoadScript())
		return false;

	if (!UserMgr.Initialize("user", playercnt))
		return false;
	if (!PlayerMgr.Initialize("player", playercnt))
		return false;
	if (!ItemMgr.Initialize("item", itemcnt))
		return false;
	if (!EventPool.Create(1000))
		return false;

	//DataModule.onLoadConfig("xml//player.xml");
	//DataModule.onLoadConfig("xml//item.xml");
	//DataModule.onLoadConfig("xml//hero.xml");

	//DataModule.Initialize("objproxy", 100);

	attrs::init_all_attrs();

	//加载数据文件
	char roleFile[256] = { 0 };
	sprintf(roleFile, "%s/data/role.csv", g_szExePath);
	if (!RoleTemplateMgr.LoadCSVData(roleFile))
		return false;
	char itemFile[256] = { 0 };
	sprintf(itemFile, "%s/data/item.csv", g_szExePath);
	if (!ItemTemplateMgr.LoadCSVData(itemFile))
		return false;
	char nameFile[256] = { 0 };
	sprintf(nameFile, "%s/data/blacklist.csv", g_szExePath);
	if (!NameTextMgr.LoadCSVData(nameFile))
		return false;

	//初始化
	char mpath[1024] = { 0 };
	sprintf(mpath, "%s//FPS_%d.sock", udPath, myid);
	this->initSelf(worldID, CBaseServer::Linker_Server_Game, myid, myport, myip, 0, NULL, mpath);

	CNetwork* servernet = (CNetwork *)this->createPlugin(CBaseServer::Plugin_Net4Server);
	if (!servernet->startup(CNet::NET_IO_SELECT, myport, connmax, sendsize, recvsize, packsize)) {
		LOGGER_ERROR("[CGameServer] create Plugin_Net4Server failed");
		return false;
	}

	char spath[1024] = { 0 };
	sprintf(spath, "%s//Server_%d.sock", udPath, myid);
	CMonitor* monitor = (CMonitor *)this->createPlugin(CBaseServer::Plugin_Monitor);
	if (!monitor->startup(spath)) {
		LOGGER_ERROR("[CGameServer] create Plugin_Monitor failed");
		return false;
	}

	if (!this->createLinker(CBaseServer::Linker_Server_Central, centralid, centralport, centralip, 0, NULL, worldID, true)) {
		LOGGER_ERROR("[CGameServer] create Central Server failed");
		return false;
	}

	return true;
}

bool CGameServer::onMessage(Packet* pack)
{
	VPROF("CGameServer::onMessage");

	if (!pack) {
		return false;
	}

	if (PlayerMgr.OnMsg(pack))
		return true;

	if (LoginModule.OnMsg(pack))
		return true;

	if (DataModule.onMessage(pack))
		return true;

	if (CBaseServer::onMessage(pack))
		return true;

	if (DebugModule.OnMsg(pack))
		return true;

	CPlayer* player = PlayerMgr.GetObj(pack->GetTrans());
	if (player && player->OnMsg(pack))
		return true;

	LOGGER_DEBUG("OnMsg no module %d", pack->Type());

	return false;
}

bool CGameServer::onLogic()
{
	CBaseServer::onLogic();

	TimerModule.OnLogic();

	return true;
}

void CGameServer::onPrint(char* output)
{
	char szUserPool[10240] = { 0 };
	char szPlayerPool[10240] = { 0 };
	char szServer[10240] = { 0 };

	UserMgr.m_pool.Output(szUserPool, 10240);
	PlayerMgr.m_pool.Output(szPlayerPool, 10240);
	CBaseServer::onPrint(szServer);

	sprintf(output,
		" GameServer monitor: User:%d Player:%d\n"
		" ======================================================\n"
		" memory pool used:\n"
		"  %s"
		"  %s"
		" ======================================================\n"
		" %s",
		UserMgr.Count(), PlayerMgr.Count(),
		szUserPool, szPlayerPool,
		szServer);
}

void CGameServer::onShutdown()
{
	CBaseServer::onShutdown();
}
bool CGameServer::onLoadScript()
{
	//加载脚本函数和脚本文件
	LuaEngine.RegUserFunc(user_funcs);
	char file[256] = { 0 };
	sprintf(file, "%sscripts//load.lua", g_szExePath);
	if (!LuaEngine.LoadLuaFile(file))
	{
		LOGGER_ERROR("[CGameServer] Load Script File Failed:%s", file);
		return false;
	}
	LOGGER_NOTICE("[CGameServer] onLoadScript");
	return true;
}

bool CGameServer::_onAddLinker(CLinker* pServer)
{
	if (!pServer) {
		return false;
	}

	switch (pServer->m_type)
	{
	case CBaseServer::Linker_Server_Data:
		{
			Message::WorldDataRequest msg;
			msg.set_world(this->getSelfWorld());
			msg.set_server(0);

			Packet pack;
			PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_SERVER_WORLD_REQUEST);
			GETSERVERNET(&GameServer)->sendMsg(pServer->m_Socket, &pack);
		}
		break;
	default:
		break;
	}

	return true;
}
