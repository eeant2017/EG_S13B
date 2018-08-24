//********************************************************************************************************
// File name		: voice.c
// Description		: 语音的相关函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "voice.h"
#include "common.h"


//********************************************** 变量声明 **********************************************//
vINT32U  BeepDelay_TimeOut;


//*********************************************** 宏定义 ***********************************************//
#define Sound_DO		956				//频率2092
#define Sound_RE		850				//频率2350
#define Sound_MI		758				//频率2636
#define Sound_FA		716				//频率2794
#define Sound_SO		668				//频率3136
#define Sound_LA		284				//频率3520
#define Sound_SI		253				//频率3952
    
#define Sound_DI		333				//频率3000
#define Sound_DING		999			    //频率1500   



//********************************************************************************************************
// Function name	: Beep_Delay_50ms
// Description		: 蜂鸣器播放的计时函数
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				:
//********************************************************************************************************
void BeepPlay_WaitTime(void)
{
    if (BeepDelay_TimeOut > 0)
	{
		BeepDelay_TimeOut--;
	}
}

//********************************************************************************************************
// Function name	: BeepPlay_Delay
// Description		: Beep的播报延时函数
// Parameter		: cnt : 延时时间
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 1ms
//*******************************************************************************************************/
void BeepPlay_Delay(uint16_t cnt)
{
    BeepDelay_TimeOut = cnt;
    while (BeepDelay_TimeOut > 0)
    {
        CyBle_ProcessEvents();
    }
}

//********************************************************************************************************
// Function name	: PowerOn_Play
// Description		: play_single
// Parameter		: Sound:播放单音频率; delay_50ms:延时时间50ms基数
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
void play_single(uint16_t Sound,uint16_t delay_1ms)
{
	TIMER_WriteCounter(0);
    TIMER_WritePeriod(0);
    TIMER_WriteCompare(0);
	TIMER_Start();
	BeepPlay_Delay(3);
	TIMER_Stop();
	
    TIMER_WriteCounter(0);
    TIMER_WritePeriod(Sound - 1);
    TIMER_WriteCompare(Sound/2 -1);
    TIMER_Start();
    BeepPlay_Delay(delay_1ms);
    TIMER_Stop();
}

//********************************************************************************************************
// Function name	: System_ShortDI_Play
// Description		: 短音
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
void System_ShortDI_Play(void)
{
	play_single(Sound_DI,200);
}

//********************************************************************************************************
// Function name	: System_ShortDI_Play
// Description		: 短音
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
void Defendalarm_Play(void)
{
	play_single(Sound_DI,200);
	BeepPlay_Delay(40);
	play_single(Sound_DI,200);
	BeepPlay_Delay(40);
	play_single(Sound_DI,200);
}


//********************************************************************************************************
// Function name	: System_LongDI_Play
// Description		: 长音
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
void System_LongDI_Play(void)
{
	play_single(Sound_DI,500);
}

//********************************************************************************************************
// Function name	: System_PowerOn_Play
// Description		: 上电播报音3-1
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
void System_PowerOn_Play(void)
{
    play_single(Sound_DO,200);
	BeepPlay_Delay(50);
	play_single(Sound_RE,200);
	BeepPlay_Delay(50);
    play_single(Sound_MI,200);
	BeepPlay_Delay(50);
	play_single(Sound_MI,200);
	BeepPlay_Delay(50);
	play_single(Sound_RE,200);
	BeepPlay_Delay(50);
    play_single(Sound_DO,200);
}

//********************************************************************************************************
// Function name	: System_PowerOn_Play
// Description		: 上电播报音3-1
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
void System_PowerOn_Play_TestSer(void)
{
    play_single(Sound_MI,200);
	BeepPlay_Delay(50);
	#if 1
	play_single(Sound_MI,200);
	BeepPlay_Delay(50);
    play_single(Sound_MI,200);
	BeepPlay_Delay(50);
	#endif
}


//********************************************************************************************************
// Function name	: Comm_Succes_play
// Description		: 常用成功音
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 用于录取指纹成功
//*******************************************************************************************************/
void Comm_Succes_play(void)
{
	play_single(Sound_DI,600);
}


//********************************************************************************************************
// Function name	: Timeout_play
// Description		: 常用超时音
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 用于录入卡超时
//*******************************************************************************************************/
void Timeout_play(void)
{
	play_single(Sound_DI,50);
	BeepPlay_Delay(50);
    play_single(Sound_DI,50);						// 按要求 50-50-50ms
	BeepPlay_Delay(50);
    play_single(Sound_DI,50);
	BeepPlay_Delay(50);
    play_single(Sound_DI,50);
}

//********************************************************************************************************
// Function name	: Comm_Failure_play
// Description		: 常用失败音
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 用于录取指纹成功
//*******************************************************************************************************/
void Comm_Failure_play(void)
{
	#if 0
    play_single(Sound_DI,300);
	BeepPlay_Delay(200);
    play_single(Sound_DI,300);
	BeepPlay_Delay(200);
    play_single(Sound_DI,300);						// 非法ID 不出声
	#endif
}

//********************************************************************************************************
// Function name	: OpenDoor
// Description		: 开门音
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
void OpenDoor(void)
{
	play_single(Sound_DI,50);
	BeepPlay_Delay(50);
    play_single(Sound_DI,50);						// 按要求 50-50-50ms
	
	#if 0
	play_single(Sound_DO,250);
	play_single(Sound_RE,250);
    play_single(Sound_MI,250);
	#endif
}

//********************************************************************************************************
// Function name	: System_PutCard_play
// Description		: 请刷身份证
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
void System_PutCard_play(void)
{
	play_single(Sound_DI,150);					// 按要求单音 150ms
}

//********************************************************************************************************
// Function name	: System_ReadCard_play
// Description		: 成功读取身份证
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
void System_ReadCard_play(void)
{
	play_single(Sound_DI,50);
	BeepPlay_Delay(50);
    play_single(Sound_DI,50);						// 按要求 50-50-50ms
}

//********************************************************************************************************
// Function name	: Beep_Play
// Description		: 所有蜂鸣器播放函数
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: --用于替换语音播报函数 VoicePlay_WithJudge和VoicePlay
//*******************************************************************************************************/
void Beep_Play(uint8_t sound_define)
{
    if (sound_define == Sound_SYSTEM_POWER_ON)
    {
        System_PowerOn_Play();
    }
	else if(Sound_SYSTEM_POWER_ON_TEST == sound_define)
	{
		System_PowerOn_Play_TestSer();
	}
	else if (sound_define == Sound_VERIFY_SUCCESS)
    {
        OpenDoor();
    }
    else if (sound_define == Sound_VERIFY_FAILURE)
    {
        Comm_Failure_play();
    }
	else if (sound_define == Sound_TIMEOUT)
    {
        Timeout_play();
    }
	
    else if ((sound_define == Sound_REG_FAILURE) 	||
			 (sound_define == Sound_DELETE_FAILURE) ||
			 (sound_define == Sound_EMPTY_FAILURE))
    {
        Comm_Failure_play();
    }
    else if ((sound_define == Sound_REG_SUCCESS) 	||
			 (sound_define == Sound_DELETE_SUCCESS) || 
			 (sound_define == Sound_EMPTY_SUCCESS))
    {
        Comm_Succes_play();
    }
	else if (sound_define == Sound_PleasePutCard)
    {
    	System_PutCard_play();
    }
	else if (sound_define == Sound_ReadCardSuccess)
    {
    	System_ReadCard_play();
    }
	else if (sound_define == Sound_ShortDI)
    {
    	System_ShortDI_Play();
    }
	else if (sound_define == Sound_LongDI)
    {
    	System_LongDI_Play();
    }
	else if(sound_define == Sound_DEFENDALRM)
	{
		Defendalarm_Play();
	}
}

//********************************************************************************************************
// Function name	: VoicePlay
// Description		: 语音播报
// Parameter		: addr
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void VoicePlay(INT8U addr)
{
    Beep_Play(addr);  
}


//****************************************** END OF FILE *********************************************//
