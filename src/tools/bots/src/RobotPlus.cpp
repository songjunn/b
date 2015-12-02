// RobotPlus.cpp : 定义控制台应用程序的入口点。
//

#include "LuaEngine.h"
#include "memorypool.h"
#include "RobotNet.h"
#include "RobotUnit.h"
#include "RobotObj.h"
#include "RobotMgr.h"
#include "PacketParser.h"


createFileSingleton(CLog);
createFileSingleton(CLuaEngine);
createFileSingleton(CRobotNet);
createFileSingleton(CRobotMgr);
createFileSingleton(CTestUnitMgr);

char g_szServerVersion[32];
char g_szServerIp[32];
int g_nServerPort;

int main()
{
	//初始化lua引擎
	if( !LuaEngine.Init() )
	{
		printf("初始化lua引擎失败！\n");
		return FALSE;
	}

	//加载配置参数
	if( !LuaEngine.LoadLuaFile("config.lua") )
	{
		printf("加载配置参数失败\n");
		return FALSE;
	}
	int world = LuaEngine.GetLuaVariableNumber( "world", "robot" );
	const char* szver  = LuaEngine.GetLuaVariableString( "version", "robot" );
	sprintf(g_szServerVersion, "%s", szver);
	const char* szip = LuaEngine.GetLuaVariableString( "serverip", "robot" );
	sprintf(g_szServerIp, "%s", szip);
	g_nServerPort = LuaEngine.GetLuaVariableNumber( "serverport", "robot" );
	int nRobotNum = LuaEngine.GetLuaVariableNumber( "num", "robot" );
	const char* szNameSt  = LuaEngine.GetLuaVariableString( "nameset", "robot" );
	int nameindex = LuaEngine.GetLuaVariableNumber( "nameindex", "robot" );
	const char* szPwd  = LuaEngine.GetLuaVariableString( "password", "robot" );
	int actionTime = LuaEngine.GetLuaVariableNumber( "actiontime", "robot" );
	int actionType = LuaEngine.GetLuaVariableNumber( "actiontype", "robot");

	if (!RobotNet.startup(CNet::NET_IO_SELECT, 20001, nRobotNum * 2 + 1, 10240, 10240, 10000))
	{
		printf("启动网络失败！\n");
		return FALSE;
	}

	//Log.Startup("Robot", LOG_LEVEL_DEBUG);

	//加载ai脚本
	if( !TestUnitMgr.LoadLuaEngine() )
	{
		return FALSE;
	}

	int index = 0;
	while(1)
	{
		if( index < nRobotNum )
		{
			char name[32] = {0};
			sprintf(name, "%s%d", szNameSt, nameindex + index);

			//创建机器人
			CRobotObj* robot = new CRobotObj(++index, name, szPwd, actionTime, world);
			if( !robot )
				return FALSE;

			RobotMgr.m_RobotList.AddToTail( robot );

			RobotNet.sendHttpRequest(robot);
			//robot->Check(g_szServerIp, g_nServerPort, g_szServerVersion);

			sleep(10);
		}

		FOR_EACH_LL(RobotMgr.m_RobotList, index)
		{
			CRobotObj *robot = RobotMgr.m_RobotList[index];
			robot->Heart();
			//robot->DoActiveBehavior(actionType);
			//robot->DoNonScriptBehavior(); // 非脚本操作 wenc 
		}

		//while (RobotMgr.m_RobotList.

		Packet* pack = NULL;
		while( pack = RobotNet.getHeadPacket() )
		{
			PacketParser( pack );

			SAFE_DELETE( pack );
		}

		sleep(1);
	}

	return 0;
}

