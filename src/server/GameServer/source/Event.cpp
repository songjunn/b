#include "Event.h"
#include "gtime.h"


CEvent* MakeEvent(Event_ID nEventID,PersonID PID,int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4,int64 nParam5,int64 nParam6,char* szParam1,bool bUpdateDB)
{
	CEvent* pEvent = EventPool.Alloc();
	if(pEvent)
	{
		char timestr[32] = {0};
		DatatimeToString(timestr);
		pEvent->Create(nEventID,PID,timestr, nParam1,nParam2,nParam3,nParam4,nParam5,nParam6,szParam1,bUpdateDB);
	}
	return pEvent;
}

CEvent* MakeEvent(Event_ID nEventID,PersonID PID,int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4,int64 nParam5,char* szParam1,bool bUpdateDB)
{
	return MakeEvent(nEventID,PID,nParam1,nParam2,nParam3,nParam4,nParam5,-1,szParam1,bUpdateDB);
}
CEvent* MakeEvent(Event_ID nEventID,PersonID PID,int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4,char* szParam1,bool bUpdateDB)
{
	return MakeEvent(nEventID,PID,nParam1,nParam2,nParam3,nParam4,-1,-1,szParam1,bUpdateDB);
}
CEvent* MakeEvent(Event_ID nEventID,PersonID PID,int64 nParam1, int64 nParam2, int64 nParam3,char* szParam1,bool bUpdateDB)
{
	return MakeEvent(nEventID,PID,nParam1,nParam2,nParam3,-1,-1,-1,szParam1,bUpdateDB);
}
CEvent* MakeEvent(Event_ID nEventID,PersonID PID,int64 nParam1, int64 nParam2,char* szParam1,bool bUpdateDB)
{
	return MakeEvent(nEventID,PID,nParam1,nParam2,-1,-1,-1,-1,szParam1,bUpdateDB);
}
CEvent* MakeEvent(Event_ID nEventID,PersonID PID,int64 nParam1,char* szParam1,bool bUpdateDB)
{
	return MakeEvent(nEventID,PID,nParam1,-1,-1,-1,-1,-1,szParam1,bUpdateDB);
}
CEvent* MakeEvent(Event_ID nEventID,PersonID PID,char* szParam1,bool bUpdateDB)
{
	return MakeEvent(nEventID,PID,-1,-1,-1,-1,-1,-1,szParam1,bUpdateDB);
}

//////////////////////////////////////////////////////////////////////////
//
CEventPool::CEventPool() : m_EventPoolSize(0), m_EventTableIndex(0), m_EventTable(NULL)
{

}

CEventPool::~CEventPool()
{
	Release();
}

bool CEventPool::Create(int nPoolSize)
{
	m_EventPoolSize = nPoolSize;

    if(m_EventPoolSize	> 0)
	{
		m_EventTable = (CEvent*)MemAlloc(sizeof(CEvent)*m_EventPoolSize);
		for(int i=0;i<m_EventPoolSize;i++)
		{
			m_EventTable[i].Init();
		}
	}

	return true;
}

void CEventPool::Release()
{
	MemFree(m_EventTable);
}

CEvent* CEventPool::Alloc()
{
	if( m_EventTableIndex >= m_EventPoolSize )
		m_EventTableIndex = 0;

	CEvent* pEvent = (CEvent*)&m_EventTable[m_EventTableIndex];
	if( !pEvent )
		return NULL;

	m_EventTableIndex++;

	return pEvent;
}
