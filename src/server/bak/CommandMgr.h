#ifndef SERVERCORE_COMMANDMGR_H
#define SERVERCORE_COMMANDMGR_H

#ifdef __linux__
#include <string.h>
#include "types.h"
#include "Singleton.h"
#include "Lock.h"
#else
#include "ServerCoreInc.h"
#endif

#include "stllist.h"

struct InputCmd
{
	InputCmd()
	{
		memset(m_name, 0, 20);
		for(int i=0;i<5;++i)
			memset(m_param[i], 0, 10);
	}

	char	m_name[20];
	char	m_param[5][32];
};

class CCommandMgr : public Singleton< CCommandMgr >
{
public:
	CCommandMgr();
	~CCommandMgr();

	bool	Startup();
	void	OnLogic();
	void	AddCommand(char* szCmd);

protected:
	bool			_CommandParser(char* szCmd, InputCmd* pstCmd);
	InputCmd*		_GetHeadCmd();
	virtual bool	_HandleCommand(InputCmd* cmd)	{return true;}

	static void _ConsoleInputThread(void * param);

protected:
	bool					m_InputMode;
	Mutex					m_CmdLock;
	CStlList<InputCmd*>		m_CmdList;

};

#define CommandMgr CCommandMgr::getSingleton()

#endif	//SERVERCORE_COMMANDMGR_H
