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
//----------- 指纹头命令包数据结构 ------------//
typedef struct zPacket
{
  	INT8U Start_H;				//包头:   固定为0xef01，传送时高字节在前.
	INT8U Start_L;
	
	INT8U Addr_1;				//地址:   默认值为0xffffffff，用户可通过指令生成新地址，模块会拒绝
	INT8U Addr_2;				//		   地址错误的数据包。传送时高字节在前.
	INT8U Addr_3;
	INT8U Addr_4;
	
	INT8U Pid;					//包标识: 0x01表示是命令包可以跟后续包; 
								//		   0x02表示是数据包，且有后续包。数据包不能单独进入执行流程，
								//		   必须跟在指令包或应答包后面;
								//		   0x07表示是应答包，可以跟后续包;
								//		   0x08表示是最后一个数据包，即结束包(EndData packet).
	
	INT8U Length_H;				//包长度: 包长度指的是包内容(指令/数据)的长度加上效验和的长度(即包
	INT8U Length_L;				//		   内容长度+2)。长度以字节为单位(即字节数)，传送时高字节在前.
	
	
	INT8U Data[33];				//由于数据长度不定，暂且定为10个字节的数据，后续有需要更大的数据再更改
								//数据:   可以是指令、数据、指令的参数、应答结果等.
								//		   (指纹特征值、指纹模板都是数据)
	
	INT8U CheckSum_H;			//校验和: 包标识、包长度和包内容的所有字节的单字节算术累计和，
	INT8U CheckSum_L;			//		   即校验和=包标识+包长度+包内容。超过2字节的进位忽略。
								//		   传送时高字节在前。
											   
}ZFMXXXSA_Packet;


//------------ 无线命令包数据结构 -------------//
typedef ZFMXXXSA_Packet         WIRELESS_Packet;


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


//----------- 开门记录信息数据结构 ------------//
typedef struct tagOpenInfo
{
	INT32U 		OpenTime;     	//开门时间
	INT32U		Copy;			//副本标记
	INT8U		UserId;      	//开锁的用户ID
	INT8U		OpenType; 		//开锁的类型：指纹/密码/指+密/无线
} OpenInfo;


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

#define LARGE							2								//大于							//
#define EQUAL							1								//等于							//
#define SMALL							0								//小于							//


//******************************************** 唤醒中断定义  *******************************************//
#define INT_USERKEY						1
#define INT_FM17550						2
#define INT_TIMER						3



//******************************************* 超时时间定义 *********************************************//
#define TIMEOUT_500MS					500								// 500ms						//
#define TIMEOUT_1S						1000							// 1s							//
#define TIMEOUT_3S						3000							// 3s							//
#define TIMEOUT_5S						5000							// 5s							//
#define TIMEOUT_8S						8000							// 8s							//
#define TIMEOUT_10S						10000							// 10s							//
#define TIMEOUT_15S						15000							// 15s							//



//******************************************** 开门方式定义 ********************************************//
#define	OpenType_Password    			0
#define	OpenType_Fingerprint    		1
#define	OpenType_Card    				2
#define	OpenType_Wireless    			3
#define	OpenType_PwdAndFP    			4



//********************************************** 按键定义  *********************************************//
// ----------数字键盘模式--------------- //
#define	Key_0    						11
#define	Key_1     						1
#define	Key_2    	 					2
#define	Key_3   						3
#define	Key_4    						4
#define	Key_5    						5
#define	Key_6     						6
#define	Key_7   						7
#define	Key_8   						8
#define	Key_9    						9
// ----------菜单键盘模式--------------- //
#define	Key_UP      					2
#define	Key_DOWN    					8
#define Key_LEFT						4
#define Key_RIGHT						6
// --------数字或键单键盘模式----------- //
#define	Key_RETURN  					10
#define	Key_OK      					12



//******************************************** 按键位置定义 ********************************************//
#define Position_1						1
#define Position_2						2
#define Position_3						3
#define Position_4						4
#define Position_5						5
#define Position_6						6
#define Position_7						7
#define Position_8						8
#define Position_9						9
#define Position_C						10
#define Position_0						11
#define Position_OK						12



//********************************************* 电量宏定义 *********************************************//
#define HigherVoltage  					43000					//电压阀值：6.0V   	  					//
#define HighVoltage						40000					//电压阀值：5.5V 						//
#define NormalVoltage  					37000					//电压阀值：5.0V   	  					//
#define LowVoltage  					33500					//电压阀值：4.6V   	  					//
#define LowerVoltage					31500					//电压阀值：4.2V 						//

#define Battery_HigherVoltage			0						//电池状态：电量饱和   	  				//
#define Battery_HighVoltage				1						//电池状态：电量充足   	  				//
#define Battery_NormalVoltage			2						//电池状态：电量一般   	  				//
#define Battery_LowVoltage				3						//电池状态：电量不足   	  				//
#define Battery_LowerVoltage			4						//电池状态：电量用尽   	  				//



//******************************************* 验证模式宏定义 *******************************************//
#define FP_Enable_MASK					0x01					//指纹验证使能   	  					//
#define PWD_Enable_MASK					0x02					//密码验证使能   	  					//
#define CARD_Enable_MASK				0x04					//IC卡验证使能   	  					//
#define WL_Enable_MASK					0x08					//无线验证使能   	  					//



#endif

//******************************************** END OF FILE *********************************************//