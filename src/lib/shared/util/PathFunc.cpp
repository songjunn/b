#include "PathFunc.h"
#ifdef _WIN
#include <windows.h>
#include <stdio.h>
#endif
#ifdef __linux__
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif


void GetExePath(char* path, int size )
{
#ifdef _WIN
	HMODULE hmodule =  GetModuleHandle( NULL );
	GetModuleFileName( hmodule, path, size );
	char * pEnd = path;
	char * p;
	char c = '\\';
	while (p = strchr(pEnd,c)){
		pEnd = ++p;
	}
	*pEnd = 0;
#else
#ifdef __linux__
	if (getcwd(path,size) == NULL)
		perror("getcwd failed");
	strcat(path,"/");
#else
	getcwd(path,nSize);
	strcat(szExePath,"/");

#endif
#endif
}

bool myCreateDir(const char* fullpath)
{
#ifdef _WIN
	return CreateDirectory( fullpath, NULL ) != 0;
#endif
#ifdef __linux__
	return mkdir( fullpath, 0700 ) == 0;
#endif
}

bool myCreateFile(const char* filename)
{
#ifdef _WIN
	HANDLE hFile = CreateFile( filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL );
	return (hFile != NULL);
#endif
	return true;
}
