#include "TimerModule.h"
#include "LuaEngine.h"


CTimerModule::CTimerModule()
{
	m_LastYear = INVALID_VALUE;
	m_LastMonth = INVALID_VALUE;
	m_LastMday = INVALID_VALUE;
	m_LastWday = INVALID_VALUE;
	m_LastHour = INVALID_VALUE;
	m_LastMinute = INVALID_VALUE;
	m_LastSecond = INVALID_VALUE;
}

CTimerModule::~CTimerModule()
{
}

void CTimerModule::OnLogic()
{
	GetLocalTime(m_NowTime);

	_OnMinuteLogic();
	_OnHourLogic();
	_OnWdayLogic();
	
}

void CTimerModule::_OnMinuteLogic()
{
	if( m_NowTime.gtm_min == m_LastMinute )
		return;

	m_LastMinute = m_NowTime.gtm_min;

	FOR_EACH_LL(m_MinuteTimers, idx)
	{
		Timer_Obj* ptimer = m_MinuteTimers[idx];
		if( !ptimer )
			continue;

		if( ptimer->m_min < 0 || ptimer->m_min != m_NowTime.gtm_min )
			continue;

		if( ptimer->m_hour >= 0 && ptimer->m_hour != m_NowTime.gtm_hour )
			continue;

		if( ptimer->m_wday >= 0 && ptimer->m_wday != m_NowTime.gtm_wday )
			continue;

		if( ptimer->m_mday >= 0 && ptimer->m_mday != m_NowTime.gtm_mday )
			continue;

		if( ptimer->m_moth >= 0 && ptimer->m_moth != m_NowTime.gtm_mon )
			continue;

		if( ptimer->m_year >= 0 && ptimer->m_year != m_NowTime.gtm_year )
			continue;

		_OnTimerFunc(ptimer);
	}
}

void CTimerModule::_OnHourLogic()
{
	if( m_NowTime.gtm_hour == m_LastHour )
		return;

	m_LastHour = m_NowTime.gtm_hour;

	FOR_EACH_LL(m_HourTimers, idx)
	{
		Timer_Obj* ptimer = m_HourTimers[idx];
		if( !ptimer )
			continue;

		if( ptimer->m_hour < 0 || ptimer->m_hour != m_NowTime.gtm_hour )
			continue;

		if( ptimer->m_wday >= 0 && ptimer->m_wday != m_NowTime.gtm_wday )
			continue;

		if( ptimer->m_mday >= 0 && ptimer->m_mday != m_NowTime.gtm_mday )
			continue;

		if( ptimer->m_moth >= 0 && ptimer->m_moth != m_NowTime.gtm_mon )
			continue;

		if( ptimer->m_year >= 0 && ptimer->m_year != m_NowTime.gtm_year )
			continue;

		_OnTimerFunc(ptimer);
	}
}

void CTimerModule::_OnWdayLogic()
{
	if( m_NowTime.gtm_wday == m_LastWday )
		return;

	m_LastWday = m_NowTime.gtm_wday;

	FOR_EACH_LL(m_WdayTimers, idx)
	{
		Timer_Obj* ptimer = m_WdayTimers[idx];
		if( !ptimer )
			continue;

		if( ptimer->m_wday < 0 || ptimer->m_wday != m_NowTime.gtm_wday )
			continue;

		_OnTimerFunc(ptimer);
	}
}

void CTimerModule::_OnTimerFunc(Timer_Obj* ptimer)
{
	if( !ptimer )
		return;

	LuaParam param[7];
	param[0].SetDataNum(m_NowTime.gtm_year);
	param[1].SetDataNum(m_NowTime.gtm_mon);
	param[2].SetDataNum(m_NowTime.gtm_mday);
	param[3].SetDataNum(m_NowTime.gtm_hour);
	param[4].SetDataNum(m_NowTime.gtm_min);
	param[5].SetDataNum(m_NowTime.gtm_sec);
	param[6].SetDataNum(m_NowTime.gtm_wday);

	LuaEngine.RunLuaFunction(ptimer->m_sfunc, ptimer->m_stable, NULL, param, 7);
}

void CTimerModule::RegistTimerFunc(int year, int month, int mday, int wday, int hour, int minute, int sec, const char* func, const char* table)
{
	if( !func || !table )
		return;

	Timer_Obj* ptimer = NEW Timer_Obj;
	if( !ptimer )
		return;

	ptimer->m_year = year;
	ptimer->m_moth = month;
	ptimer->m_mday = mday;
	ptimer->m_wday = wday;
	ptimer->m_hour = hour;
	ptimer->m_min = minute;
	ptimer->m_sec = sec;
	strncpy(ptimer->m_sfunc, func, 32);
	strncpy(ptimer->m_stable, table, 32);

	if( minute >= 0 )
		m_MinuteTimers.AddToTail(ptimer);
	else if( hour >= 0 )
		m_HourTimers.AddToTail(ptimer);
	else if( mday >= 0 )
		m_MdayTimers.AddToTail(ptimer);
	else if( wday >= 0 )
		m_WdayTimers.AddToTail(ptimer);
	
}
