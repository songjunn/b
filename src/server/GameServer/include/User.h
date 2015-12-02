#pragma once
#include <string.h>
#include "commdata.h"
#include "utllinkedlist.h"
#include "GameServer.h"
#include "PlayerMgr.h"


class CUser
{
public:
	CUser() : m_ID(INVALID_VALUE), m_PlayerID(INVALID_VALUE), m_GateSocket(INVALID_SOCKET)
	{ }
	~CUser()
	{ }

	//inline	void	RelatePlayer(PersonID id)		{int i = m_PlayerList.Find(id); if(!m_PlayerList.IsValidIndex(i)) m_PlayerList.AddToTail(id);}
	//inline	bool	HavePlayer()					{return m_PlayerList.Count() > 0;}

	inline PersonID GetLoginPlayer()				{return m_PlayerID;}
	inline	bool	HavePlayer()					{return m_PlayerID > 0;}
	inline	void	RelatePlayer(PersonID id)		
	{
		if( m_PlayerID > 0 )
		{
			LOGGER_ERROR("[CUser] RelatePlayer Error, user:"INT64_FMT" oldplayer:"INT64_FMT" newplayer:"INT64_FMT, m_ID, m_PlayerID, id);
		}

		m_PlayerID = id;
		LOGGER_DEBUG("[CUser] RelatePlayer, user:"INT64_FMT" player:"INT64_FMT, m_ID, m_PlayerID);
	}

	inline  void	SendGateMsg(Packet* pack)	{ if(pack) GETSERVERNET(&GameServer)->sendMsg(m_GateSocket, pack); }

public:
	UserID	m_ID;
	PersonID m_PlayerID;
	int		m_GateSocket;

	//CUtlLinkedList<PersonID>	m_PlayerList;

};
