#ifndef _DATAMODULE_H_
#define _DATAMODULE_H_

#include "types.h"
#include "Singleton.h"
#include "ObjMgr.h"
#include "metadata.h"


class CDataObj : public CMetadata
{
public:
	int m_save;
	int m_status;
	uint64 m_stime;
};


class Packet;
class CDataModule : public CObjMgr< CDataObj, int64 >, public Singleton< CDataModule >
{
	enum StatusType {
		STATUS_NONE = 0,
		STATUS_ONLINE,
		STATUS_OFFLINE,
		STATUS_LOAD,
	};

	enum SaveType {
		SAVE_NONE = 0,
		SAVE_INSERT,
		SAVE_UPDATE,
		SAVE_DELETE,
	};

public:
	CDataModule()	{}
	~CDataModule()	{}

	void onSave();
	bool onMessage(Packet* pack);

	CDataObj* create(std::string type, int64 id);
	CDataObj* loadBson(std::string type, mongo::BSONObj& bson, int64 id);
	CDataObj* loadDb(int status, std::string type, std::string key, int64 id);
	bool loadDb(int status, std::string type);
	void saveDb(CDataObj* obj, std::string key);
	void updateDb(CDataObj* obj, std::string key);
	void updateDb(int64 id, std::string key);

	void syncData(CDataObj* obj, int sock);
	void syncData(std::string type, int sock);
	void syncDataSeparate(CDataObj* obj, int sock);
	void syncDataObj(CDataObj* obj, std::string key, int sock);
	void syncDataMap(CDataObj* obj, rapidjson::Value& json, std::string key, std::string mapkey, int sock);
	void syncDataFinish(int sock, std::string type, int64 id = 0);

protected:
	void _setSaveType(CDataObj* obj, int type);
	void _setStatusType(CDataObj* obj, int status);

};

#define DataModule CDataModule::getSingleton()

#endif	//_DATAMODULE_H_
