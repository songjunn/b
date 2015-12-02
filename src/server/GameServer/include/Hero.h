#pragma once
#include "Fighter.h"

class CHero : public CFighter
{
public:
	CHero() {}
	virtual ~CHero() {}

	void	Init() { return; }
	void	Release() { return; }
	bool	OnCreate(int templateid) { return false; }

};
