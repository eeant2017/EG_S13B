//********************************************************************************************************
// File name		: AtProtocol.h
// Description 		: AtProtocol.c的头文件
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#ifndef __ATPROTOCOL_H__
#define __ATPROTOCOL_H__



//******************************************** 头文件声明 **********************************************//
#include <stdint.h>
#include "string.h"
#include "common.h"
#include "Uart433.h"
#include "card.h"
#include "voice.h"
#include "crc16check.h"
#include "UserRTC.h"
#include "user_data.h"


//********************************************* 宏定义 *************************************************//	
#define TCP						1
#define UDP						2
#define proto					TCP


//**************************************** GPRS模块状态定义 ********************************************//
#define INIT					0
#define CEN_CONNECT				1
#define GET_PRE_IP				2
#define CEN_CLOSED				3
#define PRE_CONNECT				4
#define NORMAL_COMM				5
#define NEED_INIT				6
#define NEED_UPDATE				7
#define PRE_CLOSED				8
#define UPDATE_CONNECT			9
#define WAIT_UPDATE				10
#define UPDATE_FINISH			11

#define gprs_resetCount			10

//******************************************* 外部函数声明 *********************************************//
extern void ble_heartbeat(void);



//********************************************* 函数声明 ***********************************************//
void	GPRS_AT_ParaInit(void);
void 	GPRS_Send_ConnectServer(char *tcp, char *ip, char *port);
void 	GPRS_Send_LengthOfSendData(uint16_t len);
uint8_t GPRS_AT_CheckAck(char *r_char);
uint8_t GPRS_AT_SendAndCheckAck(char *s_char, char *r_char, uint16_t size, uint16_t timeout);
uint8_t GPRS_WaitInitFinish(void);
uint8_t GPRS_QuerySimCard(void);
uint8_t GPRS_QueryCREG(void);
uint8_t GPRS_QueryRssi(void);
uint8_t GPRS_TestAndWaitCommonOK(void);
uint8_t GPRS_SetAPN(void);
uint8_t GPRS_SetActive(void);
uint8_t GPRS_CheckConnectState(void);
uint8_t GPRS_ClosedConnect(void);
uint8_t GPRS_SetConnectMode(void);
uint8_t GPRS_OpenNetworkLED(void);
uint8_t GPRS_ConnectCenterServer(void);
uint8_t GPRS_ConnectPreServer(void);
uint8_t GPRS_ConnectUpdateServer(void);
uint8_t GPRS_SendHexDataToServer(uint8_t *data, uint16_t size);
void 	GPRS_CheckIfRecvClosed(void);


#endif

//******************************************** END OF FILE *********************************************//
