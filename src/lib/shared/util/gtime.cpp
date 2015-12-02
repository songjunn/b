#include "gtime.h"
#include <time.h>
#include <stdio.h>
#include <sys/timeb.h>
#ifndef WIN32
#include <sys/time.h>
#endif


uint64 GetTimeSec()
{
#ifdef WIN32
	timeb t;
	ftime(&t);
	return t.time;
#else
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);//if tv or tz is NULL,the associated time information will not be return or set.
	return (uint64)tv.tv_sec;
#endif
}

uint64 GetTimeUsec()
{
#ifdef WIN32
	timeb t;
	ftime(&t);
	return 1000 * t.time + t.millitm;
#else
	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);//if tv or tz is NULL,the associated time information will not be return or set.
	return ((uint64)tv.tv_sec)*(1000ULL) + (((uint64)tv.tv_usec)/(1000ULL));
#endif
}

unsigned int timeGetTime()  
{  
	unsigned int uptime = 0;  
    struct timespec on;  
    if(clock_gettime(CLOCK_MONOTONIC, &on) == 0)  
    	uptime = on.tv_sec*1000 + on.tv_nsec/1000000;  
    return uptime;  
}

void GetLocalTime(GTime &gt)
{
	time_t t = time(NULL);
	tm* p = localtime(&t);
	if( !p )
		return;

	gt.gtm_sec	= p->tm_sec;
	gt.gtm_min	= p->tm_min;
	gt.gtm_hour	= p->tm_hour;
	gt.gtm_mday	= p->tm_mday;
	gt.gtm_mon	= p->tm_mon + 1;
	gt.gtm_year	= p->tm_year + 1900;
	gt.gtm_wday	= p->tm_wday;
}

void CoverSecToGtime(uint64 sec,GTime &gtm)
{
	tm* t = localtime((time_t*)&sec);
	if(!t)
		return;
	gtm.gtm_sec		= t->tm_sec;
	gtm.gtm_min		= t->tm_min;
	gtm.gtm_hour	= t->tm_hour;
	gtm.gtm_mday	= t->tm_mday;
	gtm.gtm_mon		= t->tm_mon + 1;
	gtm.gtm_year	= t->tm_year + 1900;
	gtm.gtm_wday	= t->tm_wday;
}

uint64 CoverGtimeToSec(const GTime &gtm)
{
	tm curTm;
	curTm.tm_year = gtm.gtm_year - 1900;
	curTm.tm_mon = gtm.gtm_mon - 1;
	curTm.tm_mday = gtm.gtm_mday;
	curTm.tm_hour = gtm.gtm_hour;
	curTm.tm_min = gtm.gtm_min;
	curTm.tm_sec = gtm.gtm_sec;

	return (uint64)mktime(&curTm);
}

int64 StringToDatetime(const char *str)  
{  
    tm tm_;  
    int year, month, day, hour, minute,second;  
    sscanf(str,"%d-%d-%d-%d:%d:%d", &year, &month, &day, &hour, &minute, &second);  
    tm_.tm_year  = year-1900;  
    tm_.tm_mon   = month-1;  
    tm_.tm_mday  = day;  
    tm_.tm_hour  = hour;  
    tm_.tm_min   = minute;  
    tm_.tm_sec   = second;  
    tm_.tm_isdst = 0;  
  
    int64 t_ = mktime(&tm_); //已经减了8个时区  
    return t_; //秒时间  
}  

void DatatimeToString(char* timestr)
{
	GTime gt;
	GetLocalTime(gt);

	sprintf(timestr, "%d-%d-%d %d:%d:%d", gt.gtm_year, gt.gtm_mon, gt.gtm_mday, gt.gtm_hour, gt.gtm_min, gt.gtm_sec);
}

bool SecSameDay(uint64 sec1, uint64 sec2)
{
	GTime gtm1, gtm2;
	CoverSecToGtime(sec1, gtm1);
	CoverSecToGtime(sec2, gtm2);

	if(gtm1.gtm_year == gtm2.gtm_year &&
		gtm1.gtm_mon == gtm2.gtm_mon &&
		gtm1.gtm_mday == gtm2.gtm_mday) {
		return true;
	}
	else {
		return false;
	}
}

bool SecSameMonth(uint64 sec1, uint64 sec2)
{
	GTime gtm1, gtm2;
	CoverSecToGtime(sec1, gtm1);
	CoverSecToGtime(sec2, gtm2);

	if(gtm1.gtm_year == gtm2.gtm_year &&
		gtm1.gtm_mon == gtm2.gtm_mon) {
			return true;
	}
	else {
		return false;
	}
}

int SecDayInterval(uint64 sec1, uint64 sec2)
{
	return (sec1 / GTIME_DAY_SEC) - (sec2 / GTIME_DAY_SEC);
}
