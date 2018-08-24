//********************************************************************************************************
// File name		: config.h	(此头文件可被所有文件调用)
// Description 		: 通用配置文件，包含了通用头文件、数据类型定义、数据结构、MCU的PORT和GPIO操作宏定义
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//********************************************************************************************************
#if !defined(__CONFIG_H__)
#define __CONFIG_H__

//********************************************** 头文件 ************************************************//
#include "cytypes.h"
#include "CyLib.h"	
#include "stdio.h"


//***************************************** 通用数据类型重定义 *****************************************//
typedef unsigned char           	BOOL;				//布尔变量										//
typedef unsigned char           	INT8U;				//无符号8位整形变量								//
typedef signed   char           	INT8S;				//有符号8位整形变量								//
typedef unsigned short int         	INT16U;				//无符号16位整形变量							//
typedef signed   short int         	INT16S;				//有符号16位整形变量							//
typedef unsigned long           	INT32U;				//无符号32位整形变量							//
typedef signed   long           	INT32S;				//有符号32位整形变量							//
typedef unsigned long long      	INT64U;				//无符号64位整形变量							//
typedef signed 	 long long     		INT64S;				//有符号64位整形变量							//
typedef float                   	FP32;				//单精度浮点数(32位)							//
typedef double                  	FP64;				//双精度浮点数(64位)							//

typedef volatile BOOL               vBOOL;				//防编译器优化定义								//
typedef volatile INT8U              vINT8U;
typedef volatile INT8S              vINT8S;
typedef volatile INT16U             vINT16U;
typedef volatile INT16S             vINT16S;
typedef volatile INT32U             vINT32U;
typedef volatile INT32S             vINT32S;
typedef volatile INT64U             vINT64U;
typedef volatile INT64S             vINT64S;
typedef volatile FP32               vFP32;
typedef volatile FP64               vFP64;

#define BYTE                        INT8S				//以下定义移植于usos系统						//
#define UBYTE                       INT8U
#define WORD                        INT16S
#define UWORD                       INT16U 
#define LONG                        INT32S 
#define ULONG                       INT32U
	

//********************************************** Struct ************************************************//
//--------------- 日期数据结构 ----------------//
typedef  struct tagDateInfo
{
	INT8U		Year;  			//年：2000~
	INT8U		Mon; 			//月：01 ~ 12
	INT8U  		Day;   			//日：01 ~ 31
} DateInfo;


//--------------- 时钟数据结构 ----------------//
typedef  struct tagClockInfo
{
	INT8U		Hour;  			//年：00 ~ 23
	INT8U  		Min; 			//月：01 ~ 12
	INT8U  		Sec;   			//日：01 ~ 31
} ClockInfo;


//--------------- 时间数据结构 ----------------//
typedef  uint32 UTCTime;
typedef  struct tagTimeInfo
{
	INT16U		Year;  			//年：2000~
	INT8U  		Mon; 			//月：01 ~ 12
	INT8U  		Day;   			//日：01 ~ 31
	INT8U  		Hour;  			//时：00 ~ 23
	INT8U  		Min;   			//分：00 ~ 59
	INT8U  		Sec;   			//秒：00 ~ 59
} TimeInfo;



//********************************************* 共用体声明 **********************************************//
typedef union tagData
{
	INT32U 		WORD;
	INT8U	  	CHAR[4];
} DataType;


//********************************************** 宏定义 ************************************************//
#define TRUE							1								//正确							//
#define FALSE							0								//错误							//

#define ON								1								//打开							//
#define OFF								0								//关闭							//
	
#define SET								1								//设置							//
#define RESET							0								//重置							//

#define NO								0								//否							//
#define YES								1								//是							//
#define TIMEOUT							2								//超时							//

#define LARGE							2								//大于							//
#define EQUAL							1								//等于							//
#define SMALL							0								//小于							//


//******************************************* 超时时间定义 *********************************************//
#define TIMEOUT_500MS					500								// 500ms						//
#define TIMEOUT_1S						1000							// 1s							//
#define TIMEOUT_3S						3000							// 3s							//
#define TIMEOUT_5S						5000							// 5s							//
#define TIMEOUT_8S						8000							// 8s							//
#define TIMEOUT_10S						10000							// 10s							//
#define TIMEOUT_15S						15000							// 15s							//


//************************************ 每个升级数据包内容大小定义 **************************************//
#define SYSTEM_VERSION					(13)							// 更新为版本号1.04_13 @05_12 	//
#define	DATABUF_SIZE					(1050)

// IC卡 合法性码存放扇区
#define VALID_BLK						(4)
#define TIMESTAMP_BLK					(5)

// 设备所有报警功能配置
#define	FireAlarm_Open					(1)
#define DefendAlarm_Open				(1)
#define MgCheck_Open					(1)
#define PowerOnCheck_Open				(1)

#endif
//******************************************** END OF FILE *********************************************//