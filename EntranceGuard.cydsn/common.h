//********************************************************************************************************
// File name		: common.h
// Description		: debug.c的头文件
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#ifndef __COMMON_H__
#define __COMMON_H__

//********************************************** 头文件 ************************************************//
#include <project.h>
#include <stdio.h>
#include "cytypes.h"
#include "UART_433_SPI_UART.h"
#include "debug.h"


//********************************************** 错误码 ************************************************//
#define RET_ERR                  	0x00
#define RET_OK                  	0x01


//********************************************* 通用定义 ***********************************************//
#define BYTE2WORD(dat1, dat2)   (  \
                                    ((uint16)((uint8)(dat1)))<<8 | \
                                    (uint16)((uint8)(dat2)) \
                                )
#define BYTE2LONG(dat1, dat2, dat3, dat4)   (   \
                                            ((uint32)((uint8)(dat1)))<<24 | \
                                            ((uint32)((uint8)(dat2)))<<16 | \
                                            ((uint32)((uint8)(dat3)))<<8  | \
                                            (uint32)((uint8)(dat4)) \
                                            )   
  
#define WORD2BYTE(val, arr)    do{ *((uint8 *)(arr)+0) = (uint8)((val)>>8); \
                                   *((uint8 *)(arr)+1) = (uint8)((val)); \
                                }while(0)  
                                

#define LONG2BYTE(val, arr)    do{   \
                                    *((uint8 *)(arr)+0) = (uint8)((val)>>24); \
                                    *((uint8 *)(arr)+1) = (uint8)((val)>>16); \
                                    *((uint8 *)(arr)+2) = (uint8)((val)>>8); \
                                    *((uint8 *)(arr)+3) = (uint8)((val)); \
                                }while(0); 	



//********************************************** 宏定义 ************************************************//
#define ENABLED                     (1u)
#define DISABLED                    (0u)

#define DEBUG_GPRS					(2)
#define DEBUG_TX					(1)


#define DEBUG_UART_ENABLED       	DISABLED
#define DBG_SOFT_ENNABLE			DISABLED
	
#if (DEBUG_UART_ENABLED == ENABLED)
    #define DBG_PRINTF(...)			(printf(__VA_ARGS__))
#else
    #define DBG_PRINTF(...)
#endif 

#if !DBG_SOFT_ENNABLE
#define DBG_SOFT_PutStr
#define DBG_SOFT_PrintfHex	
	
#define DBG_SOFT_PutString	
#define DBG_SOFT_PutHexInt	
#define DBG_SOFT_PutArray
#endif

#define RED							(1)
#define GREEN						(2)
#define BLUE						(3)

#endif

//******************************************** END OF FILE *********************************************//
