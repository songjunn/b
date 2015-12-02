#include "DataModule.h"
#include "gtime.h"
#include "GameServer.h"
#include "LoginModule.h"
#include "MessageTypeDefine.pb.h"
#include "MessageGameobj.pb.h"


bool CDataModule::onLoadConfig(const char* xmlFile)
{
	TiXmlDocument xmlDoc(xmlFile);
	xmlDoc.LoadFile();

	if(xmlDoc.ErrorId() > 0)
		return false;

	TiXmlElement* pRootElement = xmlDoc.RootElement();
	if(!pRootElement)
		return false;

	rapidjson::Value root;
	root.SetObject();
	_parseClildNode(pRootElement, root);

	rapidjson::Value name;
	name.SetString(pRootElement->Value(), objTemplate.GetAllocator());
	objTemplate.AddMember(name, root, objTemplate.GetAllocator());

	return true;
}

void CDataModule::_parseClildNode(TiXmlElement* node, rapidjson::Value& json)
{
	TiXmlElement* pNode = node->FirstChildElement();
	while (pNode) {
		_parseXmlNode(pNode, json);
		pNode = pNode->NextSiblingElement();
	}
}

void CDataModule::_parseXmlNode(TiXmlElement* node, rapidjson::Value& json)
{
	rapidjson::Value jsonName;
	rapidjson::Value jsonValue;

	const char* name = node->Attribute("name");
	const char* type = node->Attribute("type");
	const char* value = node->Attribute("default");

	jsonName.SetString(name, objTemplate.GetAllocator());

	if (!strcmp(type, "int32")) {
		jsonValue.SetInt(atoi(value));
	}
	else if (!strcmp(type, "int64")) {
		jsonValue.SetInt64(atoll(value));
	}
	else if (!strcmp(type, "string")) {
		jsonValue.SetString(value, objTemplate.GetAllocator());
	}
	else if (!strcmp(type, "vector")) {
		jsonValue.SetArray();
		_parseClildNode(node, jsonValue);
	}
	else if (!strcmp(type, "map")) {
		jsonValue.SetObject();
		_parseClildNode(node, jsonValue);
	}
	else if (!strcmp(type, "obj")) {
		jsonValue.SetObject();
		_parseClildNode(node, jsonValue);
	}

	json.AddMember(jsonName, jsonValue, objTemplate.GetAllocator());
}

CMetadata* CDataModule::create(const char* type, int64 id)
{
	CMetadata* obj = Create(id);
	if (!obj)
		return NULL;

	obj->m_id = id;
	obj->m_type = type;

	Log.Debug("[CDataModule] Create: (%s, "INT64_FMT")", type, id);

	return obj;
}

CMetadata* CDataModule::createMetadata(const char* type, int64 id)
{
	CMetadata* obj = create(type, id);
	if (!obj)
		return NULL;

	rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    objTemplate[type].Accept(writer);
	obj->m_members.Parse<0>(buffer.GetString());

	return obj;
}

void CDataModule::syncCreate(CMetadata* obj, int sock)
{
	std::string json;
	obj->toJsonstring(json);

	Message::SyncObjField msg;
	msg.set_id(obj->m_id);
	msg.set_type(obj->m_type);
	msg.set_jsonstr(json);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_CREATE);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncRemove(CMetadata* obj, int sock)
{
	Message::SyncObjField msg;
	msg.set_id(obj->m_id);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_REMOVE);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncField(CMetadata* obj, int sock, const char* field)
{
	std::string json;
	obj->toJsonstring(json, field);

	Message::SyncObjField msg;
	msg.set_id(obj->m_id);
	msg.set_key(field);
	msg.set_jsonstr(json);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_OBJFIELD_SET);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncAddMap(int64 id, int sock, const char* field, int64 mapkey, const char* jsonstr)
{
	Message::SyncObjField msg;
	msg.set_id(id);
	msg.set_key(field);
	msg.set_mapkey(mapkey);
	msg.set_jsonstr(jsonstr);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_MAPFIELD_ADD);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncSetMap(int64 id, int sock, const char* field, int64 mapkey, const char* jsonstr)
{
	Message::SyncObjField msg;
	msg.set_id(id);
	msg.set_key(field);
	msg.set_mapkey(mapkey);
	msg.set_jsonstr(jsonstr);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_MAPFIELD_SET);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncDelMap(int64 id, int sock, const char* field, int64 mapkey)
{
	Message::SyncObjField msg;
	msg.set_id(id);
	msg.set_key(field);
	msg.set_mapkey(mapkey);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_MAPFIELD_DEL);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

bool CDataModule::onMessage(Packet* pack)
{
	if (!pack)
		return false;

	switch(pack->Type()) {
		case Message::MSG_GAMEOBJ_SYNC:
			{
				Message::SyncObjField msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CMetadata* obj = this->createMetadata(msg.type().c_str(), msg.id());
				if (obj) {
					obj->fromJsonstring(msg.jsonstr());
				}
			}
			break;
		case Message::MSG_GAMEOBJ_SYNC_OBJFIELD:
			{
				Message::SyncObjField msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CMetadata* obj = this->GetObj(msg.id());
				if (obj) {
					obj->fromJsonstring(msg.jsonstr(), msg.key());
				}
			}
			break;
		case Message::MSG_GAMEOBJ_SYNC_MAPFIELD:
			{
				Message::SyncObjField msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				CMetadata* obj = this->GetObj(msg.id());
				if (obj && obj->haveMember(msg.key())) {
					obj->addFieldMap(msg.key(), msg.mapkey(), msg.jsonstr());
				}
			}
			break;
		case Message::MSG_GAMEOBJ_SYNC_FINISH:
			{
				Message::SyncObjField msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				if (msg.type() == "player") {
					CMetadata* obj = this->GetObj(msg.id());
					if (obj) {
						LoginModule.eventPlayerLoadover(msg.id());
					}
				}
			}
			break;
		default:	
			return false;
	}

	return true;
}