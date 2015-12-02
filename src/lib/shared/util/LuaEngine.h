#ifndef SHARED_LUAENGINE_H
#define SHARED_LUAENGINE_H

extern   "C"   
{   
#include   "lua.h"
#include   "lualib.h"
#include   "lauxlib.h"
#include   "lstate.h"
}
#include "Singleton.h"


class LuaParam
{
public:
	inline int Type()
	{
		return m_nType;
	}	
	inline void SetDataNum(lua_Number num)
	{
		m_nDataNumber = num;
		m_nType = LUA_TNUMBER;
	}
	inline void SetDataString(const char* string)
	{
		m_pData = (void*)string;
		m_nType = LUA_TSTRING;
	}
	inline void SetDataPointer(void* pointer)
	{
		m_pData = pointer;
		m_nType = LUA_TLIGHTUSERDATA;
	}
	inline void* Data()
	{
		if(m_nType == LUA_TNUMBER)
			return &m_nDataNumber;
		return (void*)m_pData;
	}
	inline lua_Number DataNumber()
	{
		return m_nDataNumber;
	}
	LuaParam(void) { m_nDataNumber = 0;m_nType = LUA_TNUMBER;}
	LuaParam(void* pointer){ m_pData = pointer; m_nType = LUA_TLIGHTUSERDATA; }
	virtual ~LuaParam(void){};

private:
	int				m_nType;
	void*			m_pData;
	lua_Number		m_nDataNumber;

};

class CLuaEngine : public Singleton< CLuaEngine >
{
public:
	CLuaEngine();
	virtual ~CLuaEngine();

public:
	bool			Init(int stacksize = 51200);
	bool			LoadLuaFile(const char* szName);

	void			RegUserFunc(const char *classname, const luaL_Reg *l);
	void			RegUserFunc(const luaL_Reg* l);
	void			RegGlobalFunc(const char* name, lua_CFunction func);

	bool			RunMemoryLua(const char* pLuaData, int nDataLen);
	bool			RunLuaFunction(const char* szFunName, const char* szTableName = NULL,  const char* szSubTableName = NULL,LuaParam * pInParam = NULL, int nInNum = 0, LuaParam * pOutParam = NULL, int nOutNum = 0);
	
	lua_Number		GetLuaVariableNumber(const char* szVariableName, const char* szTableName = NULL);
	const char*		GetLuaVariableString(const char* szVariableName, const char* szTableName = NULL);

private:
	const char*		GetLastError();

private:
	lua_State *		m_pLuaState;

};

#define LuaEngine CLuaEngine::getSingleton()

#endif	//SHARED_LUAENGINE_H
