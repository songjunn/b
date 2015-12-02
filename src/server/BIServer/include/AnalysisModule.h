#pragma once
#include "commdata.h"
#include "Singleton.h"


class Packet;
class CAnalysisModule : public Singleton< CAnalysisModule >
{
public:
	CAnalysisModule();
	~CAnalysisModule();

	void	OnLogic();
	bool	OnMsg(Packet* pack);

	inline int GetOnlinePlayer() {return m_OnlinePlayer;}
	
protected:
	bool	_HandlePacket_EventSync(Packet* pack);

	int		_AnalysisEvent(int nEventID, int64 parent, int64 PID, int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4, int64 nParam5, int64 nParam6, const char* szParam1);
	void	_SaveEvent(int nEventID, int64 parent, int64 PID, const char* stime, int64 nParam1, int64 nParam2, int64 nParam3, int64 nParam4, int64 nParam5, int64 nParam6, const char* szParam1);
	void	_SaveServerState();
	void _GetPlayerCountFromDB();
	void _SaveServerInfoToDB();

protected:
	int		m_OnlinePlayer;
	int        m_CreatePlayerCount; //创建角色总数
	int64       m_GoldCount; //购买金币总数
};

#define AnalysisModule CAnalysisModule::getSingleton()
