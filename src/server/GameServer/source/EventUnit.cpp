#include "EventUnit.h"
#include "PlayerMgr.h"
#include "LoginModule.h"
#include "Packet.h"
#include "MessageTypeDefine.pb.h"
#include "MessageCommon.pb.h"


CEventUnit::CEventUnit()
{

}

CEventUnit::~CEventUnit()
{

}

void CEventUnit::Initialize(CPlayer* parent)
{
	m_parent = parent;
}

void CEventUnit::Release()
{
	m_EventList.RemoveAll();
}

bool CEventUnit::OnLogic()
{
	int tempIdx;
	for(int idx = m_EventList.Head(); m_EventList.IsValidIndex(tempIdx = idx); idx = m_EventList.Next(idx))
	{
		OnEvent( m_EventList[tempIdx] );

		m_EventList.Remove(tempIdx);
	}

	return true;
}

void CEventUnit::AddEvent(CEvent* ev)
{
	m_EventList.AddToTail(ev);
}

void CEventUnit::OnEvent(CEvent* ev)
{
	if( !ev )
		return;

	switch(ev->m_nEventID)
	{
	case Event_Player_Loadover:	//param0: playerid
		{
			LoginModule.eventPlayerLoadover(ev->m_nPID);
			PlayerMgr.eventPlayerLoadover(ev->m_nPID);
			break;
		}
	case Event_Player_LoadModule:	//param0: playerid param1: module
		{
			PlayerMgr.eventPlayerLoadObserve(ev->m_nPID, ev->m_nParam[0], (int)ev->m_nParam[1]);
			break;
		}
	case Event_Item_Obtain:	//param0: user id; param2:item template;	param3:num
		{
			break;
		}
	case Event_Combat_KillMonster:	//param0:monster template
		{
			break;
		}
	case Event_City_Finish:	//param0:scene param1:record param2:mode
		{
			break;
		}
	case Event_Item_Delete:		// param0: template; param1:(-1)*num
		{
			break;
		}
	case Event_Item_Intensify:
		{
			break;
		}
	case Event_Item_UpGrade:
		{
			break;
		}
	case Event_Item_MatIntensify:
		{
			break;
		}
	case Event_Item_RecastCrystal:
		{
			break;
		}
	case Event_Hero_UpQuality:
		{
			break;
		}
	case Event_Format_Uplevel:
		{
			break;
		}
	case Event_Player_Levelup:		//param0:level
		{
			break;
		}
	case Event_Player_FightingChange:
		{
			break;
		}
	case Event_City_Active:		// param0: activity city id
		{
			break;
		}
	case Event_Player_Login:		// 
		{
			break;
		}
	case Event_Player_Logout:
		{
			break;
		}
	case Event_Hero_LeaveTeam:		// param0: hero id
		{
			break;
		}
	case Event_Order_Change_Field:
		{
			break;
		}
	case Event_Order_Join:
		{
			break;
		}
	case Event_Boss_Attack:
		{
			break;
		}
	case Event_Battle_Fight:
		{
			break;
		}
	case Event_Skill_Reset:
		{
			break;
		}
	case Event_Train_Do:
		{
			break;
		}
	case Event_Arena_Join:
		{
			break;
		}
	case Event_Worship_Do:
		{
			break;
		}

	case Event_SignIn_Do:
		{
			break;
		}
	case Event_Stars_Inlay:
		{
			break;
		}
	case Event_Item_Inlay:
		{
			break;
		}
	case Event_Daily_RandHero:
		{
			break;
		}
	case Event_Daily_WipeOut:
		{
			break;
		}
	case Event_Daily_FinishElite:
		{
			break;
		}
	case Event_Daily_Expedition:
		{
			break;
		}
	case Event_Daily_QueenTease:
		{
			break;
		}
	case Event_Branch_ZoneBox:
		{
			break;
		}
	case Event_Hero_Recruit:
		{
			break;
		}
	case Event_Branch_QualityItem:
		{
			break;
		}
	default:
		break;
	}

	if( ev->m_bUpdateDB )
	{
		Message::EventSync msg;
		msg.set_evid(ev->m_nEventID);
		msg.set_pid(ev->m_nPID);
		msg.set_param1(ev->m_nParam[0]);
		msg.set_param2(ev->m_nParam[1]);
		msg.set_param3(ev->m_nParam[2]);
		msg.set_param4(ev->m_nParam[3]);
		msg.set_param5(ev->m_nParam[4]);
		msg.set_param6(ev->m_nParam[5]);
		msg.set_param7(ev->m_szParam);
		msg.set_stime(ev->m_szTime);
		msg.set_parent(m_parent->GetID());

		Packet pack;
		PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_COMMON_EVENT);
		GETSERVERNET(&GameServer)->sendMsg(GameServer.getServerSock(CBaseServer::Linker_Server_DataAnalysis), &pack);
	}
}
