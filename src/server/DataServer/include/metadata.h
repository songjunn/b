#ifndef _METADATA_H_
#define _METADATA_H_
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include "types.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "bson/bson.h"
#include "plugin_Mongodb.h"


#define TRAVERSE_METADATA_MAP(obj, key, jsonstr, id)				\
	rapidjson::Value& member = obj->getFieldObj(key);				\
	for (rapidjson::Value::MemberIterator it = member.MemberBegin(); it != member.MemberEnd(); ++it) {	\
		std::string jsonstr;										\
		int64 id = atoll(it->name.GetString());						\
		rapidjson::Value& itor = member[it->name.GetString()];		\
		obj->toJsonstring(itor, jsonstr);							\

#define TRAVERSE_METADATA_VECTOR(obj, key, value)					\
	for (int idx = 0; idx < obj->getVecCount(key); ++idx) {			\
		rapidjson::Value& member = obj->getFieldVec(key, idx);		\
		int64 value = member.GetInt64();							\

#define GETINT_FROM_JSONSTR(json, key, value)						\
	rapidjson::Document document;									\
	document.Parse<0>(json.c_str());								\
	int value = CMetadata::getFieldInt(document[key]);				\

#define GETI64_FROM_JSONSTR(json, key, value)						\
	rapidjson::Document document;									\
	document.Parse<0>(json.c_str());								\
	int64 value = CMetadata::getFieldI64(document[key]);			\


class CMetadata
{
public:
	CMetadata()	{m_members.SetObject();}
	virtual ~CMetadata()	{}

	inline int64 getId() {return m_id;}
	inline bool haveMember(std::string key) {return _hasMember(m_members, key);}

	int getFieldInt(std::string key);
	void setFieldInt(std::string key, int value);
	
	int64 getFieldI64(std::string key);
	void setFieldI64(std::string key, int64 value);

	std::string getFieldString(std::string key);
	void setFieldString(std::string key, std::string value);

	int getFieldObjInt(std::string group, std::string key);
	void setFieldObjInt(std::string group, std::string key, int value);
	
	int64 getFieldObjI64(std::string group, std::string key);
	void setFieldObjI64(std::string group, std::string key, int64 value);

	std::string getFieldObjString(std::string group, std::string key);
	void setFieldObjString(std::string group, std::string key, std::string value);

	int getVecCount(std::string key);
	rapidjson::Value& getFieldVec(std::string key, int idx);
	template<typename T> void addFieldVec(std::string key, T value) {
		T val = value;
		rapidjson::Value& member = this->getFieldVal(m_members, key);
		this->_addFieldVec(member, val);
	}
	template<typename T> void setFieldVec(std::string key, int idx, T value) {
		T val = value;
		rapidjson::Value& member = this->getFieldVal(m_members, key);
		this->_setFieldVec(member, val, idx);
	}

	void addFieldMap(std::string key, std::string mapkey, std::string jsonstring);
	void delFieldMap(std::string key, std::string mapkey);
	void setFieldMap(std::string key, std::string mapkey, std::string jsonstring);
	void setFieldMap(std::string key, std::string mapkey, std::string field, int value);
	void setFieldMap(std::string key, std::string mapkey, std::string field, int64 value);
	void setFieldMap(std::string key, std::string mapkey, std::string field, std::string value);

	void addFieldObj(std::string key);
	rapidjson::Value& getFieldObj(std::string key);

	void toBson(mongo::BSONObj& bson);
	void toBson(mongo::BSONObj& bson, std::string key);

	void fromBson(mongo::BSONObj& bson);
	void fromBson(mongo::BSONObj& bson, std::string key);

	void toProtobuf(google::protobuf::Message& message);
	void toProtobuf(google::protobuf::Message& message, std::string key);

	void fromProtobuf(google::protobuf::Message& message);
	void fromProtobuf(google::protobuf::Message& message, std::string key);

	void toJsonstring(std::string& json);
	void toJsonstring(std::string& json, std::string key);
	static void toJsonstring(rapidjson::Value& json, std::string& str);

	void fromJsonstring(std::string json);
	void fromJsonstring(std::string json, std::string key);

	void fromJsonstringCompletely(std::string json);
	void fromJsonstringCompletely(std::string json, std::string key);

	static bool isFieldInt(rapidjson::Value& json) {return json.IsInt();}
	static bool isFieldI64(rapidjson::Value& json) {return json.IsInt64();}
	static bool isFieldVec(rapidjson::Value& json) {return json.IsArray();}
	static bool isFieldObj(rapidjson::Value& json) {return json.IsObject();}
	static bool isFieldStr(rapidjson::Value& json) {return json.IsString();}

	static int getFieldInt(rapidjson::Value& json) {return json.GetInt();}
	static int64 getFieldI64(rapidjson::Value& json) {return json.GetInt64();}
	static std::string getFieldStr(rapidjson::Value& json) {return json.GetString();}
	static rapidjson::Value& getFieldVec(rapidjson::Value& json, int key) {return json[key];}
	static rapidjson::Value& getFieldVal(rapidjson::Value& json, std::string key) {return json[key.c_str()];}

protected:
	void _vecJsonToBson(rapidjson::Value& json, mongo::BSONArrayBuilder& vecBuilder);
	void _objJsonToBson(rapidjson::Value& json, mongo::BSONObjBuilder& builder);
	void _fieldJsonToBson(rapidjson::Value& json, std::string key, mongo::BSONObjBuilder& builder);

	void _vecBsonToJson(rapidjson::Value& json, mongo::vector<mongo::BSONElement>& value);
	void _objBsonToJson(rapidjson::Value& json, mongo::BSONObj& bson);
	void _fieldBsonToJson(rapidjson::Value& json, std::string key, mongo::BSONObj& bson);

	void _vecJsonToJson(rapidjson::Value& desjson, rapidjson::Value& srcjson);
	void _objJsonToJson(rapidjson::Value& desjson, rapidjson::Value& srcjson);
	void _fieldJsonToJson(rapidjson::Value& desjson, std::string key, rapidjson::Value& srcjson);

	inline void _setFieldInt(rapidjson::Value& json, int value) {json.SetInt(value);}
	inline void _setFieldI64(rapidjson::Value& json, int64 value) {json.SetInt64(value);}
	inline void _setFieldStr(rapidjson::Value& json, std::string value) {json.SetString(value.c_str(), m_members.GetAllocator());}
	inline void _setFieldVal(rapidjson::Value& json, rapidjson::Value& value) {json = value;}

	template<typename T> inline void _addFieldVec(rapidjson::Value& json, T& value) {json.PushBack(value, m_members.GetAllocator());}
	template<typename T> inline void _setFieldVec(rapidjson::Value& json, T& value, int idx) {json[idx] = value;}

	inline void _addFiledVal(rapidjson::Value& json, std::string key, rapidjson::Value& value) {json.AddMember(key.c_str(), m_members.GetAllocator(), value, m_members.GetAllocator());}
	inline void _delFieldVal(rapidjson::Value& json, std::string key) {json.RemoveMember(key.c_str());}

	inline int _getVecCount(rapidjson::Value& json) {return json.Size();}
	inline bool	_hasMember(rapidjson::Value& json, std::string key) {return json.HasMember(key.c_str());}

public:
	int64 m_id;
	std::string m_type;
	rapidjson::Document m_members;

};

#endif //_METADATA_H_
