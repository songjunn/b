#ifndef SERVERCORE_DBCACHE_H
#define SERVERCORE_DBCACHE_H

#include "MysqlCore.h"
#include "Lock.h"
#include "memorypool.h"
#include "utllinkedlist.h"

#define SQL_STR_LEN 20580

enum SQL_TYPE {
	SQL_INVALID = -1,
	SQL_BLOCK = 0,
	SQL_RETURN,
	SQL_NORETURN,

	SQL_TYPE_END,
};

class CSqlRetObj;
typedef void (*SqlHandlerFunc)( CSqlRetObj* sql );

class CSqlObj
{
public:
	CSqlObj()	{init();}
	~CSqlObj()	{init();}

	inline void init()
	{
		ref = 0;
		type = SQL_INVALID;
		len = 0;
		param = -1;
		param64 = (int64)-1;
		memset(sql,0,sizeof(char)*SQL_STR_LEN);
		func = NULL;
	}

public:
	int			ref;
	SQL_TYPE	type;
	int			len;
	char		sql[SQL_STR_LEN];
	int			param;
	int64		param64;
	SqlHandlerFunc func;

};

class CSqlVarVector;
///////////////////////////////////////////////////////////////////////////
//
class CDBCache
{
public:
	CDBCache();
	~CDBCache();

	bool			ConnectDB(const char* dbname, const char* dbip, const char* usr, const char* pwd, bool updataProcess, bool selectProcess, int objcnt = 1024);

	bool			ExcuteSql(SQL_TYPE t, const char* sql, int nSize, SqlHandlerFunc func=NULL, int param=-1, int64 param64=-1);
	bool			ExcuteSqlBlock(char* sql, int nSize, CSqlVarVector& vec);	//×èÈûÖ´ÐÐsqlÓï¾ä£¬É÷ÓÃ
	bool			ExcuteSqlBlock(char* sql, int nSize);						//×èÈûÖ´ÐÐsqlÓï¾ä£¬É÷ÓÃ

	inline int		GetSqlListCount(SQL_TYPE t)	
	{
		m_ListLock[t].LOCK();
		int n = m_SqlList[t].Count();
		m_ListLock[t].UNLOCK();
		return n;
	}

protected:
	CSqlObj*		_GetHeadSql(SQL_TYPE t);
	bool			_DelHeadSql(CSqlObj* p);

	inline CMysqlCore* _GetSqlHD(SQL_TYPE t)	{return &m_SqlHD[t];}

	inline char*	_Getdbname()	{return m_dbname;}
	inline char*	_Getdbip()		{return m_dbip;}

	static void		_RunSqlUdt(void* param);
	static void		_RunSqlRet(void* param);

protected:
	char		m_dbname[25];
	char		m_dbip[25];

	Eventer		m_SqlUdtEvent;
	Eventer		m_SqlRetEvent;

	CMysqlCore	m_SqlHD[SQL_TYPE_END];
	Mutex		m_ListLock[SQL_TYPE_END];
	CUtlLinkedList<CSqlObj*> m_SqlList[SQL_TYPE_END];

	CObjectMemoryPool<CSqlObj>	m_UpdateSqlPool;
	CObjectMemoryPool<CSqlObj>	m_SelectSqlPool;
	
};

#endif	//SERVERCORE_DBCACHE_H
