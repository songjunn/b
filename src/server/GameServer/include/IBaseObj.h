#pragma once
#include <string>
#include <string.h>
#include "commdata.h"
#include "rapidjson/document.h"

using namespace std;

class CEvent;
class CPlayer;
class Packet;
class IBaseObj
{
public:
	IBaseObj() : m_ObjID(0), m_type(0), m_templateId(0) {}
	virtual ~IBaseObj() {}

	virtual void Init();
	virtual void Copy(const IBaseObj& obj);
	virtual	void OnEvent(CEvent* ev)	{ return; }

	int		GetFieldInt(int i);
	int64	GetFieldI64(int i);
	string	GetFieldStr(int i);

	void	SetFieldInt(int i, int value, bool client = false, bool data = false);
	void	SetFieldI64(int i, int64 value, bool client = false, bool data = false);
	void	SetFieldStr(int i, std::string value, bool client = false, bool data = false);

	void	ChangeFieldInt(int i, int value, bool client = false, bool data = false);
	void	ChangeFieldI64(int i, int64 value, bool client = false, bool data = false);

	inline	int		GetTemplateID()			{return m_templateId;}
	inline  void	SetTemplateID(int id)	{m_templateId = id;}

	inline	int64	GetID()					{return m_ObjID;}
	inline  void	SetID(int64 id)			{m_ObjID = id;}

	inline	string	GetName()					{ return m_name; }
	inline	void	SetName(const char* value)	{ m_name = value; }

	virtual void	SendClientMsg(Packet* pack)	{return;}
	virtual	void	SendDataMsg(Packet* pack)	{return;}
	virtual	void	SendObserveMsg(Packet* pack, CPlayer* player)	{return;}

	virtual void	Serialize(string& jsonstr) { return; }
	virtual void	Serialize(string name, rapidjson::Document& root) { return; }
	virtual void	Deserialize(string jsonstr) { return; }
	virtual void	Deserialize(rapidjson::Value& json) { return; }

	virtual string	GetFieldName(int i)			{ return ""; }
	virtual int		GetFieldType(string name)	{ return 0; }

	virtual void	SyncFieldIntToData(int i)	{ return; }
	virtual void	SyncFieldI64ToData(int i)	{ return; }
	virtual void	SyncFieldStrToData(int i)	{ return; }
	virtual void	SyncFieldIntToClient(int i, CPlayer* toPlayer = NULL)	{ return; }
	virtual void	SyncFieldI64ToClient(int i, CPlayer* toPlayer = NULL)	{ return; }
	virtual void	SyncFieldStrToClient(int i, CPlayer* toPlayer = NULL)	{ return; }

protected:
	virtual int*	_FindFieldInt(int i)	{ return NULL; }
	virtual int64*	_FindFieldI64(int i)	{ return NULL; }
	virtual string* _FindFieldStr(int i)	{ return NULL; }
	virtual void	_ChangeRelatedField(int i, bool client = false, bool data = false)	{ return; }

	void	_SerializeFieldInt(int i, rapidjson::Value& json, rapidjson::Document& root);
	void	_SerializeFieldI64(int i, rapidjson::Value& json, rapidjson::Document& root);
	void	_SerializeFieldStr(int i, rapidjson::Value& json, rapidjson::Document& root);

protected:
	int64	m_ObjID;
	int		m_type;
	int		m_templateId;
	string	m_name;

};
