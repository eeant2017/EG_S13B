//********************************************************************************************************
// File name		: isr.h
// Description 		: isr.c的头文件
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//********************************************************************************************************
#if !defined(__ISR_H__)
#define __ISR_H__


//******************************************** 头文件声明 **********************************************//
#include "config.h"
#include "Uart433.h"
#include "Timer_100ms.h"
#include "voice.h"



//******************************************** 外部函数声明 ********************************************//
extern void WaitTimeOut(void);
extern void CardCheckTimeOut(void);
extern void RecvTimeOut(void);
extern void UpdateTimeOut(void);
extern void Defendalarm_TimeOut(void);
extern void MgCheck_Count(void);
extern void FireCheck_Count(void);

//********************************************** 函数声明 **********************************************//
CY_ISR(WDT_Interrupt);
CY_ISR(SYSTICK_IRQ_INTERRUPT);
CY_ISR(USER_KEY_INT_IRQ_INTERRUPT);
CY_ISR(FP_INT_IRQ_INTERRUPT);
CY_ISR(FP_RX_IRQ_INTERRUPT);
CY_ISR(RX_IRQ_INTERRUPT);
CY_ISR(Timer_InterruptHandler);



#endif

//******************************************** END OF FILE *********************************************//
