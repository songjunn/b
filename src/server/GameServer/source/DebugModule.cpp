#include <vector>
#include <iostream>
#include "DebugModule.h"
#include "logger.h"
#include "PlayerMgr.h"
#include "gamedef.h"
#include "MessageTypeDefine.pb.h"
#include "MessageDebug.pb.h"


#ifdef DEBUG
#define LDEBUG(fmt, ...)	\
{\
	string nfmt = "{DEBUG:%s} ";\
	nfmt = nfmt + fmt;\
	LOGGER_DEBUG(nfmt.c_str(), __FUNCTION__, ## __VA_ARGS__ );\
}
#define LERROR(fmt, ...)	\
{\
	string nfmt = "{DEBUG:%s} ";\
	nfmt = nfmt + fmt;\
	LOGGER_ERROR(nfmt.c_str(), __FUNCTION__, ## __VA_ARGS__ );\
}
#else
#define LDEBUG(fmt, ...) \
	LOGGER_DEBUG(fmt,  ## __VA_ARGS__)
#define LERROR(fmt, ...) \
	LOGGER_ERROR(fmt,  ## __VA_ARGS__)
#endif

#define _GETPLAYER(msg,i)\
	if (msg.strings_size() <= i ) return;\
	int64 pid = atoll(msg.strings(i).c_str());\
	CPlayer *player = PlayerMgr.GetObj( pid );\
	if (player == NULL ) {\
		LDEBUG("GetObj(%llu) failed", pid);\
		return ;\
	}

#define _ASSERT_MIN(msg,num) \
	if (num > msg.strings_size()) \
		return ;

#define _GETINT(msg, i) \
	atoi(msg.strings(i).c_str())

void _setpro(Message::DebugRequest msg)
{
	if (msg.strings_size() < 4) {
		return;
	}
	if (msg.has_pid()) {
		CPlayer *player = PlayerMgr.GetObj( msg.pid() );
		if (player == NULL ) {
			return ;
		}
		int index = atoi(msg.strings(1).c_str());
		int value = atoi(msg.strings(2).c_str());	
		player->ChangeFieldInt(index, value, true);
		//player->SyncFieldToData("attr");
	} else {
		_GETPLAYER(msg,1)
		int index = atoi(msg.strings(2).c_str());
		int value = atoi(msg.strings(3).c_str());	
		player->ChangeFieldInt(index, value, true);
		//player->SyncFieldToData("attr");
	}
}

void _taskLevelUp(Message::DebugRequest msg)
{

}

void _taskAdd(Message::DebugRequest msg)
{

}

void _taskaccept(Message::DebugRequest msg)
{

}

void _taskdelete(Message::DebugRequest msg)
{

}

void _gettask(Message::DebugRequest msg)
{

}

void _additem(Message::DebugRequest msg)
{

}

void _addHero(Message::DebugRequest msg)
{

}

void _addExp(Message::DebugRequest msg)
{

}

void _addAp(Message::DebugRequest msg)
{

}

void _addMerit(Message::DebugRequest msg)
{

}

void _addStateExp(Message::DebugRequest msg)
{

}

void _addTeamExp(Message::DebugRequest msg)
{

}

void _addGold(Message::DebugRequest msg)
{

}

void _addSilver(Message::DebugRequest msg)
{

}

void _activecity(Message::DebugRequest msg)
{

}

void _setLogLevel(Message::DebugRequest msg)
{

}

void _refreshArena(Message::DebugRequest &msg)
{

}

void _setarenascore(Message::DebugRequest &msg)
{

}

void _setknight(Message::DebugRequest &msg)
{

}

void _addKnightStateExp(Message::DebugRequest &msg)
{

}

void _bossstart(Message::DebugRequest &msg)
{

}
void _bossready(Message::DebugRequest &msg)
{

}
void _bossstop(Message::DebugRequest &msg)
{

}

bool CDebugModule::OnMsg(Packet* pack)
{
#ifdef _DEBUG
	if( !pack )
		return false;

	//_setMessage(false);
	switch(pack->Type())
	{
		case Message::MSG_REQUEST_DEBUG:
			_handleDebug(pack);
			break; 

		default:	
			return false;
	}

	return true;
#endif

	return false;
}

bool CDebugModule::_handleDebug(Packet *pack)
{
	Message::DebugRequest msg;     
	PROTOBUF_CMD_PARSER( pack, msg );

	if ( strcmp(msg.strings(0).c_str(), "setpro") == 0 ) {
		_setpro(msg);
	}
	else if( strcmp(msg.strings(0).c_str(), "tasklevelup") == 0 ){
		_taskLevelUp(msg);
	}
	else if ( strcmp(msg.strings(0).c_str(), "taskadd")  == 0) {
		_taskAdd(msg);
	}
	else if ( strcmp(msg.strings(0).c_str(), "taskaccept") == 0) {
		_taskaccept(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "gettask") == 0) {
		_gettask(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "taskdelete") == 0) {
		_taskdelete(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "additem") == 0) {
		_additem(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "activecity") == 0) {
		_activecity(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "setloglevel") == 0) {
		_setLogLevel(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "addexp") == 0) {
		_addExp(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "addhero") == 0) {
		_addHero(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "addap") == 0) {
		_addAp(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "addgold") == 0) {
		_addGold(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "addsilver") == 0) {
		_addSilver(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "addmerit") == 0) {
		_addMerit(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "addstateexp") == 0) {
		_addStateExp(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "addteamexp") == 0) {
		_addTeamExp(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "arenaRefresh") == 0 ) {
		_refreshArena(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "setarenascore") == 0) {
		_setarenascore(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "setknight") == 0) {
		_setknight(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "bossstart") == 0) {
		_bossstart(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "bossstop") == 0) {
		_bossstop(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "bossready") == 0) {
		_bossready(msg);
	}
	else if (strcmp(msg.strings(0).c_str(), "addknightexp") == 0) {
		_addKnightStateExp(msg);
	}
	else {
		LOGGER_DEBUG("no this debug command");
	}

	return true;
}
