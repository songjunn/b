#pragma once
#include <string.h>
#include "types.h"
#include "memory.h"
#include "commdata.h"
#include "eventdef.h"
#include "Singleton.h"


//////////////////////////////////////////////////////////////////////////

#define STRING_PARAM_LEN	64

class CEvent
{
public:
	CEvent()	{Init();}
	~CEvent(){}

	inline void Init()
	{
		m_nEventID	= Event_Invalid;
		m_nPID		= -1;
		m_nParam[0] = 0;
		m_nParam[1] = 0;
		m_nParam[2] = 0;
		m_nParam[3] = 0;
		m_nParam[4] = 0;
		m_nParam[5] = 0;
		memset(m_szTime,0,sizeof(char)*32);
		memset(m_szParam,0,sizeof(char)*STRING_PARAM_LEN);
		m_bUpdateDB = FALSE;
	}

	inline void Create(Event_ID nEventID, PersonID PID, char* szTime, int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4, int64 nParam5, int64 nParam6, char* szParam1, bool bUpdateDB)
	{
		m_nPID = PID;
		m_nEventID = nEventID;
		strncpy(m_szTime, szTime, 32);
		m_nParam[0] = nParam1;
		m_nParam[1] = nParam2;
		m_nParam[2] = nParam3;
		m_nParam[3] = nParam4;
		m_nParam[4] = nParam5;
		m_nParam[5] = nParam6;
		if(szParam1)
			strncpy(m_szParam,szParam1,STRING_PARAM_LEN);
		else
			m_szParam[0] = '\0';
		m_bUpdateDB = bUpdateDB;
	}

public:
	Event_ID	m_nEventID;
	PersonID	m_nPID;
	char		m_szTime[32];
	int64		m_nParam[6];
	char		m_szParam[STRING_PARAM_LEN];
	bool		m_bUpdateDB;
};

CEvent*	MakeEvent(Event_ID nEventID, PersonID PID, char* szParam1 = NULL,bool bUpdateDB = false);
CEvent*	MakeEvent(Event_ID nEventID, PersonID PID, int64 nParam1, char* szParam1 = NULL,bool bUpdateDB = false);	
CEvent*	MakeEvent(Event_ID nEventID, PersonID PID, int64 nParam1, int64 nParam2, char* szParam1 = NULL,bool bUpdateDB = false);	
CEvent*	MakeEvent(Event_ID nEventID, PersonID PID, int64 nParam1, int64 nParam2, int64 nParam3, char* szParam1 = NULL,bool bUpdateDB = false);	
CEvent*	MakeEvent(Event_ID nEventID, PersonID PID, int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4, char* szParam1 = NULL,bool bUpdateDB = false);	
CEvent*	MakeEvent(Event_ID nEventID, PersonID PID, int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4, int64 nParam5, char* szParam1 = NULL,bool bUpdateDB = false);	
CEvent*	MakeEvent(Event_ID nEventID, PersonID PID, int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4, int64 nParam5, int64 nParam6,char* szParam1 = NULL,bool bUpdateDB = false);	


//////////////////////////////////////////////////////////////////////////
//
class CEventPool : public Singleton< CEventPool >
{
public:
	CEventPool();
	~CEventPool();

	bool	Create(int nPoolSize);
	void	Release();

	CEvent*	Alloc();

protected:
	int			m_EventPoolSize;
	int			m_EventTableIndex;
	CEvent*		m_EventTable;
};

#define EventPool CEventPool::getSingleton()
