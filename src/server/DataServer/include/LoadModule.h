#ifndef _LOADMODULE_H_
#define _LOADMODULE_H_

#include "types.h"
#include "Singleton.h"
#include "utllinkedlist.h"


struct CLoadObj
{
	int64 id;
	std::string key;
	std::string type;
	int sock;
	int status;
};

class Packet;
class CLoadModule : public Singleton< CLoadModule >
{
public:
	CLoadModule()	{}
	~CLoadModule()	{}

	bool	onMessage(Packet* pack);
	void	onLogic();

	void	addToLoad(std::string type, std::string key, int64 id, int sock, int status);

protected:
	bool	_handlePacket_LoadWorldData(Packet* pack);
	bool	_handlePacket_LoadPlayerCount(Packet* pack);
	bool	_handlePacket_CheckNameRepeat(Packet* pack);

protected:
	CUtlLinkedList<CLoadObj*> m_LoadList;

};

#define LoadModule CLoadModule::getSingleton()

#endif	//_LOADMODULE_H_
