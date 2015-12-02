#include "CentralServer.h"
#include "LuaEngine.h"
#include "PathFunc.h"
#include "gtime.h"
#include "logger.h"
#include "vprof.h"
#include "MessageTypeDefine.pb.h"
#include "MessageServer.pb.h"

createFileSingleton(CLuaEngine);
createFileSingleton(CCentralServer);

CCentralServer::CCentralServer()
{
	setType(CBaseServer::Linker_Server_Central);
}

CCentralServer::~CCentralServer()
{
}

bool CCentralServer::onStartup(string logconf, string logfile)
{
	CBaseServer::onStartup(logconf, logfile);

	//初始化启动参数
	char g_szExePath[512] = { 0 };
	GetExePath(g_szExePath, 512);
	char szConfile[256] = { 0 };
	sprintf(szConfile, "%ssconf.lua", g_szExePath);
	if (!LuaEngine.LoadLuaFile(szConfile))
	{
		LOGGER_ERROR("[CDataServer] Load sconf.lua Error");
		return false;
	}

	//读取启动参数
	const char* myip = LuaEngine.GetLuaVariableString("CentralServer_ip", "Sconf");
	int myid = LuaEngine.GetLuaVariableNumber("CentralServer_id", "Sconf");
	int myport = LuaEngine.GetLuaVariableNumber("CentralServer_port", "Sconf");
	int worldID = LuaEngine.GetLuaVariableNumber("WorldID", "Sconf");
	int connmax = LuaEngine.GetLuaVariableNumber("connect_count_max", "CentralServer");
	int packsize = LuaEngine.GetLuaVariableNumber("packet_pool_size", "CentralServer");
	int recvsize = LuaEngine.GetLuaVariableNumber("recv_buff_size", "CentralServer");
	int sendsize = LuaEngine.GetLuaVariableNumber("send_buff_size", "CentralServer");
	const char* udPath = LuaEngine.GetLuaVariableString("MonitorPath", "Key");

	//初始化
	char mpath[1024] = { 0 };
	sprintf(mpath, "%s//FPS_%d.sock", udPath, myid);
	this->initSelf(worldID, CBaseServer::Linker_Server_Central, myid, myport, myip, 0, NULL, mpath);

	CNetwork* servernet = (CNetwork *)this->createPlugin(CBaseServer::Plugin_Net4Server);
	if (!servernet->startup(CNet::NET_IO_SELECT, myport, connmax, sendsize, recvsize, packsize)) {
		LOGGER_ERROR("[CDataServer] create Plugin_Net4Server failed");
		return false;
	}

	char spath[1024] = { 0 };
	sprintf(spath, "%s//Server_%d.sock", udPath, myid);
	CMonitor* monitor = (CMonitor *)this->createPlugin(CBaseServer::Plugin_Monitor);
	if (!monitor->startup(spath)) {
		LOGGER_ERROR("[CDataServer] create Plugin_Monitor failed");
		return false;
	}

	return true;
}

bool CCentralServer::onMessage(Packet* pack)
{
	VPROF("CCentralServer::onMessage");

	if (!pack) {
		return false;
	}

	if (CBaseServer::onMessage(pack))
		return true;

	LOGGER_DEBUG("OnMsg no module %d", pack->Type());

	return false;
}

bool CCentralServer::onLogic()
{
	CBaseServer::onLogic();

	return true;
}

void CCentralServer::onPrint(char* output)
{
	char szServer[10240] = { 0 };

	CBaseServer::onPrint(szServer);

	sprintf(output,
		" CentralServer monitor: \n"
		" ======================================================\n"
		" %s",
		szServer);
}

void CCentralServer::onShutdown()
{
	CBaseServer::onShutdown();
}

bool CCentralServer::_onAddLinker(CLinker* pServer)
{
	if (!pServer)
		return false;

	FOR_EACH_LL(getLinkerList(), index)
	{
		CLinker* pObj = getLinkerList()[index];
		if (!pObj || pObj->m_nID == pServer->m_nID) {
			continue;
		}

		bool flag = false;
		if (pServer->m_type == CBaseServer::Linker_Server_Data && (pObj->m_type == CBaseServer::Linker_Server_Game)) flag = true;
		else if (pServer->m_type == CBaseServer::Linker_Server_Game && (pObj->m_type == CBaseServer::Linker_Server_Data || pObj->m_type == CBaseServer::Linker_Server_GateWay || pObj->m_type == CBaseServer::Linker_Server_DataAnalysis)) flag = true;
		else if (pServer->m_type == CBaseServer::Linker_Server_GateWay && (pObj->m_type == CBaseServer::Linker_Server_Game || pObj->m_type == CBaseServer::Linker_Server_Login || pObj->m_type == CBaseServer::Linker_Server_Payment)) flag = true;
		else if (pServer->m_type == CBaseServer::Linker_Server_Login && (pObj->m_type == CBaseServer::Linker_Server_GateWay)) flag = true;
		else if (pServer->m_type == CBaseServer::Linker_Server_Payment && (pObj->m_type == CBaseServer::Linker_Server_GateWay)) flag = true;
		else if (pServer->m_type == CBaseServer::Linker_Server_DataAnalysis && (pObj->m_type == CBaseServer::Linker_Server_Game)) flag = true;

		if (flag)
		{
			Message::SyncServer msg;
			msg.set_id(pObj->m_nID);
			msg.set_type(pObj->m_type);
			msg.set_port(pObj->m_nPort);
			msg.set_ip(pObj->m_szIP);
			msg.set_extport(pObj->m_extPort);
			msg.set_extip(pObj->m_extIP);
			msg.set_world(pObj->m_worldID);

			Packet pack;
			PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_SERVER_SYNCSERVER);
			GETSERVERNET(this)->sendMsg(pServer->m_Socket, &pack);
		}
	}

	return true;
}
