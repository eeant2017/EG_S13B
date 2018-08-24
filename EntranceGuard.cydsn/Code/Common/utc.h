//********************************************************************************************************
// File name		: utc.h
// Description 		: utc.c的头文件
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//********************************************************************************************************
#if !defined(__UTC_H__)
#define __UTC_H__

#include "cytypes.h"
#include "CyLib.h"
#include "config.h"



//********************************************** 宏定义 ************************************************//
#define	BeginYear  			1970					// UTC started at 00:00:00 January 1, 1970			//
#define	OneDaySeconds      	86400  					// 24 hours * 60 minutes * 60 seconds				//
//#define TimeZone			8						// Beijing											//

#define	IsLeapYear(yr)		(!((yr) % 400) || (((yr) % 100) && !((yr) % 4)))
#define	YearLength(yr)		((uint16)(IsLeapYear(yr) ? 366 : 365))


//********************************************* 函数声明 ***********************************************//
TimeInfo UtcTime_ConvertTo_Time(UTCTime secTime);
UTCTime  Time_ConvertTo_UtcTime(TimeInfo tm);


#endif

//******************************************** END OF FILE *********************************************//
