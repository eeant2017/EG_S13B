//********************************************************************************************************
// File name		: utc.c
// Description 		: ��������ʱ���ʱ����໥ת��
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//********************************************************************************************************
#include "utc.h"



//********************************************************************************************************
// Function name	: MonthLength
// Description		: �����³���
// Parameter		: LeapYearFlag	: 1(����)/0(������)
//					: Month			: �·�(1~12)
// Return			: ��ǰ�µ�����
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
// Description		: ������ʱ��(��������ʱ��)ת��Ϊ����ʱ��
// Parameter		: secTime	: ����ʱ��
// Return			: tm		: ����ʱ��
//--------------------------------------------------------------------------------------------------------
// Info				: �㷨����·ݷ�ΧΪ[0-11]��������ΧΪ[0-30]������ĩβ�ֱ�ǿ��ת��Ϊ[1-12]��[1-31].
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
// Description		: ������ʱ��ת��Ϊ����ʱ��
// Parameter		: tm		: ����ʱ��
//					: secTime	: ����ʱ��
// Return			: ��ǰ�µ�����
//--------------------------------------------------------------------------------------------------------
// Info				: �㷨�����·ݷ�ΧΪ[0-11]��������ΧΪ[0-30]������ͷ����[1-12]��[1-31]�ֱ�ǿ��ת��.
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
