#pragma once
#include "Singleton.h"
#include "utlmap.h"
#include "utllinkedlist.h"


class CRobotMgr : public Singleton< CRobotMgr >
{
public:
	CRobotMgr()		{SetDefLessFunc(m_LogonMap);}
	~CRobotMgr()	{};

	inline CRobotObj*	GetRobot(int sock)	
	{
		int index = m_LogonMap.Find(sock);
		if(m_LogonMap.IsValidIndex(index))
			return m_LogonMap[index];
		return NULL;
	}
	inline void	RemoveRobot(int sock)	
	{
		int index = m_LogonMap.Find(sock);
		if(m_LogonMap.IsValidIndex(index))
			m_LogonMap.RemoveAt(index);
	}

public:
	CUtlLinkedList<CRobotObj*>		m_RobotList;
	CUtlMap<int, CRobotObj*>		m_LogonMap;

};

#define RobotMgr CRobotMgr::getSingleton()
