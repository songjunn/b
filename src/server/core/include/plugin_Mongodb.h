#ifndef _PLUGIN_MONGODB_H_
#define _PLUGIN_MONGODB_H_

#include <list>
#include <string>
#include <vector>
#include "bson.h"
#include "bcon.h"
#include "mongoc.h"
#include "convert.h"
#include "Lock.h"
#include "ThreadLib.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "plugin.h"

#define MONGO_BSON bson_t
#define MONGO_ERROR bson_error_t
#define MONGO_CURSOR mongoc_cursor_t
#define MONGO_CLIENT mongoc_client_t
#define MONGO_COLLECTION mongoc_collection_t

class CMongoDB : public CPlugin
{
public:
	struct DBEvent {
		int		    ev_type;
		std::string	ev_value;
		std::string	ev_query;
		std::string	ev_collection;
	};

	//定义数据库操作类型 
	enum Mongo_Event {		
		Mongo_Update = 1,	//修改操作
		Mongo_Insert,		//插入操作
		Mongo_Upsert,		//修改插入
		Mongo_Query,		//查询操作
		Mongo_Remove,		//删除操作
	};

public:
	CMongoDB();
	~CMongoDB();

	bool startup(std::string host, std::string port, std::string dbname);
	bool connectReplset(std::vector<std::string> hosts, std::vector<std::string> ports, std::string rsname, std::string dbname);
	bool exit();

	void insert(std::string collection, std::string value);
	void remove(std::string collection, std::string query);
	void update(std::string collection, std::string query, std::string value);
	void upsert(std::string collection, std::string query, std::string value);
	void select(std::vector<std::string> &result, const std::string collection);
	void select(std::vector<std::string> &result, const std::string collection, std::string query);
	bool hasCollection(std::string collection);

	static std::string makeQuery(std::string key, int value);
	static std::string makeQuery(std::string key, int64 value);

protected:
    bool _connect();
	void _insert(const std::string collection, std::string value);
	void _remove(const std::string collection, std::string query);
	void _update(const std::string collection, std::string query, std::string value);
	void _upsert(const std::string collection, std::string query, std::string value);

	int	_handleError();
	DBEvent* _getHeadEvent();
	void _handleEvent(DBEvent* ev);
	void _setEvent(int type, std::string collection, std::string query, std::string value);
	static void _handleEventThread(void* args);

	inline bool	_isWorking() { return _working; }
	inline bool	_isConnected() { return _connecting; }

protected:
	bool _working;
	bool _connecting;
	std::string _host;
	std::string _dbname;
	MONGO_CLIENT* _conn;

	Mutex _mutex;
	Eventer _eventer;
	ThreadLib::ThreadID _threadID;
	std::list<DBEvent*> _eventList;
};

#endif //_PLUGIN_MONGODB_H_
