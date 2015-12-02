#ifndef SHARED_GTIME_H
#define SHARED_GTIME_H

#include "types.h"

#define  GTIME_MINUTE_SEC	60
#define	 GTIME_HOUR_SEC		(60 * GTIME_MINUTE_SEC)
#define  GTIME_DAY_SEC		(24 * GTIME_HOUR_SEC)
#define  GTIME_INFINITE_DAY	36500

struct GTime
{
	int gtm_sec;     /* [1,60] */
	int gtm_min;     /* [1,60] */
	int gtm_hour;    /* [1,24] */
	int gtm_mday;    /* [1,31] */
	int gtm_mon;     /* [1,12] */
	int gtm_year;    /* [****] */
	int gtm_wday;    /* [0,6] */
};

uint64 GetTimeSec();
uint64 GetTimeUsec();

unsigned int timeGetTime();

void GetLocalTime(GTime &t);

void CoverSecToGtime(uint64 sec,GTime &gtm);
uint64 CoverGtimeToSec(const GTime &gtm);

int64 StringToDatetime(const char *str);
void DatatimeToString(char* timestr);

const char *ConvertSecToString(uint64 sec);
int64 ConvertStringToSec(const char *str);

bool SecSameDay(uint64 sec1, uint64 sec2);
bool SecSameMonth(uint64 sec1, uint64 sec2);
int  SecDayInterval(uint64 sec1, uint64 sec2);

#endif	//SHARED_GTIME_H
