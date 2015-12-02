#include "GameWorldMgr.h"
#include "MainServer.h"
#include "CSVParser.h"
#include "Log.h"


CGameWorldMgr::CGameWorldMgr()
{
	SetDefLessFunc(m_mapGateSocket);
}

CGameWorldMgr::~CGameWorldMgr()
{
}

bool CGameWorldMgr::LoadCSVData(const char* csvfile)
{
	CCSVParser file;
	if (!csvfile || !file.Load(csvfile))
	{
		Log.Error("[InitData] Load File Failed:%s", csvfile);
		return false;
	}

	int index = 0;
	int count = file.Line();

	for (int i = 0; i<count; ++i, index = 0)
	{
		int id = 0;
		int port = 0;
		char ip[32] = { 0 };
		file.GetAttribute(i, index++, id);
		file.GetAttribute(i, index++, ip, 32);
		file.GetAttribute(i, index++, port);

		CServerObj *pServer = GETSERVERMGR->CreateServer(CServerMgr::Svr_GateWay, id, port, ip, NULL, NULL, 0, true);
		if (!pServer) {
			Log.Error("CGameWorldMgr CreateServer Failed.");
			return false;
		}

		m_mapGateSocket.InsertOrReplace(id, pServer->m_Socket);
	}

	file.Release();

	return true;
}

SOCKET CGameWorldMgr::GetGateSocket(int gateId)
{
	if (!IsConnGate(gateId)) {
		return INVALID_SOCKET;
	}

	return m_mapGateSocket.Element(m_mapGateSocket.Find(gateId));
}

bool CGameWorldMgr::IsConnGate(int gateId)
{
	return m_mapGateSocket.IsValidIndex(m_mapGateSocket.Find(gateId));
}

