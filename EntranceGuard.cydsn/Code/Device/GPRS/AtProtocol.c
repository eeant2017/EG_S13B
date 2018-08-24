//********************************************************************************************************
// File name		: AtProtocol.c
// Description		: AT指令的相关函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "AtProtocol.h"



//******************************************** AT指令定义 **********************************************//
// 发送
const char AT_AT[] = {"AT\r\n"};								// AT指令								//
const char AT_CTZR[] = {"AT+CTZR=1\r\n"};						// 开启时区报告							//
const char AT_CPIN[] = {"AT+CPIN?\r\n"};						// 查询PIN码							//
const char AT_CGACT[] = {"AT+CGACT=1,1\r\n"};					// 激活PDP上下文						//
const char AT_CLOSE[] = {"AT+IPCLOSE\r\n"};						// 关闭连接指令							//
const char AT_CMSTATE[] = {"AT+CMSTATE\r\n"};					// 主动查询服务器连接状态				//
const char AT_CMMUX[] = {"AT+CMMUX=0\r\n"};						// 设置单连接模式						//
const char AT_OPENLED[] = {"AT+CMNETLED=1\r\n"};				// 打开联网指示灯						//
const char AT_CREG[] = {"AT+CREG?\r\n"};						// 查询网络注册情况						//
const char AT_CSQ[] = {"AT+CSQ\r\n"};							// 查询RSSI								//
const char AT_APN[] = {"AT+CGDCONT=1,\"IP\",\"CMNET\"\r\n"};	// 设置APN								//



// 接收
const char AT_OK[] = {"OK"};
const char AT_ERR[] = {"+CME ERROR:"};							// 错误AT指令							//
const char AT_ERR4[] = {"+CME ERROR: 4"};						// 错误码4								//
const char AT_INIT_OK[] = {"+CTZV:"};							// 上电成功，报告当前连接时间			//
const char AT_RECPIN[] = {"+CPIN: READY,1"};					// PIN码不需要输入						//
const char AT_CONNECT_OK[] = {"CONNECT OK"};					// 建立公网IP连接成功					//
const char AT_CONNECT_FAIL[] = {"CONNECT FAIL"};				// 建立公网IP连接失败					//
const char AT_ALREADY_CONNECT[] = {"ALREADY CONNECT"};			// 设备已经连接一个服务器				//
const char AT_CONNECT_CLOSED[] = {"CONNECTION CLOSED"};			// 当前连接被远程关闭					//
const char AT_CLOSE_CONNECT[] = {"+IPCLOSE:"};					// 成功释放一个连接【多连接模式】		//
const char AT_CLOSE_R[] = {"CLOSE"};							// 连接关闭								//
const char AT_RECMSTATE_INITIAL[] = {"INITIAL"};				// CMSTATE查询结果：初始化状态			//
const char AT_RECMSTATE_CONNECTD[] = {"CONNECTED"};				// CMSTATE查询结果：已连接				//
const char AT_RECMSTATE_CLOSED[] = {"CLOSED"};					// CMSTATE查询结果：已关闭				//
const char AT_ALLOW_SEND[] = {">"};      						// 允许发送数据							//
const char AT_SEND_OK[] = {"SEND OK"};							// 非透传模式，发送数据完成				//
const char AT_RECREG[] = {"+CREG:"};						// 网络注册情况回复值   :(0-2),1表示成功//
const char AT_RECSQ[] = {"+CSQ:"};								// 信号强度回复值						//




//********************************************* 变量声明 ***********************************************//
const char *tcp = "TCP";
const char *udp = "UDP";
//const char *test1_ip = "test1.zjeja.com"; 	//第一次用的测试服务器地址 @08-22
const char *test1_ip = "183.146.22.84"; 		//第二次用的测试服务器地址 @08-22

//const char *test2_ip = "test1.zjeja.com";
//const char *test3_ip = "test1.zjeja.com";
//const char *test4_ip = "test1.zjeja.com";
const char *center_ip = "lock.zjeja.com";
//const char *center_port = "8981";
const char *center_port = "8982";

//const char *test1_ip = "106.14.140.19";
//const char *center_ip = "106.14.140.19";
//const char *center_port = "8888";


//******************************************* 外部变量声明 *********************************************//
extern uint8  domain[200];
extern uint8  pre_ip[200];
extern uint8  pre_port[5];
extern uint8  update_ip[16];
extern uint8  update_port[4];

extern uint8  gprs_state;

extern volatile uint32_t wait_timeout;

extern volatile uint8 RecvType;
extern volatile char  GPRS_AT_RecBuf[64];
extern volatile uint8 AT_RecvSta;
extern volatile uint8 AT_index;

extern volatile char  GPRS_AT_Buf[64];
extern volatile uint8 GPRS_AT_RecFinish;

extern uint32 cur_time;

//********************************************************************************************************
// Function name	: CyCharToHex
// Description		: 字符串转回16进制
// Parameter		: value
// Return			: CYRET_SUCCESS
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
unsigned char CyCharToHex(char value)
{
    if ('0' <= value && value <= '9')
        return (unsigned char)(value - '0');
    if ('a' <= value && value <= 'f')
        return (unsigned char)(10 + value - 'a');
    if ('A' <= value && value <= 'F')
        return (unsigned char)(10 + value - 'A');
    return 0;
}


//********************************************************************************************************
// Function name	: GPRS_AT_ParaInit
// Description		: GPRS AT指令相关的参数初始化
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void GPRS_AT_ParaInit(void)
{
	RecvType = 0;
	AT_RecvSta = 0;
	AT_index = 0;
	GPRS_AT_RecFinish = RESET;
	memset((char *)GPRS_AT_Buf, 0, sizeof(GPRS_AT_Buf));
	memset((char *)GPRS_AT_RecBuf, 0, sizeof(GPRS_AT_RecBuf));
}

//********************************************************************************************************
// Function name	: GPRS_Send_ConnectServer
// Description		: 发送连接服务器
// Parameter		: tcp	: TCP或UDP
//					: ip	: IP或域名
//					: port	: 端口号
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void GPRS_Send_ConnectServer(char *tcp, char *ip, char *port)
{
	printf("AT+IPSTART=\"%s\"\,\"%s\"\,%s\r\n", tcp, ip, port);
}

//********************************************************************************************************
// Function name	: GPRS_Send_LengthOfSendData
// Description		: 设置发送数据的长度
// Parameter		: len	: 发送的数据长度
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void GPRS_Send_LengthOfSendData(uint16_t len)
{
	printf("AT+IPSEND=%d\r\n",len);
}

//********************************************************************************************************
// Function name	: GPRS_AT_CheckAck
// Description		: 检测AT指令的应答
// Parameter		: r_char	: 应答的指令
// Return			: 1: OK; 0: Err
//--------------------------------------------------------------------------------------------------------
// Info				: 通用的检查是否有指定字段的函数.
//********************************************************************************************************
uint8_t GPRS_AT_CheckAck(char *r_char)
{
	uint8_t ret; 
	
    if (strstr((char *)GPRS_AT_Buf, r_char))					// 检查应答是否有指定字段				//
    {
        ret = TRUE;
    }
    else
    {   
        ret = FALSE;
    }
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_AT_SendAndCheckAck
// Description		: 发送命令并检查应答结果
// Parameter		: s_char	: 发送的字符
//					: r_char	: 准备接收的字符
//					: size		: 发送字符的长度
//					: timeout	: 超时时间
// Return			: 1: TRUE; 0: FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 通用的发送AT指令，等待应答回复并判断是否有指定字段的函数.
//********************************************************************************************************
uint8_t GPRS_AT_SendAndCheckAck(char *s_char, char *r_char, uint16_t size, uint16_t timeout)
{
	uint8_t ret;
	
	GPRS_AT_ParaInit();
	
	Uart433Send((uint8_t *)s_char, size);						// 发送指定的AT指令						//
	wait_timeout = timeout;										// 设置AT应答接收超时时间				//
	ret = FALSE;
	while ((ret != TRUE) && (wait_timeout > 0))
	{
	    if (GPRS_AT_RecFinish == SET)
		{
	        ret = GPRS_AT_CheckAck(r_char);
			GPRS_AT_RecFinish = RESET;
		}
        sys_heartbeat();
	}
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_WaitInitFinish
// Description		: 等待GPRS初始化完成
// Parameter		: none
// Return			: 结果 TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: const char AT_CTZR[] = {"AT+CTZR=1\r\n"};
//********************************************************************************************************
uint8_t GPRS_WaitInitFinish(void)
{
	uint8_t ret;
	
	GPRS_AT_ParaInit();
	
	wait_timeout = 20000;
	ret = FALSE;
	while ((ret != TRUE) && (wait_timeout > 0))
	{
		if (GPRS_AT_RecFinish == SET)
		{
			ret = GPRS_AT_CheckAck((char *)AT_INIT_OK);
			GPRS_AT_RecFinish = RESET;
		}
        sys_heartbeat();
	}
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_TestAndWaitCommonOK
// Description		: 连续发AT命令4次，共20秒钟，等待通讯正常
// Parameter		: none
// Return			: 结果 TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_TestAndWaitCommonOK(void)
{
	uint8_t i, ret;
	
	for (i = 0; i < 20; i++)
	{
		ret = GPRS_AT_SendAndCheckAck((char *)AT_AT, (char *)AT_OK, strlen(AT_AT), 200);
		if (ret == TRUE)
		{
			break;
		}
	}
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_QuerySimCard
// Description		: 查询Sim卡是否正确连接
// Parameter		: none
// Return			: 结果 TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_QuerySimCard(void)
{
	uint8_t i, ret;
	
	for (i = 0; i < 3; i++)
	{
		ret = GPRS_AT_SendAndCheckAck((char *)AT_CPIN, (char *)AT_RECPIN, strlen(AT_CPIN), 1500);
		if (ret == TRUE)
		{
			break;
		}
	}
	return ret;
}


//********************************************************************************************************
// Function name	: GPRS_QuerySimCard
// Description		: 确认CS域网络注册成功
// Parameter		: none
// Return			: 结果 TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_QueryCREG(void)
{
	uint8_t i, ret;
	uint8_t tmp[2] = {0};
	for (i = 0; i < 3; i++)
	{
		ret = GPRS_AT_SendAndCheckAck((char *)AT_CREG, (char *)AT_RECREG, strlen(AT_CREG), 1500);
		if (ret == TRUE)
		{
			tmp[0] = CyCharToHex(GPRS_AT_Buf[9]);
			tmp[1] = CyCharToHex(GPRS_AT_Buf[11]);
			DBG_SOFT_PutStr("\nCREG:");
			DBG_SOFT_PrintfHex(tmp,2,16);
			if(tmp[1] == 1)
			{
				return TRUE;
			}
			else if (tmp[1] == 0)
			{
				return FALSE;
			}
		}
	}
	return ret;
}
// Description		: 查询模块信号强度 AT+CSQ
// Parameter		: none
// Return			: 结果 TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_QueryRssi(void)
{
	uint8_t i, ret,rssi;
	uint8_t temp[2] = {0};
	
	for (i = 0; i < 3; i++)
	{
		ret = GPRS_AT_SendAndCheckAck((char *)AT_CSQ, (char *)AT_RECSQ, strlen(AT_CSQ), 1500);
		if (ret == TRUE)
		{	
			temp[0] = CyCharToHex(GPRS_AT_Buf[8]);
			temp[1] = CyCharToHex(GPRS_AT_Buf[9]);
			DBG_SOFT_PutStr("\nRssi:");
			DBG_SOFT_PrintfHex(temp,2,16);
			rssi = temp[0]*10+temp[1];
			return ((rssi/6)+((rssi%6) ? 1:0));
		}
	}
	return FALSE;	
}
//********************************************************************************************************
// Function name	: GPRS_QuerySimCard
// Description		: 设置GPRS模块APN
// Parameter		: none
// Return			: 结果 TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_SetAPN(void)
{
	uint8_t i, ret;
	for (i = 0; i < 3; i++)
	{
		ret = GPRS_AT_SendAndCheckAck((char *)AT_APN, (char *)AT_OK, strlen(AT_APN), 1500);
		if (ret == TRUE)
		{
			break;
		}
	}
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_SetActive
// Description		: 设置激活PDP上下文
// Parameter		: none
// Return			: 结果 TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_SetActive(void)
{
	uint8_t i, ret;
	
	for (i = 0; i < 3; i++)
	{
		ret = GPRS_AT_SendAndCheckAck((char *)AT_CGACT, (char *)AT_OK, strlen(AT_CGACT), 2000);
		if (ret == TRUE)
		{
			break;
		}
	}
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_CheckConnectState
// Description		: 查询服务器连接状态
// Parameter		: none
// Return			: 1(TRUE): 已连接; 0(FALSE): 其它状态
//--------------------------------------------------------------------------------------------------------
// Info				: const char AT_RECMSTATE_INITIAL[] = {"INITIAL"};
//					: const char AT_RECMSTATE_CONNECTD[] = {"CONNECTD"};
//					: const char AT_RECMSTATE_CLOSED[] = {"CLOSED"};
//********************************************************************************************************
uint8_t GPRS_CheckConnectState(void)
{
	uint8_t ret;
	
	GPRS_AT_ParaInit();
	
	Uart433Send((uint8_t *)AT_CMSTATE, strlen(AT_CMSTATE));
	wait_timeout = 3000;
	ret = FALSE;
	while (wait_timeout > 0)
	{
	    if (GPRS_AT_RecFinish == SET)
		{
			ret = GPRS_AT_CheckAck((char *)AT_RECMSTATE_CONNECTD);
			if (ret == TRUE)
			{
				wait_timeout = 0;
			}
			GPRS_AT_RecFinish = RESET;
		}
        sys_heartbeat();
	}
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_ClosedConnect
// Description		: 关闭连接
// Parameter		: none
// Return			: TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_ClosedConnect(void)
{
	uint8_t i, ret;
	
	for (i = 0; i < 3; i++)
	{
		GPRS_AT_ParaInit();
	    
		Uart433Send((uint8_t *)AT_CLOSE, strlen(AT_CLOSE));
		wait_timeout = 500;
		ret = FALSE;
		while ((ret != TRUE) && (wait_timeout > 0))
		{
		    if (GPRS_AT_RecFinish == SET)
			{
				ret = GPRS_AT_CheckAck((char *)AT_CLOSE_CONNECT);
				if (ret == FALSE)
				{
					ret = GPRS_AT_CheckAck((char *)AT_ERR4);	// 收到这个，处于已经断开状态			//
				}
				GPRS_AT_RecFinish = RESET;
			}
            sys_heartbeat();
		}
		if (ret == TRUE)
		{
			return ret;
		}
	}
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_SetConnectMode
// Description		: 设置连接模式
// Parameter		: none
// Return			: TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_SetConnectMode(void)
{
	uint8_t i, ret;
	
	for (i = 0; i < 3; i++)
	{
		ret = GPRS_AT_SendAndCheckAck((char *)AT_CMMUX, (char *)AT_OK, strlen(AT_CMMUX), 1000);
		if (ret == TRUE)
		{
			break;
		}
	}
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_OpenNetworkLED
// Description		: 打开联网指示灯
// Parameter		: none
// Return			: TRUE / FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_OpenNetworkLED(void)
{
	uint8_t i, ret;
	
	for (i = 0; i < 3; i++)
	{
		ret = GPRS_AT_SendAndCheckAck((char *)AT_OPENLED, (char *)AT_OK, strlen(AT_OPENLED), 500);
		if (ret == TRUE)
		{
			break;
		}
	}
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_ConnectCenterServer
// Description		: 连接中心服务器
// Parameter		: none
// Return			: 1: TRUE; 0: FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_ConnectCenterServer(void)
{
	uint8_t ret;
	uint8_t IpType;
	
	GPRS_AT_ParaInit();
	
	IpType = Read_CenterIpType();
	#if DBG_SOFT_ENNABLE
	DBG_SOFT_PutString("\nIpType:");
	DBG_SOFT_PutHexInt(IpType);
	#endif
	if (IpType == 1)
	{	
		#if DBG_SOFT_ENNABLE
		DBG_SOFT_PutString("\nTest1 Server.\n");
		#endif
		GPRS_Send_ConnectServer((char *)tcp, (char *)test1_ip, (char *)center_port);
	}
	else if (IpType == 2)
	{
		//GPRS_Send_ConnectServer((char *)tcp, (char *)test2_ip, (char *)center_port);
	}
	else if (IpType == 3)
	{
		//GPRS_Send_ConnectServer((char *)tcp, (char *)test3_ip, (char *)center_port);
	}
	else if (IpType == 4)
	{
		//GPRS_Send_ConnectServer((char *)tcp, (char *)test4_ip, (char *)center_port);
	}
	else if (IpType == 5)
	{	
		#if DBG_SOFT_ENNABLE
		DBG_SOFT_PutString("\nFormal Server.\n");
		#endif
		GPRS_Send_ConnectServer((char *)tcp, (char *)center_ip, (char *)center_port);
	}
	else
	{
		//没有设置过IpType 默认为1
		IpType = 1;
		Write_CenterIpType(IpType);
		GPRS_Send_ConnectServer((char *)tcp, (char *)test1_ip, (char *)center_port);
	}
	
	wait_timeout = 10000;
	ret = FALSE;
	while ((ret != TRUE) && (wait_timeout > 0))
	{
		if (GPRS_AT_RecFinish == SET)
		{
			ret = GPRS_AT_CheckAck((char *)AT_CONNECT_OK);
			if (ret == FALSE)
			{
				ret = GPRS_AT_CheckAck((char *)AT_ALREADY_CONNECT);
			}
			GPRS_AT_RecFinish = RESET;
		}
        sys_heartbeat();
	}
    return ret;
}

//********************************************************************************************************
// Function name	: GPRS_ConnectCenterServer
// Description		: 连接前置服务器
// Parameter		: none
// Return			: 1: TRUE; 0: FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_ConnectPreServer(void)
{
	uint8_t ret;
	
	GPRS_AT_ParaInit();
	
	GPRS_Send_ConnectServer((char *)tcp, (char *)pre_ip, (char *)pre_port);
	wait_timeout = 10000;
	ret = FALSE;
	while ((ret != TRUE) && (wait_timeout > 0))
	{
		if (GPRS_AT_RecFinish == SET)
		{
			ret = GPRS_AT_CheckAck((char *)AT_CONNECT_OK);
			if (ret == FALSE)
			{
				ret = GPRS_AT_CheckAck((char *)AT_ALREADY_CONNECT);
			}
			GPRS_AT_RecFinish = RESET;
		}
        sys_heartbeat();
	}
    return ret;
}

//********************************************************************************************************
// Function name	: GPRS_ConnectUpdateServer
// Description		: 连接升级服务器
// Parameter		: none
// Return			: 1: TRUE; 0: FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t GPRS_ConnectUpdateServer(void)
{
	uint8_t ret;
	
	GPRS_AT_ParaInit();
	
	GPRS_Send_ConnectServer((char *)tcp, (char *)update_ip, (char *)update_port);
	wait_timeout = 10000;
	ret = FALSE;
	while ((ret != TRUE) && (wait_timeout > 0))
	{
		if (GPRS_AT_RecFinish == SET)
		{
			ret = GPRS_AT_CheckAck((char *)AT_CONNECT_OK);
			if (ret == FALSE)
			{
				ret = GPRS_AT_CheckAck((char *)AT_ALREADY_CONNECT);
			}
			GPRS_AT_RecFinish = RESET;
		}
        sys_heartbeat();
	}
    return ret;
}

//********************************************************************************************************
// Function name	: GPRS_SendHexDataToServer
// Description		: 发送十六进制数据到服务器
// Parameter		: none
// Return			: 1: TRUE; 0: FALSE
//--------------------------------------------------------------------------------------------------------
// Info				: 发送数据时可能会遇到网络差，导致连接失败的情况，错误码为 【CONNECTION CLOSED: 0】
//********************************************************************************************************
uint8_t GPRS_SendHexDataToServer(uint8_t *data, uint16_t size)
{
	uint8_t ret;
	uint8_t buf[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
	
	GPRS_AT_ParaInit();
	
	GPRS_Send_LengthOfSendData(size);
	wait_timeout = 1000;
	ret = FALSE;
	while ((ret != TRUE) && (wait_timeout > 0))
	{
		if (GPRS_AT_RecFinish == SET)
		{
			ret = GPRS_AT_CheckAck((char *)AT_ALLOW_SEND);
			GPRS_AT_RecFinish = RESET;
		}
        sys_heartbeat();
	}
    // 这里如果有数据下发下来 怎么处理? @chensq 02-04
	if (ret == TRUE)										
	{
		GPRS_AT_ParaInit();
		
		Uart433Send(data, size);
		Uart433Send(buf, 10);									// 防止数据发少了，再补几个数			//
		wait_timeout = 5000;
		ret = FALSE;
		while ((ret != TRUE) && (wait_timeout > 0))
		{
			if (GPRS_AT_RecFinish == SET)
			{
				ret = GPRS_AT_CheckAck((char *)AT_SEND_OK);
				if (ret == FALSE)
				{
					ret = GPRS_AT_CheckAck((char *)AT_CONNECT_CLOSED);
					if (ret == TRUE)							// 连接被远程服务器关闭					//
					{
						gprs_state = GET_PRE_IP;
					}
					else
					{
						ret = GPRS_AT_CheckAck((char *)AT_CONNECT_CLOSED);
						if (ret == TRUE)						// GPRS模块状态出错						//
						{
							gprs_state = NEED_INIT;
						}
					}
				}
				GPRS_AT_RecFinish = RESET;
			}
            sys_heartbeat();
		}
	}
	else
	{
		gprs_state = NEED_INIT;
		return ret;
	}
	return ret;
}

//********************************************************************************************************
// Function name	: GPRS_CheckIfRecvClosed
// Description		: 检查是否收到CLOSE字段
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void GPRS_CheckIfRecvClosed(void)
{
	uint8_t ret;
	
	if (GPRS_AT_RecFinish == SET)
	{
		ret = GPRS_AT_CheckAck((char *)AT_CLOSE_R);
		if (ret == TRUE)
		{
			gprs_state = GET_PRE_IP;
		}
		GPRS_AT_RecFinish = RESET;
	}
}


#if 0
//********************************************************************************************************
// Function name	: test_send_disconnect
// Description		: 发给董主动断开指令
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// 说明				: 
//*******************************************************************************************************/
void test_send_disconnect(void)
{
	uint8_t disconnenct_data[17] = {0xef, 0x3A, 0x00, 0x1b, 0x01, 0x01, 0xFF, 0x01, 0x02, 0x03, 0xB4, 
									0x67, 0x25, 0x71, 0x75, 0x69, 0x74};
	GPRS_SendHexDataToServer(disconnenct_data, 17);
}
#endif


//******************************************** END OF FILE *********************************************//
