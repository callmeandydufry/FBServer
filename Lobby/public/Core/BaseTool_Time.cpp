#include "stdafx.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include "Util.h"
#include "BaseTool.h"

#define DATATIME_FORMAT_STR_SIZE (20)

/*
=======================================================================================================================
=======================================================================================================================
*/

TimeSystem::TimeSystem()
{
	/*~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	m_Now = 0;
	/*~~~~~~~~~~~~~~~~~~~~~~*/

	Init();
	__UNGUARD__
}

/*
=======================================================================================================================
=======================================================================================================================
*/
TimeSystem::~TimeSystem()
{
	__GUARD__;
	__UNGUARD__;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
BOOL TimeSystem::Init()
{
	__GUARD__
#if defined(__WINDOWS__)
		m_Begin = GetTickCount();
	m_Now = GetTickCount();
#elif defined(__LINUX__)
		m_Begin = 0;
	m_Now = 0;
	gettimeofday(&_tstart, &tz);
#endif
	Update();

	return TRUE;

	__UNGUARD__ return FALSE;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
TimeSystem &TimeSystem::instance(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	static TimeSystem	__timesystem;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	return __timesystem;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
uint32 TimeSystem::TickCount()
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	_CurrentTime();
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	return(m_Now - m_Begin);

	__UNGUARD__ return 0;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
uint32 TimeSystem::CurrentDate() const
{
	/*~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	Update();
	uint32		Date;
	/*~~~~~~~~~~~~~~~~~~~~~*/

	ConvertTU(&m_TM, Date);

	return Date;

	__UNGUARD__ return 0;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void TimeSystem::Update() const
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ time_t	newtime;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(time(&newtime) != (time_t) (-1))
	{
		m_TimeT = newtime;
	}

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#if defined(__WINDOWS__)
	tm	*ptm = localtime(&m_TimeT);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	m_TM = *ptm;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#elif defined(__LINUX__)
	tm	newtm;
	tm	*ptm = localtime_r(&m_TimeT, &newtm);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(ptm) m_TM = newtm;
#endif
	__UNGUARD__
}

/*
=======================================================================================================================
=======================================================================================================================
*/
//返回单位: 秒(second)
uint32 TimeSystem::GetCTime() const
{
	/*~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	Update();
	/*~~~~~~~~~~~~~~~~~~~~~*/

	__UNGUARD__ return(uint32) m_TimeT;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
uint32 TimeSystem::RefixANSITimeByTimeZone(uint32 uTime) const
{
#if defined(__WINDOWS__)
	uTime -= _timezone;
#elif defined(__LINUX__)
	uTime -= __timezone;
#endif
	return uTime;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
uint32 TimeSystem::Time2DWORD() const
{
	/*~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	Update();
	uint32		uRet = 0;
	/*~~~~~~~~~~~~~~~~~~~~~*/

	uRet += year();
	uRet -= 2000;
	uRet = uRet * 100;

	uRet += month() + 1;
	uRet = uRet * 100;

	uRet += day();
	uRet = uRet * 100;

	uRet += hour();
	uRet = uRet * 100;

	uRet += minute();

	return uRet;

	__UNGUARD__ return 0;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
uint32 TimeSystem::DiffTime(uint32 Date1, uint32 Date2) const
{
	/*~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__ tm	S_D1, S_D2;
	/*~~~~~~~~~~~~~~~~~~~~~~~*/

	ConvertUT(Date1, &S_D1);
	ConvertUT(Date2, &S_D2);

	/*~~~~~~~~~~~*/
	time_t	t1, t2;
	/*~~~~~~~~~~~*/

	t1 = mktime(&S_D1);
	t2 = mktime(&S_D2);

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	uint32	dif = (uint32) (difftime(t2, t1) * 1000);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	return dif;

	__UNGUARD__ return 0;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void TimeSystem::ConvertUT(uint32 Date, tm *TM) const
{
	/*~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	KCheck(TM);
	/*~~~~~~~~~~~~~~~~~~~~~~~*/

	memset(TM, 0, sizeof(tm));
	TM->tm_year = (Date >> 24) & 0xff;
	TM->tm_mon = (Date >> 20) & 0xf;
	TM->tm_mday = (Date >> 15) & 0x1f;
	TM->tm_hour = (Date >> 10) & 0x1f;
	TM->tm_min = (Date >> 4) & 0x3f;
	TM->tm_sec = (Date) & 0xf;

	__UNGUARD__
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void TimeSystem::ConvertTU(tm *TM, uint32 &Date) const
{
	/*~~~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	KCheck(TM);
	/*~~~~~~~~~~~~~~~~~~~~~~~*/

	Date = 0;
	Date += ((TM->tm_year) & 0xff) << 24;
	Date += ((TM->tm_mon) & 0xf) << 20;
	Date += ((TM->tm_mday) & 0x1f) << 15;
	Date += ((TM->tm_hour) & 0x1f) << 10;
	Date += ((TM->tm_min) & 0x3f) << 4;
	Date += ((TM->tm_sec) & 0xf);
	__UNGUARD__
}

/*
=======================================================================================================================
=======================================================================================================================
*/
uint32 TimeSystem::GetDayTime()
{
	/*~~~~~~~~~~~~~~~~~~~~~*/
	__GUARD__	Update();
	/*~~~~~~~~~~~~~~~~~~~~~*/

	return(m_TM.tm_year - 100) * 1000 + m_TM.tm_yday;

	__UNGUARD__ return 0;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
void TimeSystem::_CurrentTime()
{
	__GUARD__;

#if defined(__WINDOWS__)
	m_Now = GetTickCount();

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#elif defined(__LINUX__)
	struct timeval tend;
	int32	ret = gettimeofday(&tend, &tz);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(ret != -1)
	{
		/*~~~~~~~~~~~*/
		float64 t1, t2;
		/*~~~~~~~~~~~*/

		t1 = (float64) _tstart.tv_sec * 1000 + (float64) _tstart.tv_usec / 1000;
		t2 = (float64) tend.tv_sec * 1000 + (float64) tend.tv_usec / 1000;
		m_Now = (uint32) (t2 - t1);
	}
#endif
	__UNGUARD__;
}

BOOL TimeSystem::IsTimeInRange(DayCountType nDayCountType, int32 nDay, ulong uBeginTime, ulong uEndTime)
{
	__GUARD__;

	if (nDayCountType >= DAY_COUNT_TYPE_NUM || nDayCountType < 0)
		return FALSE;

	int32 nValue = -1;

	switch (nDayCountType)
	{
	case DATE:
		nValue = GET_TIME().day();
		break;
	case WEEK:
		nValue = GET_TIME().week();
		break;
	case ERVERY_DAY:
		nValue = -2;
		break;
	}

	int32 nCurreTime = GET_TIME().hour() * 100 + GET_TIME().minute();

	if (nValue == -2)
	{
		return (nCurreTime >= uBeginTime && nCurreTime <= uEndTime);
	}
	else
	{
		if (nValue == nDay)
		{
			return (nCurreTime >= uBeginTime && nCurreTime <= uEndTime);
		}
		else
		{
			return FALSE;
		}
	}

	__UNGUARD__ return FALSE;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
int32 TimeSystem::GetWeekNumOfYear()
{
	__GUARD__

	Update();
	int32 curYear = m_TM.tm_year + 1900;
	const int daySceonds = 24 * 3600;
    time_t secondsFrom1970 = 0;

	for( int i = 1970; i < curYear; ++i )
	{
		if( (i % 4 == 0 && i % 100 != 0) || ( i % 400 == 0 ) )
		{
			secondsFrom1970 += daySceonds * 366;
		}
		else
		{
			secondsFrom1970 += daySceonds * 365;
		}
	}
	
	tm T;
	LocalTime( secondsFrom1970, T );	
	
	int offset = T.tm_wday;
	if( offset == 0 )
		offset = 7;
	--offset;

	return ( ((int32)(m_TM.tm_yday + offset) / 7) + 1 );

	__UNGUARD__ return 0;
}

/*
=======================================================================================================================
从2011年的第一周算起,到现在的周索引数
=======================================================================================================================
*/
int32 TimeSystem::GetWeekNumFrom2011()
{
	__GUARD__

	Update();
	
	ut_id_time tmSecond = GET_TIME().GetCTime() - 41*365*24*3600 - 5*24*3600; //Since 2011.1.1
	tmSecond = GET_TIME().RefixANSITimeByTimeZone(tmSecond);
	int32 nWeekSecond = 7*24*3600;
	int32 nWeekIndex = 1;
	nWeekIndex = (tmSecond+(nWeekSecond-1)) / nWeekSecond;

	return nWeekIndex;

	__UNGUARD__	return 0;
}

/*
=======================================================================================================================
=======================================================================================================================
*/
int32 TimeSystem::GetYearAndWeekNum()
{
	int weekNum = GetWeekNumOfYear();
	return ( year() * 100 + weekNum );
}
/*
=======================================================================================================================
=======================================================================================================================
*/
int32 TimeSystem::GetTodaySecondTime()
{
	return hour() * 3600 + minute() * 60 + second();
}

/*
=======================================================================================================================
=======================================================================================================================
*/
int32	TimeSystem::GetDaysNumEplised()
{
	__GUARD__

	tm pTmOld ;
	pTmOld.tm_year =2011 - 1900;
	pTmOld.tm_mon = 0;
	pTmOld.tm_mday = 2;
	pTmOld.tm_hour =0;
	pTmOld.tm_min =0;
	pTmOld.tm_sec = 0;

	Update() ;

	time_t iSecNew = m_TimeT;
	time_t iSecOld = mktime(&pTmOld);

	return (int32)(iSecNew - iSecOld)/(24*60*60);

	__UNGUARD__	 return 0;
}

int32 TimeSystem::GetDaysNumEplised(uint32 uTime)
{
	__GUARD__

	tm pTmOld ;
	pTmOld.tm_year =2011 - 1900;
	pTmOld.tm_mon = 0;
	pTmOld.tm_mday = 2;
	pTmOld.tm_hour =0;
	pTmOld.tm_min =0;
	pTmOld.tm_sec = 0;

	Update() ;

	time_t iSecNew = uTime;
	time_t iSecOld = mktime(&pTmOld);

	return (int32)(iSecNew - iSecOld)/(24*60*60);

	__UNGUARD__	 return 0;
}

uint32 TimeSystem::GetCurrHourTime()
{
	uint32 outTime = 0;
	tm inT;
	LocalTime(GetCTime(), inT);
	inT.tm_hour = m_TM.tm_hour;
	inT.tm_min = 0;
	inT.tm_sec = 0;
	LocalTime(inT, outTime);
	return outTime;
}

uint32	TimeSystem::GetThisDayTime(int32 nHour, int32 nMin)
{
	uint32 outTime = 0;
	tm inT;
	LocalTime(GetCTime(), inT);
	inT.tm_hour = nHour;
	inT.tm_min = nMin;
	inT.tm_sec = 0;
	LocalTime(inT, outTime);
	return outTime;
}

uint32	TimeSystem::GetThisWeekTime(int32 nWeekDay, int32 nHour, int32 nMin)
{
	//nWeekDay 0-6     0 Sun [xingzhishan]
	uint32 outTime = 0;
	uint32 tempTime = GetCTime();
	tm inT;
	LocalTime(tempTime, inT);

	if (nWeekDay > inT.tm_wday)
	{
		int32 nValue = inT.tm_wday - nWeekDay;
		tempTime += nValue * (3600 * 24);
	}
	else if(nWeekDay < inT.tm_wday)
	{
		int32 nValue = 6 - inT.tm_wday + nWeekDay + 1;
		tempTime += nValue * (3600 * 24);
	}
	else
	{
		if ((nHour * 1000 + nMin * 100) >= (inT.tm_hour * 1000 + inT.tm_min * 100))
		{
			tempTime += (3600 * 24) * 7;
		}
	}

	LocalTime(tempTime, inT);
	inT.tm_hour = nHour;
	inT.tm_min = nMin;
	inT.tm_sec = 0;
	LocalTime(inT, outTime);
	return outTime;
}

void TimeSystem::FormatDataTimeWithoutSeparate(char *pStr,uint32 nTime)
{
	if (pStr)
	{
		tm t;
		LocalTime(nTime, t);
		tsnprintf(pStr,DATATIME_FORMAT_STR_SIZE,"%04d%02d%02d%02d%02d%02d",t.tm_year + 1900,t.tm_mon +1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
	}
}

void TimeSystem::FormatDataTime(char *pStr,uint32 nTime)
{
	__GUARD__
		if (pStr)
		{
			tm t;
			LocalTime(nTime, t);
			tsnprintf(pStr,DATATIME_FORMAT_STR_SIZE,"%04d-%02d-%02d %02d:%02d:%02d",t.tm_year + 1900,t.tm_mon +1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
		}
	__UNGUARD__
}

void TimeSystem::FormatDataTime(char *pStr,tm &t)
{
	__GUARD__
		if (pStr)
		{
			tsnprintf(pStr,DATATIME_FORMAT_STR_SIZE,"%04d-%02d-%02d %02d:%02d:%02d",t.tm_year + 1900,t.tm_mon +1,t.tm_mday,t.tm_hour,t.tm_min,t.tm_sec);
		}
		__UNGUARD__
}

void TimeSystem::FormatDataTime(char *pStr)
{
	__GUARD__
		if (pStr)
		{
			tsnprintf(pStr,DATATIME_FORMAT_STR_SIZE,"%04d-%02d-%02d %02d:%02d:%02d",year(),month(),day(),hour(),minute(),second());
		}
	__UNGUARD__
}

/*
=======================================================================================================================
=======================================================================================================================
*/
int32 TimeSystem::GetWeekNumFrom2011(int32 iDays)
{
	return (iDays- 1) /7;
}


/*
=======================================================================================================================
=======================================================================================================================
*/
void  TimeSystem::LocalTime( time_t in, tm& out )
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#if defined(__WINDOWS__)
	tm	*ptm = localtime(&in);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	out = *ptm;

	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#elif defined(__LINUX__)
	tm	newtm;
	tm	*ptm = localtime_r(&in, &newtm);
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(ptm) out = newtm;
#endif
}

void TimeSystem::LocalTime( tm& in, uint32 &out)
{
	out = mktime(&in);
}

uint32 TimeSystem::GetNextGameLogicDay(uint32 inTime)
{
	__GUARD__
		uint32 outTime = inTime;
	tm inT;
	LocalTime(inTime, inT);
	if (inT.tm_hour >= 0 && inT.tm_hour < 6)
	{
		inT.tm_hour = 6;
		inT.tm_min = 0;
		inT.tm_sec = 0;
		LocalTime(inT,outTime);
	}
	else
	{
		inT.tm_hour = 6;
		inT.tm_min = 0;
		inT.tm_sec = 0;
		LocalTime(inT,outTime);
		outTime += 3600 *24;
	}
	return outTime;
	__UNGUARD__ return 0;
}

uint32 TimeSystem::GetCurrGameLogicDay(uint32 inTime)
{
	return GetNextGameLogicDay(inTime) - 3600 *24;
}

uint32 TimeSystem::GetDayTimeBeginTime(uint32 uTime)
{
	__GUARD__
	uint32 outTime = uTime;
	tm inT;
	LocalTime(uTime, inT);
	inT.tm_hour = 0;
	inT.tm_min = 0;
	inT.tm_sec = 0;
	LocalTime(inT,outTime);
	return outTime;
	__UNGUARD__
		return 0;
}

uint32 TimeSystem::GetMondayGameLogicTime(uint32 uTime)
{
	uint32 outTime = uTime;
	tm inT;
	LocalTime(uTime, inT);

	int wday = inT.tm_wday;
	if (wday == 0)
		wday = 7;

	inT.tm_hour = 6;
	inT.tm_min = 0;
	inT.tm_sec = 0;
	LocalTime(inT, outTime);

	uint32 _time = outTime - 24 * 60 * 60 * (wday - 1);
	return _time;
}
uint32 TimeSystem::GetNextMondayGameLogicTime(uint32 uTime)
{
	uint32 outTime = uTime;
	tm inT;
	LocalTime(uTime, inT);
	if (inT.tm_wday == 0 && inT.tm_hour >= 0 && inT.tm_hour < 6)
	{
		//时间正好是周一 并且在早上六点之前，刷新时间为当天6点
		inT.tm_hour = 6;
		inT.tm_min = 0;
		inT.tm_sec = 0;
		LocalTime(inT, outTime);
		return outTime;
	}

	int wday = inT.tm_wday;
	if (wday == 0)
		wday = 7;

	inT.tm_hour = 6;
	inT.tm_min = 0;
	inT.tm_sec = 0;
	LocalTime(inT, outTime);

	uint32 _time = outTime + 24 * 60 * 60 * (7 - wday + 1);
	return _time;
}

uint32 TimeSystem::GetWeekDay24Time()
{
	uint32 uTime = GetCTime();
	uint32 outTime = uTime;
	tm inT;
	LocalTime(uTime, inT);
	int32 wday = inT.tm_wday;
	inT.tm_hour = 0;
	inT.tm_min = 0;
	inT.tm_sec = 0;
	LocalTime(inT, outTime);
	if (wday == 0)
	{
		wday = 7;
	}
	uint32 _time = outTime + 24 * 60 * 60 * (7 - wday + 1);
	return _time;
}


uint32 TimeSystem::GetMuchWeeksLogicTime(int32 muchWeek, int32 nWeek, int32 nHour, int32 nMin)
{
	if (muchWeek <= 0)
	{
		return 0;
	}
	uint32 outTime = 0;
	tm inT;
	LocalTime(GET_TIME().GetCTime(), inT);
	inT.tm_hour = nHour;
	inT.tm_min = nMin;
	inT.tm_sec = 0;
	LocalTime(inT, outTime);
	int32 diffDay = 7;
	if (inT.tm_wday > 0)
	{
		diffDay = 7 - inT.tm_wday;
	}
	return outTime += ((muchWeek - 1) * 7 + diffDay) * 24 * 60 * 60;
}
