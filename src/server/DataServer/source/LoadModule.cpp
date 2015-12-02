#include "LoadModule.h"
#include "DataModule.h"
#include "commdata.h"
#include "utlsymbol.h"
#include "strtools.h"
#include "gtime.h"
#include "DataServer.h"
#include "Packet.h"
#include "MessageTypeDefine.pb.h"
#include "MessageServer.pb.h"
#include "MessageUser.pb.h"
#include "MessagePlayer.pb.h"


bool CLoadModule::onMessage(Packet* pack)
{
	if( !pack )
		return false;

	switch(pack->Type())
	{
	case Message::MSG_USER_lOGIN_REQUEST:		_handlePacket_LoadPlayerCount(pack);	break;
	case Message::MSG_PLAYER_CHECKNAME_REQUEST:	_handlePacket_CheckNameRepeat(pack);	break;
	case Message::MSG_SERVER_WORLD_REQUEST:		_handlePacket_LoadWorldData(pack);		break;
	default:	return false;
	}

	return true;
}

void CLoadModule::onLogic()
{
	int n = 0;
	int index = m_LoadList.Head();

	while( m_LoadList.IsValidIndex(index) && n++ < 10 )
	{
		CLoadObj* loader = m_LoadList[index];
		if( loader )
		{
			if( loader->id > 0 )
			{
				CDataObj* obj = DataModule.loadDb(loader->status, loader->type, loader->key, loader->id);
				if (obj) {
					DataModule.syncDataSeparate(obj, loader->sock);
				}
			}
			else
			{
				if (DataModule.loadDb(loader->status, loader->type))
					DataModule.syncData(loader->type, loader->sock);
			}

			SAFE_DELETE(loader);
		}
		
		m_LoadList.Remove(index);
		index = m_LoadList.Head();
	}
}

bool CLoadModule::_handlePacket_LoadWorldData(Packet* pack)
{
	if( !pack )
		return false;

	Message::WorldDataRequest msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	mongo::Query query;
	mongo::auto_ptr<mongo::DBClientCursor> cursor;
	GETMONGODB(&DataServer)->select(cursor, "player", query.sort(BSON("playerid" << -1)));

	PersonID playerid = INVALID_VALUE;
	if (cursor.get() && cursor->more()) {
		GETMONGODB(&DataServer)->getBsonFieldI64(cursor->next(), "playerid", playerid);
	}

	Message::WorldDataResponse msgData;
	msgData.set_playerid( playerid );

	Packet packData;
	PROTOBUF_CMD_PACKAGE(packData, msgData, Message::MSG_SERVER_WORLD_RESPONSE);
	GETSERVERNET(&DataServer)->sendMsg(pack->GetNetID(), &packData);

	return true;
}

bool CLoadModule::_handlePacket_CheckNameRepeat(Packet* pack)
{
	if( !pack )
		return false;

	Message::CheckNameRequest msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	//全部转换小写进行比较
	char name[OBJ_NAME_LEN] = {0};
	strnlower(msg.name().c_str(), name, OBJ_NAME_LEN);

	CUtlSymbol symbol = CUtlSymbol::Find(name);
	bool result = !symbol.IsValid();
	if( result )
		symbol.AddString(name);

	Message::CheckNameResponse msg1;
	msg1.set_uid( msg.uid() );
	msg1.set_pid( msg.pid() );
	msg1.set_result( result );

	Packet pack1;
	PROTOBUF_CMD_PACKAGE(pack1, msg1, Message::MSG_PLAYER_CHECKNAME_RESPONSE);
	GETSERVERNET(&DataServer)->sendMsg(pack->GetNetID(), &pack1);

	return true;
}

bool CLoadModule::_handlePacket_LoadPlayerCount(Packet* pack)
{
	if( !pack )
		return false;

	Message::UserLogin msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	mongo::Query query = QUERY(msg.key()<<(int64)msg.uid());
	mongo::auto_ptr<mongo::DBClientCursor> cursor;
	GETMONGODB(&DataServer)->select(cursor, msg.type(), query);

	std::string idstr = msg.type() + "id";
	std::vector<int64> objs;
	while (cursor.get() && cursor->more())
	{
		mongo::BSONObj p = cursor->next();

		PersonID playerid = INVALID_VALUE;
		if (GETMONGODB(&DataServer)->getBsonFieldI64(p, idstr, playerid)) {
			objs.push_back(playerid);
		}
	}

	Message::PlayerCount msgCnt;
	msgCnt.set_uid(msg.uid());
	for(uint i=0; i<objs.size(); ++i)
		msgCnt.add_player(objs[i]);

	Packet packCnt;
	PROTOBUF_CMD_PACKAGE(packCnt, msgCnt, Message::MSG_PLAYER_LOAD_COUNT);
	GETSERVERNET(&DataServer)->sendMsg(pack->GetNetID(), &packCnt);

	return true;
}

void CLoadModule::addToLoad(std::string type, std::string key, int64 id, int sock, int status)
{
	CLoadObj* loader = NEW CLoadObj;
	if (!loader) {
		return;
	}

	loader->type = type;
	loader->key = key;
	loader->id = id;
	loader->sock = sock;
	loader->status = status;

	m_LoadList.AddToTail(loader);
}
