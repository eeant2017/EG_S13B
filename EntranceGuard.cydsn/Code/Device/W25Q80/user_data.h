//********************************************************************************************************
// File name		: user_data.c
// Description		: 用户数据相关的函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#ifndef __USER_DATA_H__
#define __USER_DATA_H__


//******************************************** 头文件声明 **********************************************//
#include "cytypes.h"
#include "common.h"
#include "config.h"
#include "w25qxx.h"
//#include "CmdProtocol.h"

//********************************************** 宏定义 ************************************************//
#define CARD_UID_LEN			(8)
#define TIME_LEN				(4)

#define	Delta					(30)							// 时间误差定义	s						//
	
//******************************************** 结构体声明 **********************************************//
typedef struct user_info
{
	uint8 card_uid[CARD_UID_LEN]; 								// 身份证 UID							//
	uint8 valid_time[4]; 										// 有效期限，格林威治时间，结束时间		//
} user_info_t;

typedef struct open_door
{
	uint8 card_uid[CARD_UID_LEN]; 								// 身份证 UID							//
	uint8 open_time[4]; 										// 开门时间								//
	uint8 send_time[4];											// 发送时间								//
} open_door_t;  


//******************************************* 产品信息声明 *********************************************//
#define MAX_USR_NUM				(500)							// 用户数量								//
#define MAX_OPEN_NUM			(5000)							// 开门记录数量							//

#define USER_FLAG_BASE			(0x0)							// 用户标志位存储地址					//
#define USER_FLAG_SIZE			(500)							// 用户标志位总数						//
#define USER_INFO_BASE			(0x200)							// 用戶信息开始地址						//

#define OPEN_FLAG_BASE			(0x1A00)						// 开门标志位存储地址					//
#define OPEN_FLAG_SIZE			(5000)							// 开门标志位总数						//
#define OPEN_INFO_BASE			(0x2E00)						// 开门信息存储地址						//

#define OPEN_QTY_BASE			(0x19000)						// 开门数量存储地址						//

#define REBOOT_TYPE			    (0x19100)
#define CenterIP_TYPE			(0x19101)

#define UPDATE_SYSTEMVER		(0x19103)						// 2B 升级系统版本号
#define UPDATE_FLAG				(0x19105)						// 是否升级的标志位


#define MAC_BASE				(0x19200)						// 生产设置MAC地址						//
#define MACFLAG_BASE			(0x19206)						// 生产设置MAC地址						//
#define	DEBUG_PRJ				(1)								// 代表测试工程							//




//********************************************* 函数声明 ***********************************************//
void init_eeprom(void);
void user_init_valid_flag(void);
void user_set_valid_flag(uint16 user_id);
void user_clr_valid_flag(uint16 user_id);
void user_add_info(uint16 user_id, user_info_t *info);
void user_get_info(uint16 user_id, user_info_t *info);
void user_get_card_uid(uint16 user_id, uint8 *card_uid);
void user_get_user_id(uint16 *user_id, uint8 *card_uid);
void user_get_valid_time(uint16 user_id, uint8 *valid_time);
void user_get_type(uint16 user_id, uint8 *type);

uint8  user_find_free_id(uint16 *user_id);
uint8  user_id_if_valid(uint16 user_id);
uint16 user_get_reg_qty(void);

uint8 add_user(uint16 user_id, user_info_t *info);
uint8 chg_user(uint16 user_id, user_info_t *info);
uint8 del_user(uint16 user_id);
uint8 clr_user(void);

uint8 UserData_Add_User(user_info_t *info);
uint8 UserData_DEL_User(uint8 *card_uid);
uint8 UserData_CLR_User(void);


uint8 UserData_Judge_If_Open(user_info_t *info);


void 	Write_Open_UploadFlag(uint16 open_num, uint8 UploadFlag);
uint8 	Read_Open_UploadFlag(uint16 open_num);
void 	Write_Open_Type(uint16 open_num, uint8 type);
uint8 	Read_Open_Type(uint16 open_num);
void 	Write_Open_Qty(uint16 openQty);
uint16 	Read_Open_Qty(void);
void 	Write_OpenInfo(open_door_t *openInfo, uint8_t openType);
void 	Read_OpenInfo(uint16 open_num, open_door_t *openInfo);
uint16 	Find_Open_StartNum(uint32 start_time);
uint16 	Find_Open_EndNum(uint32 end_time);
uint16 	Find_Open_ValidQty(uint32 start_time, uint32 end_time);
void 	Get_Open_Info(uint16 start_num, uint16 qty, uint8 *data);
void 	FindAndSet_UploadTime_UploadFlag(uint8 *open_time, uint8 *utc_time);

void 	Clear_AllOPeninfo(void);
void 	test_Flash(void);

void    Write_RebootType(uint8 rType);
uint8   Read_RebootType(void);

void 	Write_CenterIpType(uint8 rType);
uint8 	Read_CenterIpType(void);

void Write_UpdateInfo(uint8 flag, uint16 ver);
uint8 Read_Updateflag(void);
uint8 Read_UpdateVer(void);

#endif


//******************************************** END OF FILE *********************************************//
