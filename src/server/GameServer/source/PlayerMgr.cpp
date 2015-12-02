#include "PlayerMgr.h"
#include "MessageTypeDefine.pb.h"
#include "MessagePlayer.pb.h"
#include "MessageGameobj.pb.h"


CPlayer* CPlayerMgr::Create(PersonID playerid)
{
	PersonID id = playerid;
	if (id <= 0)
	{
		m_FactId = g_MakePlayerID(m_FactId);
		id = m_FactId;
	}

	CPlayer* player = CObjMgr< CPlayer, PersonID >::Create(id);
	if (!player)
		return NULL;

	player->Init();
	player->SetID(id);

	return player;
}

CPlayer* CPlayerMgr::Create(int templateid, PersonID playerid)
{
	CPlayer* player = this->Create(playerid);
	if (!player)
	{
		return NULL;
	}

	if( !player->OnCreate(templateid) )
	{
		CObjMgr< CPlayer, PersonID >::Delete( player->GetID() );
		return NULL;
	}

	return player;
}

void CPlayerMgr::Delete(PersonID id)
{
	CPlayer* player = GetObj(id);
	if( player )
		player->Release();
	CObjMgr< CPlayer, PersonID >::Delete(id, player);

	LOGGER_DEBUG("[CPlayerMgr] Delete Player:"INT64_FMT, id);
}

void CPlayerMgr::OnLogic()
{
	/*PersonID temp;
	PersonID id = m_list.Head();
	while( CPlayer* player = GetObj(temp=id) )
	{
		id = m_list.Next(id);

		//删除非在线的数据，30分钟 //已废除此缓存机制，不应该出现此log
		if( player->GetOnline() != Online_Flag_On && timeGetTime() - player->GetLoadTime() > 60000 )
		{
			LOGGER_ERROR("[Logout] Offline Player:"INT64_FMT, temp);

			Delete(temp);
		}
	}*/
}

bool CPlayerMgr::OnMsg(Packet* pack)
{
	if( !pack )
		return false;

	switch( pack->Type() )
	{
	case Message::MSG_PLAYER_SYNC_ATTRINT:	_HandlePacket_SyncAttrInt(pack);	break;
	case Message::MSG_PLAYER_SYNC_ATTRI64:	_HandlePacket_SyncAttrI64(pack);	break;
	case Message::MSG_PLAYER_LOAD_OVER:		_HandlePacket_PlayerLoadOver(pack);	break;
	case Message::MSG_REQUEST_PLAYER_OBSERVE:	_HandlePacket_ObservePlayer(pack);	break;
	default:	return false;
	}

	return true;
}

bool CPlayerMgr::_HandlePacket_SyncAttrInt(Packet* pack)
{
	if( !pack )
		return false;

	/*Message::PlayerAttribInt msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CPlayer* player = GetObj( msg.pid() );
	if( !player )
		return false;

	player->SetFieldInt( msg.attr(), msg.value() );*/

	return true;
}

bool CPlayerMgr::_HandlePacket_SyncAttrI64(Packet* pack)
{
	if( !pack )
		return false;

	/*Message::PlayerAttribI64 msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CPlayer* player = GetObj( msg.pid() );
	if( !player )
		return false;

	player->SetFieldI64( msg.attr(), msg.value() );*/

	return true;
}

bool CPlayerMgr::_HandlePacket_PlayerLoadOver(Packet* pack)
{
	if( !pack )
		return false;

	Message::PlayerLoadOver msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CPlayer* player = this->GetObj( msg.pid() );
	if( !player )
		return false;

	player->DataInit();

	CEvent* evnt = MakeEvent(Event_Player_Loadover, player->GetID());
	player->OnEvent(evnt);

	return true;
}

bool CPlayerMgr::_HandlePacket_ObservePlayer(Packet* pack)
{
	if( !pack )
		return false;

	Message::ObservePlayerRequest msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CPlayer* player = this->GetObj(pack->GetTrans());
	if( player )
		LoadPlayerRequest(player, msg.playerid(), CPlayerMgr::EReqPlayer_Observe);

	return true;
}

void CPlayerMgr::LoadPlayerRequest(CPlayer* reqPlayer, PersonID tarID, int module)
{
	if( !reqPlayer )
		return;

	CPlayer* player = this->GetObj(tarID);
	if( player && player->GetOnline() != Online_Flag_Load )
	{
		//在线，直接返回事件
		_HandleLoadRequest(reqPlayer, tarID, module);
	}
	else
	{
		//不在线，缓存处理
		_HandleLoadCache(reqPlayer->GetID(), tarID, module);
	}
}

void CPlayerMgr::eventPlayerLoadover(PersonID tarID)
{
	m_LoadMap.Remove(tarID);

	int tmpIdx;
	for(int idx=m_LoadList.Head(); m_LoadList.IsValidIndex(tmpIdx=idx); )
	{
		idx = m_LoadList.Next(idx);

		Loading* load = m_LoadList.Element(tmpIdx);
		if( load && load->tarID == tarID )
		{
			CPlayer* player = this->GetObj(load->reqID);
			if( player )
			{
				_HandleLoadRequest(player, load->tarID, load->module);

				CPlayer* tarPlayer = this->GetObj(tarID);
				if( !tarPlayer )
				{
					/*CMetadata* json = DataModule.GetObj( tarID );
					if( json )
						tarPlayer = this->Create( json->getFieldInt("template"), tarID );*/
				}
				if( tarPlayer && tarPlayer->GetOnline() == Online_Flag_Off )
				{
					this->Delete(tarID);
					
					LOGGER_NOTICE("[Logout] Offline Player:"INT64_FMT, tarID);
				}
			}

			SAFE_DELETE(load);
			m_LoadList.Remove(tmpIdx);
		}
	}
}

void CPlayerMgr::eventPlayerLoadObserve(PersonID reqID, PersonID tarID, int module)
{
	if( module != CPlayerMgr::EReqPlayer_Observe )
		return;

	CPlayer* req = this->GetObj(reqID);
	CPlayer* tar = this->GetObj(tarID);

	SyncObservePlayer(req, tar);
}

void CPlayerMgr::_HandleLoadCache(PersonID reqID, PersonID tarID, int module)
{
	Loading* load = NEW Loading(reqID, tarID, module);
	if( !load )
		return;

	m_LoadList.AddToTail(load);

	int idx = m_LoadMap.Find(tarID);
	if( m_LoadMap.IsValidIndex(idx) )
		return;

	m_LoadMap.Insert(tarID, tarID);

	Message::ReqPlayerData msg;
	msg.set_pid(tarID);
	msg.set_type("player");
	msg.set_key("playerid");
	
	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_LOAD_REQUEST);
	GETSERVERNET(&GameServer)->sendMsg(GameServer.getServerSock(CBaseServer::Linker_Server_Data), &pack);
}

void CPlayerMgr::_HandleLoadRequest(CPlayer* reqPlayer, PersonID tarID, int module)
{
	if( reqPlayer )
	{
		CEvent* evnt = MakeEvent(Event_Player_LoadModule, reqPlayer->GetID(), tarID, (int64)module);
		reqPlayer->OnEvent(evnt);
	}
}

void CPlayerMgr::SyncObservePlayer(CPlayer* player, CPlayer* toPlayer)
{
	if( !player || !toPlayer )
		return;

	player->SyncAllAttrToClient(toPlayer);
}
