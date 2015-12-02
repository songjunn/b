#ifndef SERVERCORE_DBRESULT_H
#define SERVERCORE_DBRESULT_H

#include "MysqlCore.h"
#include "DBCache.h"

///////////////////////////////////////////////////////////////////////////
//
class CSqlRetObj
{
public:
	CSqlRetObj()	{init();}
	~CSqlRetObj()	{init();}

	inline void init()
	{
		ref = 0;
		func = NULL;
		param = -1;
		param64 = (int64)-1;
		vec.Release();
	}

public:
	int				ref;
	SqlHandlerFunc	func;
	CSqlVarVector	vec;
	int				param;
	int64			param64;

};

///////////////////////////////////////////////////////////////////////////
//
class CDBResult : public Singleton< CDBResult >
{
public:
	CDBResult();
	~CDBResult();

	bool			Init(int size);

	bool			AddSqlRet(CSqlRetObj* pSql);
	CSqlRetObj*		GetHeadSql();

	void			SqlEventLogic();

protected:
	Mutex			m_ListLock;
	CUtlLinkedList<CSqlRetObj*>		m_List;

public:
	CObjectMemoryPool<CSqlRetObj>	m_SqlRetPool;

};

#define DBResult CDBResult::getSingleton()

#endif	//SERVERCORE_DBRESULT_H
