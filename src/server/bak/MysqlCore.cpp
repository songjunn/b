#include "SharedInc.h"
#include "MysqlCore.h"

//////////////////////////////////////////////////////////////////////////
//
CMysqlCore::CMysqlCore()
{
	m_Connected = false;

	memset(m_Database,0,sizeof(char)*25);
	memset(m_Host,0,sizeof(char)*25);
	memset(m_User,0,sizeof(char)*25);
	memset(m_Password,0,sizeof(char)*25);
}

CMysqlCore::~CMysqlCore()
{

}

bool CMysqlCore::Connect(const char* Database,const char* Host,const char* User,const char* Password)
{
	mysql_init(&m_Mysql);

	if (!mysql_real_connect(&m_Mysql,Host, User, Password, Database,0,NULL,0))
	{
		Log.Error("+-Connect %s error:%s", Database, mysql_error(&m_Mysql));
		return false;
	}

	//mysql_set_character_set(&m_Mysql, "utf8");
	
	m_Connected = true;

	strcpy(m_Database,Database);
	strcpy(m_Host,Host);
	strcpy(m_User,User);
	strcpy(m_Password,Password);

	Log.Notice("+-Connected Database(%s %s) OK...", Host, Database);

	return true;
}

bool CMysqlCore::Reconnect()
{
	mysql_init(&m_Mysql);
	
	if (!mysql_real_connect(&m_Mysql,m_Host,m_User,m_Password,m_Database,0,NULL,0))
	{
		Log.Error("+-Reconnect %s error:%s", m_Database, mysql_error(&m_Mysql));
		return false;
	}

	m_Connected = true;	
	Log.Notice("+-ReConnected   %s  MySql  OK...", m_Host);
	return true;
}

void CMysqlCore::DisConnect()
{	
	mysql_close(&m_Mysql);

	m_Connected = false;
	Log.Notice("Disconnect With MySql");
}

MYSQL_ERRNO CMysqlCore::Execute(char* szSql, int len)
{		
	if(!szSql || len<=0 || !strlen(szSql))
		return MYSQL_ERRNO_OK;

	if(mysql_real_query(&m_Mysql,szSql,len))
	{
		Log.Error("+-Mysql Error:ID=%d , Database:%s, %s", mysql_errno(&m_Mysql),m_Database, szSql);
		return HandleError();
	}	

	return MYSQL_ERRNO_OK;
}

MYSQL_ERRNO CMysqlCore::Execute(char* szSql, int len, CSqlVarVector& Vec)
{
	if(!szSql || !strlen(szSql) || len <= 0)
		return MYSQL_ERRNO_ERROR;

	if(mysql_real_query(&m_Mysql,szSql,len))
	{
		Log.Error("+-mysql_real_query Error %d : %s",mysql_errno(&m_Mysql),szSql);
		return HandleError();
	}

	//将数据存储在本地 返回集
	MYSQL_RES* res = mysql_store_result(&m_Mysql);  	
	//这里有隐患，可能返回NULL
	//原因有集合太大，数据不能被读取， 查询没有返回数据 by zhao
	if(!res)
	{
		Log.Error("+-mysql_store_result error:%d %s",mysql_errno(&m_Mysql),szSql);
		return HandleError();
	}

	// 字段信息
	MYSQL_FIELD* fields = mysql_fetch_fields(res);
	if(!fields)
	{
		Log.Error("+-mysql_fetch_fields error:%s",mysql_errno(&m_Mysql));
		return HandleError();
	}

	UINT nField = mysql_num_fields(res); 
	//UINT nRow  = mysql_num_rows(res);

	MYSQL_ROW row;
	while((row = mysql_fetch_row(res)))
	{
		for(UINT t=0;t<nField;t++)
		{				
			// 构造自定义元素
			//sql_var* var = g_SqlVarPool().Alloc();
			sql_var* var = NEW sql_var;
			if(var)
			{
				var->Format(fields[t].type,fields[t].max_length,row[t]);					
				Vec.Add(var);
			}
			
		}		
	}
	
	mysql_free_result(res);

	return MYSQL_ERRNO_OK;
}

MYSQL_ERRNO CMysqlCore::HandleError()
{
	MYSQL_ERRNO errorid = mysql_errno(&m_Mysql);

	switch(errorid)
	{
	case MYSQL_ERROR_ID_DISCONNECT:
		{
			DisConnect();
			Reconnect();
		}
		break;
	default:
		break;
	}

	return errorid;
}
