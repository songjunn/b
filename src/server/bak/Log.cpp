#include "Log.h"
#include "PathFunc.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
static const char* g_LogLevelString[LOG_LEVEL_END] =
{
	"[NONE]",
	"[DEBUG]",
	"[NOTICE]",
	"[WARNING]",
	"[ERROR]"
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
bool CLog::Startup(const char * szName, int level)
{
	m_LogFileCount = 0;
	m_LogLevel = level;
	m_LogRun = true;
	memcpy( m_LogBaseName, szName, LOG_FILENAME_SIZE/2 );

	_FormatFilename();

	char path[256] = {0};
	GetExePath( path, 256 );

	//创建日志目录
	char dir[256] = {0};
	sprintf( dir, "%s%s", path, LOG_PATH_STRING );
	myCreateDir( dir );

#ifdef _WIN
	m_LogEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
#else
	sem_init( &m_LogEvent, 1, 0 );
#endif

	m_LogThread = ThreadLib::Create(LogThread, this);

	Log.Notice("[CLog]Create Thread:%lu", m_LogThread);

	return true;
}

void CLog::Shutdown()
{
	m_LogRun = false;
	ThreadLib::WaitForFinish(m_LogThread);
}

void CLog::Debug(char const * msg, ...)
{
	if( !m_LogRun || LOG_LEVEL_DEBUG < m_LogLevel )
		return;

	va_list args;
	va_start( args, msg );

	_FormatString( LOG_OUT_FILE, msg, args , LOG_LEVEL_DEBUG);

	va_end(args);
}

void CLog::Notice(char const * msg, ...)
{
	if( !m_LogRun || LOG_LEVEL_NOTICE < m_LogLevel )
		return;

	va_list args;
	va_start( args, msg );

	_FormatString( /*LOG_OUT_CONSOLE |*/ LOG_OUT_FILE, msg, args , LOG_LEVEL_NOTICE);

	va_end(args);
}

void CLog::Warning(char const * msg, ...)
{
	if( !m_LogRun || LOG_LEVEL_WARNING < m_LogLevel )
		return;

	va_list args;
	va_start( args, msg );

	_FormatString( LOG_OUT_FILE, msg, args , LOG_LEVEL_WARNING);

	va_end(args);
}

void CLog::Error(char const * msg, ...)
{
	if( !m_LogRun || LOG_LEVEL_ERROR < m_LogLevel )
		return;

	va_list args;
	va_start( args, msg );

	_FormatString( /*LOG_OUT_CONSOLE |*/ LOG_OUT_FILE, msg, args , LOG_LEVEL_ERROR);

	va_end(args);
}

void CLog::_FormatTime(char * pTime)
{
	if( !pTime )
		return;

	struct tm *newtime;
	time_t aclock;
	time( &aclock );
	newtime = localtime( &aclock );
	//char * sztime = asctime( newtime );
	//int len = strlen( sztime );
	//if( len > 0 )
	//{
		//sztime[len-1] = 0;
		//sprintf( pTime, "%s%s%s", "[", sztime, "]" );
	//}
	sprintf( pTime, "[%02d:%02d:%02d %02d:%02d:%02d]", newtime->tm_year+1900, 
			newtime->tm_mon+1, newtime->tm_mday, newtime->tm_hour,
			newtime->tm_min, newtime->tm_sec);

	return;
}

void CLog::_FormatString(int outtype, char const * msg, va_list args, int level)
{
	char szMsg[LOG_ITEM_SIZE-64] = { 0 };
	vsnprintf( szMsg, LOG_ITEM_SIZE-64, msg, args );

	LogItem * pLog = NEW LogItem;
	if( !pLog )
		return;

	pLog->m_OutType = outtype;
	_FormatTime( pLog->m_szBuffer );
	strcat( pLog->m_szBuffer, g_LogLevelString[level] );
	strcat( pLog->m_szBuffer, szMsg );
	//strcat( pLog->m_szBuffer, "\n" );
	strcat( pLog->m_szBuffer, "\0" );

	m_LogLock.LOCK();
	m_LogList.push_back( pLog );
	m_LogLock.UNLOCK();

#ifdef _WIN
	SetEvent( m_LogEvent );
#else
	sem_post( &m_LogEvent );
#endif

	return;
}

void CLog::_FormatFilename()
{
	struct tm *newtime;
	time_t aclock;
	time( &aclock );
	newtime = localtime( &aclock );

	char szTime[LOG_FILENAME_SIZE/2] = { 0 };
	sprintf( szTime, "%s%d%s%d%s%d%d%d%s", "_", newtime->tm_mon+1, "-", newtime->tm_mday, "-", newtime->tm_hour, newtime->tm_min, newtime->tm_sec, ".log" );
	sprintf( m_LogFileName, "%s//%s%s", LOG_PATH_STRING, m_LogBaseName, szTime );
}

void CLog::_OutputToFile(const char * msg)
{
	FILE * p_hFile = fopen( m_LogFileName, "at+" );
	if( p_hFile )
	{
		fprintf( p_hFile, "%s\n", msg );
		fclose( p_hFile );
		++m_LogFileCount;
	}
	else
	{
		printf("Error:open log file failed:%s\n", m_LogFileName);
	}

	//如果超过最大记录条数将更换文件名
	if( LOG_FILE_COUNT <= m_LogFileCount )
	{
		_FormatFilename();
		m_LogFileCount = 0;
	}
}

void CLog::_OutputToConsole(const char * msg)
{
	printf( msg );
	printf( "\n" );
}

LogItem * CLog::_GetHeadLog()
{
	LogItem * pLog = NULL;
	m_LogLock.LOCK();
	if( !m_LogList.empty() )
	{
		pLog = m_LogList.front();
		m_LogList.pop_front();
	}
	m_LogLock.UNLOCK();
	return pLog;
}

///////////////////////////////////////////////////////////////////////////////////
//
void CLog::LogThread(void * param)
{
	if( !param )
		return;

	LogItem * pLog = NULL;
	CLog * pThis = (CLog *)param;

	for(;;)
	{
#ifdef _WIN
 		WaitForSingleObject( pThis->m_LogEvent, INFINITE );
#else
 		sem_wait( &pThis->m_LogEvent );
#endif

		if( !pThis->m_LogRun )
			break;

		while( (pLog = pThis->_GetHeadLog()))
		{
			if( pLog->m_szBuffer[0] != 0 )
			{
				if( pLog->m_OutType & LOG_OUT_FILE )	pThis->_OutputToFile( pLog->m_szBuffer );
				if( pLog->m_OutType & LOG_OUT_CONSOLE )	pThis->_OutputToConsole( pLog->m_szBuffer );
			}

			delete pLog;
		}

#ifdef _WIN
		ResetEvent( pThis->m_LogEvent );
#endif
	}

#ifdef _WIN
	if( pThis->m_LogEvent )
	{
		CloseHandle( pThis->m_LogEvent );
		pThis->m_LogEvent = NULL;
	}
#else
		sem_destroy( &pThis->m_LogEvent );
#endif

	/*if( pThis->m_LogThread )
	{
		ThreadLib::WaitForFinish( pThis->m_LogThread );
#ifdef __linux__
		pThis->m_LogThread = 0;
#else
		pThis->m_LogThread = NULL;
#endif
	}*/

	while( (pLog = pThis->_GetHeadLog()) )
	{
		delete pLog;
	}

}
