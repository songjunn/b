#include "LoginModule.h"
#include "gtime.h"
#include "error.h"
#include "User.h"
#include "UserMgr.h"
#include "PlayerMgr.h"
#include "NameText.h"
#include "GameServer.h"
#include "NoticeModule.h"
#include "DataModule.h"
#include "LuaEngine.h"
#include "Event.h"
#include "gtime.h"
#include "Net.h"
#include "MessageTypeDefine.pb.h"
#include "MessageServer.pb.h"
#include "MessageUser.pb.h"
#include "MessagePlayer.pb.h"
#include "MessageGameobj.pb.h"


CLoginModule::CLoginModule()
{
	SetDefLessFunc(m_LoginMap);
}

CLoginModule::~CLoginModule()
{
}

bool CLoginModule::OnMsg(Packet* pack)
{
	if( !pack )
		return false;

	switch( pack->Type() )
	{
	case Message::MSG_USER_lOGIN_REQUEST:		_HandlePacket_UserLogin(pack);		break;
	case Message::MSG_PLAYER_LOGOUT_REQEUST:	_HandlePacket_PlayerLogout(pack);	break;
	case Message::MSG_PLAYER_LOAD_COUNT:		_HandlePacket_PlayerCount(pack);	break;
	case Message::MSG_REQUEST_PLAYER_CREATE:	_HandlePacket_PlayerCreate(pack);	break;
	case Message::MSG_PLAYER_CHECKNAME_RESPONSE:_HandlePacket_PlayerOnCreate(pack);	break;
	case Message::MSG_SERVER_WORLD_RESPONSE:	_HandlePacket_LoadWorldData(pack);	break;
	default:	return false;
	}

	return true;
}

bool CLoginModule::_HandlePacket_UserLogin(Packet* pack)
{
	if( !pack )
		return false;

	Message::UserLogin msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CUser* pUser = UserMgr.GetObj( msg.uid() );
	if( pUser )
	{
		if( pUser->m_GateSocket != pack->GetNetID() )
		{
			//发送踢号
			Message::UserDisplace msgKick;
			msgKick.set_uid( msg.uid() );
			Packet packKick;
			PROTOBUF_CMD_PACKAGE(packKick, msgKick, Message::MSG_USER_DISPLACE);
			pUser->SendGateMsg( &packKick );

			LOGGER_NOTICE("[Login] Displace Online User:"INT64_FMT, pUser->m_ID);
		}

		pUser->m_GateSocket = pack->GetNetID();

		if( pUser->HavePlayer() )
		{
			PersonID pid = pUser->GetLoginPlayer();
			CPlayer* player = PlayerMgr.GetObj(pid);
			if( !player )
			{
				LOGGER_ERROR("[Login] Player Error(User:"INT64_FMT", Player:"INT64_FMT")", pUser->m_ID, pid);
				return false;
			}

			OnPlayerLogin(player);
		}
		else
		{
			LOGGER_DEBUG("[Login] Online User not have player: (User:"INT64_FMT", Player:"INT64_FMT")", pUser->m_ID, pUser->GetLoginPlayer());

			//向GateServer同步该账号无角色
			Message::PlayerCount msgCount;
			msgCount.set_uid( msg.uid() );
			//msgCount.set_count( 0 );

			Packet packCount;
			PROTOBUF_CMD_PACKAGE(packCount, msgCount, Message::MSG_PLAYER_LOAD_COUNT);
			pUser->SendGateMsg( &packCount );
		}
	}
	else
	{
		pUser = UserMgr.Create( msg.uid() );
		if( !pUser )
			return false;

		pUser->m_ID = msg.uid();
		pUser->m_GateSocket = pack->GetNetID();

		Message::UserLogin message;
		message.set_uid(msg.uid());
		message.set_world(msg.world());
		message.set_server(msg.server());
		message.set_type("player");
		message.set_key("userid");

		//向data发送请求，加载角色数据
		Packet packet;
		PROTOBUF_CMD_PACKAGE(packet, message, Message::MSG_USER_lOGIN_REQUEST);
		GETSERVERNET(&GameServer)->sendMsg(GameServer.getServerSock(CBaseServer::Linker_Server_Data), &packet);
	}

	LOGGER_NOTICE( "[Login] User:"INT64_FMT, msg.uid() );

	return true;
}

bool CLoginModule::_HandlePacket_PlayerLogout(Packet* pack)
{
	if( !pack )
		return false;

	Message::PlayerLogout msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	return OnPlayerLogout( msg.pid() );
}

bool CLoginModule::_HandlePacket_PlayerCount(Packet* pack)
{
	if( !pack )
		return false;

	Message::PlayerCount msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CUser* pUser = UserMgr.GetObj( msg.uid() );
	if( !pUser )
		return false;

	if( msg.player_size() <= 0 )
	{	
		pUser->SendGateMsg(pack);
	}
	else
	{
		PersonID playerid = msg.player(0);
		m_LoginMap.Insert(playerid, playerid);

		Message::ReqPlayerData msgData;
		msgData.set_pid(playerid);
		msgData.set_type("player");
		msgData.set_key("playerid");
		
		Packet packData;
		PROTOBUF_CMD_PACKAGE(packData, msgData, Message::MSG_GAMEOBJ_LOGIN_REQUEST);
		GETSERVERNET(&GameServer)->sendMsg(GameServer.getServerSock(CBaseServer::Linker_Server_Data), &packData);
	}

	return true;
}

bool CLoginModule::_HandlePacket_PlayerCreate(Packet* pack)
{
	if( !pack )
		return false;

	Message::CreatePlayer msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	//验证名字合法性
	if( !NameTextMgr.CheckName(msg.name().c_str()) )
	{
		NoticeModule.SendErrorMsg(pack->GetNetID(), msg.uid(), Error_Create_NameInvalid);
		LOGGER_ERROR("[Login] Check Name Invalid:%s User:"INT64_FMT, msg.name().c_str(), msg.uid());
		return false;
	}

	//以职业作为模板id
	CPlayer* player = PlayerMgr.Create( msg.roletemplate() );
	if( !player )
		return false;

	player->SetLoadTime(timeGetTime());
	player->SetOnline(Online_Flag_Load);
	player->SetName( msg.name().c_str() );
	player->SetFieldI64( Role_Attrib_UserID, msg.uid() );
	player->SetGateSocket( pack->GetNetID() );

	char timestr[32] = { 0 };
	DatatimeToString(timestr);
	player->SetFieldStr(Role_Attrib_CreateTime, timestr);

	//验证重名
	Message::CheckNameRequest msg1;
	msg1.set_uid( msg.uid() );
	msg1.set_pid( player->GetID() );
	msg1.set_name( msg.name().c_str() );
	Packet pack1;
	PROTOBUF_CMD_PACKAGE(pack1, msg1, Message::MSG_PLAYER_CHECKNAME_REQUEST);
	player->SendDataMsg( &pack1 );

	return true;
}

bool CLoginModule::_HandlePacket_PlayerOnCreate(Packet* pack)
{
	if( !pack )
		return false;

	Message::CheckNameResponse msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	CPlayer* player = PlayerMgr.GetObj( msg.pid() );
	if( !player )
		return false;

	if( !msg.result() )
	{
		NoticeModule.SendErrorMsg(player->GetGateSocket(), player->GetFieldI64(Role_Attrib_UserID), Error_Create_NameRepeat);
		LOGGER_DEBUG("[Login] Check Name Repeat:%s User:"INT64_FMT, player->GetName().c_str(), msg.uid());
		PlayerMgr.Delete( msg.pid() );
		return false;
	}

	//执行脚本
	LuaParam param[1];
	param[0].SetDataNum( player->GetID() );
	if( !LuaEngine.RunLuaFunction("OnCreate", "Player", NULL, param, 1) )
	{
		LOGGER_DEBUG("[Login] RunLuaFunction Error, Player:"INT64_FMT" User:"INT64_FMT, player->GetID(), msg.uid());
		PlayerMgr.Delete( player->GetID() );
		return false;
	}

	//同步DataServer创建
	DataModule.syncCreate(player, "player", GameServer.getServerSock(CBaseServer::Linker_Server_Data));

	CEvent* evnt = MakeEvent(Event_Player_Create, player->GetID(), player->GetFieldI64(Role_Attrib_UserID), NULL, true);
	player->OnEvent(evnt);

	OnPlayerLogin(player);

	return true;
}

bool CLoginModule::_HandlePacket_LoadWorldData(Packet* pack)
{
	if( !pack )
		return false;

	Message::WorldDataResponse msg;
	PROTOBUF_CMD_PARSER( pack, msg );

	int64 id = msg.playerid() > 0 ? msg.playerid() : g_MakeInitPlayerID(GameServer.getSelfWorld());

	PlayerMgr.SetLoadFactID(id);

	return true;
}

bool CLoginModule::OnPlayerLogin(CPlayer* player)
{
	if( !player )
		return false;

	_OnPlayerSync(player);

	//执行脚本
	LuaParam param[1];
	param[0].SetDataNum(player->GetID());
	LuaEngine.RunLuaFunction("OnLogin", "Player", NULL, param, 1);

	char timestr[32] = { 0 };
	DatatimeToString(timestr);
	player->SetFieldStr(Role_Attrib_LoginTime, timestr, false, true);
	player->SetOnline(Online_Flag_On);

	player->m_ItemUnit.GainItem(10001001, Item_Reason_UnKonw);

	CEvent* evnt = MakeEvent(Event_Player_Login, player->GetID(), NULL, true);
	player->OnEvent(evnt);

	LOGGER_NOTICE("[Login] Online User:"INT64_FMT" Player:"INT64_FMT" Name:%s", player->GetFieldI64(Role_Attrib_UserID), player->GetID(), player->GetName().c_str());

	return true;
}

bool CLoginModule::OnPlayerLogout(PersonID id)
{
	CPlayer* player = PlayerMgr.GetObj(id);
	if (!player)
		return false;

	//下线前同步属性存盘
	char timestr[32] = { 0 };
	DatatimeToString(timestr);
	player->SetFieldStr(Role_Attrib_LogoutTime, timestr, false, true);

	//登出事件
	CEvent* evnt = MakeEvent(Event_Player_Logout, player->GetID(), NULL, true);
	player->OnEvent(evnt);

	//同步DataServer退出
	DataModule.syncRemove(player, GameServer.getServerSock(CBaseServer::Linker_Server_Data));

	LOGGER_NOTICE("[Logout] Online User:"INT64_FMT" Player:"INT64_FMT, player->GetFieldI64(Role_Attrib_UserID), id);

	UserMgr.Delete(player->GetFieldI64(Role_Attrib_UserID));
	PlayerMgr.Delete(id);

	return true;
}

bool CLoginModule::_OnPlayerSync(CPlayer* player)
{
	if( !player )
		return false;

	//同步客户端
	Message::PlayerLogin msgLogin;
	msgLogin.set_uid(player->GetFieldI64(Role_Attrib_UserID));
	msgLogin.set_pid(player->GetID());
	Packet packLogin;
	PROTOBUF_CMD_PACKAGE(packLogin, msgLogin, Message::MSG_PLAYER_LOGIN_REQUEST);
	player->SendClientMsg(&packLogin);
	
	player->DataSync();

	Message::PlayerLoadOver msg;
	msg.set_pid(player->GetID());
	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_PLAYER_LOAD_OVER);
	player->SendClientMsg(&pack);

	return true;
}

void CLoginModule::eventPlayerLoadover(PersonID id)
{
	int idx = m_LoginMap.Find(id);
	if( !m_LoginMap.IsValidIndex(idx) )
		return;
	m_LoginMap.RemoveAt(idx);

	CPlayer* player = PlayerMgr.GetObj(id);
	if( !player )
	{
		return;
	}

	CUser* pUser = UserMgr.GetObj( player->GetFieldI64(Role_Attrib_UserID) );
	if( !pUser )
	{
		PlayerMgr.Delete(id);
		return;
	}

	pUser->RelatePlayer( player->GetID() );
	player->SetGateSocket( pUser->m_GateSocket );
	player->SetLoadTime(timeGetTime());
	player->SetOnline(Online_Flag_Load);

	OnPlayerLogin(player);
}
