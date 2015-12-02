#include "DBResult.h"

//////////////////////////////////////////////////////////////////////////
//
CDBResult::CDBResult()
{

}

CDBResult::~CDBResult()
{
	FOR_EACH_LL(m_List,i)
	{
		CSqlRetObj* pSql_t = m_List.Element(i);
		if(pSql_t)
			pSql_t->init();
	}

	m_List.RemoveAll();
}

bool CDBResult::Init(int size)
{
	return m_SqlRetPool.Init("SqlRetPool", size);
}

bool CDBResult::AddSqlRet(CSqlRetObj* pSql)
{
	if(!pSql)
		return false;

	m_ListLock.LOCK();
	m_List.AddToTail(pSql);
	m_ListLock.UNLOCK();

	return true;
}

CSqlRetObj* CDBResult::GetHeadSql()
{
	CSqlRetObj* pSql = NULL;

	m_ListLock.LOCK();
	int index = m_List.Head();
	if(m_List.IsValidIndex(index))
	{
		pSql = m_List.Element(index);
        m_List.Remove(index);
	}
	m_ListLock.UNLOCK();

	return pSql;
}

void CDBResult::SqlEventLogic()
{
	CSqlRetObj* obj = NULL;
	while( (obj = GetHeadSql()) )
	{
		obj->func(obj);

		m_SqlRetPool.Free(obj);
	}
}
