#ifndef SHARED_MYSQLDEFINE_H
#define SHARED_MYSQLDEFINE_H

#ifdef _WIN32
#include <WinSock2.h>
#elif defined(__linux__)
#include <stdlib.h>
#include "types.h"
#endif
#include "mysql.h"
#include "utlvector.h"

const int DB_STRING_SIZE_MAX = 102400;

enum sql_var_type_define
{
	sql_var_null = 0,	//无效 空
	sql_var_byte,
	sql_var_int,
	sql_var_int64,	
	sql_var_string,
	sql_blob,
};

class sql_var
{	
public:
	sql_var()
	{
		m_nRef=0;
		iVal =0;
		nVal =0;				
		nSize=0;
		dType = sql_var_null;
		szVal = NULL;		
		
	}
	~sql_var()
	{
		if(szVal){
			delete szVal;
			szVal = NULL;
		}
		m_nRef=0;
		iVal =0;
		nVal =0;				
		nSize=0;
		dType = sql_var_null;
		
	}

	void init()
	{
		m_nRef=0;
		iVal =0;
		nVal =0;				
		nSize=0;
		dType = sql_var_null;
		if(szVal){
			delete szVal;
			szVal = NULL;
		}
	}

	inline bool Format(enum_field_types type,int nLen,char* szValue)
	{
		switch(type)
		{
		case MYSQL_TYPE_TINY:
			{
				if(!szVal)
					return false;
				dType = sql_var_byte;
				nVal = atoi(szValue);		
				nSize=nLen;
				break;
			}			
		case MYSQL_TYPE_SHORT:
		case MYSQL_TYPE_LONG:
		case MYSQL_TYPE_INT24:
			{
				if(!szValue)
					return false;

				dType = sql_var_int;
				nVal = atoi(szValue);
				nSize = nLen;
				break;
			}			
		case MYSQL_TYPE_LONGLONG:
			{
				if(!szValue)
					return false;

				dType = sql_var_int64;
#ifdef _WIN32
				iVal=_atoi64(szValue);
#else
				iVal = atol(szValue);
#endif
				nSize = nLen;
				break;
			}			
		case MYSQL_TYPE_VARCHAR:
		case MYSQL_TYPE_VAR_STRING:
		case MYSQL_TYPE_STRING:
			{

				dType = sql_var_string;	
				szVal = NEW char[nLen+1];
				memset(szVal,0,sizeof(char) * (nLen+1));
				if(szVal)
				{				
					if(strlen(szValue) > 0)
					{
						memcpy(szVal,szValue,nLen);	
						szVal[nLen] = '\0';
					}
					else
					{
						szVal[0] = '\0';
					}
				}
				nSize = nLen;
				break;
			}		
		case MYSQL_TYPE_BLOB:
			{				
				if(!szValue || nLen == 0)
					break;

				dType = sql_blob;	
				szVal = NEW char[nLen];
				if(szVal)
				{
					memcpy(szVal,szValue,nLen);
				}
				nSize = nLen;
				break;
			}			
		default:			
			return false;
		}
		return true;
	}
public:
	sql_var_type_define	dType;
	int					nVal;
	int64				iVal;
	int					nSize;		//长度
	char*				szVal;
	UINT				m_nRef;		//内存引用计数
};

// 保存查询结果
class CSqlVarVector
{
public:
	CSqlVarVector()
	{

	}
	~CSqlVarVector()
	{
		Release();
	};

	inline void Release()
	{
		for( int i=0;i<m_sqlvar.Count();i++)
		{
			sql_var* var = m_sqlvar[i];
			//g_SqlVarPool().Free(var);
			if( var )
				delete var;
		}
		m_sqlvar.RemoveAll();
	}
	
	inline void Add(sql_var* pSqlVar)
	{
		m_sqlvar.AddToTail(pSqlVar);
	}

	inline int Count()
	{
		return m_sqlvar.Count();
	}

	inline sql_var* Element( int i )
	{		
		return m_sqlvar.Element(i);
	}

protected:
	CUtlVector<sql_var*>	m_sqlvar;
};

#endif	//SHARED_MYSQLDEFINE_H
