//********************************************************************************************************
// File name		: CmdProtocol.h
// Description 		: x33AppProtocol.c的头文件
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#ifndef __CMDPROTOCOL_H__
#define __CMDPROTOCOL_H__


//******************************************** 头文件声明 **********************************************//
#include <project.h>
#include <stdint.h>
#include <string.h>
#include "common.h"
#include "crc16check.h"
#include "Bluetooth.h"
#include "AtProtocol.h"
#include "card.h"
#include "voice.h"
#include "UserRTC.h"
#include "user_data.h"



//********************************************* 协议版本 ***********************************************//
#define PROTOCOL_VER  						1					// 不加密01; 加密02
	
	
//********************************************* 配置说明 ***********************************************//
#define DEF_RSSI							0					// 心跳包开启GPRS信号强度上报
	

//***************************************** 信道及发送/应答包 ******************************************//
#define HEAD_BYTE1 							0xEF
#define HEAD_BYTE2 							0x3A
#define PACK_SEND 							0
#define PACK_RESP 							1


//****************************************** 智能门禁错误码 ********************************************//
#define RT_SUCC								0x01				// 成功									//
#define RT_FAIL								0x02				// 失败									//
#define RT_PACK_ERR							0x03				// 包错误								//
#define RT_NEWEST							0x04				// 最新版本								//
#define RT_MAC_ERR							0x05				// MAC错误								//
#define RT_UID_ERR							0x06				// UID错误								//
#define RT_UTC_ERR							0x07				// 时间戳错误							//
#define RT_ValidTime_ERR					0x08				// 有效期错误							//
#define RT_StartOrEnd_ERR					0x09				// 开始时间或结束时间错误				//
#define RT_AlarmType_ERR					0x0A				// 报警类型错误							//
#define RT_IPorPort_ERR						0x0B				// 域名或端口错误						//
#define RT_GroupNum_ERR						0x0C				// 组号错误								//
#define RT_Version_ERR						0x0D				// 版本号错误							//
#define RT_Timeout_ERR						0x0F				// 超时错误								//
#define RT_Invalid_ERR						0x10				// 未发现可用信息						//
#define RT_IP_NUM_ERR						0x11				// 域名错误								//
	
	

//******************************************** 开门方式 ************************************************//
#define OTHERS	     	 					0x01                
#define BLE_OPEN     	 					0x02
#define GPRS_OPEN	  	 					0x03
#define BLE_KEY_OPEN	  	 				0x04
#define FIRE_OPEN	  	 					0x05
#define CODE_OPEN	  	 					0x06
#define CARD_OPEN	  	 					0x07
#define HAND_OPEN	  	 					0x08


//******************************************** 心跳类型 ************************************************//
#define DoorOpened     	 					0x01                
#define DoorClosed  	 					0x02


//******************************************** 报警类型 ************************************************//
#define ALARM_Fire     	 					0x01                
#define ALARM_Destroy  	 					0x02
#define ALARM_ErrOpenQty 					0x03
#define ALARM_ErrDoorState	  	 			0x04
#define ALARM_LowPower	  	 				0x05
#define ALARM_PowerDown	  	 				0x06
#define ALARM_PowerUp  	 					0x07
#define ALARM_IcCardErr	 					0x08
#define ALARM_UpdateOk 	 					0x09
#define ALARM_UpdateErr	 					0x0A


//********************************************* 宏定义 *************************************************//
#define	CMD_GET_PRESEVER					0x01
#define	CMD_GET_MAC_AND_KEY					0x02
#define	CMD_READ_UID						0x03
#define	CMD_WRITE_UID						0x04
#define	CMD_DEL_UID							0x05
#define	CMD_OPEN_DOOR						0x06
#define	CMD_SEND_OPEN_INFO					0x07
#define	CMD_PLUSE							0x08
#define	CMD_CLEAR_UID						0x09
#define	CMD_TIME_CHECK						0x0A
#define	CMD_SEND_WARN_INFO					0x0B	
#define	CMD_READ_INFO						0x0C
#define	CMD_REMOTE_UPDATE					0x0D
#define	CMD_REMOTE_RESTART					0x0E
#define CMD_DOWNLOAD	                	0x0F
#define CMD_CHANGE_IP	                	0x10
#define CMD_MANUFAC                			0xF1
#define CMD_CLROPENDATA              		0xF2


//***************************************** 蓝牙地址长度定义 *******************************************//
#define BD_ADDR_LEN							6	
#define OPENDATA_UPPAGE_LEN					5					// 指定时间内开门信息单次上报条数	

//********************************************* 函数声明 ***********************************************//
void Cmd_Protocol_ParaInit(void);
uint8_t send_get_pre_service_ip(void);
uint8_t send_send_mac_and_get_blekey(void);
uint8_t send_upload_open_info(open_door_t* open_info, uint8_t uidlen, uint8_t open_mode);
uint8_t send_heartbeat(uint8_t lock_state);
uint8_t send_time_adjust(void);
uint8_t send_alarm_info(uint8_t alarm_mode);
uint8_t send_update_req(uint16 page);
void Protocol_SendAckPacket(uint8 CommChanle, uint8 cPackType, uint8 cCmd,uint8 *UserData, uint16 UserDataLen);
void Protocol_CmdParse(void);
void Protocol_CmdParse_BLE(void);


#endif

//******************************************** END OF FILE *********************************************//
