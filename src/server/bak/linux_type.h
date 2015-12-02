/*
 * =====================================================================================
 *
 *       Filename:  types.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年08月27日 15时01分19秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *        Company:  ciwen.com
 *
 * =====================================================================================
 */


#ifndef _LINUX_TYPE_H
#define _LINUX_TYPE_H

#ifdef __linux__
#include<sys/types.h>
#include<string.h>

#define FALSE false
#define TRUE true 
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define closesocket close
#define SERVERCORE_API
#define DllMain
#define __stdcall

typedef unsigned char UCHAR;
typedef unsigned char BYTE;
typedef bool BOOL;
typedef int HANDLE;
typedef unsigned int DWORD;
typedef time_t FILETIME;
typedef unsigned short WORD;
typedef unsigned long HINSTANCE;
typedef const char* LPCSTR;
typedef unsigned long DWORD64; 
typedef void* DWORD_PTR;
typedef unsigned long ULONG;
typedef long LONG;
typedef int SOCKET;
typedef unsigned short USHORT;
typedef unsigned int UINT;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr* PSOCKADDR;
typedef char* LPSTR;

// 临时设置
#define PCONTEXT char*
#define PSYMBOL_INFO char*
#define PVOID void*
#define STACKFRAME char
#define PEXCEPTION_RECORD char*
#define PTSTR char*
#define CALLBACK

#endif
#endif
