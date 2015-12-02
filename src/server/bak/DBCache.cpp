#include "DBCache.h"
#include "DBResult.h"
#include "exception.h"


CDBCache::CDBCache()
{
}

CDBCache::~CDBCache()
{
	for(int i=0; i<SQL_TYPE_END; i++)
	{
		//在CSqlObj内存池释放之后会造成野指针，不init也没事
		/*FOR_EACH_LL(m_SqlList[i],n)
		{
			CSqlObj* pSql_t = m_SqlList[i].Element(n);
			if(pSql_t)
				pSql_t->init();
		}*/
		m_SqlList[i].RemoveAll();
	}

}

bool CDBCache::ConnectDB(const char* dbname, const char* dbip, const char* usr, const char* pwd, bool updataProcess, bool selectProcess, int objcnt /*= 1024*/)
{
	if(!dbname || !dbip || !usr || !pwd)
		return false;

	for(int i=0; i<SQL_TYPE_END; i++)
	{
		if(!m_SqlHD[i].Connect(dbname, dbip, usr, pwd))
		{
			Log.Error("+-连接数据库 %s 失败, %s %s", dbname, dbip, usr);
			return false;
		}
	}

	strcpy(m_dbname,dbname);
	strcpy(m_dbip,dbip);

	//初始化对象池
	char upName[128] = {0};
	sprintf(upName, "%s_Updatepool", dbname);
	if( !m_UpdateSqlPool.Init(upName, objcnt) )
	{
		return false;
	}
	char seName[128] = {0};
	sprintf(seName, "%s_Selectpool", dbname);
	if( !m_SelectSqlPool.Init(seName, objcnt) )
	{
		return false;
	}

	if (updataProcess)
		ThreadLib::Create(_RunSqlUdt, this);

	if (selectProcess)
		ThreadLib::Create(_RunSqlRet, this);

	return true;
}

bool CDBCache::ExcuteSql(SQL_TYPE t, const char* sql,int nSize, SqlHandlerFunc func/*=NULL*/, int param/*=-1*/, int64 param64/*=(int64)-1*/)
{
	if(nSize >= SQL_STR_LEN || nSize <= 0 || t >= SQL_TYPE_END)
	{
		Log.Error("sql size error %d %s %d\n", nSize,__FILE__,__LINE__);
		return false;
	}

	CSqlObj* pSql = NULL;
	if(SQL_RETURN == t)
		pSql = m_SelectSqlPool.Alloc();
	else if(SQL_NORETURN == t)
		pSql = m_UpdateSqlPool.Alloc();

	if(!pSql)
	{
		if(SQL_RETURN == t)
			m_SqlRetEvent.Event();
		else if(SQL_NORETURN == t)
			m_SqlUdtEvent.Event();

		Log.Error("SQL内存分配失败(类型:%d)!", t);
		Log.Error("丢失SQL数据:%s", sql);

		return false;
	}

	pSql->func = func;
	pSql->type = t;
	pSql->len = nSize;
	pSql->param = param;
	pSql->param64 = param64;
	memcpy(pSql->sql,sql,nSize);

	m_ListLock[t].LOCK();
	if( m_SqlList[t].AddToTail(pSql) == m_SqlList[t].InvalidIndex() )
	{
		m_ListLock[t].UNLOCK();

		if(SQL_RETURN == t)
			m_SelectSqlPool.Free(pSql);
		else if(SQL_NORETURN == t)
			m_UpdateSqlPool.Free(pSql);

		Log.Error("m_SqlList add to tail failed\n");

		return false;
	}
	m_ListLock[t].UNLOCK();

	if(SQL_RETURN == t)
		m_SqlRetEvent.Event();
	else if(SQL_NORETURN == t)
		m_SqlUdtEvent.Event();

	return true;
}

bool CDBCache::ExcuteSqlBlock(char* sql, int nSize, CSqlVarVector& vec)
{
	return MYSQL_ERROR_ID_OK == m_SqlHD[SQL_BLOCK].Execute(sql, nSize, vec);
}

bool CDBCache::ExcuteSqlBlock(char* sql, int nSize)
{
	return MYSQL_ERROR_ID_OK == m_SqlHD[SQL_BLOCK].Execute(sql, nSize);
}

CSqlObj* CDBCache::_GetHeadSql(SQL_TYPE t)
{
	if( t >= SQL_TYPE_END )
		return NULL;

	CSqlObj* pSql = NULL;

	m_ListLock[t].LOCK();
	int index = m_SqlList[t].Head();
	if(m_SqlList[t].IsValidIndex(index))
	{
		pSql = m_SqlList[t].Element(index);		
		m_SqlList[t].Remove(index);		
	}
	m_ListLock[t].UNLOCK();

	return pSql;
}

bool CDBCache::_DelHeadSql(CSqlObj* p)
{
	if(!p || p->type >= SQL_TYPE_END )
		return false;

	SQL_TYPE t = p->type;

	if(SQL_RETURN == p->type)
		m_SelectSqlPool.Free(p);
	else if(SQL_NORETURN == p->type)
		m_UpdateSqlPool.Free(p);

	return true;
}

void CDBCache::_RunSqlUdt(void* param)
{
#if (!defined(_DEBUG)) && defined(WIN32)
	__try
#endif
	{
		CDBCache* pParam = (CDBCache*)param;
		while(true)
		{
			pParam->m_SqlUdtEvent.Wait(1);
			//检查连接状态 
			CMysqlCore* pdb = pParam->_GetSqlHD(SQL_NORETURN);
			if(!pdb || !pdb->IsConnected())
			{
				Log.Error("get %s db handle error!\n", pParam->_Getdbname());

				pdb->Reconnect();
				continue;
			}

			CSqlObj* pSql_t = NULL;
			while(pSql_t = pParam->_GetHeadSql(SQL_NORETURN))
			{
				MYSQL_ERRNO errorID = pdb->Execute(pSql_t->sql, pSql_t->len);
				if(errorID != MYSQL_ERROR_ID_OK)
				{
					Log.Error("execute sql error %s %d!\n", pParam->_Getdbname(), errorID);
				}

				pParam->_DelHeadSql(pSql_t);
			}
		}
	}
#if (!defined(_DEBUG)) && defined(WIN32)
	__except (HandleException(GetExceptionInformation(), "RunSqlUdt"))
	{

	}
#endif	
	return;
}

void CDBCache::_RunSqlRet(void* param)
{
#if (!defined(_DEBUG)) && defined(WIN32)
	__try
#endif
	{
		CDBCache* pParam = (CDBCache*)param;
		while(true)
		{
			pParam->m_SqlRetEvent.Wait(1);
			//检查连接状态 
			CMysqlCore* pdb = pParam->_GetSqlHD(SQL_RETURN);
			if(!pdb || !pdb->IsConnected())
			{
				Log.Error("get %s db handle error!\n", pParam->_Getdbname());

				pdb->Reconnect();
				continue;
			}

			CSqlObj* pSql_t = NULL;
			while(pSql_t = pParam->_GetHeadSql(SQL_RETURN))
			{
				CSqlRetObj* pRetObj = DBResult.m_SqlRetPool.Alloc();
				if(!pRetObj)
				{
					Log.Error("SqlRetPool Alloc failed!\n");
					continue;
				}
				pRetObj->func = pSql_t->func;
				pRetObj->param = pSql_t->param;
				pRetObj->param64 = pSql_t->param64;

				MYSQL_ERRNO errorID = pdb->Execute(pSql_t->sql, pSql_t->len, pRetObj->vec);
				if(errorID != MYSQL_ERROR_ID_OK)
				{
					Log.Error("execute sql error %s %d!\n", pParam->_Getdbname(), errorID);
				}
				else
				{
					//将vid和sql事件id加入结果链表
					DBResult.AddSqlRet(pRetObj);
				}

				pParam->_DelHeadSql(pSql_t);
			}
		}

	}
#if (!defined(_DEBUG)) && defined(WIN32)
	__except (HandleException(GetExceptionInformation(), "RunSqlRet"))
	{

	}
#endif	
	return;
}
