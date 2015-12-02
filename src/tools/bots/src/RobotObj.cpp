#include "RobotObj.h"
#include "RobotNet.h"
#include "RobotMgr.h"
#include "gtime.h"
#include "Packet.h"
#include "LuaEngine.h"
#include "MessageTypeDefine.pb.h"
#include "MessageUser.pb.h"
#include "MessagePlayer.pb.h"
#include "MessageDebug.pb.h"


extern  char g_szServerVersion[32];
extern  char	g_szServerIp[32];
extern  int	g_nServerPort;

const char *test_data[] = {
	"1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111", 
	"22222222222", 
	"333333333333333333333", 
	"4444444444444444444444444444444"};

CRobotObj::CRobotObj(int index, const char* name, const char* pwd, int actionTime, int world)
{
	m_index = index;
	strcpy(m_szName, name);
	strcpy(m_szPwd, pwd);

	m_sock = INVALID_SOCKET;
	m_SendNO = 0;
	m_RecvNO = 0;
	m_LastTime = 0;
	m_Stat = ESTAT_NONE;
	m_ActionTime = actionTime;
	m_server = world;
	m_HeartTime = 0;
	m_PlayerID = 0;

	m_OperateCount = 0;
	m_WorshipType = 0;
	m_SignInType = 0;
}

CRobotObj::~CRobotObj()
{
}

bool CRobotObj::Check(const char* szip, int port, const char* ver)
{
	m_sock = RobotNet.connect(szip, port);
	if( m_sock != INVALID_SOCKET )
	{
		//for(int i=0; i<10; ++i)
		//{
		//	Message::GuestCheck msg;
		//	//msg.set_device(m_szName);
		//	msg.set_server(m_server);

		//	Packet pack;
		//	PROTOBUF_CMD_PACKAGE(pack, msg, 60000);
		//	RobotNet.SendMsg(m_uid, m_sock, &pack);

		//	Sleep(1000);
		//}

		printf("%s连接LoginServer成功\n", m_szName);

		RobotMgr.m_LogonMap.Insert(m_sock, this);

		Message::GuestCheck msg;
		msg.set_device(m_szName);
		msg.set_server(m_server);

		Packet pack;
		PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_REQUEST_GUEST_CHECK);
		RobotNet.sendMsg(m_uid, m_sock, &pack);

		return true;
	}
	return false;
}

bool CRobotObj::Logon(const char* szip, int port)
{
	RobotMgr.RemoveRobot( m_sock );

	m_sock = RobotNet.connect(szip, port);
	if( m_sock != INVALID_SOCKET )
	{
		printf("%s连接GateServer成功\n", m_szName);

		RobotMgr.m_LogonMap.Insert(m_sock, this);

		Message::ClientLogin msg;
		msg.set_uid(m_uid);
		msg.set_key(m_key);
		msg.set_server(m_server);

		Packet pack;
		PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_REQUEST_USER_LOGIN);
		RobotNet.sendMsg(m_uid, m_sock, &pack);

		return true;
	}
	return false;
}

void CRobotObj::Logout()
{
	/*Message::UserLogout msg;
	msg.set_uid( m_uid );

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::P2A_REQUEST_USER_LOGOUT);
	RobotNet.SendMsg(m_uid, m_sock, &pack);*/
}

void CRobotObj::Heart()
{
	if( m_PlayerID <= 0 )
		return;

	uint64 t = GetTimeSec();
	if( t - m_HeartTime >= 30 )
	{
		Message::UserHeartRequest msg;
		msg.set_pid( m_PlayerID );

		Packet pack;
		PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_REQUEST_USER_HEART);
		RobotNet.sendMsg(m_uid, m_sock, &pack);

		m_HeartTime = t;
	}
}

void CRobotObj::CreatePlayer(int64 uid, int server, const char* name, int vocation, int sex, int camp)
{
	//wchar_t* wname = new wchar_t[32];
	//int nCount = strlen( name ) + 1;
	//MultiByteToWideChar(CP_ACP, 0, name, nCount, wname, nCount );

	Message::CreatePlayer msg;
	msg.set_uid(uid);
	msg.set_server(server);
	msg.set_name(name);
	msg.set_roletemplate(vocation);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_REQUEST_PLAYER_CREATE);
	RobotNet.sendMsg(m_uid, m_sock, &pack);
}

void CRobotObj::AttackMonster(int64 pid, int scene, int monster)
{
}

void CRobotObj::ActiveCity(int64 pid, int scene)
{

}

void CRobotObj::AddExp(int exp)
{
	/*char id[1024] = {0};
	_i64toa(m_PlayerID, id, 10);

	Message::DebugRequest msg;
	msg.add_strings("addexp");
	msg.add_strings(id);
	msg.add_strings("100");

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_REQUEST_DEBUG);
	RobotNet.sendMsg(m_uid, m_sock, &pack);*/
}

void CRobotObj::AddItem()
{
	/*char id[1024] = {0};
	_i64toa(m_PlayerID, id, 10);

	Message::DebugRequest msg;
	msg.add_strings("additem");
	msg.add_strings(id);
	msg.add_strings("10101001");
	msg.add_strings("1");

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_REQUEST_DEBUG);
	RobotNet.sendMsg(m_uid, m_sock, &pack);*/
}

void CRobotObj::SendChat()
{

}

void CRobotObj::RequestObserver()
{
	Message::ObservePlayerRequest msg;
	msg.set_playerid(970200000020000000);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_REQUEST_PLAYER_OBSERVE);
	RobotNet.sendMsg(m_uid, m_sock, &pack);
}

void CRobotObj::RequestJoinBattle()
{

}

void CRobotObj::RequestAttackBoss()
{
}

void CRobotObj::RequestHelper()
{
}

void CRobotObj::ActiveStar()
{

}

void CRobotObj::UplevelStar()
{
	
}

void CRobotObj::AddHero()
{
	/*char id[1024] = {0};
	_i64toa(m_PlayerID, id, 10);

	Message::DebugRequest msg;
	msg.add_strings("addhero");
	msg.add_strings(id);
	msg.add_strings("10");
	msg.add_strings("0");

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_REQUEST_DEBUG);
	RobotNet.sendMsg(m_uid, m_sock, &pack);*/
}

void CRobotObj::DoNetTest()
{
	Message::NetTestRequest msg;

	msg.set_no(m_SendNO);
	for (int i = 0; i < 0; ++i)
	{
		msg.add_idata(m_SendNO + i);
	}
	msg.set_sdata(test_data[m_SendNO%4]);
	//msg.set_sdata(test_data[1]);
	++m_SendNO;

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_REQUEST_NET_TEST);
	RobotNet.sendMsg(m_uid, m_sock, &pack); 
	//printf("send message [%d:%s]\n", P2G_REQUEST_NET_TEST, msg.ShortDebugString().c_str());
}

void CRobotObj::OnNetTest(int RecvNO, const char* sdata)
{
	if (m_RecvNO != RecvNO || strcmp(sdata, test_data[m_RecvNO%4]) != 0)
	{
		fprintf(stderr, "receive data is error [%d, %s] [%d, %s]\n", m_RecvNO,test_data[m_RecvNO%4],RecvNO, sdata);
		Log.Error("receive data is error [%d, %s] [%d, %s]\n", m_RecvNO,test_data[m_RecvNO%4],RecvNO, sdata);
		return;
	}
	++m_RecvNO;
}

void CRobotObj::DoLoginTest()
{
	RobotNet.shutdown(m_sock);
	this->Check(g_szServerIp, g_nServerPort, g_szServerVersion);
	//Log.Notice("DoLoginTest");
}

bool CRobotObj::DoActiveBehavior(int type)
{
	if (getStat() != ESTAT_LOGINGATE)
	{
		return false;
	}
	if ( timeGetTime() - m_LastTime > 20 )
	{
		m_LastTime = timeGetTime();

		LuaParam pParam[1];	
		pParam[0].SetDataNum(this->m_sock);
		LuaEngine.RunLuaFunction("DoActiveBehavior", "ActiveUnit", NULL, pParam, 1);
	}

	return true;
}

void CRobotObj::DoWorship()
{

}

void CRobotObj::DoSignIn()
{

}

void CRobotObj::ExpandBag()
{

}

void CRobotObj::DoNonScriptBehavior()
{
}
