#include "DataModule.h"
#include "IBaseObj.h"
#include "PlayerMgr.h"
#include "GameServer.h"
#include "LoginModule.h"
#include "attrs_defines.h"
#include "MessageTypeDefine.pb.h"
#include "MessageGameobj.pb.h"


void CDataModule::syncCreate(IBaseObj* obj, std::string type, int sock)
{
	std::string json;
	obj->Serialize(json);

	Message::SyncAttrs msg;
	msg.set_id(obj->GetID());
	msg.set_type(type);
	msg.set_jsonstr(json);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_CREATE);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncRemove(IBaseObj* obj, int sock)
{
	Message::SyncAttrs msg;
	msg.set_id(obj->GetID());

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_REMOVE);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncFieldInt(IBaseObj* obj, std::string group, int i, int sock)
{
	Message::SyncAttrsObjField msg;
	msg.set_id(obj->GetID());
	msg.set_group(group);
	msg.set_field(obj->GetFieldName(i));
	msg.set_vali32(obj->GetFieldInt(i));

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_OBJFIELD_SETI32);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncFieldI64(IBaseObj* obj, std::string group, int i, int sock)
{
	Message::SyncAttrsObjField msg;
	msg.set_id(obj->GetID());
	msg.set_group(group);
	msg.set_field(obj->GetFieldName(i));
	msg.set_vali64(obj->GetFieldI64(i));

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_OBJFIELD_SETI64);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncFieldStr(IBaseObj* obj, std::string group, int i, int sock)
{
	Message::SyncAttrsObjField msg;
	msg.set_id(obj->GetID());
	msg.set_group(group);
	msg.set_field(obj->GetFieldName(i));
	msg.set_valstr(obj->GetFieldStr(i));

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_OBJFIELD_SETSTR);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncGroupAll(IBaseObj* obj, std::string group, std::string json, int sock)
{
	Message::SyncAttrs msg;
	msg.set_id(obj->GetID());
	msg.set_key(group);
	msg.set_jsonstr(json);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_OBJFIELD_SETALL);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncAddMap(int64 id, IBaseObj* obj, std::string group, int sock)
{
	std::string json;
	obj->Serialize(json);

	char mapkey[32] = { 0 };
	sprintf(mapkey, INT64_FMT, obj->GetID());

	Message::SyncAttrs msg;
	msg.set_id(id);
	msg.set_key(group);
	msg.set_mapkey(mapkey);
	msg.set_jsonstr(json);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_MAPFIELD_ADD);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncDelMap(int64 id, IBaseObj* obj, std::string group, int sock)
{
	char mapkey[32] = { 0 };
	sprintf(mapkey, INT64_FMT, obj->GetID());

	Message::SyncAttrs msg;
	msg.set_id(id);
	msg.set_key(group);
	msg.set_mapkey(mapkey);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_MAPFIELD_DEL);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncSetMap(int64 id, IBaseObj* obj, std::string group, std::string field, int value, int sock)
{
	char mapkey[32] = { 0 };
	sprintf(mapkey, INT64_FMT, obj->GetID());

	Message::SyncAttrsMapField msg;
	msg.set_id(id);
	msg.set_group(group);
	msg.set_mapkey(mapkey);
	msg.set_field(field);
	msg.set_vali32(value);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_MAPFIELD_SETI32);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncSetMap(int64 id, IBaseObj* obj, std::string group, std::string field, int64 value, int sock)
{
	char mapkey[32] = { 0 };
	sprintf(mapkey, INT64_FMT, obj->GetID());

	Message::SyncAttrsMapField msg;
	msg.set_id(id);
	msg.set_group(group);
	msg.set_mapkey(mapkey);
	msg.set_field(field);
	msg.set_vali64(value);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_MAPFIELD_SETI64);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

void CDataModule::syncSetMap(int64 id, IBaseObj* obj, std::string group, std::string field, string value, int sock)
{
	char mapkey[32] = { 0 };
	sprintf(mapkey, INT64_FMT, obj->GetID());

	Message::SyncAttrsMapField msg;
	msg.set_id(id);
	msg.set_group(group);
	msg.set_mapkey(mapkey);
	msg.set_field(field);
	msg.set_valstr(value);

	Packet pack;
	PROTOBUF_CMD_PACKAGE(pack, msg, Message::MSG_GAMEOBJ_MAPFIELD_SETSTR);
	GETSERVERNET(&GameServer)->sendMsg(sock, &pack);
}

bool CDataModule::onMessage(Packet* pack)
{
	if (!pack)
		return false;

	switch(pack->Type()) {
		case Message::MSG_GAMEOBJ_SYNC:
			{
				Message::SyncAttrs msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				if (msg.type() == "player") {
					rapidjson::Document obj;
					obj.Parse<0>(msg.jsonstr().c_str());
					CPlayer* player = PlayerMgr.Create(obj["templateid"].GetInt(), msg.id());
					if (player) {
						player->Deserialize(obj);
					}
				}
			}
			break;
		case Message::MSG_GAMEOBJ_SYNC_OBJFIELD:
			{
				Message::SyncAttrs msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				if (msg.type() == "player") {
					CPlayer* player = PlayerMgr.GetObj(msg.id());
					if (player) {
						if (msg.key() == GROUP_ATTRS) {
							player->Deserialize(msg.jsonstr());
						}
					}
				}
			}
			break;
		case Message::MSG_GAMEOBJ_SYNC_MAPFIELD:
			{
				Message::SyncAttrs msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				if (msg.type() == "player") {
					CPlayer* player = PlayerMgr.GetObj(msg.id());
					if (player) {
						if (msg.key() == GROUP_ITEMS) {
							player->m_ItemUnit.LoadItem(atoll(msg.mapkey().c_str()), msg.jsonstr());
						}
					}
				}
			}
			break;
		case Message::MSG_GAMEOBJ_SYNC_FINISH:
			{
				Message::SyncAttrs msg;
				PROTOBUF_CMD_PARSER(pack, msg);

				if (msg.type() == "player") {
					LoginModule.eventPlayerLoadover(msg.id());
				}
			}
			break;
		default:	
			return false;
	}

	return true;
}
