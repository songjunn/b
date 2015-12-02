#pragma once

#include "commdata.h"
#include "Singleton.h"

#define MSG_VAR_TYPE		"mType"
#define MSG_VAR_USER		"userid"
#define MSG_VAR_PLAYER		"playerid"
#define MSG_VAR_FTIME		"forcetime"
#define MSG_VAR_REASON		"reason"
#define MSG_VAR_MAILTITLE	"mailtitle"
#define MSG_VAR_MAILITEM	"item"
#define MSG_VAR_MAILEXP		"exp"
#define MSG_VAR_MAILSILVER	"silver"
#define MSG_VAR_MAILGOLD	"gold"
#define MSG_VAR_MAILCREDIT	"credit"
#define MSG_VAR_MAILAP		"ap"
#define MSG_VAR_MAILMERIT	"merit"
#define MSG_VAR_MAILKNIGHT	"knight"
#define MSG_VAR_STARTTIME	"starttime"
#define MSG_VAR_INTERVAL	"interval"
#define MSG_VAR_CASTTIMES	"broadcasttimes"
#define MSG_VAR_CONTENT		"content"

#define MSG_VAR_LEN		32

#define MSG_OPT_CIRCLE	3

enum Manage_Msg_Type
{
	Manage_Msg_Forbid = 1,
	Manage_Msg_Notice,
	Manage_Msg_Mail,
	Manage_Msg_UnForbid,
};

struct mg_connection;
class Packet;

class CManageModule : public Singleton< CManageModule >
{
public:
	CManageModule();
	~CManageModule();

	bool	OnManageRequest(struct mg_connection *conn);

private:
	bool	_HandleConn_Forbid(struct mg_connection *conn);
	bool	_HandleConn_Notice(struct mg_connection *conn);
	bool	_HandleConn_Mail(struct mg_connection *conn);
	bool	_HandleConn_UnForbid(struct mg_connection *conn);

	void	SendGameMsg(Packet *pack);
	
private:
	int64	m_LastOptTime;
};

#define ManageModule CManageModule::getSingleton()
