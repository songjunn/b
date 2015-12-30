#include "plugin_Mongodb.h"
#include "logger.h"

CMongoDB::CMongoDB() : _working(false), _connecting(false)
{
	mongoc_init();
}

CMongoDB::~CMongoDB()
{
	mongoc_client_destroy(_conn);
    mongoc_cleanup();
}

bool CMongoDB::startup(std::string host, std::string port, std::string dbname)
{
	_dbname = dbname;
	_host = "mongodb://" + host + ":" + port + "/" + dbname;

	if (!_connect()) {
		return false;
	}

	_threadID = ThreadLib::Create(_handleEventThread, this);

	return true;
}

bool CMongoDB::exit()
{
    _working = false;
	ThreadLib::WaitForFinish(_threadID);
}

std::string CMongoDB::makeQuery(std::string key, int value)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	rapidjson::Value val;
	val.SetInt(value);

	rapidjson::Document doc;
	doc.SetObject();
	doc.AddMember(key.c_str(), doc.GetAllocator(), val, doc.GetAllocator());
	doc.Accept(writer);

	std::string querystr;
	querystr.assign(buffer.GetString(), buffer.Size());
	return querystr;
}

std::string CMongoDB::makeQuery(std::string key, int64 value)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	rapidjson::Value val;
	val.SetInt64(value);

	rapidjson::Document doc;
	doc.SetObject();
	doc.AddMember(key.c_str(), doc.GetAllocator(), val, doc.GetAllocator());
	doc.Accept(writer);

	std::string querystr;
	querystr.assign(buffer.GetString(), buffer.Size());
	return querystr;
}

void CMongoDB::insert(std::string collection, std::string value)
{
	_setEvent(Mongo_Insert, collection, "", value);
}

void CMongoDB::remove(std::string collection, std::string query)
{
	_setEvent(Mongo_Remove, collection, query, "");
}

void CMongoDB::update(std::string collection, std::string query, std::string value)
{
	_setEvent(Mongo_Update, collection, query, value);
}

void CMongoDB::_setEvent(int type, std::string collection, std::string query, std::string value)
{
	DBEvent* event = new DBEvent;
	if (event == NULL) {
		return;
	}

	event->ev_type = type;
	event->ev_collection = collection;
	if (type != Mongo_Insert) {
		event->ev_query = query;
	}
	if (type != Mongo_Remove) {
		event->ev_value = value;
	}

	_mutex.LOCK();
	_eventList.push_back(event);
	_mutex.UNLOCK();
}

bool CMongoDB::_connect()
{
	_conn = mongoc_client_new(_host.c_str());
	if (_conn == NULL) {
		LOGGER_ERROR("[MongonDB] Connect %s Error", _host.c_str());
		return false;
	}

    _working = true;
	_connecting = true;
	
	LOGGER_ERROR("[MongonDB] Connect %s Success", _host.c_str());

	return true;
}

void CMongoDB::_insert(const std::string collection, std::string value)
{
    MONGO_COLLECTION *client;
    MONGO_ERROR error;
	MONGO_BSON *bson;

	bson = bson_new_from_json((const uint8_t*)value.c_str(), value.length(), &error);
	client = mongoc_client_get_collection (_conn, _dbname.c_str(), collection.c_str());

	if (!mongoc_collection_insert (client, MONGOC_INSERT_NONE, bson, NULL, &error)) {
		LOGGER_ERROR("[MongonDB] Insert Failed, %s:%s, %s", _dbname.c_str(), collection.c_str(), value.c_str());
		LOGGER_ERROR("[MongonDB] Exception: %s ", error.message);
    }

	bson_destroy (bson);
    mongoc_collection_destroy (client);
}

void CMongoDB::_remove(const std::string collection, std::string query)
{
	MONGO_COLLECTION *client;
	MONGO_ERROR error;
	MONGO_BSON *bson_query;

	bson_query = bson_new_from_json((const uint8_t*)query.c_str(), query.length(), &error);
	client = mongoc_client_get_collection (_conn, _dbname.c_str(), collection.c_str());

	if (!mongoc_collection_remove (client, MONGOC_REMOVE_SINGLE_REMOVE, bson_query, NULL, &error)) {
		LOGGER_ERROR("[MongonDB] Delete Failed, %s:%s, %s", _dbname.c_str(), collection.c_str(), query.c_str());
		LOGGER_ERROR("[MongonDB] Exception: %s ", error.message);
    }

	bson_destroy (bson_query);
	mongoc_collection_destroy (client);
}

void CMongoDB::_update(const std::string collection, std::string query, std::string value)
{
	MONGO_COLLECTION *client;
	MONGO_ERROR error;
	MONGO_BSON *bson_value, *bson_query;

	bson_query = bson_new_from_json((const uint8_t*)query.c_str(), query.length(), &error);
	bson_value = bson_new_from_json((const uint8_t*)value.c_str(), value.length(), &error);
	client = mongoc_client_get_collection (_conn, _dbname.c_str(), collection.c_str());

	if (!mongoc_collection_update (client, MONGOC_UPDATE_NONE, bson_query, bson_value, NULL, &error)) {
		LOGGER_ERROR("[MongonDB] Update Failed, %s:%s, %s, %s", _dbname.c_str(), collection.c_str(), query.c_str(), value.c_str());
		LOGGER_ERROR("[MongonDB] Exception: %s ", error.message);
    }

	bson_destroy (bson_query);
	bson_destroy (bson_value);
	mongoc_collection_destroy (client);
}

void CMongoDB::select(std::vector<std::string> &result, const std::string collection, std::string query)
{
    MONGO_COLLECTION *client;
	MONGO_ERROR error;
	MONGO_CURSOR *cursor;
	MONGO_BSON *bson_query;
	const MONGO_BSON *doc;
	char *str;

	bson_query = bson_new_from_json((const uint8_t*)query.c_str(), query.length(), &error);
	client = mongoc_client_get_collection (_conn, _dbname.c_str(), collection.c_str());
	cursor = mongoc_collection_find (client, MONGOC_QUERY_NONE, 0, 0, 0, bson_query, NULL, NULL);

	while (mongoc_cursor_next (cursor, &doc)) {
		str = bson_as_json (doc, NULL);
		result.push_back(str);
		bson_free (str);
	}
	
	bson_destroy (bson_query);
	mongoc_cursor_destroy (cursor);
	mongoc_collection_destroy (client);
}

CMongoDB::DBEvent* CMongoDB::_getHeadEvent()
{
	DBEvent* ev = NULL;

	_mutex.LOCK();
	if (_eventList.size() > 0) {
		ev = _eventList.front();
		_eventList.pop_front();
	}
	_mutex.UNLOCK();

	return ev;
}

void CMongoDB::_handleEvent(DBEvent* ev)
{
	switch (ev->ev_type) {
		case Mongo_Update: _update(ev->ev_collection, ev->ev_query, ev->ev_value); break;
		case Mongo_Insert: _insert(ev->ev_collection, ev->ev_value); break;
		//case Mongo_Query: _select(ev->ev_collection, ev->ev_query); break;
		case Mongo_Remove: _remove(ev->ev_collection, ev->ev_query); break;
		default: break;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CMongoDB::_handleEventThread(void* args)
{
	CMongoDB* instance = (CMongoDB*)args;

	while (instance->_isWorking()) {
	    if (!instance->_conn)
			break;

		if (!instance->_isConnected()) {
			LOGGER_ERROR("[MongonDB] Mongodb connection error");
			instance->_connect();
			continue;
		}

		DBEvent* ev = NULL;
		while (ev = instance->_getHeadEvent()) {
			instance->_handleEvent(ev);
			delete ev;
		}
	}
}
