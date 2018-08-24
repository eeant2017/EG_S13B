//********************************************************************************************************
// File name		: isr.c
// Description		: 中断服务的相关信息
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "isr.h"



//********************************************************************************************************
// Function name	: WDT_Interrupt
// Description		: 看门狗中断
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
CY_ISR(WDT_Interrupt)
{
	if (CySysWdtGetInterruptSource() & CY_SYS_WDT_COUNTER0_INT)
	{
		/* Clear interrupts state */
		CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);
    	WDT_ISR_ClearPending();
		RTC_Update();
	}
}

//********************************************************************************************************
// Function name	: SYSTICK_IRQ_INTERRUPT
// Description		: SYSTICK定时器中断的服务函数
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 1ms进入中断一次
//********************************************************************************************************
CY_ISR(SYSTICK_IRQ_INTERRUPT)
{

}

//********************************************************************************************************
// Function name	: Timer_InterruptHandler
// Description		: 定时器中断的服务函数
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				:
//********************************************************************************************************
CY_ISR(Timer_InterruptHandler)
{
	Timer_100ms_WriteCounter(1000);
	Timer_100ms_ClearInterrupt(Timer_100ms__INTR_MASK_CC_MATCH);
	WaitTimeOut();
	RecvTimeOut();
	UpdateTimeOut();
	BeepPlay_WaitTime();									// 蜂鸣器计时器								//
    CardCheckTimeOut();
	Defendalarm_TimeOut();
	MgCheck_Count();
	FireCheck_Count();
}

//********************************************************************************************************
// Function name	: RX_IRQ_INTERRUPT
// Description		: 串口中断的服务函数
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				:
//********************************************************************************************************
CY_ISR(RX_IRQ_INTERRUPT)
{
	#if 0 
    uint8 i, rxData;
	uint32 qty;
	
	if (UART_433_GetRxInterruptSourceMasked() & UART_433_INTR_RX_NOT_EMPTY)
	{
		qty = UART_433_SpiUartGetRxBufferSize();
		for (i = 0; i < qty; i++)
		{
			rxData = UART_433_SpiUartReadRxData();
			Uart433Recv(rxData);
		}
	}
    UART_433_SpiUartClearRxBuffer();
	#endif
	// 优化数据接收速率问题！ @chensq 03-07
	if (0u != UART_433_SpiUartGetRxBufferSize())
	{
		Uart433Recv(UART_433_RX_FIFO_RD_REG);	
	}
		
    UART_433_ClearRxInterruptSource(UART_433_INTR_RX_NOT_EMPTY);
}


//******************************************** END OF FILE *********************************************//
