#include "CommandMgr.h"
#ifdef _WIN32
#include <conio.h>
#elif defined(__linux__)
#include <stdlib.h>
//#include <curses.h>
#include "ThreadLibFunctions.h"
#include "memory.h"
#include "Log.h"
#include "string.h"
#endif

CCommandMgr::CCommandMgr()
{
	m_InputMode = false;
}

CCommandMgr::~CCommandMgr()
{

}

bool CCommandMgr::Startup()
{
	//创建控制台输入线程
	ThreadLib::Create(_ConsoleInputThread, this);

	return true;
}

void CCommandMgr::OnLogic()
{
	if( !m_InputMode )
		return;
	
	InputCmd* cmd = NULL;
	while( cmd = _GetHeadCmd() )
	{
		_HandleCommand(cmd);
	}
}

void CCommandMgr::AddCommand(char* szCmd)
{
	if( !szCmd )
		return;

	InputCmd* pCmd = NEW InputCmd;
	if( !pCmd )
		return;

	if( !_CommandParser(szCmd, pCmd) )
		return;

	m_CmdLock.LOCK();
	m_CmdList.AddToTail(pCmd);
	m_CmdLock.UNLOCK();
}

InputCmd* CCommandMgr::_GetHeadCmd()
{
	InputCmd * cmd = NULL;
	m_CmdLock.LOCK();
	if( m_CmdList.Count() > 0 )
	{
		cmd = m_CmdList.Head();
		m_CmdList.RemoveFromHead();
	}
	m_CmdLock.UNLOCK();
	return cmd;
}

bool CCommandMgr::_CommandParser(char* szCmd, InputCmd* pstCmd)
{
	if( !szCmd || !strlen(szCmd) || !pstCmd )
		return false;

	int n = 0;
	bool name = true;
	char* cursor = NULL;
	char* szTemp = szCmd;

	while( cursor = strchr(szTemp, '|') )
	{
		if( name )
		{
			if ((cursor - szTemp) < sizeof(pstCmd->m_name))
			{
				strncpy(pstCmd->m_name, szTemp, cursor - szTemp);
				pstCmd->m_name[cursor - szTemp] = '\0';
				name = false;
			}
			else
			{
				Log.Error("%s :指令名解析失败，文件: %s \t ; 行号 : %d\n",szCmd, __FILE__, __LINE__);
				return false;
			}
		}
		else
		{
			if ((cursor - szTemp) < sizeof(pstCmd->m_param[n]))
			{
				strncpy(pstCmd->m_param[n], szTemp, cursor - szTemp);
				pstCmd->m_param[n++][cursor - szTemp] = '\0';
			}
			else
			{
				Log.Error("%s :指令参数解析失败，文件: %s \t ; 行号 : %d\n",szCmd, __FILE__, __LINE__);
				return false;
			}
			
		}
			
		szTemp = cursor + 1;
	}

	if( '\0' != *szTemp)
	{
		if( name )
		{
			if (strlen(szTemp) < sizeof(pstCmd->m_name))
			{
				strcpy(pstCmd->m_name, szTemp);
				pstCmd->m_name[strlen(szTemp)] = '\0';
			}
			else
			{
				Log.Error("%s :指令名解析失败，文件: %s \t ; 行号 : %d\n",szCmd, __FILE__, __LINE__);
				return false;
			}
		}
		else
		{
			if (strlen(szTemp) < sizeof(pstCmd->m_param[n]))
			{
				strcpy(pstCmd->m_param[n], szTemp);
				pstCmd->m_param[n][strlen(szTemp)] = '\0';
			} 
			else
			{
				Log.Error("%s :指令参数解析失败，文件: %s \t ; 行号 : %d\n",szCmd, __FILE__, __LINE__);
				return false;
			}
			
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
void CCommandMgr::_ConsoleInputThread(void * param)
{
	if( !param )
		return;

	CCommandMgr * pThis = (CCommandMgr *)param;

	while (true)
	{
		if( !pThis->m_InputMode )
		{
#ifdef __linux__
			int nchar = getchar();
#else
			int nchar = getch();
#endif
			if( nchar == 68 )	//F10
			{
				pThis->m_InputMode = true;

#ifdef _WIN32
				system("cls");
#else
				system("clear");
#endif
				printf("Input command:\n");
			}
		}
		else
		{
			char szCmd[1024] = {0};
			//scanf("%s", szCmd);
			fgets(szCmd, sizeof(szCmd)-1, stdin);
			if( strlen(szCmd) )
			{
				szCmd[strlen(szCmd)-1] = '\0';	//输入流末尾会有\n
				pThis->AddCommand( szCmd );
			}
		}
	}
}
