//********************************************************************************************************
// File name		: Bluetooth.c
// Description		: �������շ��͵���غ���
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "Bluetooth.h"



//********************************************************************************************************
// Function name	: Protocol_BleSend
// Description		: ��������ָ�����ȵ����ݰ�
// Parameter		: data		: ����
//					: length	: ����
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
int BleSend(uint8 *data, uint16 length)
{	
	uint8 *ptr = data;
	uint16 sign_len = (uint16)length;				// �������ĳ���										//
	uint16 active_len = 0;							// ������Ҫ���͵����ݳ��ȣ�������CYBLE_MTU_LEN(20)	//
	CYBLE_API_RESULT_T ret = CYBLE_ERROR_OK;
	CYBLE_GATTS_HANDLE_VALUE_IND_T locationParam;   // �����������ݸ�ʽ��								//
	
	while (sign_len > 0) 
	{
		// ��ȡ��ǰ����İ�����																			//
		active_len = sign_len > CYBLE_MTU_LEN? CYBLE_MTU_LEN: sign_len;	
		
		// �����Ҫ���͵�����       																	//
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
		
		sign_len -= active_len;						// ����һ�Σ��ܳ��ȼ�ȥһ�����ĳ���					//
		ptr += active_len;							// ����һ�Σ�����ָ����ǰ�ƶ�һ�����ĳ���			//
	}

	DBG_PRINTF("AppProtocol_Send--->sent finish.\n");
	return ret;
}

//********************************************************************************************************
// Function name	: BleRecv
// Description		: �����������APP�������ݲ���ϳ����������
// Parameter		: uint8  *cmd	APP���������ݴ���ڸ�����
//					: uint16 len	����
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
