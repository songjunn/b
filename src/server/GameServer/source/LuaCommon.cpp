#include "LuaFuncDefine.h"
#include "TimerModule.h"
#include "random.h"


int Lua_RegistTimer(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 9)
	{
		lua_pop(L,n);
		return 0;
	}

	int year = lua_tointeger(L,-9);
	int month = lua_tointeger(L,-8);
	int mday = lua_tointeger(L,-7);
	int wday = lua_tointeger(L,-6);
	int hour = lua_tointeger(L,-5);
	int minute = lua_tointeger(L,-4);
	int sec = lua_tointeger(L,-3);
	const char* func = lua_tostring(L, -2);
	const char* table = lua_tostring(L, -1);
	lua_pop(L,n);

	TimerModule.RegistTimerFunc(year, month, mday, wday, hour, minute, sec, func, table);

	return 0;
}

extern int Lua_GetNowTime(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 0)
	{
		lua_pop(L,n);
		return 0;
	}
	lua_pop(L,n);

	GTime now;
	GetLocalTime(now);

	lua_pushinteger(L, now.gtm_sec);
	lua_pushinteger(L, now.gtm_min);
	lua_pushinteger(L, now.gtm_hour);
	lua_pushinteger(L, now.gtm_mday);
	lua_pushinteger(L, now.gtm_mon);
	lua_pushinteger(L, now.gtm_year);
	lua_pushinteger(L, now.gtm_wday);

	return 7;
}

extern int Lua_GetDataTime(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 1)
	{
		lua_pop(L,n);
		return 0;
	}

	int64 t = lua_tonumber(L,-1);
	lua_pop(L,n);

	GTime data;
	CoverSecToGtime(t, data);

	lua_pushinteger(L, data.gtm_sec);
	lua_pushinteger(L, data.gtm_min);
	lua_pushinteger(L, data.gtm_hour);
	lua_pushinteger(L, data.gtm_mday);
	lua_pushinteger(L, data.gtm_mon);
	lua_pushinteger(L, data.gtm_year);
	lua_pushinteger(L, data.gtm_wday);

	return 7;
}

int Lua_GetTimeSec(lua_State* L)
{
	int n = lua_gettop(L);
	if (n != 0)
	{
		lua_pop(L, n);
		return 0;
	}
	lua_pop(L, n);

	int64 timeSec = GetTimeSec();
	lua_pushnumber(L, timeSec);

	return 1;
}

int Lua_RandomInt(lua_State* L)
{
	int n = lua_gettop(L);
	if(n != 2)
	{
		lua_pop(L,n);
		return 0;
	}

	int iMin = lua_tointeger(L, -2);
	int iMax = lua_tointeger(L, -1);
	lua_pop(L, n);

	int randNum = RandomInt(iMin, iMax);
	lua_pushinteger(L, randNum);

	return 1;
}
