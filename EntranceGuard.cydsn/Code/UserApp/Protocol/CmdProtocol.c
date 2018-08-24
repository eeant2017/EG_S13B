//********************************************************************************************************
// File name		: x33AppProtocol.c
// Description		: 蓝牙协议相关函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "CmdProtocol.h"



//******************************************* 外部变量声明 *********************************************//
extern uint8 system_version;

extern volatile uint8 Mac_Addr[6];

extern uint8 domain[200];
extern uint8 pre_ip[200];
extern uint8 pre_port[6];
extern uint8 update_ip[16];
extern uint8 update_port[5];
extern uint8 update_flag;
extern uint16 update_version;
extern uint16 update_page;
#if 0
extern uint16 update_offset;
extern uint16 Image_size;
#endif

extern uint32 update_offset;
extern uint32 Image_size;

extern uint8 SendMacFlag;
extern uint8 gprs_state;

extern uint8 GPRSCmdParseFlag1;
extern uint8 GPRSCmdParseFlag2;
extern volatile uint8 GPRSCmd_buf1[DATABUF_SIZE];
extern volatile uint8 GPRSCmd_buf2[DATABUF_SIZE];

extern volatile uint8  RecvType;
extern volatile uint8  CmdRev_buf[DATABUF_SIZE];
extern volatile uint16 Cmd_Baglen;
extern volatile uint8  Cmd_Revsta;
extern volatile uint16 Cmd_index;

extern uint32 cur_time;
extern uint32 next_network_time;
extern uint32 next_heartbeat_time;
extern uint32 next_gprs_operation_time;
extern uint32 next_gprs_upload_time;									// 下一次GPRS主动上报时间				//
extern uint32 next_allupload_time;		
extern uint8  uploadpolling_stop;										// 轮训上报开门信息 停止位				//


extern uint8  update_timeout_count;
extern volatile uint32 update_timeout_time;
extern volatile uint32 wait_timeout;
extern uint8  gprs_operation_ErrCount;
extern uint8  heartbeat_SendCount;

extern uint8  Card_ErrCount;
extern uint8  flag_reboot;
extern uint8  stop_reboot;

extern uint8  LED_State;


//********************************************* 变量声明 ***********************************************//
uint16 UploadQty;
uint16 UploadStartnum;



//******************************************* 外部函数声明 *********************************************//
extern void delay_with_ble(uint32 ms);
extern void ble_heartbeat(void);
extern void Led_RedON(void);
extern void Led_GreenON(void);
extern void Led_SetState(uint8 Sta);
extern void DoorOpen(void);
extern void DoorClose(void);
extern void Updatealarm_check(void);


int Endpoint_exchange(uint8 card_data[])
{
	uint8 databuf[8] = {0};

	DBG_SOFT_PutStr("Endpoint_exchange1:\n");
	DBG_SOFT_PrintfHex(card_data,8,16);
	
	// 如果是A卡 前/后面4B是0 转换位置 
	if((card_data[4] == 0 && card_data[5] == 0 
	 && card_data[6] == 0 && card_data[7] == 0) ||
	   (card_data[0] == 0 && card_data[1] == 0 
	 && card_data[2] == 0 && card_data[3] == 0))
	{
		memcpy(databuf+4, card_data, 4);						// 前4个UID拷贝到后四个 
		memcpy(databuf, card_data+4, 4);						// 后4个UID拷贝到前四个 
		
		memcpy(card_data,databuf,8);							// 全部考回
	}
	DBG_SOFT_PutStr("Endpoint_exchange2:\n");
	DBG_SOFT_PrintfHex(card_data,8,16);
	return 0;
}



//********************************************************************************************************
// Function name	: Cmd_Protocol_ParaInit
// Description		: 命令相关参数初始化
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Cmd_Protocol_ParaInit(void)
{
	RecvType = 0;
	Cmd_Baglen = 0;
	Cmd_Revsta = 0;
	Cmd_index = 0;
	GPRSCmdParseFlag1 = RESET;
	GPRSCmdParseFlag2 = RESET;
	memset((char *)CmdRev_buf, 0, sizeof(CmdRev_buf));
	memset((char *)GPRSCmd_buf1, 0, sizeof(GPRSCmd_buf1));
	memset((char *)GPRSCmd_buf2, 0, sizeof(GPRSCmd_buf2));
}

//===【开始】===================== 业务层主动上报服务器指令 ==============================================

//********************************************************************************************************
// Function name	: send_get_forntserviceip
// Description		: 获取服务器地址  0x01
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
uint8_t send_get_pre_service_ip(void)
{
	uint8 uart_buf[96];
	uint8 time_arr[4];
	uint32_t time_tmp;
	uint16_t TempCrc = 0;
	uint16_t cnt = 0;
	uint16_t i = 0;
	uint16_t pack_len = 0x0F;
	
	uart_buf[cnt++] = 0xef;
	uart_buf[cnt++] = 0x3a;
	uart_buf[cnt++] = (pack_len >> 8) & 0xFF;					// 包长度高字节							//
	uart_buf[cnt++] = pack_len & 0xFF;							// 包长度低字节							//
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x01;
	
	for(i = 0; i < 6; i++)										// MAC地址								//
	{
		uart_buf[cnt++] = Mac_Addr[i];
	}
	
	time_tmp = (uint32)RTC_GetUnixTime();						// 时间戳								//
	LONG2BYTE(time_tmp, time_arr);
	uart_buf[cnt++] = time_arr[0];
	uart_buf[cnt++] = time_arr[1];
	uart_buf[cnt++] = time_arr[2];
	uart_buf[cnt++] = time_arr[3];
	
	TempCrc = MyCrc16Check(uart_buf, pack_len + 0x02);			// CRC校验								//
	uart_buf[cnt++] = (TempCrc >> 8) & 0xFF;
	uart_buf[cnt++] = TempCrc & 0xFF;
	
	GPRS_SendHexDataToServer((uint8_t *)uart_buf, ((uart_buf[2] << 8) + uart_buf[3] + 4));
	return 0;
}

//********************************************************************************************************
// Function name	: send_send_mac_and_get_blekey
// Description		: 申报mac和获取blekey  0x02
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
uint8_t send_send_mac_and_get_blekey(void)
{
	uint8 ver;
	uint8 uart_buf[96];
	uint8 time_arr[4];
	uint32_t time_tmp;
	uint16_t TempCrc = 0;
	uint16_t cnt = 0;
	uint16_t i = 0;
	uint16_t pack_len = 0x11;
    
	uart_buf[cnt++] = 0xef;
	uart_buf[cnt++] = 0x3a;
	uart_buf[cnt++] = (pack_len >> 8) & 0xFF;					// 包长度高字节							//
	uart_buf[cnt++] = pack_len & 0xFF;							// 包长度低字节							//
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x02;
	
	for(i = 0; i < 6; i++)										// MAC地址								//
	{
		uart_buf[cnt++] = Mac_Addr[i];
	}

	ver = system_version;										// 固件版本号							//
	uart_buf[cnt++] = (ver >> 8) & 0xFF;
	uart_buf[cnt++] = ver & 0xFF;
	    
	time_tmp = (uint32)RTC_GetUnixTime();						// 时间戳								//
	LONG2BYTE(time_tmp, time_arr);
	uart_buf[cnt++] = time_arr[0];
	uart_buf[cnt++] = time_arr[1];
	uart_buf[cnt++] = time_arr[2];
	uart_buf[cnt++] = time_arr[3];
	
	TempCrc = MyCrc16Check(uart_buf, pack_len + 0x02);			// CRC校验								//
	uart_buf[cnt++] = (TempCrc >> 8) & 0xFF;
	uart_buf[cnt++] = TempCrc & 0xFF;
	
	GPRS_SendHexDataToServer((uint8_t *)uart_buf, ((uart_buf[2] << 8) + uart_buf[3] + 4));
	return 0;
}


//********************************************************************************************************
// Function name	: send_upload_open_info
// Description		: 开门信息上报  0x07
// Parameter		: open_info	: 开门信息
//					: uidlen	: UID长度
//					: open_mode	: 开门方式
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
uint8_t send_upload_open_info(open_door_t* open_info, uint8_t uidlen, uint8_t open_mode)
{
	uint8 uart_buf[96];
	uint8 time_arr[4];
	uint32_t time_tmp;
	uint16_t TempCrc = 0;
	uint16_t cnt = 0;
	uint16_t i = 0;
	open_door_t *pstr = open_info;
    uint16_t pack_len = 0x1c;
		
	uart_buf[cnt++] = 0xef;
	uart_buf[cnt++] = 0x3a;
	uart_buf[cnt++] = (pack_len >> 8) & 0xFF;					// 包长度高字节							//
	uart_buf[cnt++] = pack_len & 0xFF;							// 包长度低字节							//
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x07;
	
	for(i = 0; i < 6; i++)										// MAC地址								//
	{
		uart_buf[cnt++] = Mac_Addr[i];
	}
	
	Endpoint_exchange(pstr->card_uid);							// A卡数据位置转换 @ 18-1-17			//
	{
		for(i = 0; i < uidlen; i++)								// B卡不变								//
		{
			uart_buf[cnt++] = pstr->card_uid[i];
		}
	}
	
	uart_buf[cnt++] = open_mode;								// 开门方式								//
	
	uart_buf[cnt++] = pstr->open_time[0];						// 开门时间								//
	uart_buf[cnt++] = pstr->open_time[1];
	uart_buf[cnt++] = pstr->open_time[2];
	uart_buf[cnt++] = pstr->open_time[3];
	
	time_tmp = (uint32)RTC_GetUnixTime();						// 时间戳								//
	LONG2BYTE(time_tmp, time_arr);
	uart_buf[cnt++] = time_arr[0];
	uart_buf[cnt++] = time_arr[1];
	uart_buf[cnt++] = time_arr[2];
	uart_buf[cnt++] = time_arr[3];
	
	TempCrc = MyCrc16Check(uart_buf, pack_len + 0x02);			// CRC校验								//
	uart_buf[cnt++] = (TempCrc >> 8) & 0xFF;
	uart_buf[cnt++] = TempCrc & 0xFF;
	
	cur_time = (uint32)RTC_GetUnixTime();
	if ((cur_time + 300) > next_heartbeat_time)					// 下一次心跳在5min以内					//
	{
		next_heartbeat_time = cur_time + 300;
	}
	
	if ((cur_time + 60) > next_gprs_upload_time)				// 下次轮询上报的时间延长1min 找空闲	//
	{
		next_gprs_upload_time = cur_time + 60;
	}
	#if 0
	if ((cur_time + 15) > next_allupload_time)					// 下次立即开门上报的时间延长15s 找空闲//
	{
		next_allupload_time = cur_time + 15;
	}
	#endif
	GPRS_SendHexDataToServer((uint8_t *)uart_buf, ((uart_buf[2] << 8) + uart_buf[3] + 4));
	
	uploadpolling_stop = 0;
	return 0;
}

//********************************************************************************************************
// Function name	: send_heartbeat
// Description		: 心跳包  0x08
// Parameter		: lock_state	: 锁具状态
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
uint8_t send_heartbeat(uint8_t lock_state)
{
	uint8 uart_buf[96];
	uint8 time_arr[4];
	uint32_t time_tmp;
	uint16_t TempCrc = 0;
	uint16_t cnt = 0;
	uint16_t i = 0;
	uint16_t pack_len = 0x10;

		
	uart_buf[cnt++] = 0xef;
	uart_buf[cnt++] = 0x3a;
	uart_buf[cnt++] = (pack_len >> 8) & 0xFF;					// 包长度高字节							//
	uart_buf[cnt++] = pack_len & 0xFF;							// 包长度低字节							//
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x08;
	
	for(i = 0; i < 6; i++)										// MAC地址								//
	{
		uart_buf[cnt++] = Mac_Addr[i];
	}
	
	uart_buf[cnt++] = lock_state;								// 锁具状态								//
	#if DEF_RSSI
	Rssi = GPRS_QueryRssi();
	uart_buf[cnt++] = Rssi;										// GPRS模块信号强度	@10.31.9.00			//
	#endif
	
	time_tmp = (uint32)RTC_GetUnixTime();						// 时间戳								//
	LONG2BYTE(time_tmp, time_arr);
	uart_buf[cnt++] = time_arr[0];
	uart_buf[cnt++] = time_arr[1];
	uart_buf[cnt++] = time_arr[2];
	uart_buf[cnt++] = time_arr[3];
	
	TempCrc = MyCrc16Check(uart_buf, pack_len + 0x02);			// CRC校验								//
	uart_buf[cnt++] = (TempCrc >> 8) & 0xFF;
	uart_buf[cnt++] = TempCrc & 0xFF;
	//DBG_SOFT_PutStr("\nsend_heartbeat:\n");
	//DBG_SOFT_PrintfHex(uart_buf,32,16);
	GPRS_SendHexDataToServer((uint8_t *)uart_buf, ((uart_buf[2] << 8) + uart_buf[3] + 4));
	
    return 0;
}

//********************************************************************************************************
// Function name	: send_timeadjust
// Description		: 时间校准  0x0A
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
uint8_t send_time_adjust(void)
{
	uint8 uart_buf[96];
	uint16_t TempCrc = 0;
	uint16_t cnt = 0;
	uint16_t i = 0;
	uint16_t pack_len = 0x0D;
	
	uart_buf[cnt++] = 0xef;
	uart_buf[cnt++] = 0x3a;
	uart_buf[cnt++] = (pack_len >> 8) & 0xFF;					// 包长度高字节							//
	uart_buf[cnt++] = pack_len & 0xFF;							// 包长度低字节							//
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x0A;
	
	for(i = 0; i < 6; i++)										// MAC地址								//
	{
		uart_buf[cnt++] = Mac_Addr[i];
	}
	
	uart_buf[cnt++] = 0xff;										// 时间戳								//
	uart_buf[cnt++] = 0xff;
	uart_buf[cnt++] = 0xff;
	uart_buf[cnt++] = 0xff;
	
	TempCrc = MyCrc16Check(uart_buf, pack_len + 0x02);			// CRC校验								//
	uart_buf[cnt++] = (TempCrc >> 8) & 0xFF;
	uart_buf[cnt++] = TempCrc & 0xFF;
	
	cur_time = (uint32)RTC_GetUnixTime();
	if ((cur_time + 300) > next_heartbeat_time)					// 下一次心跳在5min以内					//
	{
		next_heartbeat_time = cur_time + 300;
	}
	GPRS_SendHexDataToServer((uint8_t *)uart_buf, ((uart_buf[2] << 8) + uart_buf[3] + 4));
	return 0;
}

//********************************************************************************************************
// Function name	: send_alarminfo
// Description		: 报警信息上传  0x0B
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
uint8_t send_alarm_info(uint8_t alarm_mode)
{
	uint8 uart_buf[96];
	uint8 time_arr[4];
	uint32_t time_tmp;
	uint16_t TempCrc = 0;
	uint16_t cnt = 0;
	uint16_t i = 0;
	uint16_t pack_len = 0x10;
	
	uart_buf[cnt++] = 0xef;
	uart_buf[cnt++] = 0x3a;
	uart_buf[cnt++] = (pack_len >> 8) & 0xFF;					// 包长度高字节							//
	uart_buf[cnt++] = pack_len & 0xFF;							// 包长度低字节							//
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x0B;
	
	for(i = 0; i < 6; i++)										// MAC地址								//
	{
		uart_buf[cnt++] = Mac_Addr[i];
	}
	
	uart_buf[cnt++] = alarm_mode;								// 报警类型								//
	
	time_tmp = (uint32)RTC_GetUnixTime();						// 时间戳								//
	LONG2BYTE(time_tmp, time_arr);
	uart_buf[cnt++] = time_arr[0];
	uart_buf[cnt++] = time_arr[1];
	uart_buf[cnt++] = time_arr[2];
	uart_buf[cnt++] = time_arr[3];
	
	TempCrc = MyCrc16Check(uart_buf, pack_len + 0x02);			// CRC校验								//
	uart_buf[cnt++] = (TempCrc >> 8) & 0xFF;
	uart_buf[cnt++] = TempCrc & 0xFF;
	
	cur_time = (uint32)RTC_GetUnixTime();
	if ((cur_time + 300) > next_heartbeat_time)					// 下一次心跳在5min以内					//
	{
		next_heartbeat_time = cur_time + 300;
	}
	GPRS_SendHexDataToServer((uint8_t *)uart_buf, ((uart_buf[2] << 8) + uart_buf[3] + 4));
	return 0;
}


//********************************************************************************************************
// Function name	: send_update_req(uint16 page)
// Description		: 请求下发升级包  0x0F
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
uint8_t send_update_req(uint16 page)
{
	uint8 uart_buf[96];
	uint8 time_arr[4];
	uint32_t time_tmp;
	uint16_t TempCrc = 0;
	uint16_t cnt = 0;
	uint16_t i = 0;
	uint16_t pack_len = 0x13;
	
	uart_buf[cnt++] = 0xef;
	uart_buf[cnt++] = 0x3a;
	uart_buf[cnt++] = (pack_len >> 8) & 0xFF;					// 包长度高字节							//
	uart_buf[cnt++] = pack_len & 0xFF;							// 包长度低字节							//
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x01;
	uart_buf[cnt++] = 0x0F;
	
	for(i = 0; i < 6; i++)										// MAC地址								//
	{
		uart_buf[cnt++] = Mac_Addr[i];
	}
	
	uart_buf[cnt++] = (page >> 8) & 0xFF;						// 组号
	uart_buf[cnt++] = page & 0xFF;	
	
	uart_buf[cnt++] = (update_version>> 8) & 0xFF;				// 升级版本
	uart_buf[cnt++] = update_version & 0xFF;								
		
	time_tmp = (uint32)RTC_GetUnixTime();						// 时间戳								//
	LONG2BYTE(time_tmp, time_arr);
	uart_buf[cnt++] = time_arr[0];
	uart_buf[cnt++] = time_arr[1];
	uart_buf[cnt++] = time_arr[2];
	uart_buf[cnt++] = time_arr[3];

	TempCrc = MyCrc16Check(uart_buf, pack_len + 0x02);			// CRC校验								//
	uart_buf[cnt++] = (TempCrc >> 8) & 0xFF;
	uart_buf[cnt++] = TempCrc & 0xFF;
	GPRS_SendHexDataToServer((uint8_t *)uart_buf, ((uart_buf[2] << 8) + uart_buf[3] + 4));
	
	return 0;
}

//********************************************************************************************************
// Function name	: Protocol_SendAckPacket
// Description		: 向APP(蓝牙)发送标准应答包
// Parameter		: uint8 CommChanle		信道
//                  : uint8 cPackType       0-发送包，1-应答包
//                  : uint8 cCmd           	命令码
//				    : uint8 *UserData		待发送的用户数据(内容)
//				    : uint16 UserDataLen	待发送的用户数据长度（不包含应答码）
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Protocol_SendAckPacket(uint8 CommChanle, uint8 cPackType, uint8 cCmd, uint8 *UserData, uint16 UserDataLen)
{
	uint8 AppResponseBuf[256] = {0};
	uint8 i;
	uint16 NoPtr;
	uint16 TempCrc;
	uint16 nPacketLen;
	
	NoPtr = 0;
	nPacketLen = UserDataLen + 5;
	memset(AppResponseBuf, 0, sizeof(AppResponseBuf));
	AppResponseBuf[NoPtr++] = HEAD_BYTE1;
	AppResponseBuf[NoPtr++] = HEAD_BYTE2;
	AppResponseBuf[NoPtr++] = (nPacketLen >> 8) & 0xff;
	AppResponseBuf[NoPtr++] = nPacketLen & 0xff;
	AppResponseBuf[NoPtr++] = CommChanle | (cPackType << 4);
	AppResponseBuf[NoPtr++] = PROTOCOL_VER; 
	AppResponseBuf[NoPtr++] = cCmd;

	#ifdef	AES_ENCRY

	#else

	#endif

	if (UserDataLen)
	{
		for (i = 0; i < UserDataLen; i++)
		{
			AppResponseBuf[NoPtr++] = *(UserData + i);
		}
	}
	
	TempCrc = MyCrc16Check(AppResponseBuf, NoPtr);

	AppResponseBuf[NoPtr++] = (TempCrc >> 8) & 0xFF;
	AppResponseBuf[NoPtr++] = TempCrc & 0xFF;
	
	DBG_SOFT_PutStr("\r\n AppResponseBuf:\r\n");
	DBG_SOFT_PrintfHex(AppResponseBuf,64,16);
	
	if (CommChanle == 0x02)                          			// 蓝牙通道								//
	{
		BleSend(AppResponseBuf, NoPtr);
        #if 0
		for (i = 0; i < 100; i++)
        {
            ble_heartbeat();
        }
		#endif
	}
	else if(CommChanle == 0x01)                     			// GPRS通道								//
	{
		cur_time = (uint32)RTC_GetUnixTime();
		if ((cur_time + 300) > next_heartbeat_time)				// 下一次心跳在5min以内					//
		{
			next_heartbeat_time = cur_time + 300;
		}
	    GPRS_SendHexDataToServer(AppResponseBuf, NoPtr);
	}
}

//===【结束】===================== 业务层主动上报服务器指令 ==============================================




//===【开始】=================== 业务层处理接收到的服务器/蓝牙指令 =======================================

//********************************************************************************************************
// Function name	: Protocol_CmdGetPreSever
// Description		: 从中心服务器获取具体要链接的前置服务器域名或IP
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x01命令，服务器应答
//********************************************************************************************************
static void Protocol_CmdGetPreSever(uint8* RecBuf)
{
	uint8 *pstr = RecBuf;
	uint8 port_tmp[2];
	uint8 port_str[6];
	uint16 port_hex;

	// 获取前置服务器地址和端口的应答信息																//
	pstr += 13;
	memcpy(domain, pstr, 200);									// 200 字节IP或域名						//
	pstr += 200;
	memcpy(port_tmp, pstr, 2);									// 端口号								//

	//port_hex = BYTE2WORD(port_tmp[0], port_tmp[1]);
	#if 0
	port_str[0]= port_tmp[0]/0x10 + '0';
	port_str[1]= port_tmp[0]%0x10 + '0';
	port_str[2]= port_tmp[1]/0x10 + '0';
	port_str[3]= port_tmp[1]%0x10 + '0';
	#endif

	port_hex = (port_tmp[0]<<8) + port_tmp[1];
	if (port_hex < 10000)
	{
		port_str[0] = port_hex/1000 + '0';
		port_str[1] = port_hex%1000/100 + '0';	
		port_str[2] = port_hex%100/10 + '0';
		port_str[3] = port_hex%10 + '0';
		port_str[4] = '\0';
		port_str[5] = '\0';
	}
	else
	{
		port_str[0] = port_hex/10000 + '0';
		port_str[1] = port_hex%10000/1000 + '0';	
		port_str[2] = port_hex%1000/100 + '0';
		port_str[3] = port_hex%100/10 + '0';
		port_str[4] = port_hex%10 + '0';
		port_str[5] = '\0';
	}
	memcpy(pre_port, port_str, 6);
	memcpy((char *)pre_ip, (char *)domain, strlen((char *)domain));
	
	#if DBG_SOFT_ENNABLE
	DBG_SOFT_PutString("\nPre_domain:");	
	DBG_SOFT_PutArray(domain, strlen((char *)domain));
	DBG_SOFT_PutString("\nPre_PORT:");
	DBG_SOFT_PutArray(port_str, 4);
	DBG_SOFT_PutString("\n");
	#endif
	
	//===【开始】=== 设备层的处理 ======================================================================//
	gprs_state = GET_PRE_IP;
	gprs_operation_ErrCount = 0;
	next_gprs_operation_time = cur_time;
	//===【结束】=== 设备层的处理 ======================================================================//

	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}

//********************************************************************************************************
// Function name	: Protocol_CmdGetMacAndKey
// Description		: 智能门禁向服务器申报MAC 码并获取蓝牙密钥指令（智能门禁→服务器） 
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x02命令，服务器应答
//********************************************************************************************************
static void Protocol_CmdGetMacAndKey(uint8* RecBuf)
{
	uint8 ble_key[6];
	uint8 *pstr = RecBuf;
	
	// 获取MAC地址和蓝牙钥匙命令的应答包																//
	pstr += 13;
	#if 0
	uint8 ptime[4] = {0};
	memcpy(ptime, pstr, 4);										// 时间戳								//
	cur_time = BYTE2LONG(ptime[0], ptime[1], ptime[2], ptime[3]);
	RTC_SetUnixTime(cur_time);
	#endif
	
	pstr += 4;
	memcpy(ble_key, pstr, 6);									// 6字节蓝牙密钥						//
	
	//===【开始】=== 设备层的处理 ======================================================================//
	SendMacFlag = 1;
	gprs_state = NORMAL_COMM;
	gprs_operation_ErrCount = 0;
	next_gprs_operation_time = cur_time;
	//===【结束】=== 设备层的处理 ======================================================================//
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}


//********************************************************************************************************
// Function name	: Protocol_CmdReadUID
// Description		: 平台 --> 从智能门禁读取一条UID 指令 
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x03命令，服务器发送
//********************************************************************************************************
static void Protocol_CmdReadUID(uint8* RecBuf)
{
	uint8 ret;
	uint8 DataBuf[64];
	uint8 *pT = DataBuf;
	uint8 pUID[8] = {0};
	uint8 comm_chanle = RecBuf[4];								// 取出信道值							//
	uint16 nCmdPackLen;

	nCmdPackLen= (RecBuf[2]<<8) + RecBuf[3] + 4;
	 
	memset(pT, 0, sizeof(DataBuf));
	
	//if (0 != memcmp((uint8 *)Mac_Addr, pMAC, BD_ADDR_LEN))
	
	memcpy(pT, RecBuf + 7, 6);			  						// 取出MAC地址，理论应加上MAC校验		//
	pT += 6;
	
	//===【开始】=== 设备层的处理 ======================================================================//
	VoicePlay((uint8)Sound_PleasePutCard);						// 从设备读取一张uid == PUID			//
	wait_timeout = 20000;
	ret = RET_ERR;
	DBG_SOFT_PutStr("\nProtocol_CmdReadUID\n");
	while (wait_timeout > 0)
	{
		ret = detect_nfc_card(pUID, CARD_UID_LEN);
		// 滤除不准确的卡ID
		if((pUID[0] == 0) && (pUID[1] == 0) &&(pUID[2] == 0) && (pUID[3] == 0))
		{
			continue;
		}
		if (ret == RET_OK)
		{
			VoicePlay((uint8)Sound_ReadCardSuccess);
			break;
		}
	}
	if(wait_timeout == 0)										// 超时播报
	{	
		memset(pUID,0xFF,7);									// 超时UID 替代值 FFFFFFFFFFFFFF03
		pUID[7] = 0x03;
		VoicePlay((uint8)Sound_TIMEOUT);
	}
	
	//===【结束】=== 设备层的处理 ======================================================================//
	
	Endpoint_exchange(pUID);									// A卡数据位置转换 @ 18-1-17			//
	
	memcpy(pT, pUID, 8);										// 复制读到的卡UID						//
	pT += 8;

	memcpy(pT, RecBuf + nCmdPackLen - 6, 4);					// 拷贝时间戳							//
	pT += 4;
	
	if (ret == RET_ERR)											// 没读到卡，回传结果码失败				//
	{
		*pT = RT_Timeout_ERR;									// 超时上报								//
	}
	else
	{
		*pT = RT_SUCC;
	}
	
	Protocol_SendAckPacket(comm_chanle, PACK_RESP, RecBuf[6], DataBuf, 19);
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}

//********************************************************************************************************
// Function name	: Protocol_CmdWriteUID
// Description		: 平台 --> 写一条UID和有效期到智能门禁指令
// Parameter		: uint8* GPRSCmdBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x04命令，服务器发送
//********************************************************************************************************
static void Protocol_CmdWriteUID(uint8* RecBuf)
{
	uint8 DataBuf[64];
	uint8 *pT = DataBuf;
	uint8 pUID[8];
	uint8 pTime[4];
	uint8 comm_chanle = RecBuf[4];								// 取出信道值							//
	user_info_t user_info;
	uint16 nCmdPackLen;
	
    nCmdPackLen= (RecBuf[2] << 8) + RecBuf[3] + 4;
    
	memset(pT, 0, sizeof(DataBuf));
	memcpy(pT, RecBuf + 7, 6);									// 取出MAC地址，理论应加上MAC校验		//
	pT += 6;
	
	memcpy(pT, RecBuf + 13, 8);									// 取出uid								//
	memcpy(pUID, RecBuf + 13, 8);
	pT += 8;
	
	memcpy(pT, RecBuf + 21, 4);									// 取出有效日期							//
	memcpy(pTime, RecBuf + 21, 4);	
	pT += 4;
	memcpy(pT, RecBuf + nCmdPackLen - 6, 4);					// 时间戳								//
	pT += 4;
    
	//===【开始】=== 设备层的处理 ======================================================================//
	Endpoint_exchange(pUID);									// A卡数据位置转换 @ 18-1-17			//
	
	memcpy(user_info.card_uid, pUID, 8);
	memcpy(user_info.valid_time, pTime, 4);
	if (TRUE == UserData_Add_User(&user_info))
	{
		VoicePlay((uint8)Sound_REG_SUCCESS);
		*pT = RT_SUCC;
	}
	else
	{
		VoicePlay((uint8)Sound_REG_FAILURE);
		*pT = RT_FAIL;
	}
	//===【结束】=== 设备层的处理 ======================================================================//
	
	Protocol_SendAckPacket(comm_chanle, PACK_RESP, RecBuf[6], DataBuf, 23);
	
	#if DBG_SOFT_ENNABLE
	test_Flash();
	#endif
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}

//********************************************************************************************************
// Function name	: Protocol_CmdDelUID
// Description		: 平台 -->从智能门禁删除一条UID 指令
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x05命令，服务器发送
//********************************************************************************************************
static void Protocol_CmdDelUID(uint8* RecBuf)
{
	uint8 ret = 0;
	uint8 DataBuf[64];
	uint8 *pT = DataBuf;
	uint8 pUID[8];
	uint8 comm_chanle = RecBuf[4];								// 取出信道值							//
	uint16 nCmdPackLen;

	nCmdPackLen= (RecBuf[2]<<8) + RecBuf[3] + 4;
	 
	memset(pT, 0, sizeof(DataBuf));
	memcpy(pT, RecBuf + 7, 6);									// 取出MAC地址，理论应加上MAC校验		//
	pT += 6;

	memcpy(pT, RecBuf + 13, 8);									// 取出uid								//
	memcpy(pUID, RecBuf + 13, 8);
	pT += 8;

	memcpy(pT, RecBuf + nCmdPackLen - 6, 4);					// 时间戳								//
	pT += 4;
 	
	//===【开始】=== 设备层的处理 ======================================================================//
	Endpoint_exchange(pUID);									// A卡数据位置转换 @ 18-1-17			//
	
	ret = UserData_DEL_User(pUID);
	if (TRUE == ret)
	{
		VoicePlay((uint8)Sound_DELETE_SUCCESS);
		*pT = RT_SUCC;
	}
	else if(RT_Invalid_ERR == ret)
	{
		VoicePlay((uint8)Sound_DELETE_FAILURE);
		*pT = RT_Invalid_ERR;
	}
	else if(FALSE == ret)
	{
		VoicePlay((uint8)Sound_DELETE_FAILURE);
		*pT = RT_FAIL;
	}
	
	
	//===【结束】=== 设备层的处理 ======================================================================//
	
	Protocol_SendAckPacket(comm_chanle, PACK_RESP, RecBuf[6], DataBuf, 19);	
	
	#if DBG_SOFT_ENNABLE
	test_Flash();
	#endif
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}  

//********************************************************************************************************
// Function name	: Protocol_CmdOpenDoor
// Description		: APP(或平台)通过预定的信道向智能门禁发出开门命令，智能门禁返回处理结果到APP(或平台)
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x06命令，服务器发送
//********************************************************************************************************
static void Protocol_CmdOpenDoor(uint8* RecBuf)
{
	uint8 ret = 0;
	uint8 RetCode = RT_SUCC;
	uint8 DataBuf[256];
	uint8 pUID[8];
	uint8 pMAC[6];
	uint8 pTime[4];
	uint8 OpenType;  
	uint8 *pT=DataBuf;
	uint8 comm_chanle = RecBuf[4];								// 取出信道值							//
	uint16 nCmdPackLen;
	user_info_t user_info;
	open_door_t openInfo;

    nCmdPackLen= (RecBuf[2] << 8) + RecBuf[3] + 4;
	
	memset(&openInfo, 0, sizeof(open_door_t));
    memset(pT, 0, sizeof(DataBuf));
    
	memcpy(pT, RecBuf + 7, 6);									// 取出MAC地址，理论应加上MAC校验		//
    memcpy(pMAC, RecBuf + 7, 6);
	pT += 6;
	
	memcpy(pT, RecBuf + 13, 8);									// 取出uid								//
	memcpy(pUID, RecBuf + 13,8);
    pT += 8;
	
    *pT = *(RecBuf + 13 + 8);									// 开门方式								//
	OpenType = *(RecBuf + 13 + 8);
	pT += 1;
	
    memcpy(pT, RecBuf + nCmdPackLen - 6, 4);					// 时间戳								//
    memcpy(pTime, RecBuf + nCmdPackLen - 6, 4);
	pT += 4;
	
	//===【开始】=== 设备层的处理 ======================================================================//
	Endpoint_exchange(pUID);									// A卡数据位置转换 @ 18-1-17			//
	
	memcpy(user_info.card_uid, pUID, 8);
	memcpy(user_info.valid_time, pTime, 4);
	
	if (0 != memcmp((uint8 *)Mac_Addr, pMAC, BD_ADDR_LEN))
	{
		RetCode = RT_FAIL;										// 丢弃？
	}
	else
	{ 
		
		ret = UserData_Judge_If_Open(&user_info);
		if (RT_UID_ERR == ret)
		{
			RetCode = RT_UID_ERR;
		}
		else if(RT_ValidTime_ERR == ret)
		{
			RetCode = RT_ValidTime_ERR;
		}
		else if (TRUE == ret)
		{
			RetCode = RT_SUCC;
		}
	}
	*pT = RetCode;
	//===【结束】=== 设备层的处理 ======================================================================//
	
	Protocol_SendAckPacket(comm_chanle, PACK_RESP, RecBuf[6], DataBuf, 20);
		
	if (RetCode == RT_SUCC)
	{
		//===【开始】=== 设备层的处理 ==================================================================//
	    Led_GreenON();
		DoorOpen();
		VoicePlay((uint8)Sound_VERIFY_SUCCESS);
        cur_time = (uint32)RTC_GetUnixTime();
		if (cur_time >= 1483203600)
		{
			memcpy(openInfo.card_uid, pUID, CARD_UID_LEN);
			memcpy(openInfo.open_time, pTime, 4);
			memset(pTime, 0, 4);
			memcpy(openInfo.send_time, pTime, 4);
			Write_OpenInfo(&openInfo, OpenType);
		}
		delay_with_ble(1000);
		DoorClose();
		Led_SetState(LED_State);
		
		
		//===【结束】=== 设备层的处理 ==================================================================//
		#if 0
		cur_time = (uint32)RTC_GetUnixTime();
		if ((cur_time + 300) > next_heartbeat_time)					// 下一次心跳在30秒以内				//
		{
			next_heartbeat_time = cur_time + 300;
		}
		#endif
		if (cur_time >= 1483203600)
		{
			if (gprs_state == NORMAL_COMM)
			{
				cur_time = (uint32)RTC_GetUnixTime();
				if(cur_time > next_allupload_time)
				{
					next_allupload_time = cur_time + 15;
					send_upload_open_info(&openInfo, CARD_UID_LEN, OpenType);
				}
			}
		}
	}
	else
	{
		Card_ErrCount++;
		Led_RedON();
		VoicePlay(Sound_VERIFY_FAILURE);
		delay_with_ble(1000);
	}
		
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}

//********************************************************************************************************
// Function name	: Protocol_CmdSendOpenInfo
// Description		: 开门信息上报到服务器指令（智能门禁→服务器）
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x07命令，服务器应答
//********************************************************************************************************
static void Protocol_CmdSendOpenInfo(uint8* RecBuf)
{
	//===【开始】=== 设备层的处理 ======================================================================//		
	uint8 opentime[4] = {0};
	uint8 pTime[4] = {0};
	uint8 ret_code = 0;
	
	memcpy(opentime, RecBuf + 22, 4);								// 开门时间							//
	memcpy(pTime, RecBuf + 26, 4);									// 时间戳 -- 记作发送时间 			//
	ret_code =  RecBuf[30];											// 结果码							//
	
	if(ret_code == 0x01)
		FindAndSet_UploadTime_UploadFlag(opentime, pTime);
	else
	{	
		//Fail_openinfo
		DBG_SOFT_PutStr("\r\n 07 reponse err:\r\n");
		DBG_SOFT_PrintfHex(&ret_code,1,16);		
	}	
	//===【结束】=== 设备层的处理 ======================================================================//
	
	#if DBG_SOFT_ENNABLE
	test_Flash();
	#endif
	
	next_allupload_time = (uint32)RTC_GetUnixTime(); 				// 恢复延时上报的时延				//
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
} 

//********************************************************************************************************
// Function name	: Protocol_CmdPluse
// Description		: 心跳包给服务器（智能门禁→服务器）
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x08命令，服务器应答
//********************************************************************************************************
static void Protocol_CmdPluse(uint8* RecBuf)
{
	uint8 ret;
	uint8 *pstr = RecBuf;
	uint8 arr_adjtime[4] = {0};
	uint16 nCmdPackLen = (RecBuf[2] << 8) + RecBuf[3] + 4;
	
	//===【开始】=== 设备层的处理 ======================================================================//
	heartbeat_SendCount = 0;										// 收到，错误次数清零				//
	ret = *(pstr + nCmdPackLen - 3);
	if (ret == RT_SUCC)
	{
		pstr += 17;
		memcpy(arr_adjtime, pstr, 4);
		SetSysRTCTime(arr_adjtime);
		cur_time = (uint32)RTC_GetUnixTime();						// 更新时间							//
		next_heartbeat_time = cur_time + 300;
	}
	Updatealarm_check();											// 上报是否升级成功										
	
	//===【结束】=== 设备层的处理 ======================================================================//
	
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}  

//********************************************************************************************************
// Function name	: Protocol_CmdClearUID
// Description		: 清空智能门禁所有UID 和有效期指令
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x09命令，服务器发送
//********************************************************************************************************
static void Protocol_CmdClearUID(uint8* RecBuf)
{
	uint8 RetCode = RT_SUCC;
	uint8 DataBuf[64];
	uint8 *pT = DataBuf;
	uint8 comm_chanle = RecBuf[4];								// 取出信道值							//
		 
	memset(pT, 0, sizeof(DataBuf));
	memcpy(pT, RecBuf + 7, 10);									// 取出MAC地址和时间戳					//
	pT += 10;

	//===【开始】=== 设备层的处理 ======================================================================//
	if (TRUE == UserData_CLR_User())
	{
		RetCode = RT_SUCC;
		VoicePlay((uint8)Sound_EMPTY_SUCCESS);
	}
	else
	{
		RetCode = RT_FAIL;
		VoicePlay((uint8)Sound_EMPTY_FAILURE);
	}
	//===【结束】=== 设备层的处理 ======================================================================//

	*pT = RetCode;
	Protocol_SendAckPacket(comm_chanle, PACK_RESP, RecBuf[6], DataBuf, 11);
	
	#if DBG_SOFT_ENNABLE
	Clear_AllOPeninfo();
	#endif
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}  

//********************************************************************************************************
// Function name	: Protocol_CmdTimeCheck
// Description		: 时间校准请求（智能门禁→服务器）
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x0A命令，服务器应答
//********************************************************************************************************
static void Protocol_CmdTimeCheck(uint8* RecBuf)
{	
	uint8 *pstr = RecBuf;
	uint8 arr_adjtime[4] = {0};
	
	//===【开始】=== 设备层的处理 ======================================================================//
	pstr += 17;
	memcpy(arr_adjtime, pstr, 4);
	SetSysRTCTime(arr_adjtime);
	cur_time = (uint32)RTC_GetUnixTime();
	//===【结束】=== 设备层的处理 ======================================================================//
 
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}  

//********************************************************************************************************
// Function name	: Protocol_CmdSendWarnInfo
// Description		: 报警信息上报到服务器指令（智能门禁→服务器）
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x0B命令，服务器应答
//********************************************************************************************************
static void Protocol_CmdSendWarnInfo(uint8* RecBuf)
{
	uint8 *pstr = RecBuf;
	uint8 nCmdPackLen= (RecBuf[2] << 8) + RecBuf[3] + 4;

	//===【开始】=== 设备层的处理 ======================================================================//
	//
	//检查报警的应答包是否成功,如失败需重新上报，暂时不做
	//
	//===【开始】=== 设备层的处理 ======================================================================//
		
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}  

//********************************************************************************************************
// Function name	: Protocol_CmdReadInfo
// Description		: 读取给定时间范围内开门日志指令
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x0C命令，服务器发送
//uint16 UploadQty;
//uint16 UploadStartnum;
//********************************************************************************************************
static void Protocol_CmdReadInfo(uint8* RecBuf)
{
	uint8 i;
	uint8 RetCode = RT_SUCC;
	uint8 DataBuf[256];
	uint8 *pT = DataBuf;
	uint8 comm_chanle = RecBuf[4];								// 取出信道值							//
	uint8 data_openinfo[256];
	uint8  start_t[4];
	uint8  end_t[4];
	uint32 start_time;
	uint32 end_time;
	uint16 app_group = 0;
	uint16 nCmdPackLen = (RecBuf[2] << 8) + RecBuf[3] + 4;
	uint16 tUploadQty;											// 存放本次发送的长度					//
	uint8 cmd;
	
    memset(pT, 0, sizeof(DataBuf));
	cmd = RecBuf[6];
	memcpy(pT, RecBuf + 7, 6);								// 取出MAC地址和时间戳					//
    pT += 6;
	
	//===【开始】=== 设备层的处理 ======================================================================//
	app_group = (RecBuf[21] << 8) + RecBuf[22];					// 组号									//
	
	if (app_group == 1)
	{
		memcpy(start_t, RecBuf + 13, 4);           				// 取始末时间							//
		memcpy(end_t, RecBuf + 13 + 4, 4);
		start_time = BYTE2LONG(start_t[0], start_t[1], start_t[2], start_t[3]);
		end_time = BYTE2LONG(end_t[0], end_t[1], end_t[2], end_t[3]);
		
		if(start_time > end_time)
		{
			RetCode = RT_StartOrEnd_ERR;
		}
		
		UploadQty = Find_Open_ValidQty(start_time, end_time);
		if (UploadQty > 0)
		{
			UploadStartnum = Find_Open_StartNum(start_time);
		}
	}
	
	if (UploadQty <= OPENDATA_UPPAGE_LEN)
	{
		*pT++ = 0;												// 组号									//
		*pT++ = 0;
		
		tUploadQty = UploadQty;
		memcpy(pT, &tUploadQty, 1);								// 本次发送数量							//
		pT += 1;
		
		for (i = 0; i < tUploadQty; i++)
		{
		    Get_Open_Info(UploadStartnum + i, 1, data_openinfo);
		    memcpy(pT, data_openinfo, 16);
		    pT += 16;
			*pT++ = Read_Open_Type(UploadStartnum + i);			// 开门方式								//
		}
		UploadQty = 0;
		UploadStartnum = 0;
	}
	else
	{
		*pT++ = RecBuf[21];										// 组号									//
		*pT++ = RecBuf[22];
		
		tUploadQty = OPENDATA_UPPAGE_LEN;
		*pT++ = OPENDATA_UPPAGE_LEN;							// 本次发送数量							//
		
		for (i = 0; i < OPENDATA_UPPAGE_LEN; i++)
		{
		    Get_Open_Info(UploadStartnum + i, 1, data_openinfo);
		    memcpy(pT, data_openinfo, 16);
		    pT += 16;
			*pT++ = Read_Open_Type(UploadStartnum + i);			// 开门方式								//
		}
		UploadQty -= tUploadQty;
		UploadStartnum += tUploadQty;
	}
	
	memcpy(pT, RecBuf + nCmdPackLen - 6, 4);					// 时间戳								//
	pT += 4;        

	*pT = RetCode;												// 结果码								//
	
	Protocol_SendAckPacket(comm_chanle, PACK_RESP, cmd, DataBuf, 14 + (tUploadQty * (1 + sizeof(open_door_t))));
	
	#if DBG_SOFT_ENNABLE
	test_Flash();
	#endif
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}

//********************************************************************************************************
// Function name	: Protocol_CmdRemoteUpdate
// Description		: 服务器向智能门禁发送远程升级指令（服务器→智能门禁）
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x0D命令，服务器发送
//********************************************************************************************************
static void Protocol_CmdRemoteUpdate(uint8* RecBuf)
{
	uint8 RetCode = RT_SUCC;
	uint8 DataBuf[64];
	uint8 *pT = DataBuf;
	uint8 comm_chanle = RecBuf[4];								// 取出信道值							//
	uint8 port_str[4];
	uint16 port_hex;
	
	memcpy(pT, RecBuf + 7, 6);									// 取出MAC地址，理论应加上MAC校验		//
	pT += 6;
	
	memcpy(domain, RecBuf + 13, 200);							// 200字节IP或域名						//
	memcpy((char *)update_ip, (char *)domain, strlen((char *)domain));
	
	port_hex = (RecBuf[213] << 8) + RecBuf[214];				// 端口号								//
	port_str[0]= port_hex/1000 + '0';
	port_str[1]= port_hex%1000/100 + '0';
	port_str[2]= port_hex%100/10 + '0';
	port_str[3]= port_hex%10 + '0';
	memcpy(update_port, port_str, 4);
	
	*pT++ = RecBuf[215];										// 升级版本								//
	*pT++ = RecBuf[216];
	update_version = (RecBuf[215] << 8) + RecBuf[216];
	
	memcpy(pT, RecBuf + 219, 4);								// 时间戳								//
	pT += 4;
	
	if (update_version <= system_version)
	{
		RetCode = RT_NEWEST;
	}
	else
	{
		if (0 == strlen((char *)update_ip))
		{
			RetCode = RT_IPorPort_ERR;
		}
		else
		{
			//存下待升级版本号和升级标志位 重启的时候判断falg==1和系统版本号和它是否相等？
			update_flag = 1;
			Write_UpdateInfo(update_flag,update_version);
			RetCode = RT_SUCC;
		}
	}
	
	*pT++ = RetCode;
	Protocol_SendAckPacket(comm_chanle, PACK_RESP, RecBuf[6], DataBuf, 13);
	
	//===【开始】=== 设备层的处理 ======================================================================//
	
	//===【结束】=== 设备层的处理 ======================================================================//

	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}  

//********************************************************************************************************
// Function name	: Protocol_CmdRemoteRestart
// Description		: 服务器向智能门禁发送远程重启指令（服务器→智能门禁）
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x0E命令，服务器发送
//********************************************************************************************************
static void Protocol_CmdRemoteRestart(uint8* RecBuf)
{
    uint8 RetCode = RT_SUCC;
    uint8 DataBuf[64];
	uint8 *pT = DataBuf;
    uint8 comm_chanle = RecBuf[4];								// 取出信道值							//
    uint16 nCmdPackLen;
    
    nCmdPackLen= (RecBuf[2] << 8) + RecBuf[3] + 4;
	 
    memset(pT, 0, sizeof(DataBuf));
    memcpy(pT, RecBuf + 7, 6);									// 取出MAC地址，理论应加上MAC校验		//
    pT += 6; 
    
	memcpy(pT, RecBuf + nCmdPackLen - 6, 4);					// 时间戳								//
    pT += 4; 
    
	*pT = RetCode;
 
	//===【开始】=== 设备层的处理 ======================================================================//
	flag_reboot = 1;
	//===【结束】=== 设备层的处理 ======================================================================//
	
	Protocol_SendAckPacket(comm_chanle, PACK_RESP, RecBuf[6], DataBuf, 11);
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
} 

//********************************************************************************************************
// Function name	: Protocol_CmdRemoteDownload
// Description		: 外部云服务器下载升级包（智能门禁 - 服务器件）
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x0F命令，服务器应答
//--------------------------------------------------------------------------------------------------------
//					: 
//********************************************************************************************************
static void Protocol_CmdRemoteDownload(uint8* RecBuf)
{
	uint8 ReadDataBuf[DATABUF_SIZE];
	uint8 data_size[4];
	uint16 recv_page;
	uint16 nCmdPackLen= (RecBuf[2] << 8) + RecBuf[3];
	uint16 curr_packet_len = nCmdPackLen - 18;					// 当前升级数据包内容长度				//
	
	recv_page = (RecBuf[13] << 8) + RecBuf[14];					// 获取组号值							//
	
	DBG_SOFT_PutString("Protocol_CmdRemoteDownload-->recv_page:");
	DBG_SOFT_PutHexInt(recv_page);
	DBG_SOFT_PutString("\n");
	
	//===【开始】=== 设备层的处理 ======================================================================//
	if ((recv_page == update_page) || (recv_page == 0))
	{
		W25QXX_Write(RecBuf + 15, W25X_IMAGESTART_ADDR + update_offset, curr_packet_len);
		W25QXX_Read(ReadDataBuf, W25X_IMAGESTART_ADDR + update_offset, curr_packet_len);
		if (memcmp(RecBuf + 15, ReadDataBuf, curr_packet_len) == 0)
		{
			update_offset +=  curr_packet_len;
			if (recv_page != 0)
			{	
				update_page += 1;
				Image_size += curr_packet_len;
				send_update_req(update_page);
				update_timeout_time = 10000;
				update_timeout_count = 0;
			}
			else
			{
				Image_size += curr_packet_len;
				data_size[2] = (uint8)(update_offset >> 24);
				data_size[3] = (uint8)(update_offset >> 16);	// 高位在后[2-3]
				data_size[0] = (uint8)(update_offset >> 8);
				data_size[1] = (uint8)update_offset;
				
				W25QXX_Write(data_size, W25X_IMAGE_METDATA_ADDR, 4);
				gprs_state = UPDATE_FINISH;
			}
		}
		else
		{
			update_timeout_count++;
			if (update_timeout_count >= 3)
			{
				update_timeout_count = 0;
				gprs_state = NEED_INIT;
			}
			else
			{
				Cmd_Protocol_ParaInit();
				send_update_req(update_page);
				update_timeout_time = 10000;
			}
		}
	}
	else
	{
		update_timeout_count++;
		if (update_timeout_count >= 3)
		{
			update_timeout_count = 0;
			gprs_state = NEED_INIT;
		}
		else
		{
			Cmd_Protocol_ParaInit();
			send_update_req(update_page);
			update_timeout_time = 10000;
		}
	}
	//===【结束】=== 设备层的处理 ======================================================================//
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}

//********************************************************************************************************
// Function name	: Protocol_CmdChangeIP
// Description		: 更改中心服务器地址（服务器/APP - 智能门禁）
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0x10命令，服务器发送
//********************************************************************************************************
static void Protocol_CmdChangeIP(uint8* RecBuf)
{
    uint8 RetCode = RT_SUCC;
    uint8 DataBuf[64];
	uint8 *pT = DataBuf;
    uint8 comm_chanle = RecBuf[4];                              // 取出信道值							//
    uint16 nCmdPackLen;
    uint8 IpNum;
	
    nCmdPackLen= (RecBuf[2] << 8) + RecBuf[3] + 4;
	 
    memset(pT, 0, sizeof(DataBuf));
    memcpy(pT, RecBuf + 7, 6);									// 取出MAC地址，理论应加上MAC校验		//
    pT += 6; 
    
	*pT++ = RecBuf[13];
	IpNum = RecBuf[13];
	
	if ((IpNum == 0) || (IpNum > 5))
	{
		RetCode = RT_IP_NUM_ERR;
	}
	
	memcpy(pT, RecBuf + nCmdPackLen - 6, 4);					// 时间戳								//
    pT += 4; 
    
	*pT = RetCode;
	
	Protocol_SendAckPacket(comm_chanle, PACK_RESP, RecBuf[6], DataBuf, 12);
	
	//===【开始】=== 设备层的处理 ======================================================================//
	if (IpNum == 0x01)											 // 测试服域名1							//
	{
		Write_CenterIpType(IpNum);
		VoicePlay(Sound_ShortDI);
	}
	else if (IpNum == 0x02)
	{
		VoicePlay(Sound_ShortDI);
		BeepPlay_Delay(300);
		VoicePlay(Sound_ShortDI);
	}
	else if (IpNum == 0x03)
	{
		VoicePlay(Sound_ShortDI);
		BeepPlay_Delay(300);
		VoicePlay(Sound_ShortDI);
		BeepPlay_Delay(300);
		VoicePlay(Sound_ShortDI);
	}
	else if (IpNum == 0x04)
	{
		VoicePlay(Sound_LongDI);
	}
	else if (IpNum == 0x05)										// 正式服域名							//
	{
		Write_CenterIpType(IpNum);
		VoicePlay(Sound_LongDI);
		BeepPlay_Delay(300);
		VoicePlay(Sound_ShortDI);
	}
	
	flag_reboot = 1;
	//===【结束】=== 设备层的处理 ======================================================================//
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}


//********************************************************************************************************
// Function name	: Protocol_CmdManufac
// Description		: 蓝牙烧写设备MAC功能
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0xF1命令，手机发送
//********************************************************************************************************
static void Protocol_CmdManufac(uint8* RecBuf)
{
	uint8 Newmac[6] = {0};
		//1.取出mac 写入本地flash
	memcpy(Newmac, RecBuf+13, 6);
	uint8 flag = 0;
	W25QXX_Read(&flag, MACFLAG_BASE, 1);
	
	if(DEBUG_PRJ == 1)
	{	//2.测试程序 正确则播放长音 可以重复写
		if((Newmac[0] = 0x06) && (Newmac[1] = 0x01))
		{
			
			flag = 1;
			W25QXX_Write(Newmac, MAC_BASE, (uint16)sizeof(Newmac));
			W25QXX_Write(&flag, MACFLAG_BASE, 1);
			VoicePlay((uint8)Sound_REG_SUCCESS);	
		}
		else
		{
			//3.错误则播报短音3声
			VoicePlay((uint8)Sound_VERIFY_FAILURE);
			
		}
	
	}
	
	else if(DEBUG_PRJ == 0)
	{	//2.出厂程序 只能写一次		
		if((Newmac[0] = 0x06) && (Newmac[1] = 0x01) && (flag != 1))
		{
			
			flag = 1;
			W25QXX_Write(Newmac, MAC_BASE, (uint16)sizeof(Newmac));
			W25QXX_Write(&flag, MACFLAG_BASE, 1);
			VoicePlay((uint8)Sound_REG_SUCCESS);	
		}
		else
		{
			//3.错误则播报短音3声
			VoicePlay((uint8)Sound_VERIFY_FAILURE);
		}
	}
	
	BleSend(Newmac, 6);											// 蓝牙应答
	BeepPlay_Delay(1000);
	flag_reboot = 1;	
}

//********************************************************************************************************
// Function name	: Protocol_ClrOpenData_Qty
// Description		: 出厂前 蓝牙清除开门记录 和 数量内容
// Parameter		: uint8* RecBuf	命令包指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 0xF2命令，手机发送
//********************************************************************************************************
static void Protocol_ClrOpenData_Qty(uint8* RecBuf)
{
	uint8 dev_mac[6] = {0};
	uint8 RetCode = RT_SUCC;
    uint8 DataBuf[64];
	uint8 *pT = DataBuf;
    uint8 comm_chanle = RecBuf[4];                              // 取出信道值							//
	uint16 nCmdPackLen;
	
	W25QXX_Read(dev_mac, MAC_BASE, (uint16)sizeof(dev_mac));
	
	nCmdPackLen= (RecBuf[2] << 8) + RecBuf[3] + 4;
	 
    memset(pT, 0, sizeof(DataBuf));
    memcpy(pT, RecBuf + 7, 6);									// 取出MAC地址							//
    pT += 6; 
	memcpy(pT, RecBuf + nCmdPackLen - 6, 4);					// 时间戳								//
	pT += 4;

	*pT = RetCode;
	Protocol_SendAckPacket(comm_chanle, PACK_RESP, RecBuf[6], DataBuf, 11);
	
	Clear_AllOPeninfo();
	VoicePlay((uint8)Sound_REG_SUCCESS);
		
}


//********************************************************************************************************
// Function name	: Protocol_CmdParse
// Description		: 对收到的服务器命令的解析并处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Protocol_CmdParse(void)
{
	uint8 	TempCmdBuf[DATABUF_SIZE];
	uint16 	CmdPacketTotalSize;
	uint16 	TempCrc;
    
	if (GPRSCmdParseFlag2 == 1)
	{
		memcpy(TempCmdBuf, (uint8 *)GPRSCmd_buf2, sizeof(GPRSCmd_buf2));
		DBG_SOFT_PutStr("\nGPRSCmdParseFlag2 chanel\n");
		DBG_SOFT_PrintfHex(GPRSCmd_buf2, 32, 16);
		memset((uint8 *)GPRSCmd_buf2, 0, sizeof(GPRSCmd_buf2));
		GPRSCmdParseFlag2 = 0;

	}
	else if (GPRSCmdParseFlag1 == 1)
	{
		memcpy(TempCmdBuf, (uint8 *)GPRSCmd_buf1, sizeof(GPRSCmd_buf1));
		DBG_SOFT_PutStr("\nGPRSCmdParseFlag1 chanel\n");
		DBG_SOFT_PrintfHex(GPRSCmd_buf1, 32, 16);
		memset((uint8 *)GPRSCmd_buf1, 0, sizeof(GPRSCmd_buf1));
		GPRSCmdParseFlag1 = 0;

	}
	else if (BleCmdValidFlag == 1)
	{
		memcpy(TempCmdBuf, BleCmdBuf, sizeof(BleCmdBuf));
		memset(BleCmdBuf, 0, sizeof(BleCmdBuf));
		BleCmdValidFlag = 0;
	}
	
	CmdPacketTotalSize = (TempCmdBuf[2] << 8) + TempCmdBuf[3] + 4;
	DBG_SOFT_PutStr("\nProtocol_CmdParse-->Cmd:");
	DBG_SOFT_PrintfHex(TempCmdBuf + 6, 1, 16);
	//DBG_SOFT_PrintfHex(TempCmdBuf, CmdPacketTotalSize);
	DBG_SOFT_PrintfHex(TempCmdBuf, CmdPacketTotalSize+4, 16);
	DBG_SOFT_PutStr("\n");
	
	TempCrc = MyCrc16Check(TempCmdBuf, CmdPacketTotalSize - 2);
	if (((TempCmdBuf[CmdPacketTotalSize-2] << 8) + TempCmdBuf[CmdPacketTotalSize - 1]) != TempCrc)
	{
		DBG_SOFT_PutStr("\nCrc Err!\n");
		return;
	}
	DBG_SOFT_PutStr("\nCrc Ok!\n");
	
	switch (TempCmdBuf[6])
	{
		
		case CMD_GET_PRESEVER:									// 0x01命令								//
			Protocol_CmdGetPreSever(TempCmdBuf);
			break;
		case CMD_GET_MAC_AND_KEY:								// 0x02命令								//
			Protocol_CmdGetMacAndKey(TempCmdBuf);
			break;
		case CMD_READ_UID:										// 0x03命令								//
			Protocol_CmdReadUID(TempCmdBuf);
			break;
		case CMD_WRITE_UID:										// 0x04命令								//
			Protocol_CmdWriteUID(TempCmdBuf);
			break;
		case CMD_DEL_UID:										// 0x05命令								//
			Protocol_CmdDelUID(TempCmdBuf);
			break;
		case CMD_OPEN_DOOR:										// 0x06命令								//
			Protocol_CmdOpenDoor(TempCmdBuf);
			break;
		case CMD_SEND_OPEN_INFO:								// 0x07命令								//
			Protocol_CmdSendOpenInfo(TempCmdBuf);
			break;
		case CMD_PLUSE:											// 0x08命令								//
			Protocol_CmdPluse(TempCmdBuf);
			break;
		case CMD_CLEAR_UID:										// 0x09命令								//
			Protocol_CmdClearUID(TempCmdBuf);
			break;
		case CMD_TIME_CHECK:									// 0x0A命令								//
			Protocol_CmdTimeCheck(TempCmdBuf);
			break;
        case CMD_SEND_WARN_INFO:								// 0x0B命令								//
			Protocol_CmdSendWarnInfo(TempCmdBuf);
			break;
		case CMD_READ_INFO:										// 0x0C命令								//
			Protocol_CmdReadInfo(TempCmdBuf);
			break;
		case CMD_REMOTE_UPDATE:									// 0x0D命令								//
			Protocol_CmdRemoteUpdate(TempCmdBuf);
			break;
		case CMD_REMOTE_RESTART:								// 0x0E命令								//
			Protocol_CmdRemoteRestart(TempCmdBuf);
			break;
		case CMD_DOWNLOAD:										// 0x0F命令								//
		    Protocol_CmdRemoteDownload(TempCmdBuf);
		    break;
		case CMD_CHANGE_IP:										// 0x10命令								//
		    Protocol_CmdChangeIP(TempCmdBuf);
		    break;

		case CMD_MANUFAC:										// 0xF1命令								//
			Protocol_CmdManufac(TempCmdBuf);
			break;
			
		default:
			break;
	}
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}

//********************************************************************************************************
// Function name	: Protocol_CmdParse_BLE
// Description		: 对收到的服务器命令的解析并处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Protocol_CmdParse_BLE(void)
{
	uint8 	TempCmdBuf[DATABUF_SIZE];
	uint16 	CmdPacketTotalSize;
	uint16 	TempCrc;
    
	if (BleCmdValidFlag == 1)
	{
		memcpy(TempCmdBuf, BleCmdBuf, DATABUF_SIZE);
		memset(BleCmdBuf, 0, DATABUF_SIZE);
		BleCmdValidFlag = 0;
	}
	
	CmdPacketTotalSize = (TempCmdBuf[2] << 8) + TempCmdBuf[3] + 4;
	DBG_SOFT_PutStr("\nProtocol_CmdParse_BLE-->Cmd:");
	DBG_SOFT_PrintfHex(TempCmdBuf + 6, 1, 16);
	//DBG_SOFT_PrintfHex(TempCmdBuf, CmdPacketTotalSize);
	//DBG_SOFT_PrintfHex(TempCmdBuf, CmdPacketTotalSize, 16);
	
	DBG_SOFT_PutStr("\n");
	
	TempCrc = MyCrc16Check(TempCmdBuf, CmdPacketTotalSize - 2);
	if (((TempCmdBuf[CmdPacketTotalSize-2] << 8) + TempCmdBuf[CmdPacketTotalSize - 1]) != TempCrc)
	{
		return;
	}
	
	switch (TempCmdBuf[6])
	{
		case CMD_GET_PRESEVER:									// 0x01命令								//
			Protocol_CmdGetPreSever(TempCmdBuf);
			break;
		case CMD_GET_MAC_AND_KEY:								// 0x02命令								//
			Protocol_CmdGetMacAndKey(TempCmdBuf);
			break;
		case CMD_READ_UID:										// 0x03命令								//
			Protocol_CmdReadUID(TempCmdBuf);
			break;
		case CMD_WRITE_UID:										// 0x04命令								//
			Protocol_CmdWriteUID(TempCmdBuf);
			break;
		case CMD_DEL_UID:										// 0x05命令								//
			Protocol_CmdDelUID(TempCmdBuf);
			break;
		case CMD_OPEN_DOOR:										// 0x06命令								//
			Protocol_CmdOpenDoor(TempCmdBuf);
			break;
		case CMD_SEND_OPEN_INFO:								// 0x07命令								//
			Protocol_CmdSendOpenInfo(TempCmdBuf);
			break;
		case CMD_PLUSE:											// 0x08命令								//
			Protocol_CmdPluse(TempCmdBuf);
			break;
		case CMD_CLEAR_UID:										// 0x09命令								//
			Protocol_CmdClearUID(TempCmdBuf);
			break;
		case CMD_TIME_CHECK:									// 0x0A命令								//
			Protocol_CmdTimeCheck(TempCmdBuf);
			break;
        case CMD_SEND_WARN_INFO:								// 0x0B命令								//
			Protocol_CmdSendWarnInfo(TempCmdBuf);
			break;
		case CMD_READ_INFO:										// 0x0C命令								//
			Protocol_CmdReadInfo(TempCmdBuf);
			break;
		case CMD_REMOTE_UPDATE:									// 0x0D命令								//
			Protocol_CmdRemoteUpdate(TempCmdBuf);
			break;
		case CMD_REMOTE_RESTART:								// 0x0E命令								//
			Protocol_CmdRemoteRestart(TempCmdBuf);
			break;
		case CMD_DOWNLOAD:										// 0x0F命令								//
		    Protocol_CmdRemoteDownload(TempCmdBuf);
		    break;
		case CMD_CHANGE_IP:										// 0x10命令								//
		    Protocol_CmdChangeIP(TempCmdBuf);
		    break;	
		
		case CMD_MANUFAC:										// 0xF1命令								//
			Protocol_CmdManufac(TempCmdBuf);
		break;	
			
		case CMD_CLROPENDATA:									// 0xF2命令								//
			Protocol_ClrOpenData_Qty(TempCmdBuf);	
		break;	
			
	    default:
            break;
	}
	
	DBG_PRINTF("leaving %s\n", __FUNCTION__);
}

//===【结束】=================== 业务层处理接收到的服务器/蓝牙指令 =======================================

