#pragma once
#include "shared.h"
#include "utllinkedlist.h"
#include "utlmap.h"
#include "LuaEngine.h"


extern const luaL_Reg user_funcs[];

class CTestUnit
{
public:
	CTestUnit(const char* name)
	{
		strcpy(m_szName, name);
	}
	~CTestUnit();

	bool	Logic(int s);
	bool	Event(int evid, int s);

public:
	char	m_szName[32];
	CUtlMap<int, const char*>	m_MessageList;

};

class CMessageHandler
{
public:
	CMessageHandler(int type, const char* unit, const char* func)
	{
		m_type = type;
		strncpy(m_szUnit, unit, 32);
		strncpy(m_szFunc, func, 32);
	}
	~CMessageHandler();

public:
	int		m_type;
	char	m_szUnit[32];
	char	m_szFunc[32];

};

class CTestUnitMgr : public Singleton< CTestUnitMgr >
{
public:
	CTestUnitMgr()	{ SetDefLessFunc(m_MessageList); }
	~CTestUnitMgr()	{};

	bool	LoadLuaEngine();
	bool	RegTestUnit(const char* szTestUnitName);

	bool	RegMessage(int type, const char* unit, const char* func);
	CMessageHandler* GetMessageHandler(int type);

	bool	HandleLogic(int s);
	bool	HandleEvent(int evid, int s);

protected:
	CUtlLinkedList<CTestUnit*>		m_TestUnitList;
	CUtlMap<int, CMessageHandler*>	m_MessageList;

};

#define TestUnitMgr CTestUnitMgr::getSingleton()
