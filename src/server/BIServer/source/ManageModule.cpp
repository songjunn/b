#include "ManageModule.h"
#include "Log.h"
#include "gtime.h"
#include "Packet.h"
#include "PacketDefine.h"
#include "HttpServer.h"
#include "MainServer.h"
#include "ServerMgr.h"
#include "MessageGMManager.pb.h"

CManageModule::CManageModule()
{
	m_LastOptTime = 0;
}

CManageModule::~CManageModule()
{

}

bool CManageModule::OnManageRequest(struct mg_connection *conn)
{
	if (conn == NULL) {
		Log.Error("[ManageModule] conn == NULL.");
		return false;
	}

	if (GetTimeSec() - m_LastOptTime <= MSG_OPT_CIRCLE) {
		return false;
	}

	char mType[MSG_VAR_LEN] = { 0 };
	mg_get_var(conn, MSG_VAR_TYPE, mType, MSG_VAR_LEN);

	switch (atoi(mType))
	{
	case Manage_Msg_Forbid:		return _HandleConn_Forbid(conn);
	case Manage_Msg_Notice:		return _HandleConn_Notice(conn);
	case Manage_Msg_Mail:		return _HandleConn_Mail(conn);
	case Manage_Msg_UnForbid:	return _HandleConn_UnForbid(conn);

	default: 
		Log.Error("[ManageModule] msg type %s wrong.", mType);
		return false;
	}

	Log.Notice("Manage done. ip: %s, port: %d.", conn->remote_ip, conn->remote_port);
}

bool CManageModule::_HandleConn_Forbid(struct mg_connection *conn)
{
	char userid[MSG_VAR_LEN] = { 0 };
	char forcetime[MSG_VAR_LEN] = { 0 };
	char reason[MSG_VAR_LEN] = { 0 };
	mg_get_var(conn, MSG_VAR_USER, userid, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_FTIME, forcetime, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_REASON, reason, MSG_VAR_LEN);
	
	int64 uid = atoll(userid);
	int64 tforce = atoll(forcetime) * GTIME_HOUR_SEC + GetTimeSec();
	int treason = atol(reason);
	if (uid < 1 || tforce < 1) {
		Log.Error("[ManageModule] forbid user param wrong. user: %s, forcetime: %s", userid, forcetime);
		return false;
	}

	Message::GMManager msg;
	msg.set_userid(uid);
	msg.set_forcetime(tforce);
	msg.set_reason(treason);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, B2G_MANAGE_FORBID);
	SendGameMsg(&pack);

	Log.Notice("forbid user: %s forcetime: %s success.", userid, forcetime);

	return true;
}

bool CManageModule::_HandleConn_Notice(struct mg_connection *conn)
{
	char starttime[MSG_VAR_LEN] = { 0 };
	char interval[MSG_VAR_LEN] = { 0 };
	char broadcasttimes[MSG_VAR_LEN] = { 0 };
	char content[MSG_VAR_LEN] = { 0 };
	mg_get_var(conn, MSG_VAR_STARTTIME, starttime, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_INTERVAL, interval, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_CASTTIMES, broadcasttimes, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_CONTENT, content, MSG_VAR_LEN);

	int64 tStartTime = atoll(starttime);
	int nInterVal = atoi(interval);
	int nCastTimes = atoi(broadcasttimes);

	if (tStartTime < 1 || nInterVal < 1 || nCastTimes < 1 || !content[0]) {
		Log.Error("Wrong notice. starttime: %lld, interval: %d, casttimes: %d, content: %s", tStartTime, nInterVal, nCastTimes, content);
		return false;
	}

	Message::GMManager msg;
	msg.set_starttime(tStartTime);
	msg.set_interval(nInterVal);
	msg.set_casttimes(nCastTimes);
	msg.set_content(content);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, B2G_MANAGE_NOTICE);
	SendGameMsg(&pack);

	Log.Notice("broadcast notice. starttime: %s, interval: %s, casttimes: %s, content: %s", starttime, interval, broadcasttimes, content);

	return true;
}

bool CManageModule::_HandleConn_Mail(struct mg_connection *conn)
{
	char playerid[MSG_VAR_LEN] = { 0 };
	char mailtitle[MSG_VAR_LEN] = { 0 };
	char item[MSG_VAR_LEN] = { 0 };
	char exp[MSG_VAR_LEN] = { 0 };
	char silver[MSG_VAR_LEN] = { 0 };
	char gold[MSG_VAR_LEN] = { 0 };
	char credit[MSG_VAR_LEN] = { 0 };
	char ap[MSG_VAR_LEN] = { 0 };
	char merit[MSG_VAR_LEN] = { 0 };
	char knight[MSG_VAR_LEN] = { 0 };
	mg_get_var(conn, MSG_VAR_PLAYER, playerid, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_MAILTITLE, mailtitle, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_MAILITEM, item, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_MAILEXP, exp, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_MAILSILVER, silver, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_MAILGOLD, gold, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_MAILCREDIT, credit, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_MAILAP, ap, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_MAILMERIT, merit, MSG_VAR_LEN);
	mg_get_var(conn, MSG_VAR_MAILKNIGHT, knight, MSG_VAR_LEN);

	PersonID pid = atoll(playerid);
	int nType = atoi(mailtitle);
	int nItem = atoi(item);
	int nExp = atoi(exp);
	int nSilver = atoi(silver);
	int nGold = atoi(gold);
	int nCredit = atoi(credit);
	int nAp = atoi(ap);
	int nMerit = atoi(merit);
	int nKnight = atoi(knight);

	if (pid < 1 || nType < 1) {
		Log.Error("Wrong Mail. playerid: %lld, mailtype: %d, item: %d, gold: %d, silver: %d", pid, nType, nItem, nGold, nSilver);
		return false;
	}

	Message::GMManager msg;
	msg.set_playerid(pid);
	msg.set_mailtitle(nType);
	msg.set_item(nItem);
	msg.set_exp(nExp);
	msg.set_silver(nSilver);
	msg.set_gold(nGold);
	msg.set_credit(nCredit);
	msg.set_ap(nAp);
	msg.set_merit(nMerit);
	msg.set_knight(nKnight);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, B2G_MANAGE_MAIL);
	SendGameMsg(&pack);

	Log.Notice("send mail. player: %s, item: %s, gold: %s, silver: %s", playerid, item, gold, silver);

	return true;
}

bool CManageModule::_HandleConn_UnForbid(struct mg_connection *conn)
{
	char userid[MSG_VAR_LEN] = { 0 };
	mg_get_var(conn, MSG_VAR_USER, userid, MSG_VAR_LEN);

	int64 uid = atoll(userid);
	if (uid < 1) {
		Log.Error("[ManageModule] unforbid user param wrong. user: %s", userid);
		return false;
	}

	Message::GMManager msg;
	msg.set_userid(uid);
	msg.set_forcetime(GetTimeSec() - GTIME_DAY_SEC);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, B2G_MANAGE_FORBID);
	SendGameMsg(&pack);

	Log.Notice("unforbid user: %s success.", userid);

	return true;
}

void CManageModule::SendGameMsg(Packet *pack)
{
	MainServer.SendMsgToServer(ServerMgr.getGameSock(), pack);
}
