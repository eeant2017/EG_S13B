//********************************************************************************************************
// File name		: main.c
// Description		: 门禁系统的OTA引导程序
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "main.h"


//********************************************************************************************************
// Function name	: PrintStartProject
// Description		: BootLoader工程启动时打印的Logo
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
static void PrintStartProject(void)
{
    UART_PutString("\r\n");
    UART_PutString("\r\n");
    UART_PutString("===============================================================================\r\n");
    UART_PutString("=  BLE_OTA_External_Memory_Bootloader Application Started\r\n");
    UART_PutString("=  Version: 1.40\r\n");    
    DBG_PRINTF    ("=  Compile Date and Time : %s %s\r\n", __DATE__,__TIME__);
    UART_PutString("===============================================================================\r\n");
    UART_PutString("\r\n"); 
    UART_PutString("\r\n");
}

//********************************************************************************************************
// Function name	: PrintUpdateImage
// Description		: BootLoader工程执行更新Image程序
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
static void PrintUpdateImage(void)
{
    UART_PutString("\r\n");
    UART_PutString("===============================================================================\r\n");
    UART_PutString("=  Start Update Image!\r\n");
	DBG_PRINTF    ("=  Compile Date and Time : %s %s\r\n", __DATE__,__TIME__);
    UART_PutString("===============================================================================\r\n");
    UART_PutString("\r\n");
}

//********************************************************************************************************
// Function name	: PrintLauchApp
// Description		: BootLoader工程执行启动App程序
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
static void PrintLauchApp(void)
{
    UART_PutString("\r\n");
    UART_PutString("===============================================================================\r\n");
    UART_PutString("=  Start_Lauch App\r\n");
	DBG_PRINTF    ("=  Compile Date and Time : %s %s\r\n", __DATE__,__TIME__);
    UART_PutString("===============================================================================\r\n");
    UART_PutString("\r\n"); 
}

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
		ret = CyBle_GattsNotification(cyBle_connHandle, &locationParam);
		
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
// Description		: 蓝牙分组接收APP传来数据
// Parameter		: uint8  *cmd	APP传来的数据存放于该数组
//					: uint16 len	长度
// Return			: always TRUE
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void BleRecv(uint8 *cmd, uint16 len)
{
	DBG_PRINTF("%s\n", __FUNCTION__);
	BleSend(cmd, len);								// 收到什么回传什么，防止被优化						//
}

//********************************************************************************************************
// Function name	: AppCallback
// Description		: 蓝牙的回调函数
// Parameter		: event
//					: eventParam
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void AppCallback(uint32 event, void* eventParam)
{
    CYBLE_API_RESULT_T apiResult;
    CYBLE_GAP_AUTH_INFO_T *authInfo;
    CYBLE_BLESS_PWR_IN_DB_T bleSsPowerLevel;
	CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T *locationWCRP = (CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T *)eventParam;	
	
    switch (event)
	{
        /**********************************************************
        *                       General Events
        ***********************************************************/
		case CYBLE_EVT_STACK_ON: /* This event received when component is Started */
			/* Set Tx Power Level for advertising and connection channels */
            bleSsPowerLevel.blePwrLevelInDbm = CYBLE_LL_PWR_LVL_NEG_12_DBM;
            bleSsPowerLevel.bleSsChId = CYBLE_LL_ADV_CH_TYPE;
            (void) CyBle_SetTxPowerLevel (&bleSsPowerLevel);
			

			/* Enter into discoverable mode so that remote can search it. */
            apiResult = CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            if(apiResult != CYBLE_ERROR_OK)
            {
                DBG_PRINTF("StartAdvertisement API Error: %d \r\n", apiResult);
            }
			DBG_PRINTF("\r\n");
            break;
		case CYBLE_EVT_TIMEOUT: 
            DBG_PRINTF("CYBLE_EVT_TIMEOUT: %x \r\n", *(CYBLE_TO_REASON_CODE_T *)eventParam);
			break;
		case CYBLE_EVT_HARDWARE_ERROR:    /* This event indicates that some internal HW error has occurred. */
            DBG_PRINTF("Hardware Error \r\n");
			break;
        case CYBLE_EVT_HCI_STATUS:
            DBG_PRINTF("CYBLE_EVT_HCI_STATUS: %x \r\n", *(uint8 *)eventParam);
			break;

        /**********************************************************
        *                       GAP Events
        ***********************************************************/
        case CYBLE_EVT_GAP_AUTH_REQ:
            DBG_PRINTF("CYBLE_EVT_AUTH_REQ: security=%x, bonding=%x, ekeySize=%x, err=%x \r\n", 
                (*(CYBLE_GAP_AUTH_INFO_T *)eventParam).security, 
                (*(CYBLE_GAP_AUTH_INFO_T *)eventParam).bonding, 
                (*(CYBLE_GAP_AUTH_INFO_T *)eventParam).ekeySize, 
                (*(CYBLE_GAP_AUTH_INFO_T *)eventParam).authErr);
            break;
        case CYBLE_EVT_GAP_PASSKEY_ENTRY_REQUEST:
            DBG_PRINTF("CYBLE_EVT_PASSKEY_ENTRY_REQUEST \r\n");
            break;
        case CYBLE_EVT_GAP_PASSKEY_DISPLAY_REQUEST:
            DBG_PRINTF("CYBLE_EVT_PASSKEY_DISPLAY_REQUEST %6.6ld \r\n", *(uint32 *)eventParam);
            break;
        case CYBLE_EVT_GAP_AUTH_COMPLETE:
            authInfo = (CYBLE_GAP_AUTH_INFO_T *)eventParam;
            (void)authInfo;
            DBG_PRINTF("AUTH_COMPLETE: security:%x, bonding:%x, ekeySize:%x, authErr %x \r\n", 
                                    authInfo->security, authInfo->bonding, authInfo->ekeySize, authInfo->authErr);
            break;
        case CYBLE_EVT_GAP_AUTH_FAILED:
            DBG_PRINTF("CYBLE_EVT_AUTH_FAILED: %x \r\n", *(uint8 *)eventParam);
            break;
        case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
            DBG_PRINTF("CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP, state: %x\r\n", CyBle_GetState());
            if(CYBLE_STATE_DISCONNECTED == CyBle_GetState())
            {   
                /* Fast and slow advertising period complete, go to low power  
                 * mode (Hibernate mode) and wait for an external
                 * user event to wake up the device again */
                DBG_PRINTF("Hibernate \r\n");
            #if (DEBUG_UART_ENABLED == ENABLED)
            	//while((UART_DEB_SpiUartGetTxBufferSize() + UART_DEB_GET_TX_FIFO_SR_VALID) != 0);
            #endif /* (DEBUG_UART_ENABLED == ENABLED) */
            }
            break;
        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
            DBG_PRINTF("CYBLE_EVT_GAP_DEVICE_CONNECTED: %x \r\n", *(uint8 *)eventParam);
            break;
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            DBG_PRINTF("CYBLE_EVT_GAP_DEVICE_DISCONNECTED\r\n");
            /* Put the device to discoverable mode so that remote can search it. */
            apiResult = CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            if(apiResult != CYBLE_ERROR_OK)
            {
                DBG_PRINTF("StartAdvertisement API Error: %x \r\n", apiResult);
            }
            break;
        case CYBLE_EVT_GAP_ENCRYPT_CHANGE:
            DBG_PRINTF("ENCRYPT_CHANGE: %x \r\n", *(uint8 *)eventParam);
            break;
        case CYBLE_EVT_GAPC_CONNECTION_UPDATE_COMPLETE:
            DBG_PRINTF("CYBLE_EVT_CONNECTION_UPDATE_COMPLETE: %x \r\n", *(uint8 *)eventParam);
            break;
        case CYBLE_EVT_GAP_KEYINFO_EXCHNGE_CMPLT:
            DBG_PRINTF("CYBLE_EVT_GAP_KEYINFO_EXCHNGE_CMPLT \r\n");
            break;

        /**********************************************************
        *                       GATT Events
        ***********************************************************/
        case CYBLE_EVT_GATT_CONNECT_IND:
            DBG_PRINTF("CYBLE_EVT_GATT_CONNECT_IND: %x, %x \r\n", cyBle_connHandle.attId, cyBle_connHandle.bdHandle);
            break;
        case CYBLE_EVT_GATT_DISCONNECT_IND:
            DBG_PRINTF("CYBLE_EVT_GATT_DISCONNECT_IND \r\n");
            break;
        case CYBLE_EVT_GATTS_WRITE_REQ:
            DBG_PRINTF("CYBLE_EVT_GATT_WRITE_REQ: %x = ",((CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam)->handleValPair.attrHandle);
            (void)CyBle_GattsWriteRsp(((CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam)->connHandle);
            break;
		case CYBLE_EVT_GATTS_WRITE_CMD_REQ:
			DBG_PRINTF("CYBLE_EVT_GATT_WRITE_CMD_REQ: = 0x%03xu \n",((CYBLE_GATTS_WRITE_CMD_REQ_PARAM_T *)eventParam)->handleValPair.attrHandle);
			//该属性的句柄值:
			DBG_PRINTF("locationWCRP->handleValPair.value.attrHandle =0x%03xu\n",locationWCRP->handleValPair.attrHandle);
			//数据长度:
			DBG_PRINTF("locationWCRP->handleValPair.value.len = %d\n",locationWCRP->handleValPair.value.len);
			//数据actualen长度:
			DBG_PRINTF("locationWCRP->handleValPair.value.actualLen = 0x%x\n",locationWCRP->handleValPair.value.actualLen);
			//数据内容:
			BleRecv(locationWCRP->handleValPair.value.val, locationWCRP->handleValPair.value.len);
            break;
        case CYBLE_EVT_GATTS_INDICATION_ENABLED:
            DBG_PRINTF("CYBLE_EVT_GATTS_INDICATION_ENABLED \r\n");
            break;
        case CYBLE_EVT_GATTS_INDICATION_DISABLED:
            DBG_PRINTF("CYBLE_EVT_GATTS_INDICATION_DISABLED \r\n");
            break;
        case CYBLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ:
            /* Triggered on server side when client sends read request and when
            * characteristic has CYBLE_GATT_DB_ATTR_CHAR_VAL_RD_EVENT property set.
            * This event could be ignored by application unless it need to response
            * by error response which needs to be set in gattErrorCode field of
            * event parameter. */
            DBG_PRINTF("CYBLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ: handle: %x \r\n", 
                ((CYBLE_GATTS_CHAR_VAL_READ_REQ_T *)eventParam)->attrHandle);
            break;

        /**********************************************************
        *                       Other Events
        ***********************************************************/
        case CYBLE_EVT_PENDING_FLASH_WRITE:
            /* Inform application that flash write is pending. Stack internal data 
            * structures are modified and require to be stored in Flash using 
            * CyBle_StoreBondingData() */
            DBG_PRINTF("CYBLE_EVT_PENDING_FLASH_WRITE\r\n");
            break;

		default:
            DBG_PRINTF("OTHER event: %lx \r\n", event);
			break;
	}
}

//********************************************************************************************************
// Function name	: metadatacheckSum
// Description		: 蓝牙的回调函数
// Parameter		: event
//					: eventParam
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint16 metadatacheckSum(const uint8 buffer[], uint16 Rowcnt)
{
	uint16 sum = 0u;
	
	#if 0
	while (size > 0u)
	{
	    sum += buffer[size - 1u];
	    size--;
	}
	return(( uint16 )1u + ( uint16 )(~sum));
	#endif
	
	uint16 extMemRowIdx;
    uint16 extMemAppRowsTotal = Rowcnt;
	const uint8  *extMemRow = buffer;
	uint16 size;
	
	for (extMemRowIdx = 0u; extMemRowIdx < extMemAppRowsTotal; extMemRowIdx++)
    {
        extMemRow = buffer + (extMemRowIdx * CY_FLASH_SIZEOF_ROW);
        size = CY_FLASH_SIZEOF_ROW;
        while (size > 0u)
        {
            size--;
            sum += extMemRow[size];
        }
    }
	return(( uint16 )1u + ( uint16 )(~sum));
}

//********************************************************************************************************
// Function name	: main
// Description		: 主函数
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
int main(void)
{   
	W25QXX_Init();
    CyGlobalIntEnable;
    PrintStartProject();
	CyBle_Start(AppCallback);
	
	#if 0
	UpdateImage();
	#endif
	
	#if 1
	if (Bootloader_GET_RUN_TYPE  == Bootloader_SCHEDULE_BTLDR)
	{
		PrintUpdateImage();
		if (UpdateImage() == FALSE)
		{
			PrintLauchApp();
			LaunchApp();
		}
	}
	else
	{	
		PrintLauchApp();
		LaunchApp();
    }
	#endif
	
	for(;;)
    {
	
    }
}

//******************************************** END OF FILE *********************************************//
 