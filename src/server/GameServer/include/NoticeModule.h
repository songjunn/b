#pragma once
#include "commdata.h"
#include "Singleton.h"
#include "ObjMgr.h"
#include "utllinkedlist.h"


struct CNotice
{
	int		m_errid;
	int		m_int[3];
	int64	m_int64[3];
	std::string m_string[3];
};

class CFighter;
class CPlayer;
class CNoticeModule : public CObjMgr< CNotice, int >, public Singleton< CNoticeModule >
{
public:
	CNoticeModule();
	~CNoticeModule();

	void	OnLogic();

	void	SendBroadcast(CPlayer* player, CNotice* notice);		//发送系统广播
	void	SendBroadcast(int errid, int int1=0, int int2=0, int int3=0, int64 long1=0, int64 long2=0, int64 long3=0, const char* str1=0, const char* str2=0, const char* str3=0);						//发送系统广播

	void	SendErrorMsg(CFighter* role, int errid, int int1=0, int int2=0, int int3=0, int64 long1=0, int64 long2=0, int64 long3=0, const char* str1=0, const char* str2=0, const char* str3=0);		//发送提示信息
	void	SendErrorMsg(int sock, UserID uid, int errid, int int1=0, int int2=0, int int3=0, int64 long1=0, int64 long2=0, int64 long3=0, const char* str1=0, const char* str2=0, const char* str3=0);	//发送提示信息

protected:
	int		m_key;
	CUtlLinkedList<std::string>	m_BroadcastList;

};

#define NoticeModule CNoticeModule::getSingleton()
