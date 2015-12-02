#pragma once
#include "commdata.h"
#include "Singleton.h"
#include "utllinkedlist.h"
#include "gtime.h"


struct Timer_Obj
{
	int		m_year;			//年
	int		m_moth;			//月
	int		m_mday;			//日
	int		m_wday;			//星期
	int 	m_hour;			//小时
	int 	m_min;			//分钟
	int		m_sec;
	char	m_sfunc[32];
	char	m_stable[32];
};

class CTimerModule : public Singleton< CTimerModule >
{
public:
	CTimerModule();
	~CTimerModule();

	void	OnLogic();

	void	RegistTimerFunc(int year, int month, int mday, int wday, int hour, int minute, int sec, const char* func, const char* table);

protected:
	void	_OnMinuteLogic();
	void	_OnHourLogic();
	void	_OnWdayLogic();
	void	_OnMdayLogic();

	void	_OnTimerFunc(Timer_Obj* ptimer);

protected:
	int		m_LastYear;
	int		m_LastMonth;
	int		m_LastMday;
	int		m_LastWday;
	int		m_LastHour;
	int		m_LastMinute;
	int		m_LastSecond;
	GTime	m_NowTime;

	CUtlLinkedList<Timer_Obj*>	m_MinuteTimers;
	CUtlLinkedList<Timer_Obj*>	m_HourTimers;
	CUtlLinkedList<Timer_Obj*>	m_WdayTimers;
	CUtlLinkedList<Timer_Obj*>	m_MdayTimers;

};

#define TimerModule CTimerModule::getSingleton()
