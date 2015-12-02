#pragma once
#include "commdata.h"
#include "ObjMgr.h"


class CUser;
class CUserMgr : public CObjMgr< CUser, UserID >, public Singleton< CUserMgr >
{
public:
	CUserMgr()	{};
	~CUserMgr()	{};

};

#define UserMgr CUserMgr::getSingleton()
