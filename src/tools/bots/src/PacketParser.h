#pragma once
#include "RobotUnit.h"
#include "RobotObj.h"
#include "Packet.h"
#include "MessageTypeDefine.pb.h"
#include "MessageCommon.pb.h"
#include "MessageUser.pb.h"
#include "MessagePlayer.pb.h"
#include "MessageDebug.pb.h"


void PacketParser( Packet* pack )
{
	if( !pack )
		return;

	printf("recv pack %d\n", pack->Type());

	CMessageHandler* handler = TestUnitMgr.GetMessageHandler( pack->Type() );
    if( !handler )
		return;

	int index = 0;
	LuaParam pParam[50];	

	switch( pack->Type() )
	{
	case Message::MSG_USER_GET_GATESVR:
		{
			Message::ConnectGate msg;
			PROTOBUF_CMD_PARSER( pack, msg );

			pParam[index++].SetDataNum( pack->GetNetID() );
			pParam[index++].SetDataNum( msg.uid() );
			pParam[index++].SetDataNum( msg.key() );
			pParam[index++].SetDataNum( msg.server() );
			pParam[index++].SetDataString( msg.ip().c_str() );
			pParam[index++].SetDataNum( msg.port() );

			printf("user:%I64d\n", msg.uid());

			LuaEngine.RunLuaFunction(handler->m_szFunc, handler->m_szUnit, NULL, pParam, index);
		}
		break;
	case Message::MSG_COMMON_ERROR:
		{
			Message::ErrorNo msg;
			PROTOBUF_CMD_PARSER( pack, msg );

			printf("Error:%d\n", msg.error());
		} 
		break;
	case Message::MSG_PLAYER_LOAD_COUNT:
		{
			Message::PlayerCount msg;
			PROTOBUF_CMD_PARSER( pack, msg );

			pParam[index++].SetDataNum( pack->GetNetID() );
			pParam[index++].SetDataNum( msg.player_size() );

			LuaEngine.RunLuaFunction(handler->m_szFunc, handler->m_szUnit, NULL, pParam, index);
		}
		break;
	case Message::MSG_PLAYER_LOAD_DATA:
		{
			/*Message::PlayerAttrib msg;
			PROTOBUF_CMD_PARSER( pack, msg );

			pParam[index++].SetDataNum( pack->GetNetID() );
			pParam[index++].SetDataNum( msg.pid() );

			printf("Message %d size %u: {%s}", pack->Type(), pack->Size(), msg.Utf8DebugString().c_str());
			//LuaEngine.RunLuaFunction(handler->m_szFunc, handler->m_szUnit, NULL, pParam, index);*/
		}
		break;
	case Message::MSG_PLAYER_SYNC_ATTRINT:
		{
			/*Message::PlayerAttribInt msg;
			PROTOBUF_CMD_PARSER( pack, msg );

			printf("SetAttrInt player:%I64d attr:%d value:%d\n", msg.pid(), msg.attr(), msg.value());*/
		}
		break;
	case Message::MSG_PLAYER_SYNC_ATTRI64:
		{
			/*Message::PlayerAttribI64 msg;
			PROTOBUF_CMD_PARSER( pack, msg );

			printf("SetAttrI64 player:%I64d attr:%d value:%I64d\n", msg.pid(), msg.attr(), msg.value());*/
		}
		break;
	case Message::MSG_PLAYER_LOAD_OVER:
		{
			Message::PlayerLoadOver msg;
			PROTOBUF_CMD_PARSER( pack, msg );

			CRobotObj* robot = RobotMgr.GetRobot( pack->GetNetID() );
			if (robot != NULL)
			{
				robot->m_PlayerID = msg.pid();
				robot->setStat(ESTAT_LOGINGATE);
				//robot->AddExp(1000);
				//robot->AddItem();
				//robot->AddHero();
				//robot->SendChat();
				//robot->RequestObserver();
				//robot->RequestJoinBattle();
				//robot->RequestAttackBoss();
				//robot->RequestHelper();
			}

			pParam[index++].SetDataNum( pack->GetNetID() );
			pParam[index++].SetDataNum( msg.pid() );

			LuaEngine.RunLuaFunction(handler->m_szFunc, handler->m_szUnit, NULL, pParam, index);
		}
		break;
	case Message::MSG_REQUEST_NET_TEST:
		{
			Message::NetTestRequest msg;
			PROTOBUF_CMD_PARSER(pack, msg);

			pParam[index++].SetDataNum( pack->GetNetID() );
			pParam[index++].SetDataNum( msg.no() );
			pParam[index++].SetDataString( msg.sdata().c_str() );

			LuaEngine.RunLuaFunction(handler->m_szFunc, handler->m_szUnit, NULL, pParam, index);
		}
		break;
	default:
		break;
	}

	return;
}

