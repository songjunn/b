#ifndef SHARED_MYSQLCORE_H
#define SHARED_MYSQLCORE_H

#include "MysqlDefine.h"

typedef unsigned int MYSQL_ERRNO;

#define MYSQL_ERRNO_ERROR	0
#define MYSQL_ERRNO_OK		1

enum MYSQL_ERROR_ID
{
	MYSQL_ERROR_ID_ERROR	 = 0,
	MYSQL_ERROR_ID_OK		 = 1,			// 执行成功的消息
	MYSQL_ERROR_ID_KEY_ERROR = 1062,		//主键冲突错误
	MYSQL_ERROR_ID_SYNTAX	 = 1064,		//语法错误，可能是要把单引号变成双引号
	MYSQL_ERROR_ID_QUERYEMPT = 1065,		//Query was empty
	MYSQL_ERROR_TABLE_EXIT	 = 1146,		//表格不存在
	MYSQL_ERROR_ID_CREATE_THREAD_ERROR = 1136, //插入新记录错误
	MYSQL_ERROR_ID_DISCONNECT = 2006,		//mysql断开连接
};

class CMysqlCore
{
public:
	CMysqlCore();
	~CMysqlCore();

	bool				Connect(const char* Database,const char* Host,const char* User,const char* Password);
	bool				Reconnect();
	void				DisConnect();

	MYSQL_ERRNO 		Execute(char* szSql, int len, CSqlVarVector& Vector);
	MYSQL_ERRNO			Execute(char* szSql, int len);

	MYSQL_ERRNO			HandleError();

	inline bool			IsConnected()	{return m_Connected;}
	
protected:
	MYSQL				m_Mysql;
	char				m_Database[25];
	char				m_Host[25];
	char				m_User[25];
	char				m_Password[25];
	bool				m_Connected;

};

#endif	//SHARED_MYSQLCORE_H
