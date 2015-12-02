#include <stdio.h>
#include "console.h"
#ifdef __linux__
#include <string.h>
#include <stdarg.h>
#include "udsvr.h"
#endif


void ConsoleScroll(int nTop,int nBottom)
{
#ifdef WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	SMALL_RECT srctScrollRect, srctClipRect;
	CHAR_INFO chiFill;
	COORD coordDest;

	HANDLE hStdout;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// Get the screen buffer size.

	if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
	{
		//printf("GetConsoleScreenBufferInfo failed %d\n", GetLastError());
		return;
	}


	//CONSOLE_SCREEN_BUFFER_INFO  buffinfo;
	//GetConsoleScreenBufferInfo(hStdout,&buffinfo);
	if(csbiInfo.dwCursorPosition.Y< csbiInfo.dwSize.Y-1-nBottom-1)
	{
		return;
	}

	// The scrolling rectangle is the bottom 15 rows of the
	// screen buffer.
	srctScrollRect.Top =    nTop;
	srctScrollRect.Bottom = csbiInfo.dwSize.Y -1-nBottom;
	srctScrollRect.Left = 0;
	srctScrollRect.Right = csbiInfo.dwSize.X - 1;

	// The destination for the scroll rectangle is one row up.

	coordDest.X = 0;
	coordDest.Y = nTop-1;

	// The clipping rectangle is the same as the scrolling rectangle.
	// The destination row is left unchanged.

	srctClipRect = srctScrollRect;

	// Fill the bottom row with green blanks.

	chiFill.Attributes = 0;//BACKGROUND_INTENSITY| FOREGROUND_INTENSITY;
	chiFill.Char.AsciiChar = (char)' ';

	// Scroll up one line.

	if(!ScrollConsoleScreenBuffer(
		hStdout,         // screen buffer handle
		&srctScrollRect, // scrolling rectangle
		&srctClipRect,   // clipping rectangle
		coordDest,       // top left destination cell
		&chiFill))       // fill character and color
	{
		//printf("ScrollConsoleScreenBuffer failed %d\n", GetLastError());
		return;
	}

	COORD curPos=csbiInfo.dwSize;
	curPos.X=0;
	curPos.Y=csbiInfo.dwSize.Y -1-nBottom-2;

	SetConsoleCursorPosition(hStdout,curPos);
#endif	//WIN32

}

void ConsoleSetSize(int nCharLen,int nLine)
{
#ifdef WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
	{
		return;
	}


	COORD sz={nCharLen,nLine};
	/*BOOL bRet = */SetConsoleScreenBufferSize(hStdout,sz);

	//这里设置窗口大小
	//SMALL_RECT rt={0,0,nCharLen-1,nLine-1};
	//SetConsoleWindowInfo(hStdout,1,&rt);
#endif	//WIN32

}


void Print(int nLine,const char* psz,...)
{
#ifdef WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	HANDLE hStdout;
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	//保存光标位置
	if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
		return;

	//跳转
	COORD curPos=csbiInfo.dwSize;
	curPos.X=0;
	curPos.Y=nLine;
	SetConsoleCursorPosition(hStdout,curPos);

	va_list arg_ptr;
	va_start(arg_ptr, psz);
	vprintf(psz,arg_ptr);
	va_end(arg_ptr);

	if(nLine>=g_nConsoleTop)
		g_nConsoleTop=nLine+1;

	curPos=csbiInfo.dwCursorPosition;
	if(curPos.Y<g_nConsoleTop)
		curPos.Y=g_nConsoleTop;
	SetConsoleCursorPosition(hStdout,curPos);
#else

	//	printf( "Print line %s\n", udsvr::IsConn()?"true":"false" );
#ifdef FREEBSD
	if( udsvr::IsConn() )
	{
		//*
		char stmp[512];
		memset(stmp,0,512);
		va_list arg_ptr;
		va_start(arg_ptr, psz);
		vsprintf(stmp,psz,arg_ptr);
		va_end(arg_ptr);

		udsvr::tomoniter::SendToMoniter( nLine, stmp );
		//*/
	}
#endif
#ifdef __linux__
	if( udsvr.IsConnected() )
	{
		//*
		char stmp[512];
		memset(stmp,0,512);
		va_list arg_ptr;
		va_start(arg_ptr, psz);
		vsprintf(stmp,psz,arg_ptr);
		va_end(arg_ptr);

		udsvr.SendToMonitor( nLine, stmp );
		//*/
	}
#endif

#endif

}
