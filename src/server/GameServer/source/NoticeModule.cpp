#include "NoticeModule.h"
#include "logger.h"
#include "PlayerMgr.h"
#include "GameServer.h"
#include "Packet.h"
#include "MessageTypeDefine.pb.h"
#include "MessageCommon.pb.h"


CNoticeModule::CNoticeModule()
{
	m_key = 0;
}

CNoticeModule::~CNoticeModule()
{
}

void CNoticeModule::OnLogic()
{
	int nid = m_list.Head();
	while( CNotice* notice = GetObj(nid) )
	{
		int temId = nid;
		nid = m_list.Next(temId);

		PersonID pid = PlayerMgr.m_list.Head();
		while( CPlayer* player = PlayerMgr.GetObj(pid) )
		{
			pid = PlayerMgr.m_list.Next(pid);

			SendBroadcast(player, notice);
		}

		Delete(temId);
	}
}

void CNoticeModule::SendBroadcast(int errid, int int1, int int2, int int3, int64 long1, int64 long2, int64 long3, const char* str1, const char* str2, const char* str3)
{
	CNotice* notice = Create(m_key++);
	if( !notice )
		return;

	notice->m_errid = errid;
	notice->m_int[0] = int1;
	notice->m_int[1] = int2;
	notice->m_int[2] = int3;
	notice->m_int64[0] = long1;
	notice->m_int64[1] = long2;
	notice->m_int64[2] = long3;
	if(str1)
		notice->m_string[0] = str1;
	if(str2)
		notice->m_string[1] = str2;
	if(str3)
		notice->m_string[2] = str3;
}

void CNoticeModule::SendBroadcast(CPlayer* player, CNotice* notice)
{
	if(!player || !notice)
		return;

	Message::ErrorNo msg;
	msg.set_error(notice->m_errid);
	msg.set_type(Notice_Broadcast);
	msg.set_paramint1(notice->m_int[0]);
	msg.set_paramint2(notice->m_int[1]);
	msg.set_paramint3(notice->m_int[2]);
	msg.set_paramlong1(notice->m_int64[0]);
	msg.set_paramlong2(notice->m_int64[1]);
	msg.set_paramlong3(notice->m_int64[2]);
	msg.set_paramstr1(notice->m_string[0]);
	msg.set_paramstr2(notice->m_string[1]);
	msg.set_paramstr3(notice->m_string[2]);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_COMMON_ERROR);
	player->SendClientMsg(&pack);
}

void CNoticeModule::SendErrorMsg(CFighter* role, int errid, int int1, int int2, int int3, int64 long1, int64 long2, int64 long3, const char* str1, const char* str2, const char* str3)
{
	Message::ErrorNo msg;
	msg.set_error(errid);
	msg.set_type(Notice_Error);
	msg.set_paramint1(int1);
	msg.set_paramint2(int2);
	msg.set_paramint3(int3);
	msg.set_paramlong1(long1);
	msg.set_paramlong2(long2);
	msg.set_paramlong3(long3);
	if(str1) 
		msg.set_paramstr1(str1);
	if(str2)
		msg.set_paramstr2(str2);
	if(str3)
		msg.set_paramstr3(str3);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_COMMON_ERROR);
	role->SendClientMsg(&pack);
}

void CNoticeModule::SendErrorMsg(int sock, UserID uid, int errid, int int1, int int2, int int3, int64 long1, int64 long2, int64 long3, const char* str1, const char* str2, const char* str3)
{
	Message::ErrorNo msg;
	msg.set_error(errid);
	msg.set_userid(uid);
	msg.set_type(Notice_Error);
	msg.set_paramint1(int1);
	msg.set_paramint2(int2);
	msg.set_paramint3(int3);
	msg.set_paramlong1(long1);
	msg.set_paramlong2(long2);
	msg.set_paramlong3(long3);
	if(str1)
		msg.set_paramstr1(str1);
	if(str2)
		msg.set_paramstr2(str2);
	if(str3)
		msg.set_paramstr3(str3);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_COMMON_ERROR);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}
