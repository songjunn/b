#pragma once
#include "Singleton.h"
#include "commdata.h"
#include "utlmap.h"

class CPlayer;
class Packet;
class CLoginModule : public Singleton< CLoginModule >
{
public:
	CLoginModule();
	~CLoginModule();

	bool	OnMsg(Packet* pack);

	void	eventPlayerLoadover(PersonID id);

	bool	OnPlayerLogin(CPlayer* player);
	bool	OnPlayerLogout(PersonID id);

protected:
	bool	_HandlePacket_UserLogin(Packet* pack);
	bool	_HandlePacket_PlayerLogout(Packet* pack);
	bool	_HandlePacket_PlayerCount(Packet* pack);
	bool	_HandlePacket_PlayerCreate(Packet* pack);
	bool	_HandlePacket_PlayerOnCreate(Packet* pack);
	bool	_HandlePacket_LoadWorldData(Packet* pack);

	bool	_OnPlayerSync(CPlayer* player);

protected:
	CUtlMap<PersonID, PersonID>		m_LoginMap;

};

#define LoginModule CLoginModule::getSingleton()
