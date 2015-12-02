#ifndef _GAMEWORLDMGR_H_
#define _GAMEWORLDMGR_H_

#include "Singleton.h"
#include "utlmap.h"
#include "types.h"

class CGameWorld
{
public:
	int		m_id;	
	int		m_port;
	char    m_ip[32];
};

class CGameWorldMgr : public Singleton< CGameWorldMgr >
{
public:
	CGameWorldMgr();
	~CGameWorldMgr();

	bool	LoadCSVData(const char* csvfile);

	SOCKET	GetGateSocket(int gateId);
	bool	IsConnGate(int gateId);

private:
	CUtlMap<int, SOCKET> m_mapGateSocket;
};

#define GameWorldMgr CGameWorldMgr::getSingleton()

#endif	//_GAMEWORLDMGR_H_
