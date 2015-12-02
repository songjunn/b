#ifndef _PLUGIN_MONGODB_H_
#define _PLUGIN_MONGODB_H_

#include "plugin.h"
#include <string>
#include "bson.h"
#include "bcon.h"
#include "mongoc.h"
#include "ThreadLib.h"
#include "Lock.h"
#include "utllinkedlist.h"

#define MONGO_BSON bson_t
#define MONGO_QUERY bson_t
#define MONGO_CURSOR mongoc_cursor_t

class CMongoDB : public CPlugin
{
	struct OperObj {
		int		    opr_type;
		MONGO_QUERY	opr_condition;
		MONGO_BSON	opr_bson;
		std::string	opr_collection;
	};

	//定义数据库操作类型 
	enum Mongo_Opr_Type {		
		Mongo_Update = 1,	//修改操作
		Mongo_Insert,		//插入操作
		Mongo_Query,		//查询操作
		Mongo_Delete,		//删除操作
	};

public:
	CMongoDB();
	~CMongoDB();

	bool startup(std::string host, std::string port, std::string dbname);
	bool exit();
	void execute(int opr_type, std::string opr_collection, MONGO_QUERY& opr_condition, MONGO_BSON& opr_bson);
	void select(MONGO_CURSOR* cursor, const std::string collection, MONGO_QUERY& condition);

protected:
	void _insert(const std::string collection, MONGO_BSON& obj);
	void _delete(const std::string collection, MONGO_QUERY& condition);
	void _update(const std::string collection, MONGO_QUERY& condition, MONGO_BSON& obj);

	bool _reconnect();
	void _disConnect();

	void _executeOpr(OperObj* opr);
	OperObj* _getHeadOpr();
	int	_handleError();

	inline bool	_isWorking() { return m_working; }
	inline bool	_isConnected() { return m_connect; }

	static void	_execOperation(void* param);

protected:
	ThreadLib::ThreadID m_threadID;
	bool m_working;
	bool m_connect;
	int  m_port;
	char m_host[32];
	std::string m_dbname;
	mongoc_client_t* m_conn;

	Eventer m_Eventer;
	Mutex	m_Locker;
	CUtlLinkedList<OperObj*> m_ObjList;
};

#endif //_PLUGIN_MONGODB_H_
