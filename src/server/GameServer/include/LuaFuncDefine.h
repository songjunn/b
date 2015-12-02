#pragma once
#include "LuaEngine.h"

extern int Lua_RegistTimer(lua_State* L);
extern int Lua_GetNowTime(lua_State* L);
extern int Lua_GetDataTime(lua_State* L);
extern int Lua_GetTimeSec(lua_State* L);
extern int Lua_RandomInt(lua_State* L);	


extern char g_szExePath[512];
static int Lua_LoadLuaFile(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 1)
	{
		lua_pop(L,n);
		return 0;
	}
	const char* file = lua_tostring(L,-1);
	lua_pop(L,n);
	char script[256]= { 0 };
	sprintf(script,"%s//%s", g_szExePath, file);
	LuaEngine.LoadLuaFile(script);
	return 0;
}


const luaL_Reg user_funcs[] = 
{
	{"LoadLuaFile",				Lua_LoadLuaFile},

	{"RegistTimer",				Lua_RegistTimer},
	{"GetNowTime",				Lua_GetNowTime},
	{"GetDataTime",				Lua_GetDataTime},
	{"GetTimeSec",				Lua_GetTimeSec},
	{"RandomInt",				Lua_RandomInt},

	{ NULL, NULL},
};
