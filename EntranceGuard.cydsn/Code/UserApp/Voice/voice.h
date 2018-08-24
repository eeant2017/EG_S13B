//********************************************************************************************************
// File name		: voice.h
// Description 		: voice.c的头文件
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//********************************************************************************************************
#if !defined(__VOICE_H__)
#define __VOICE_H__


//******************************************** 头文件声明 **********************************************//
#include "common.h"
#include "config.h"


//*********************************************** 宏定义 ***********************************************//
#define Sound_KEY						1
#define Sound_SUCCESS					2
#define Sound_FAILURE					3
#define Sound_REG_SUCCESS				4
#define Sound_REG_FAILURE				5
#define Sound_DELETE_SUCCESS			6
#define Sound_DELETE_FAILURE			7
#define Sound_EMPTY_SUCCESS				8
#define Sound_EMPTY_FAILURE				9
#define Sound_VERIFY_SUCCESS			10
#define Sound_VERIFY_FAILURE			11
#define Sound_SYSTEM_POWER_ON			12
#define Sound_PleasePutCard				13
#define Sound_ReadCardSuccess			14
#define Sound_ShortDI					15
#define Sound_LongDI					16
#define Sound_SYSTEM_POWER_ON_TEST		17
#define Sound_TIMEOUT					18
#define Sound_DEFENDALRM				19
	

//********************************************** 函数声明 **********************************************//
void BeepPlay_WaitTime(void);
void BeepPlay_Delay(uint16_t cnt);
void VoicePlay(INT8U addr);


#endif

//******************************************** END OF FILE *********************************************//
