#include "AnalysisModule.h"
#include "BIServer.h"
#include "Log.h"
#include "Packet.h"
#include "eventdef.h"
#include "MessageTypeDefine.pb.h"
#include "MessageCommon.pb.h"


CAnalysisModule::CAnalysisModule()
{
	m_OnlinePlayer = 0;
	m_CreatePlayerCount = 0; //创建角色总数
	m_GoldCount = 0; //购买金币总数
}

void CAnalysisModule::_GetPlayerCountFromDB()
{
	mongo::Query query = QUERY("serverID" << 0);
	mongo::auto_ptr<mongo::DBClientCursor> cursor;
	GETMONGODB(&BIServer)->select(cursor, "serverInfo", query);

	Log.Debug("++++++++LogIn playerCount is %d", m_OnlinePlayer);
	while (cursor.get() && cursor->more())
	{
		mongo::BSONObj p = cursor->next();
		GETMONGODB(&BIServer)->getBsonFieldInt(p, "playerCount", m_OnlinePlayer);
	}
	Log.Debug("__________________LogIn playerCount is %d", m_OnlinePlayer);
	return;
}

CAnalysisModule::~CAnalysisModule()
{

}

bool CAnalysisModule::OnMsg(Packet* pack)
{
	if( !pack )
		return false;

	switch( pack->Type() )
	{
	case Message::MSG_COMMON_EVENT:	_HandlePacket_EventSync(pack);	break;
	default:	return false;
	}

	return true;
}

bool CAnalysisModule::_HandlePacket_EventSync(Packet* pack)
{
	if( !pack )
		return false;

	Message::EventSync msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	//分析事件
	int saveType = _AnalysisEvent(msg.evid(), msg.parent(), msg.pid(), msg.param1(), msg.param2(), msg.param3(), msg.param4(), msg.param5(), msg.param6(), msg.param7().c_str());

	//存储事件
	_SaveEvent(msg.evid(), msg.parent(), msg.pid(), msg.stime().c_str(), msg.param1(), msg.param2(), msg.param3(), msg.param4(), msg.param5(), msg.param6(), msg.param7().c_str());

	if (saveType) {
		_SaveServerState();
	}

	return true;
}

int CAnalysisModule::_AnalysisEvent(int nEventID, int64 parent, int64 PID, int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4, int64 nParam5, int64 nParam6, const char* szParam1)
{
	int saveType = 0;
	switch(nEventID)
	{
		case Event_Player_Login:	
			m_OnlinePlayer++;	
			saveType = 1; 
			Log.Debug("LogIn playerCount is %d", m_OnlinePlayer); 
			_SaveServerInfoToDB();
			break;

		case Event_Player_Logout:	
			m_OnlinePlayer--;	
			saveType = 1; 
			Log.Debug("LogOut playerCount is %d", m_OnlinePlayer); 
			_SaveServerInfoToDB();
			break;

		case Event_Player_Create: 
			m_CreatePlayerCount++;
			saveType = 1;
			Log.Debug("Create playerCount is %d", m_CreatePlayerCount);
			_SaveServerInfoToDB();
			break;

		case Event_Gold_Trade:
			m_GoldCount += nParam1;
			saveType = 1;
			Log.Debug("TradeGoldCount is %d", m_GoldCount);
			_SaveServerInfoToDB();
			break;

		default:	
			break;
	}

	return saveType;
}

void CAnalysisModule::_SaveEvent(int nEventID, int64 parent, int64 PID, const char* stime, int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4, int64 nParam5, int64 nParam6, const char* szParam1)
{
	mongo::Query query;
	mongo::BSONObjBuilder obj;

	obj.append("evid", nEventID);
	obj.append("parent", parent);
	obj.append("role", PID);
	obj.append("time", stime);
	obj.append("param1", nParam1);
	obj.append("param2", nParam2);
	obj.append("param3", nParam3);
	obj.append("param4", nParam4);
	obj.append("param5", nParam5);
	obj.append("param6", nParam6);
	obj.append("param7", szParam1);

	GETMONGODB(&BIServer)->execute(CMongoDB::Mongo_Insert, "event", query, obj.obj());
}

void CAnalysisModule::_SaveServerInfoToDB()
{
	mongo::Query query = QUERY("serverID"<<0);
	mongo::BSONObjBuilder obj;

	obj.append("playerCount", m_OnlinePlayer);
	obj.append("registerCount", m_CreatePlayerCount);
	obj.append("rechargeCount", m_GoldCount);
	obj.append("serverID", 0);

	GETMONGODB(&BIServer)->execute(CMongoDB::Mongo_Update, "serverInfo", query, obj.obj());
	return;
}


void	CAnalysisModule::_SaveServerState()
{
	/*char szSQL[1024]={0};
	sprintf(szSQL,"INSERT INTO server_state (\
					time, \
					online_player) \
					VALUES(\
					now(), \
					%d)"
					,m_OnlinePlayer	
				  );
	MainServer.GetMysqlDBClient()->ExcuteSql(SQL_NORETURN, szSQL, strlen(szSQL));
	Log.Debug("Excute sql: %s", szSQL);*/


}
