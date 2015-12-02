#include "LuaFunc.h"
#include "LuaEngine.h"
#include "RobotUnit.h"
#include "RobotObj.h"
#include "RobotMgr.h"


static int Lua_LoadLuaFile(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 1)
	{
		lua_pop(L,n);
		return 0;
	}

	const char* szFileName = lua_tostring(L,-1);

	LuaEngine.LoadLuaFile(szFileName);

	lua_pop(L,n);
	return 0;
}

static int Lua_RegTestUnit(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 1)
	{
		lua_pop(L,n);
		return 0;
	}

	const char* name = lua_tostring(L,-1);

	TestUnitMgr.RegTestUnit(name);

	lua_pop(L,n);
	return 0;
}

static int Lua_RegMessage(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 3)
	{
		lua_pop(L,n);
		return 0;
	}

	int message = lua_tonumber(L,-3);
	const char* unit = lua_tostring(L,-2);
	const char* func = lua_tostring(L,-1);

	TestUnitMgr.RegMessage(message, unit, func);

	lua_pop(L,n);
	return 0;
}

static int Lua_UserLogout(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 1)
	{
		lua_pop(L,n);
		return 0;
	}

	int socket = lua_tonumber(L,-1);

	CRobotObj* robot = RobotMgr.GetRobot( socket );
	if( robot )
	{
		robot->Logout();
	}

	lua_pop(L,n);
	return 0;
}

static int Lua_ConnectGate(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 6)
	{
		lua_pop(L,n);
		return 0;
	}

	int socket = lua_tonumber(L,-6);
	int64 uid = lua_tonumber(L,-5);
	int64 key = lua_tonumber(L,-4);
	int server = lua_tonumber(L,-3);
	const char* ip = lua_tostring(L,-2);
	int port = lua_tonumber(L,-1);

	CRobotObj* robot = RobotMgr.GetRobot( socket );
	if( robot )
	{
		robot->SetUid( uid );
		robot->SetKey( key );
		robot->Logon( ip, port );
	}

	lua_pop(L,n);
	return 0;
}

static int Lua_CreatePlayer(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 6)
	{
		lua_pop(L,n);
		return 0;
	}

	int socket = lua_tonumber(L,-6);
	int server = lua_tonumber(L,-5);
	const char* name = lua_tostring(L,-4);
	int vocation = lua_tonumber(L,-3);
	int sex = lua_tonumber(L,-2);
	int camp = lua_tonumber(L,-1);

	CRobotObj* robot = RobotMgr.GetRobot( socket );
	if( robot )
	{
		robot->CreatePlayer( robot->m_uid, server, name, vocation, sex, camp );
	}

	lua_pop(L,n);
	return 0;
}

static int Lua_AttackMonster(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 4)
	{
		lua_pop(L,n);
		return 0;
	}

	int socket = lua_tonumber(L,-4);
	int64 pid = lua_tonumber(L,-3);
	int scene = lua_tonumber(L,-2);
	int monster = lua_tonumber(L,-1);

	CRobotObj* robot = RobotMgr.GetRobot( socket );
	if( robot )
	{
		robot->AttackMonster( pid, scene, monster );
	}

	lua_pop(L,n);
	return 0;
}
static int Lua_NetTest(lua_State* L)
{
	int n = lua_gettop(L);
	if (n != 3)
	{
		lua_pop(L, n);
	}
	int socket = lua_tonumber(L, -3);
	int no = lua_tonumber(L, -2);
	const char * sdata = lua_tostring(L, -1);

	CRobotObj* robot = RobotMgr.GetRobot( socket );
	if (robot)
	{
		robot->OnNetTest(no, sdata);
	} else {
		printf("no robot (%d)\n", socket);
	}

	return 0;
}

static int Lua_DoNetTest(lua_State* L)
{
	int n = lua_gettop(L);
	if (n != 1)
	{
		lua_pop(L, n);
	}
	int socket = lua_tonumber(L, -1);

	CRobotObj* robot = RobotMgr.GetRobot( socket );
	if (robot)
	{
		robot->DoNetTest();
	} else {
		printf("no robot (%d)\n", socket);
	}

	return 0;
}

static int Lua_DoLoginTest(lua_State* L)
{
	int n = lua_gettop(L);

	if (n != 1)
	{
		lua_pop(L, n);
	}

	int socket = lua_tonumber(L, -1);

	CRobotObj* robot = RobotMgr.GetRobot( socket );
	if (robot)
	{
		robot->DoLoginTest();
	} else {
		printf("no robot (%d)\n", socket);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
const luaL_Reg user_funcs[] = 
{
	{"LoadLuaFile",						Lua_LoadLuaFile},
	{"RegTestUnit",						Lua_RegTestUnit},
	{"RegMessage",						Lua_RegMessage},
	//{"UserLogin",						Lua_UserLogin},
	{"UserLogout",						Lua_UserLogout},
	{"ConnectGate",						Lua_ConnectGate},
	{"CreatePlayer",					Lua_CreatePlayer},
	{"AttackMonster",					Lua_AttackMonster},
	{"NetTest",							Lua_NetTest},
	{"DoNetTest",						Lua_DoNetTest},
	{"DoLoginTest",						Lua_DoLoginTest},

	{ NULL, NULL},
};

