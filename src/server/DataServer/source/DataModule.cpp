#include "DataModule.h"
#include "gtime.h"
#include "DataServer.h"
#include "LoadModule.h"
#include "MessageTypeDefine.pb.h"
#include "MessageGameobj.pb.h"


void CDataModule::_setSaveType(CDataObj* obj, int type)
{
	if (obj->m_save != SAVE_INSERT || type != SAVE_UPDATE) {
		obj->m_save = type;
	}
}

void CDataModule::_setStatusType(CDataObj* obj, int status)
{
	if (obj->m_status != STATUS_ONLINE || status != STATUS_LOAD) {
		obj->m_status = status;
	}
	if (obj->m_status != STATUS_ONLINE) {
		obj->m_stime = GetTimeSec();
	}
}

CDataObj* CDataModule::create(std::string type, int64 id)
{
	CDataObj* obj = Create(id);
	if (!obj)
		return NULL;

	obj->m_id = id;
	obj->m_type = type;
	obj->m_save = SAVE_NONE;
	obj->m_status = STATUS_NONE;
	obj->m_stime = 0;

	Log.Debug("[CDataModule] Create: (%s, "INT64_FMT")", type.c_str(), id);

	return obj;
}

CDataObj* CDataModule::loadBson(std::string type, mongo::BSONObj& bson, int64 id)
{
	CDataObj* obj = create(type, id);
	if( !obj )
		return NULL;

	obj->fromBson(bson);

	Log.Debug("[CDataModule] loadDb: (%s, "INT64_FMT"): %s", type.c_str(), id, bson.toString().c_str());

	return obj;
}

CDataObj* CDataModule::loadDb(int status, std::string type, std::string key, int64 id)
{
	mongo::Query query = QUERY(key<<id);
	mongo::auto_ptr<mongo::DBClientCursor> cursor;
	GETMONGODB(&DataServer)->select(cursor, type, query);

	if (cursor.get() && cursor->more()) {
	    mongo::BSONObj p = cursor->next();

		CDataObj* obj = create(type, id);
		if( !obj )
			return NULL;

		obj->fromBson(p);
		_setStatusType(obj, status);

		Log.Debug("[CDataModule] loadDb: (%s, "INT64_FMT"): %s", type.c_str(), id, p.toString().c_str());

		return obj;
	}

	return NULL;
}

bool CDataModule::loadDb(int status, std::string type)
{
	mongo::Query query;
	mongo::auto_ptr<mongo::DBClientCursor> cursor;
	GETMONGODB(&DataServer)->select(cursor, type, query);

	while (cursor.get() && cursor->more()) {
	    mongo::BSONObj p = cursor->next();

		int64 id;
		std::string idstr = type + "id";
		if (!GETMONGODB(&DataServer)->getBsonFieldI64(p, idstr, id)) {
			return false;
		}

		CDataObj* obj = create(type, id);
		if( !obj )
			return false;

		obj->fromBson(p);
		_setStatusType(obj, status);

		Log.Debug("[CDataModule] loadDb: (%s, "INT64_FMT"): %s", type.c_str(), id, p.toString().c_str());
	}

	return true;
}

void CDataModule::saveDb(CDataObj* obj, std::string key)
{
	mongo::Query query = QUERY(key<<obj->getId());
	mongo::BSONObj objbson;
	obj->toBson(objbson);

	GETMONGODB(&DataServer)->execute(CMongoDB::Mongo_Insert, obj->m_type, query, objbson);

	Log.Debug("[CDataModule] saveDb: (%s, "INT64_FMT"): %s", obj->m_type.c_str(), obj->getId(), objbson.toString().c_str());
}

void CDataModule::updateDb(CDataObj* obj, std::string key)
{
	mongo::Query query = QUERY(key<<obj->getId());
	mongo::BSONObj objbson;
	obj->toBson(objbson);

	GETMONGODB(&DataServer)->execute(CMongoDB::Mongo_Update, obj->m_type, query, objbson);

	Log.Debug("[CDataModule] updateDb: (%s, "INT64_FMT"): %s", obj->m_type.c_str(), obj->getId(), objbson.toString().c_str());
}

void CDataModule::updateDb(int64 id, std::string key)
{
	CDataObj* obj = this->GetObj(id);
	if (obj)
		this->updateDb(obj, key);
}

void CDataModule::syncData(CDataObj* obj, int sock)
{
	std::string json;
	obj->toJsonstring(json);

	Message::SyncAttrs msg;
	msg.set_id(obj->m_id);
	msg.set_type(obj->m_type);
	msg.set_jsonstr(json);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_SYNC);
	GETSERVERNET(&DataServer)->sendMsg(sock, &pack);
}

void CDataModule::syncDataObj(CDataObj* obj, std::string key, int sock)
{
	std::string json;
	obj->toJsonstring(json, key);

	Message::SyncAttrs msg;
	msg.set_id(obj->m_id);
	msg.set_type(obj->m_type);
	msg.set_key(key);
	msg.set_jsonstr(json);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_SYNC_OBJFIELD);
	GETSERVERNET(&DataServer)->sendMsg(sock, &pack);
}

void CDataModule::syncDataMap(CDataObj* obj, rapidjson::Value& json, std::string key, std::string mapkey, int sock)
{
	std::string jsonstr;
	CMetadata::toJsonstring(json, jsonstr);

	Message::SyncAttrs msg;
	msg.set_id(obj->m_id);
	msg.set_type(obj->m_type);
	msg.set_key(key);
	msg.set_mapkey(mapkey);
	msg.set_jsonstr(jsonstr);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_SYNC_MAPFIELD);
	GETSERVERNET(&DataServer)->sendMsg(sock, &pack);
}

void CDataModule::syncDataFinish(int sock, std::string type, int64 id)
{
	Message::SyncAttrs msg;
	msg.set_id(id);
	msg.set_type(type);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_SYNC_FINISH);
	GETSERVERNET(&DataServer)->sendMsg(sock, &pack);
}

void CDataModule::syncDataSeparate(CDataObj* obj, int sock)
{
	CDataObj gameObj;
	gameObj.m_id = obj->m_id;
	gameObj.m_type = obj->m_type;

	for (rapidjson::Value::MemberIterator it = obj->m_members.MemberBegin(); it != obj->m_members.MemberEnd(); ++it) {
		rapidjson::Value& member = obj->getFieldObj(it->name.GetString());
		
		if (CMetadata::isFieldI64(member)) {
			rapidjson::Value jsonValue;
			jsonValue.SetInt64(CMetadata::getFieldI64(member));
			gameObj.m_members.AddMember(it->name.GetString(), jsonValue, gameObj.m_members.GetAllocator());
		}
		else if (CMetadata::isFieldInt(member)) {
			rapidjson::Value jsonValue;
			jsonValue.SetInt(CMetadata::getFieldInt(member));
			gameObj.m_members.AddMember(it->name.GetString(), jsonValue, gameObj.m_members.GetAllocator());
		}
		else if (CMetadata::isFieldStr(member)) {
			rapidjson::Value jsonValue;
			jsonValue.SetString(CMetadata::getFieldStr(member).c_str(), gameObj.m_members.GetAllocator());
			gameObj.m_members.AddMember(it->name.GetString(), jsonValue, gameObj.m_members.GetAllocator());
		}
	}
	this->syncData(&gameObj, sock);

	for (rapidjson::Value::MemberIterator it = obj->m_members.MemberBegin(); it != obj->m_members.MemberEnd(); ++it) {
		rapidjson::Value& member = obj->getFieldObj(it->name.GetString());
		if (CMetadata::isFieldVec(member)) {
			this->syncDataObj(obj, it->name.GetString(), sock);
		}
		else if (CMetadata::isFieldObj(member)) {
			rapidjson::Value::MemberIterator child = member.MemberBegin();
			if (child != member.MemberEnd() && CMetadata::isFieldObj(CMetadata::getFieldVal(member, child->name.GetString()))) {
				do {
					rapidjson::Value& childjson = CMetadata::getFieldVal(member, child->name.GetString());
					this->syncDataMap(obj, childjson, it->name.GetString(), child->name.GetString(), sock);
				} while (++child != member.MemberEnd());
			} else {
				this->syncDataObj(obj, it->name.GetString(), sock);
			}
		}
	}

	syncDataFinish(sock, obj->m_type, obj->m_id);
}

void CDataModule::syncData(std::string type, int sock)
{
	int64 id = m_list.Head();
	while (CDataObj* obj = GetObj(id)) {
		id = m_list.Next(id);
	
		if (type == obj->m_type)
			syncData(obj, sock);
	}

	syncDataFinish(sock, type);
}

bool CDataModule::onMessage(Packet* pack)
{
	if (!pack)
		return false;

	switch(pack->Type()) {
		case Message::MSG_GAMEOBJ_OBJFIELD_SETI32:
			{
				 Message::SyncAttrsObjField msg;
				 PROTOBUF_CMD_PARSER(pack, msg);

				 CDataObj* obj = this->GetObj(msg.id());
				 if (obj) {
					 if (msg.field() == "") {
						 obj->setFieldInt(msg.group(), msg.vali32());
					 } else {
						 obj->setFieldObjInt(msg.group(), msg.field(), msg.vali32());
					 }
					 this->_setSaveType(obj, SAVE_UPDATE);
				 }
			}
			break;
		case Message::MSG_GAMEOBJ_OBJFIELD_SETI64:
			{
				Message::SyncAttrsObjField msg;
			    PROTOBUF_CMD_PARSER(pack, msg);

			    CDataObj* obj = this->GetObj(msg.id());
			    if (obj) {
					if (msg.field() == "") {
						obj->setFieldI64(msg.group(), msg.vali64());
				    } else {
						obj->setFieldObjI64(msg.group(), msg.field(), msg.vali64());
				    }
				    this->_setSaveType(obj, SAVE_UPDATE);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_OBJFIELD_SETSTR:
			{
				Message::SyncAttrsObjField msg;
				PROTOBUF_CMD_PARSER(pack, msg);
				
				CDataObj* obj = this->GetObj(msg.id());
				if (obj) {
					if (msg.field() == "") {
						obj->setFieldString(msg.group(), msg.valstr());
					} else {
						obj->setFieldObjString(msg.group(), msg.field(), msg.valstr());
					}
					this->_setSaveType(obj, SAVE_UPDATE);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_OBJFIELD_SETALL:
			{
				Message::SyncAttrs msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = this->GetObj(msg.id());
				if (obj) {
					obj->fromJsonstringCompletely(msg.jsonstr(), msg.key());
					this->_setSaveType(obj, SAVE_UPDATE);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_MAPFIELD_SETI32:
			{
				Message::SyncAttrsMapField msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = this->GetObj(msg.id());
				if (obj) {
					obj->setFieldMap(msg.group(), msg.mapkey(), msg.field(), msg.vali32());
					this->_setSaveType(obj, SAVE_UPDATE);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_MAPFIELD_SETI64:
			{
				Message::SyncAttrsMapField msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = this->GetObj(msg.id());
				if (obj) {
					obj->setFieldMap(msg.group(), msg.mapkey(), msg.field(), (int64)msg.vali64());
					this->_setSaveType(obj, SAVE_UPDATE);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_MAPFIELD_SETSTR:
			{
				Message::SyncAttrsMapField msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = this->GetObj(msg.id());
				if (obj) {
					obj->setFieldMap(msg.group(), msg.mapkey(), msg.field(), msg.valstr());
					this->_setSaveType(obj, SAVE_UPDATE);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_MAPFIELD_SETALL:
			{
				Message::SyncAttrs msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = this->GetObj(msg.id());
				if (obj) {
					obj->setFieldMap(msg.key(), msg.mapkey(), msg.jsonstr());
					this->_setSaveType(obj, SAVE_UPDATE);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_MAPFIELD_ADD:
			{
				Message::SyncAttrs msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = this->GetObj(msg.id());
				if (obj) {
					if (!obj->haveMember(msg.key())) {
						obj->addFieldObj(msg.key());
					}
					obj->addFieldMap(msg.key(), msg.mapkey(), msg.jsonstr());
					this->_setSaveType(obj, SAVE_UPDATE);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_MAPFIELD_DEL:
			{
				Message::SyncAttrs msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = this->GetObj(msg.id());
				if (obj) {
					obj->delFieldMap(msg.key(), msg.mapkey());
					this->_setSaveType(obj, SAVE_UPDATE);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_LOAD_REQUEST:
			{
				Message::ReqPlayerData msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = this->GetObj(msg.pid());
				if (obj) {
					_setStatusType(obj, STATUS_LOAD);
					DataModule.syncDataSeparate(obj, pack->GetNetID());
				} else {
					LoadModule.addToLoad(msg.type(), msg.key(), msg.pid(), pack->GetNetID(), STATUS_LOAD);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_LOGIN_REQUEST:
			{
			    Message::ReqPlayerData msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = this->GetObj(msg.pid());
				if (obj) {
					_setStatusType(obj, STATUS_ONLINE);
					DataModule.syncDataSeparate(obj, pack->GetNetID());
				} else {
					LoadModule.addToLoad(msg.type(), msg.key(), msg.pid(), pack->GetNetID(), STATUS_ONLINE);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_CREATE:
			{
				Message::SyncAttrs msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = create(msg.type().c_str(), msg.id());
				if (obj) {
					obj->fromJsonstringCompletely(msg.jsonstr());
					this->_setSaveType(obj, SAVE_INSERT);
				}
			}
			break;
		case Message::MSG_GAMEOBJ_REMOVE:
			{
				Message::SyncAttrs msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CDataObj* obj = this->GetObj(msg.id());
				if (obj) {
					_setStatusType(obj, STATUS_OFFLINE);
				}
			}
			break;
		default:	
			return false;
	}

	return true;
}

void CDataModule::onSave()
{
	uint64 t = GetTimeSec();
	int64 id = m_list.Head();
	while (CDataObj* obj = GetObj(id)) {
		id = m_list.Next(id);

		if (obj->m_save == SAVE_INSERT) {
			saveDb(obj, obj->m_type + "id");
			obj->m_save = SAVE_NONE;
		}
		else if (obj->m_save == SAVE_UPDATE) {
			updateDb(obj, obj->m_type + "id");
			obj->m_save = SAVE_NONE;
		}

		if ((obj->m_status == STATUS_OFFLINE || obj->m_status == STATUS_LOAD) && t - obj->m_stime > 5) {
			updateDb(obj, obj->m_type + "id");
			Log.Debug("[CDataModule] Delete: %s "INT64_FMT, obj->m_type.c_str(), obj->m_id);
			Delete(obj->m_id);
		}
	}
}
