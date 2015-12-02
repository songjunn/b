#ifndef SHARED_CONSOLE_H
#define	SHARED_CONSOLE_H

#ifdef WIN32
#include <windows.h>
#else
#ifdef FREEBSD
#include "unixdomsvr.h"
#endif
#endif //WIN32

#ifdef WIN32
#define CONSOLE_TOP		8
static int g_nConsoleTop = CONSOLE_TOP;
#endif

void ConsoleScroll(int nTop,int nBottom);
void ConsoleSetSize(int nCharLen,int nLine);

void Print(int nLine,const char* psz,...);

#endif	//SHARED_CONSOLE_H
