//********************************************************************************************************
// File name		: main.h
// Description 		: main.c的头文件
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include <project.h>
#include "variable.h"
#include "common.h"
#include "UserRTC.h"
#include "UserWDT.h"
#include "Bluetooth.h"
#include "AtProtocol.h"
#include "CmdProtocol.h"
#include "isr.h"
#include "card.h"
#include "voice.h"
#include "user_data.h"
#include "mifare_card.h"
#include "type_a.h"

//********************************************* 函数声明 ***********************************************//
void AppCallback(uint32 event, void* eventParam);
void LowPowerImplementation(void);
void ble_heartbeat(void);
void delay_with_ble(uint32 ms);
void Led_AllOff(void);
void Led_RedON(void);
void Led_GreenON(void);
void Led_BlueON(void);
void Led_SetState(uint8 Sta);
void DoorOpen(void);
void DoorClose(void);
void GPRSPowerOn(void);
void GPRSPowerDown(void);
void ResetGPRS(void);
void BLE_Tick(void);
void GPRS_Tick(void);
void CARD_Tick(void);
void KeyOpen_Tick(void);
void FireOpen_Tick(void);
void DefendAlarm_Tick(void);
void Defend_Tick(void);
void Reboot_Check(void);


//******************************************** END OF FILE *********************************************//
