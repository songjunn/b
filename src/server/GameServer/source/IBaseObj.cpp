#include "IBaseObj.h"

void IBaseObj::Init()
{
	m_ObjID = 0;
	m_type = 0;
	m_templateId = 0;
	m_name = "";
}

void IBaseObj::Copy(const IBaseObj& obj)
{
	m_ObjID = obj.m_ObjID;
	m_type = obj.m_type;
	m_templateId = obj.m_templateId;
	m_name = obj.m_name;
}

int	IBaseObj::GetFieldInt(int i)
{
	int* field = _FindFieldInt(i);
	if (!field)
		return INVALID_VALUE;
	return *field;
}

int64 IBaseObj::GetFieldI64(int i)
{
	int64* field = _FindFieldI64(i);
	if (!field)
		return INVALID_VALUE;
	return *field;
}

string IBaseObj::GetFieldStr(int i)
{
	string* field = _FindFieldStr(i);
	if (!field)
		return "";
	return *field;
}

void IBaseObj::SetFieldInt(int i, int value, bool client, bool data)
{
	int* field = _FindFieldInt(i);
	if (!field)
		return;
	*field = value;

	_ChangeRelatedField(i, client, data);

	if (client)
		SyncFieldIntToClient(i);
	if (data)
		SyncFieldIntToData(i);
}

void IBaseObj::SetFieldI64(int i, int64 value, bool client, bool data)
{
	int64* field = _FindFieldI64(i);
	if (!field)
		return;
	*field = value;

	_ChangeRelatedField(i, client, data);

	if (client)
		SyncFieldI64ToClient(i);
	if (data)
		SyncFieldI64ToData(i);
}

void IBaseObj::SetFieldStr(int i, string value, bool client, bool data)
{
	string* field = _FindFieldStr(i);
	if (!field)
		return;
	*field = value;

	if (client)
		SyncFieldStrToClient(i);
	if (data)
		SyncFieldStrToData(i);
}

void IBaseObj::ChangeFieldInt(int i, int value, bool client, bool data)
{
	if (value == 0)
		return;

	int* field = _FindFieldInt(i);
	if (!field)
		return;
	*field += value;

	_ChangeRelatedField(i, client, data);

	if (client)
		SyncFieldIntToClient(i);
	if (data)
		SyncFieldIntToData(i);
}

void IBaseObj::ChangeFieldI64(int i, int64 value, bool client, bool data)
{
	if (value == 0)
		return;

	int64* field = _FindFieldI64(i);
	if (!field)
		return;
	*field += value;
	*field = *field >= 0 ? *field : 0;

	_ChangeRelatedField(i, client, data);

	if (client)
		SyncFieldI64ToClient(i);
	if (data)
		SyncFieldI64ToData(i);
}

void IBaseObj::_SerializeFieldInt(int i, rapidjson::Value& json, rapidjson::Document& root)
{
	rapidjson::Value jsonName;
	rapidjson::Value jsonValue;

	jsonName.SetString(GetFieldName(i).c_str(), root.GetAllocator());
	jsonValue.SetInt(GetFieldInt(i));

	json.AddMember(jsonName, jsonValue, root.GetAllocator());
}

void IBaseObj::_SerializeFieldI64(int i, rapidjson::Value& json, rapidjson::Document& root)
{
	rapidjson::Value jsonName;
	rapidjson::Value jsonValue;

	jsonName.SetString(GetFieldName(i).c_str(), root.GetAllocator());
	jsonValue.SetInt64(GetFieldI64(i));

	json.AddMember(jsonName, jsonValue, root.GetAllocator());
}

void IBaseObj::_SerializeFieldStr(int i, rapidjson::Value& json, rapidjson::Document& root)
{
	rapidjson::Value jsonName;
	rapidjson::Value jsonValue;

	jsonName.SetString(GetFieldName(i).c_str(), root.GetAllocator());
	jsonValue.SetString(GetFieldStr(i).c_str());

	json.AddMember(jsonName, jsonValue, root.GetAllocator());
}
