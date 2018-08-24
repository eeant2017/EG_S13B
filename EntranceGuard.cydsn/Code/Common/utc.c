//********************************************************************************************************
// File name		: utc.c
// Description 		: 格林威治时间和时间的相互转化
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//********************************************************************************************************
#include "utc.h"



//********************************************************************************************************
// Function name	: MonthLength
// Description		: 计算月长度
// Parameter		: LeapYearFlag	: 1(闰年)/0(非闰年)
//					: Month			: 月份(1~12)
// Return			: 当前月的天数
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
static uint8 MonthLength(uint8 LeapYearFlag, uint8 Month)
{
	uint8 days = 31;

	if (Month == 1)
	{
		days = (28 + LeapYearFlag);
	}
	else
	{
		if (Month > 6)
		{
			Month--;
		}

		if (Month & 1)
		{
			days = 30;
		}
	}

	return (days);
}


//********************************************************************************************************
// Function name	: UtcTime_ConvertTo_Time
// Description		: 将世界时间(格林威治时间)转化为日历时间
// Parameter		: secTime	: 世界时间
// Return			: tm		: 日历时间
//--------------------------------------------------------------------------------------------------------
// Info				: 算法输出月份范围为[0-11]，天数范围为[0-30]，函数末尾分别强制转化为[1-12]和[1-31].
//********************************************************************************************************
TimeInfo UtcTime_ConvertTo_Time(UTCTime secTime)
{
	TimeInfo tm;
	
	//secTime += TimeZone * 3600;
	{
		uint32 day = secTime % OneDaySeconds;

		tm.Sec  =  day % 60UL;
		tm.Min  = (day % 3600UL) / 60UL;
		tm.Hour =  day / 3600UL;
	}

	{
		uint16 numDays = secTime / OneDaySeconds;
		uint8 monthLen;
		tm.Year = BeginYear;

		while (numDays >= YearLength(tm.Year))
		{
			numDays -= YearLength(tm.Year);
			tm.Year++;
		}

		tm.Mon = 0;

		monthLen = MonthLength(IsLeapYear(tm.Year), tm.Mon);

		while (numDays >= monthLen)
		{
			numDays -= monthLen;
			tm.Mon++;

			monthLen = MonthLength(IsLeapYear(tm.Year), tm.Mon);
		}

		tm.Day = numDays;
	}
	
	tm.Mon++;
	tm.Day++;
	
	return tm;
}

//********************************************************************************************************
// Function name	: Time_ConvertTo_UtcTime
// Description		: 将日历时间转化为世界时间
// Parameter		: tm		: 日历时间
//					: secTime	: 世界时间
// Return			: 当前月的天数
//--------------------------------------------------------------------------------------------------------
// Info				: 算法输入月份范围为[0-11]，天数范围为[0-30]，函数头部将[1-12]和[1-31]分别强制转化.
//********************************************************************************************************
UTCTime Time_ConvertTo_UtcTime(TimeInfo tm)
{
	UTCTime seconds;
	
	tm.Mon--;
	tm.Day--;
	
	seconds = (((tm.Hour * 60UL) + tm.Min) * 60UL) + tm.Sec;

	{
		uint16 days = tm.Day;
		{
			int8 month = tm.Mon;
			while (--month >= 0)
			{
				days += MonthLength(IsLeapYear(tm.Year), month);
			}
		}

		{
			uint16 year = tm.Year;
			while (--year >= BeginYear)
			{
				days += YearLength(year);
			}
		}

		seconds += (days * OneDaySeconds);
	}
	
	//seconds -= TimeZone * 3600;
	
	return (seconds);
}
