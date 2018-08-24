//********************************************************************************************************
// File name		: variable.h
// Description 		: 项目中用到的变量
// Author			: Keno
// Date				: 2017.8.25
// Version			: V02
//********************************************************************************************************
#include "config.h"


//******************************************** 变量声明 ************************************************//
uint8  system_version = SYSTEM_VERSION;							// 系统版本								//

uint8  LED_State = 0;											// 系统状态指示灯						//
uint8  gprs_state;                                            	// GPRS状态					            //
uint8  EnterGuide_State = 0;									// 门禁状态：开门/关门					//
uint8  SendMacFlag = 0;											// 申报mac地址标志						//

uint8  domain[200];												// 域名									//
uint8  pre_ip[200];												// 前置服务器地址						//
uint8  pre_port[6];												// 前置服务器端口						//

uint8  update_ip[16];											// 远程升级服务器地址					//
uint8  update_port[5];											// 远程升级服务器端口					//
uint16 update_version;											// 升级版本								//
uint8  update_flag;												// 升级标志								//
uint8  update_report;											// 升级成功与否 报警信息上报情况		//

uint16	update_page;
#if 0
uint16	update_offset;
uint16	Image_size;
#endif

uint32	update_offset;
uint32	Image_size;	

volatile uint8 Read_Mac_Flag;									// 读取MAC地址标志						//
volatile uint8 Mac_Addr[6];										// MAC地址								//

volatile uint32 wait_timeout;									// 通用等待超时时间						//
volatile uint32 recv_timeout;									// 收到包头后，等待一个包收完的超时时间	//

volatile char  GPRS_AT_Buf[64];      							// AT指令缓存，用于AT指令解析			//
volatile uint8 GPRS_AT_RecFinish;								// AT命令包收完标志						//

volatile uint8 GPRSCmdParseFlag1;								// 协议命令包1收完标志					//
volatile uint8 GPRSCmdParseFlag2;								// 协议命令包2收完标志					//
volatile uint8 GPRSCmd_buf1[DATABUF_SIZE];						// 协议命令包1缓存						//
volatile uint8 GPRSCmd_buf2[DATABUF_SIZE];						// 协议命令包2缓存						//

uint16 CurrentCommandPktTotalSize;								// 蓝牙包总大小							//
uint8  CurrentDataNum;											// 已收取长度							//
uint8  BleCmdBuf[DATABUF_SIZE];									// 蓝牙包缓存							//
uint8  BleCmdValidFlag;											// 蓝牙包有效位标志						//

uint32 cur_time;												// 当前时间								//
uint32 next_network_time;										// 下一次查询网络的时间					//
uint32 next_heartbeat_time;										// 下一次发送心跳的时间					//
uint32 next_gprs_operation_time;								// 下一次GPRS操作时间					//
uint32 next_gprs_upload_time = 0;								// 下一次GPRS主动上报时间				//
uint32 next_allupload_time;										// 下一次GPRS主动上报时间				//
uint32 next_keyopen_time;
uint16 openinfopage_offset;										// 开门信息flag 256个一页来分			//
uint8  uploadpolling_stop;										// 轮训上报开门信息 停止位				//
uint32 next_firealarm_time;										// 1min一次报警计时

uint32 last_readcardtime;										// 主循环读卡间隔						//
uint32 defendalarm_time;										// 防撬报警 计时间隔					//
uint32 MgCheck_time;											// 门磁检测 计时间隔					//
uint32 FireCheck_time;											// 火警开门 上报 计时					//

uint8 uploadflag_cnt;
uint16 last_index;
uint8  update_timeout_count = 0;								// 远程升级超时次数						//
volatile uint32 update_timeout_time = 0;						// 远程升级超时时间						//

uint8  gprs_operation_ErrCount = 0;								// gprs操作失败计数						//
uint8  heartbeat_SendCount = 0;								    // 心跳包發送计数					//
volatile uint32  heartbeat_lasttime;                            // 上次的心跳发送时间
volatile uint32  CardCheck_CountTime = 0;                       // 刷卡计时

uint8  Card_ErrCount;											// 刷卡验证错误次数						//
uint8  flag_reboot;												// 重启标志	    						//
uint8  stop_reboot;	 											// 升级的过程中不重启					//
uint8  reboot_count;
uint32 mem_var;													// 加密IC的内存计数变量					//
uint8 next_defend_flag;											// 防止撬锁 报警标志位					//
uint8 Code_Action_flag = 0;										// 程序执行到主循环标志位				//
uint8 once_poweron_flag;										// 上电报警一次标志
uint8 firealarm_flag;											// 火警报警触发标志


//******************************************** END OF FILE *********************************************//
