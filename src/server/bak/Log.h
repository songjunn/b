/*
 * 多线程日志输出模块，支持多级日志，向文件、终端输出
 * 日志文件名包含当前时间字符串，以log作为扩展名，并且单文件最大写入LOG_FILE_COUNT条日志，超出则新建文件写入
 *
 *				songjun 2013.1.2
 */
#ifndef SHARED_LOG_H
#define SHARED_LOG_H

#ifdef __linux__
#include <stdarg.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <string>

#include <list>

#include "memory.h"
#include "Singleton.h"
#include "Lock.h"
#include "ThreadException.h"
#include "ThreadLibFunctions.h"
#else
#include "shared.h"
#endif

#define LOG_PATH_STRING		"log"		//存盘log文件的文件夹名
#define LOG_ITEM_SIZE		5120		//每个log的长度
#define LOG_FILE_COUNT		200000		//每个log文件最大记录的条数(20万条大概10M)

const int LOG_FILENAME_SIZE	= 64;					//日志文件名长度，包含时间字符串和扩展名
const int LOG_BASENAME_SIZE	= LOG_FILENAME_SIZE / 2;//日志文件名长度，不包含时间字符串和扩展名

const int LOG_OUT_FILE		= 1<<0;		//日志输出到文件
const int LOG_OUT_CONSOLE	= 1<<1;		//日志输出到控制台

enum LOG_LEVEL		//日志输出级别
{
	LOG_LEVEL_NONE = 0,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_NOTICE,
	LOG_LEVEL_WARNING,
	LOG_LEVEL_ERROR,

	LOG_LEVEL_END,
};

class LogItem
{
public:
	LogItem()
	{
		m_OutType = LOG_OUT_FILE;
	}
	~LogItem()
	{
		m_OutType = 0;
	}

public:
	int		m_OutType;
	char 	m_szBuffer[LOG_ITEM_SIZE];
};

class CLog : public Singleton< CLog >
{
public:
	CLog(){};
	~CLog(){};

	bool	Startup(const char * szName, int level=LOG_LEVEL_NOTICE);
	void	Shutdown();

	void	Debug(char const * msg, ...);
	void	Notice(char const * msg, ...);
	void	Warning(char const * msg, ...);
	void	Error(char const * msg, ...);

	void	setLevel(int iLevel) { m_LogLevel = iLevel; }

protected:
	void	_Color(int color);
	void	_FormatFilename();
	void	_FormatTime(char * szTime);
	void	_FormatString(int outtype, char const * msg, va_list args, int level);

	void	_OutputToFile(const char * msg);
	void	_OutputToConsole(const char * msg);

	LogItem * _GetHeadLog();

	static void	LogThread(void * param);

private:
	bool	m_LogRun;
	int		m_LogLevel;
	int		m_LogFileCount;
	char	m_LogFileName[LOG_FILENAME_SIZE];
	char	m_LogBaseName[LOG_BASENAME_SIZE];
	Mutex	m_LogLock;
#ifdef WIN32
	HANDLE	m_LogEvent;
#else
	sem_t	m_LogEvent;
#endif
	ThreadLib::ThreadID m_LogThread;
	std::list<LogItem*>	m_LogList;

};

#define Log CLog::getSingleton()

#endif	//SHARED_LOG_H
