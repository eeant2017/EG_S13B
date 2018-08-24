//********************************************************************************************************
// File name		: Bluetooth.c
// Description		: 蓝牙接收发送的相关函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "Bluetooth.h"



//********************************************************************************************************
// Function name	: Protocol_BleSend
// Description		: 蓝牙发送指定长度的数据包
// Parameter		: data		: 数据
//					: length	: 长度
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
int BleSend(uint8 *data, uint16 length)
{	
	uint8 *ptr = data;
	uint16 sign_len = (uint16)length;				// 整个包的长度										//
	uint16 active_len = 0;							// 单次需要发送的数据长度，不大于CYBLE_MTU_LEN(20)	//
	CYBLE_API_RESULT_T ret = CYBLE_ERROR_OK;
	CYBLE_GATTS_HANDLE_VALUE_IND_T locationParam;   // 蓝牙传输数据格式包								//
	
	while (sign_len > 0) 
	{
		// 获取当前序包的包长度																			//
		active_len = sign_len > CYBLE_MTU_LEN? CYBLE_MTU_LEN: sign_len;	
		
		// 填充需要传送的数据       																	//
	    locationParam.attrHandle = CYBLE_LOCK_SERVICE_UP_CHARACTERISTIC_CHAR_HANDLE;
	    locationParam.value.actualLen = active_len;
	    locationParam.value.len = active_len;
	    locationParam.value.val = ptr;
		ret = CyBle_GattsNotification(cyBle_connHandle,&locationParam);
		
		if (ret != CYBLE_ERROR_OK)
		{
			DBG_PRINTF("AppProtocol_Send--->send error\n");
			break;
		}
		
		sign_len -= active_len;						// 发完一次，总长度减去一个包的长度					//
		ptr += active_len;							// 发完一次，数据指针往前移动一个包的长度			//
	}

	DBG_PRINTF("AppProtocol_Send--->sent finish.\n");
	return ret;
}

//********************************************************************************************************
// Function name	: BleRecv
// Description		: 蓝牙分组接收APP传来数据并组合成完整命令包
// Parameter		: uint8  *cmd	APP传来的数据存放于该数组
//					: uint16 len	长度
// Return			: always TRUE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 BleRecv(uint8 *cmd, uint16 len)
{
	DBG_PRINTF("%s\n", __FUNCTION__);
	
	if (CurrentDataNum == 0)
	{
		if ((cmd[0] != HEAD_BYTE1) || (cmd[1] != HEAD_BYTE2))
		{
			return TRUE;
		}
		else
		{
			memcpy(BleCmdBuf, cmd, len);
			CurrentDataNum = len;
			CurrentCommandPktTotalSize = (cmd[2] << 8) + cmd[3] + 4;
            if(CurrentDataNum < CurrentCommandPktTotalSize)
            {
                return TRUE;
            }
            else
            {
                CurrentDataNum = 0;
                CurrentCommandPktTotalSize = 0;
                BleCmdValidFlag = 1;
                return TRUE;
            }
		}
	}
	else
	{
		memcpy(BleCmdBuf + CurrentDataNum, cmd, len);
		CurrentDataNum += len;
		if (CurrentDataNum < CurrentCommandPktTotalSize)
		{
			return TRUE;
		}
		else
		{
			CurrentDataNum = 0;
			CurrentCommandPktTotalSize = 0;
			BleCmdValidFlag = 1;
			return TRUE;
		}
	}
}

//******************************************** END OF FILE *********************************************//
