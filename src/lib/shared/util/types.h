#ifndef SHARED_PLATFORM_H
#define SHARED_PLATFORM_H

#include <assert.h>

// for when we care about how many bits we use
typedef signed char      int8;
typedef signed short     int16;
typedef signed int       int32;
typedef signed long long int64;
//typedef signed __int64   int64;

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;
//typedef unsigned __int64   uint64;

typedef unsigned char byte;
typedef unsigned long ULONG;

typedef float  float32;
typedef double float64;

// for when we don't care about how many bits we use
typedef unsigned int uint;

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifdef _WIN
#define INT64_FMT 	"%I64d"
#define UINT64_FMT 	"%I64u"
#else
#define INT64_FMT 	"%lld"
#define UINT64_FMT 	"%llu" 
#endif

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


//-----------------------------------------------------------------------------
// Some Functions
//-----------------------------------------------------------------------------
#ifdef _WIN
#define	int64_t		int64
#define atoll		_atoi64
#define __func__	__FUNCTION__
#endif

//-----------------------------------------------------------------------------
// Some Ignored Warnings
//-----------------------------------------------------------------------------
#ifdef _WIN
#pragma warning (disable:4251) //“identifier”: 类“type”需要有 dll 接口由类“type2”的客户端使用
#pragma warning (disable:4996) //This function or variable may be unsafe. Consider using _s function instead. To disable deprecation, use _CRT_SECURE_NO_DEPRECATE. See online help for details.
#pragma warning (disable:4661) //没有为显式模板实例化请求提供适当的定义
#endif	//_WIN


//-----------------------------------------------------------------------------
// Methods to invoke the constructor, copy constructor, and destructor
//-----------------------------------------------------------------------------
template <class T>
inline void Construct( T* pMemory )
{
	new( pMemory ) T;
}

template <class T>
inline void CopyConstruct( T* pMemory, T const& src )
{
	new( pMemory ) T(src);
}

template <class T>
inline void Destruct( T* pMemory )
{
	pMemory->~T();
}


#endif	//SHARED_PLATFORM_H
