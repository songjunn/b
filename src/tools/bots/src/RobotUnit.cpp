#include "RobotUnit.h"


//////////////////////////////////////////////////////////////////////////////////
//
bool CTestUnit::Logic(int s)
{
	LuaParam pParam[1];	
	pParam[0].SetDataNum(s);

	if(!LuaEngine.RunLuaFunction("Logic", m_szName, NULL, pParam, 1))
	{
		return false;
	}
	return true;
}

bool CTestUnit::Event(int evid, int s)
{
	LuaParam pParam[2];	
	pParam[0].SetDataNum(evid);
	pParam[1].SetDataNum(s);

	if(!LuaEngine.RunLuaFunction("Event", m_szName, NULL, pParam, 2))
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
//
bool CTestUnitMgr::LoadLuaEngine()
{
	LuaEngine.RegUserFunc(user_funcs);

	if( !LuaEngine.LoadLuaFile("load.lua") )
	{
		return false;
	}

	return true;
}

bool CTestUnitMgr::RegTestUnit(const char* szTestUnitName)
{
	CTestUnit* pTest = new CTestUnit(szTestUnitName);
	if(!pTest)
		return false;

	m_TestUnitList.AddToTail(pTest);
	return true;
}

bool CTestUnitMgr::RegMessage(int type, const char* unit, const char* func)
{
	int i = m_MessageList.Find( type );
	if( !m_MessageList.IsValidIndex(i) )
	{
		CMessageHandler* handler = new CMessageHandler(type, unit, func);
		m_MessageList.Insert( type, handler );
	}

	return true;
}

CMessageHandler* CTestUnitMgr::GetMessageHandler(int type)
{
	int index = m_MessageList.Find(type);
	if( m_MessageList.IsValidIndex(index) )
		return m_MessageList[index];
	return NULL;
}

bool CTestUnitMgr::HandleLogic(int s)
{
	for(int index=m_TestUnitList.Head(); m_TestUnitList.IsValidIndex(index); index=m_TestUnitList.Next(index))
	{
		CTestUnit* pTest = m_TestUnitList.Element(index);
		if(!pTest)
			continue;
		pTest->Logic(s);
	}
	return true;
}

bool CTestUnitMgr::HandleEvent(int evid, int s)
{
	for(int index=m_TestUnitList.Head(); m_TestUnitList.IsValidIndex(index); index=m_TestUnitList.Next(index))
	{
		CTestUnit* pTest = m_TestUnitList.Element(index);
		if(!pTest)
			continue;
		pTest->Event(evid, s);
	}
	return true;
}
