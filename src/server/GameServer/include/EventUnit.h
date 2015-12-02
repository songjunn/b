#pragma once
#include "Event.h"
#include "utllinkedlist.h"


class CPlayer;
class CEventUnit
{
public:
	CEventUnit();
	~CEventUnit();

	void	Initialize(CPlayer* parent);
	void	Release();

	bool	OnLogic();

	void	AddEvent(CEvent* ev);
	void	OnEvent(CEvent* ev);

protected:
	CPlayer*		m_parent;
	CUtlLinkedList<CEvent*>	m_EventList;

};
