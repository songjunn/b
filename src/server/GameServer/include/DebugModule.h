#pragma once
#include "Singleton.h"
#include "Packet.h"

class CDebugModule: public Singleton<CDebugModule>
{
public:
	CDebugModule(){}
	~CDebugModule(){}
public:
	bool OnMsg(Packet* pack);
	bool _handleDebug(Packet* pack);

};

#define DebugModule CDebugModule::getSingleton()
