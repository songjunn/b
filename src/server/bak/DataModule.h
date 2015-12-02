#ifndef _DATAMODULE_H_
#define _DATAMODULE_H_

#include "types.h"
#include "Singleton.h"
#include "tinyxml.h"   
#include "tinystr.h"
#include "ObjMgr.h"
#include "metadata.h"


class Packet;
class CDataModule : public CObjMgr< CMetadata, int64 >, public Singleton< CDataModule >
{
public:
	CDataModule()	{objTemplate.SetObject();}
	~CDataModule()	{}

	bool onMessage(Packet* pack);
	bool onLoadConfig(const char* xmlFile);

	CMetadata* create(const char* type, int64 id);
	CMetadata* createMetadata(const char* type, int64 id);

	void syncCreate(CMetadata* obj, int sock);
	void syncRemove(CMetadata* obj, int sock);
	void syncField(CMetadata* obj, int sock, const char* field);
	void syncAddMap(int64 id, int sock, const char* field, int64 mapkey, const char* jsonstr);
	void syncSetMap(int64 id, int sock, const char* field, int64 mapkey, const char* jsonstr);
	void syncDelMap(int64 id, int sock, const char* field, int64 mapkey);

protected:
	void _parseXmlNode(TiXmlElement* node, rapidjson::Value& json);
	void _parseClildNode(TiXmlElement* node, rapidjson::Value& json);

protected:
	rapidjson::Document objTemplate;

};

#define DataModule CDataModule::getSingleton()

#endif	//_DATAMODULE_H_