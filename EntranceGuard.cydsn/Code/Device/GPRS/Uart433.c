//********************************************************************************************************
// File name		: Protocol_CmdParse.c
// Description		: GPRS�����������غ���
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "Uart433.h"
#include "config.h"


//******************************************* �ⲿ�������� *********************************************//
extern volatile char  GPRS_AT_Buf[64];      					// ATָ��棬����ATָ�����			//
extern volatile uint8 GPRS_AT_RecFinish;

extern volatile uint8 GPRSCmdParseFlag1;
extern volatile uint8 GPRSCmdParseFlag2;
extern volatile uint8 GPRSCmd_buf1[DATABUF_SIZE];
extern volatile uint8 GPRSCmd_buf2[DATABUF_SIZE];



//********************************************* �������� ***********************************************//
volatile uint8  RecvType = 0;

volatile char   GPRS_AT_RecBuf[64];
volatile uint8  AT_RecvSta;
volatile uint8  AT_index;

volatile uint8  CmdRev_buf[DATABUF_SIZE];
volatile uint16 Cmd_Baglen;
volatile uint8  Cmd_Revsta;
volatile uint16 Cmd_index;



//********************************************************************************************************
// Function name	: Uart433Send
// Description		: ���ڷ���ָ����С������
// Parameter		: *data
//					: size
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: ͨ�õķ������ݸ�GPRSģ��ĺ���.
//********************************************************************************************************
void Uart433Send(uint8 *data, uint16 size)
{
    uint16 i;
    
    for (i = 0; i < size; i++)
    {
		UART_433_UartPutChar(data[i]);
    }
}

//********************************************************************************************************
// Function name	: Uart433Recv
// Description		: ���ڽ���GPRSģ�������
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: �ڴ����ж��е���
//--------------------------------------------------------------------------------------------------------
// ע��				: '>'���ź�û�л��У�
//********************************************************************************************************
void Uart433Recv(uint8 uData)
{
	switch (AT_RecvSta)
	{
		case HEAD_R:
			if ((uData == '\r') && (RecvType == 0))
			{
				RecvType = 1;
				AT_index = 0;
				AT_RecvSta = HEAD_N;
				GPRS_AT_RecBuf[AT_index++] = uData;
			}
			break;
		case HEAD_N:
			if (uData == '\n')
			{
				AT_RecvSta = REV_DATA;
				GPRS_AT_RecBuf[AT_index++] = uData;
			}
			break;
		case REV_DATA:
			if (uData == '>')
			{
				GPRS_AT_RecBuf[AT_index++] = uData;
				AT_RecvSta = HEAD_R;
				AT_index = 0;
				GPRS_AT_RecFinish = 1;
				memcpy((uint8 *)GPRS_AT_Buf, (uint8 *)GPRS_AT_RecBuf, sizeof(GPRS_AT_RecBuf));
				memset((uint8 *)GPRS_AT_RecBuf, 0, sizeof(GPRS_AT_RecBuf));
			}
			else
			{
				GPRS_AT_RecBuf[AT_index++] = uData;
				if (uData == '\r')
				{
					AT_RecvSta = TAIL_N;
				}
			}
			break;
		case TAIL_N:
			if (uData == '\n')
			{
				GPRS_AT_RecBuf[AT_index++] = uData;
				AT_RecvSta = HEAD_R;
				AT_index = 0;
				RecvType = 0;
				if (GPRS_AT_RecFinish == 0)
				{
					GPRS_AT_RecFinish = 1;
					memcpy((uint8 *)GPRS_AT_Buf, (uint8 *)GPRS_AT_RecBuf, sizeof(GPRS_AT_RecBuf));
					memset((uint8 *)GPRS_AT_RecBuf, 0, sizeof(GPRS_AT_RecBuf));
				}
			}
			break;
		default:
			break;
	}
		
	switch (Cmd_Revsta)
	{
		case HEX_EF:
			if ((uData == 0xef) && (RecvType == 0))
			{
				RecvType = 2;
				Cmd_index = 0;
				Cmd_Revsta = HEX_3A;
				CmdRev_buf[Cmd_index++] = uData;
			}
			break;
		case HEX_3A:
			if (uData == 0x3a)
			{
				Cmd_Revsta = HEX_LEN_H;
				CmdRev_buf[Cmd_index++] = uData;
			}
			break;
		case HEX_LEN_H:
			Cmd_Revsta = HEX_LEN_L;
			CmdRev_buf[Cmd_index++] = uData;
			break;
		case HEX_LEN_L:
			Cmd_Revsta = HEX_DATA;
			CmdRev_buf[Cmd_index++] = uData;
			Cmd_Baglen = (CmdRev_buf[2] << 8) | CmdRev_buf[3];
			break;
		case HEX_DATA:
			CmdRev_buf[Cmd_index++] = uData;
			if (Cmd_index == Cmd_Baglen + 4)
			{
				Cmd_Revsta = HEX_EF;
				Cmd_index = 0;
				RecvType = 0;
				
				if (GPRSCmdParseFlag1 == 0)
				{
					memcpy((uint8 *)GPRSCmd_buf1, (uint8 *)CmdRev_buf, Cmd_Baglen + 4);
					GPRSCmdParseFlag1 = 1;
				}
				else if(GPRSCmdParseFlag2 == 0)
				{
					// 
					memcpy((uint8 *)GPRSCmd_buf2, (uint8 *)CmdRev_buf, Cmd_Baglen + 4);
					GPRSCmdParseFlag2 = 1;
				}
				Cmd_Baglen = 0;
				memset((uint8 *)CmdRev_buf, 0, sizeof(CmdRev_buf));
			}
			break;
		default:
			break;
	}
}

//******************************************** END OF FILE *********************************************//
