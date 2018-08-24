//********************************************************************************************************
// File name		: UserWDT.c
// Description		: 看门狗配置的相关函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "UserWDT.h"



//********************************************************************************************************
// Function name	: SetSysRTCTime
// Description		: 设置系统RTC时间
// Parameter		: *arr	: 
//					: size	
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void WDT_Start(void)
{  
	#if 0  //关闭wdt
	/* Set WDT counter 0 to generate interrupt on match */
	CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
	CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WDT_COUNT0_MATCH);
	CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, 1u);
	CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK);
	#endif
	
	#if 1
	CySysWdtWriteMode(CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT);
	CySysWdtWriteMatch(CY_SYS_WDT_COUNTER0, WDT_COUNT0_MATCH);
	CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER0, 1u);	
	
	/* Enable WDT counters 0 and 1 cascade */
	CySysWdtWriteCascade(CY_SYS_WDT_CASCADE_01);
    
	/* Set WDT counter 1 to generate reset on match */
	CySysWdtWriteMatch(CY_SYS_WDT_COUNTER1, WDT_COUNT1_MATCH);
	CySysWdtWriteMode(CY_SYS_WDT_COUNTER1, CY_SYS_WDT_MODE_RESET);
    CySysWdtWriteClearOnMatch(CY_SYS_WDT_COUNTER1, 1u);
	
	/* Enable WDT counters 0 and 1 */
	CySysWdtEnable(CY_SYS_WDT_COUNTER0_MASK | CY_SYS_WDT_COUNTER1_MASK);
	#endif
}

//********************************************************************************************************
// Function name	: WDT_FeedDog
// Description		: 设置系统RTC时间
// Parameter		: *arr	: 
//					: size	
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void WDT_FeedDog(void)
{
	CySysWatchdogFeed(CY_SYS_WDT_COUNTER1);
}

//******************************************** END OF FILE *********************************************//
