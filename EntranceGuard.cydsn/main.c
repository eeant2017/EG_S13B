//********************************************************************************************************
// File name		: main.c
// Description		: 门禁系统的相关函数
// Author			: Keno
// Date				: 2016.8.8
// Version			: V02
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#include "main.h"


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
    CYBLE_GAP_BD_ADDR_T localAddr;
    CYBLE_GAP_AUTH_INFO_T *authInfo;
    CYBLE_BLESS_PWR_IN_DB_T bleSsPowerLevel;
    uint8 i;
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
			
			if (Read_Mac_Flag == 0)
			{
            	DBG_PRINTF("Bluetooth On, StartAdvertisement with addr: \n");
            	localAddr.type = 0u;
            	CyBle_GetDeviceAddress(&localAddr);
            	for(i = CYBLE_GAP_BD_ADDR_SIZE; i > 0u; i--)
            	{
                	DBG_PRINTF("%2.2x", localAddr.bdAddr[i-1]);
            	}
				DBG_PRINTF("\n");
				
				for (i = CYBLE_GAP_BD_ADDR_SIZE; i > 0u; i--)
				{
					Mac_Addr[CYBLE_GAP_BD_ADDR_SIZE - i] = localAddr.bdAddr[i - 1];
				}
				Read_Mac_Flag = 1;				
				
			}
			
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
			//DBG_SOFT_PrintfHex(locationWCRP->handleValPair.value.val,locationWCRP->handleValPair.value.len);  
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
// Function name	: LowPowerImplementation
// Description		: 进入低功耗睡眠的函数
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void LowPowerImplementation(void)
{
    CYBLE_LP_MODE_T bleMode;
    uint8 interruptStatus;
    
    /* For advertising and connected states, implement deep sleep 
     * functionality to achieve low power in the system. For more details
     * on the low power implementation, refer to the Low Power Application 
     * Note.
     */
    if((CyBle_GetState() == CYBLE_STATE_ADVERTISING) || 
       (CyBle_GetState() == CYBLE_STATE_CONNECTED))
    {
        /* Request BLE subsystem to enter into Deep-Sleep mode between connection and advertising intervals */
        bleMode = CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
        /* Disable global interrupts */
        interruptStatus = CyEnterCriticalSection();
        /* When BLE subsystem has been put into Deep-Sleep mode */
        if(bleMode == CYBLE_BLESS_DEEPSLEEP)
        {
            /* And it is still there or ECO is on */
            if((CyBle_GetBleSsState() == CYBLE_BLESS_STATE_ECO_ON) || 
               (CyBle_GetBleSsState() == CYBLE_BLESS_STATE_DEEPSLEEP))
            {
            #if (DEBUG_UART_ENABLED == ENABLED)
                /* Put the CPU into the Deep-Sleep mode when all debug information has been sent */
                if((UART_433_SpiUartGetTxBufferSize() + UART_433_GET_TX_FIFO_SR_VALID) == 0u)
                {
                    CySysPmDeepSleep();
                }
                else /* Put the CPU into Sleep mode and let SCB to continue sending debug data */
                {
                    CySysPmSleep();
                }
            #else
                CySysPmDeepSleep();
            #endif /* (DEBUG_UART_ENABLED == ENABLED) */
            }
        }
        else /* When BLE subsystem has been put into Sleep mode or is active */
        {
            /* And hardware doesn't finish Tx/Rx opeation - put the CPU into Sleep mode */
            if(CyBle_GetBleSsState() != CYBLE_BLESS_STATE_EVENT_CLOSE)
            {
                CySysPmSleep();
            }
        }
        /* Enable global interrupt */
        CyExitCriticalSection(interruptStatus);
    }
}

//********************************************************************************************************
// Function name	: ble_heartbeat
// Description		: 蓝牙的心跳包
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void ble_heartbeat(void)
{
	WDT_FeedDog();
	CyBle_ProcessEvents();
    
    #if 1
    if (BleCmdValidFlag == 1)
    {
		Protocol_CmdParse_BLE();
    }
    #endif
}

//********************************************************************************************************
// Function name	: sys_heartbeat
// Description		: 系统的心跳包
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void sys_heartbeat(void)
{
    uint16 i;
    
	WDT_FeedDog();
    for (i = 0; i < 50; i++)
    {
	    CyBle_ProcessEvents();
	}
    
    #if 1
    if (BleCmdValidFlag == 1)
    {
		Protocol_CmdParse_BLE();
    }
    #endif
    
    if ((gprs_state != NORMAL_COMM) && (gprs_state != UPDATE_CONNECT) &&
        (gprs_state != WAIT_UPDATE) && (gprs_state != UPDATE_FINISH))
    {
		CARD_Tick();
        Defend_Tick();
    }
}

//********************************************************************************************************
// Function name	: delay_with_ble
// Description		: 带蓝牙心跳的延时函数
// Parameter		: ms	: 延时时间
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void delay_with_ble(uint32 ms)
{
	uint32 loop = 0;
	for (loop = 0; loop < ms / 5; loop++)
	{
		CyDelay(5);
		ble_heartbeat();
	}
}

//********************************************************************************************************
// Function name	: Led_AllOff
// Description		: 关闭所有LED灯
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Led_AllOff(void)
{
    LED_RED_Write(1);
    LED_BLUE_Write(1);
    LED_GREEN_Write(1);
}

//********************************************************************************************************
// Function name	: Led_RedON
// Description		: 红灯亮
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Led_RedON(void)
{
    Led_AllOff();
	LED_RED_Write(0);
}

//********************************************************************************************************
// Function name	: Led_GreenON
// Description		: 绿灯亮
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Led_GreenON(void)
{
    Led_AllOff();
	LED_GREEN_Write(0);
}

//********************************************************************************************************
// Function name	: Led_BlueON
// Description		: 蓝灯亮
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Led_BlueON(void)
{
    Led_AllOff();
	LED_BLUE_Write(0);
}

//********************************************************************************************************
// Function name	: Led_SetState
// Description		: 设置LED的状态
// Parameter		: Sta	: 待设置的状态
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Led_SetState(uint8 Sta)
{
	if (Sta == 0)
	{
		Led_AllOff();
	}
	else if (Sta == 1)
	{
		Led_RedON();
	}
	else if (Sta == 2)
	{
		Led_GreenON();
	}
	else if (Sta == 3)
	{
		Led_BlueON();
	}
}

//********************************************************************************************************
// Function name	: DoorOpen
// Description		: 门打开
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void DoorOpen(void)
{
    DOOR_OPEN_Write(1);
}

//********************************************************************************************************
// Function name	: DoorClose
// Description		: 门关闭
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void DoorClose(void)
{
    DOOR_OPEN_Write(0);
}

//********************************************************************************************************
// Function name	: DoorClose
// Description		: 门磁检测
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8 GetDoorState(void)
{
	uint8 ret = 0;
	
	if(0 == MG_STATE_Read())
		ret = DoorClosed; 
	else
		ret = DoorOpened;
	
	return ret;	
}

//********************************************************************************************************
// Function name	: GPRS_SetON
// Description		: GPRS PowerKey On
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: GPRS模组开机操作
//********************************************************************************************************
void GPRS_SetON(void)
{
	uint16 count = 0;
	
	if (GPRS_STATE_Read() == 1)
	{	// 如果是关机状态 执行开机
		CyDelay(30);
		GPRS_PWRKEY_Write(1);
		while ((GPRS_STATE_Read() == 1) && (count <= 30))
		{
			delay_with_ble(100);
			count++;
		}
		GPRS_PWRKEY_Write(0);
	}
}

//********************************************************************************************************
// Function name	: GPRSPowerOn
// Description		: GPRS上电
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void GPRSPowerOn(void)
{
	uint8 count = 0;
	
	if (GPRS_STATE_Read() == 1)
	{	// 如果是关机状态
		CyDelay(30);
		GPRS_PWRKEY_Write(1);
		while ((GPRS_STATE_Read() == 1) && (count <= 30))
		{
			delay_with_ble(100);
			count++;
		}
		GPRS_PWRKEY_Write(0);
		GPRS_WaitInitFinish();
		delay_with_ble(3000);						// 等待3s
		
		GPRS_QuerySimCard();
		delay_with_ble(4000);						// 等待网络注册 连接
		wait_timeout = 30000;						// 等待超时 30s
		while(wait_timeout > 0)
		{
			if(TRUE == GPRS_QueryCREG())			// 查询网络注册情况 AT+CREG? 
			{
				break;		
			}
			else
			{
				delay_with_ble(1500);	
			}
		}
		delay_with_ble(500);
		if(0 == GPRS_QueryRssi())					// 查询信号情况 AT+CSQ 不查没关系，除非是排除0 
		{
		}		
		GPRS_SetAPN();								// 设置APN情况 AT+CGDCONT=1,"IP","CMNET" 
		delay_with_ble(500);	
		GPRS_SetActive();							// 激活PDP上下文 AT+CGACT=1,1  (6311-R开启)
	}
}

//********************************************************************************************************
// Function name	: GPRSPowerOn
// Description		: GPRS上电
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void GPRSPowerOn_0321(void)
{
	GPRS_SetON();
	
	cmd433_INT_ISR_StartEx(RX_IRQ_INTERRUPT);						// 开启串口接收中断					//
    UART_433_Start();												// 串口初始化						//
	
	GPRS_WaitInitFinish();
	//delay_with_ble(3000);						// 等待3s
	delay_with_ble(2000);						// 等待3s
	GPRS_QuerySimCard();
	delay_with_ble(4000);						// 等待网络注册 连接
	wait_timeout = 30000;						// 等待超时 30s
	while(wait_timeout > 0)
	{
		if(TRUE == GPRS_QueryCREG())			// 查询网络注册情况 AT+CREG? 
		{
			break;		
		}
		else
		{
			delay_with_ble(1500);	
		}
	}
	delay_with_ble(500);
	if(0 == GPRS_QueryRssi())					// 查询信号情况 AT+CSQ 不查没关系，除非是排除0 
	{
	}		
	GPRS_SetAPN();								// 设置APN情况 AT+CGDCONT=1,"IP","CMNET" 
	delay_with_ble(500);	
	GPRS_SetActive();							// 激活PDP上下文 AT+CGACT=1,1  (6311-R开启)

}


//********************************************************************************************************
// Function name	: GPRSPowerDown
// Description		: GPRS掉电
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void GPRSPowerDown(void)
{
	uint8 count = 0;
	
	if (GPRS_STATE_Read() == 0)
	{
		GPRS_PWRKEY_Write(1);
		while ((GPRS_STATE_Read() == 0) && (count <= 30))
		{
			delay_with_ble(100);
			count++;
		}
		GPRS_PWRKEY_Write(0);
	}
    delay_with_ble(10000);
}

//********************************************************************************************************
// Function name	: ResetGPRS
// Description		: 复位GPRS模块
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void ResetGPRS(void)
{
    GPRS_RESET_Write(1);
	delay_with_ble(25);
	GPRS_RESET_Write(0);
}

//********************************************************************************************************
// Function name	: WaitTimeOut
// Description		: 通用等待超时时间
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void WaitTimeOut(void)
{				
	if (wait_timeout > 0)
    {
        wait_timeout--;
    }
}

//********************************************************************************************************
// Function name	: CardCheckTimeOut
// Description		: 刷卡计时
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void CardCheckTimeOut(void)
{				
	if (CardCheck_CountTime < 10000)
    {
        CardCheck_CountTime++;
    }
}


//********************************************************************************************************
// Function name	: CardCheckTimeOut
// Description		: 刷卡计时
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: if (CardCheck_CountTime > 1500)
//********************************************************************************************************
void Defendalarm_TimeOut(void)
{				
	if (defendalarm_time < 10000)
    {
        defendalarm_time++;
    }
}

//********************************************************************************************************
// Function name	: MgCheck_Count
// Description		: 门磁检测计时
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void MgCheck_Count(void)
{				
	if (MgCheck_time < 1000000)
    {
        MgCheck_time++;
    }
}


//********************************************************************************************************
// Function name	: FireCheck_Count
// Description		: 火警上报
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void FireCheck_Count(void)
{				
	if (FireCheck_time < 1000000)
    {
        FireCheck_time++;
    }
}


//********************************************************************************************************
// Function name	: RecvTimeOut
// Description		: GPRS收到包头后，到整个包收完的超时时间
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void RecvTimeOut(void)
{				
	if (recv_timeout > 0)
    {
        recv_timeout--;
    }
}

//********************************************************************************************************
// Function name	: UpdateTimeOut
// Description		: 在线升级超时时间
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void UpdateTimeOut(void)
{				
	if (update_timeout_time > 0)
    {
        update_timeout_time--;
    }
}

//********************************************************************************************************
// Function name	: Reboot_Check
// Description		: 重启检测
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Reboot_Check(void)
{
    if ((flag_reboot == 1) && (stop_reboot == 0))
    {
        flag_reboot = 0;
        CySoftwareReset();
    }
}

//********************************************************************************************************
// Function name	: Updatealarm_check
// Description		: 重启检测
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 查看是否升级记录 有，检查是否升级成功 若失败上报失败信息
//********************************************************************************************************
void Updatealarm_check(void)
{
	uint8 tmp[3] = {0};
	uint8 flag = 0;
	uint16 firmver;
	
	if ((gprs_state == NORMAL_COMM)&&(update_report == 0))
	{
		DBG_SOFT_PutStr("\npolling update info:\n");
		update_report = 1;
		W25QXX_Read((uint8 *)&tmp, UPDATE_SYSTEMVER, 3);
		flag = tmp[2];
		DBG_SOFT_PrintfHex(tmp,3,16);
		if(flag == 1)
		{
			firmver = (tmp[0] <<8) | tmp[1];
			if(firmver == SYSTEM_VERSION)
			{
				//上报报警 升级ok信息
				EnterGuide_State = ALARM_UpdateOk;
			}
			else if(firmver > SYSTEM_VERSION)
			{
				//上报报警 升级ERR信息
				EnterGuide_State = ALARM_UpdateErr;
			
			}
			send_alarm_info(EnterGuide_State);						// 升级是否成功报警					//	
			
			flag =  0;
			firmver = SYSTEM_VERSION;
			Write_UpdateInfo(flag, firmver);						// 清空 还原升级信息				//
			
			FireCheck_time = 0;										// 防止火警马上上报					//
		}	
 	}
}

//********************************************************************************************************
// Function name	: BLE_Tick
// Description		: 蓝牙的处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void BLE_Tick(void)
{
	ble_heartbeat();
	if(CyBle_GetState() == CYBLE_STATE_CONNECTED)
	{
		if (BleCmdValidFlag == 1)
        {					
			Protocol_CmdParse();
			BleCmdValidFlag = 0;
			DBG_PRINTF("BLE handle cmd done.\n");
		}
	}
}

//********************************************************************************************************
// Function name	: GPRS_Tick
// Description		: GPRS的处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void GPRS_Tick(void)
{
	uint8 ret;
	uint32 count = 0;
	
	if ((GPRSCmdParseFlag1 == 1) || (GPRSCmdParseFlag2 == 1))		// 服务器发出的命令					//
	{
		Protocol_CmdParse();
	}
	
	if (update_flag == 1)
	{
		update_flag = 0;
		gprs_state = NEED_UPDATE;									// 开始断开当前前置服务器连接		//
	}
	
	//--实时检测STATUS管脚状态，如果是关机状态 立即执行重启开机操作
	if (GPRS_STATE_Read() == 1)
	{	
        #if 0
		GPRS_PWRKEY_Write(1);
		while ((GPRS_STATE_Read() == 0) && (count <= 30))
		{
			delay_with_ble(100);
			count++;
		}
		GPRS_PWRKEY_Write(0);
		#endif
		flag_reboot = 1;
        Reboot_Check();
		
	}
	//-------------------------------------------------------------
	
	cur_time = (uint32)RTC_GetUnixTime();
	if (gprs_state == INIT)
	{
		LED_State = RED;
		Led_SetState(LED_State);
		if (cur_time > next_gprs_operation_time)
		{
			if (gprs_operation_ErrCount >= gprs_resetCount)						// 中心服务器3次连不上，则复位模块	//
			{
				gprs_state = NEED_INIT;
			}
			else
			{
				next_gprs_operation_time = cur_time + 5;                        // 5s一次尝试连接服务器
				gprs_operation_ErrCount++;
				ret = GPRS_ConnectCenterServer();
				if (ret == TRUE)
				{
					gprs_state = CEN_CONNECT;
					next_gprs_operation_time = cur_time;
					gprs_operation_ErrCount = 0;
				}
			}
		}
	}
	else if (gprs_state == CEN_CONNECT)
	{
		LED_State = RED;
		Led_SetState(LED_State);
		if (cur_time > next_gprs_operation_time)
		{
			if (gprs_operation_ErrCount > gprs_resetCount)
			{
				gprs_state = NEED_INIT;
			}
			else
			{
				next_gprs_operation_time = cur_time + 20;
				gprs_operation_ErrCount++;
				send_get_pre_service_ip();
			}
		}
	}
	else if (gprs_state == GET_PRE_IP)
	{
		LED_State = RED;
		Led_SetState(LED_State);
		if (cur_time > next_gprs_operation_time)
		{
			if (gprs_operation_ErrCount > gprs_resetCount)
			{
				gprs_state = NEED_INIT;
			}
			else
			{
				next_gprs_operation_time = cur_time + 5;
				gprs_operation_ErrCount++;
				ret =  GPRS_ClosedConnect();
				if (ret == TRUE)
				{
					gprs_state = CEN_CLOSED;
					next_gprs_operation_time = cur_time;
					gprs_operation_ErrCount = 0;
				}
			}
		}
	}
	else if (gprs_state == CEN_CLOSED)
	{
		LED_State = RED;
		Led_SetState(LED_State);
		if (gprs_operation_ErrCount > 3)
		{
			gprs_state = INIT;
		}
		else if (cur_time > next_gprs_operation_time)
		{
			next_gprs_operation_time = cur_time + 5;
			gprs_operation_ErrCount++;
			ret = GPRS_ConnectPreServer();
			if (ret == TRUE)
			{
				gprs_state = PRE_CONNECT;
				next_gprs_operation_time = cur_time;
				gprs_operation_ErrCount = 0;
			}
		}
	}
	else if (gprs_state == PRE_CONNECT)
	{
		LED_State = RED;
		Led_SetState(LED_State);
		if (gprs_operation_ErrCount > 3)
		{
			gprs_state = INIT;
		}
		else if (cur_time > next_gprs_operation_time)
		{
			next_gprs_operation_time = cur_time + 20;
			gprs_operation_ErrCount++;
            send_send_mac_and_get_blekey();
            #if 0
			if (SendMacFlag == 0)
			{
				send_send_mac_and_get_blekey();
			}
			else
			{
				gprs_state = NORMAL_COMM;
			}
            #endif
		}
	}
    #if 0
	else if (gprs_state == NORMAL_COMM)
	{
		LED_State = GREEN;
		Led_SetState(LED_State);
		if (cur_time > next_heartbeat_time)
		{
			EnterGuide_State = DoorClosed;
			send_heartbeat(EnterGuide_State);
			heartbeat_SendCount++;
			if (heartbeat_SendCount > 1)
			{
				gprs_state = NEED_INIT;
			}
			else
			{
				cur_time = (uint32)RTC_GetUnixTime();
				next_heartbeat_time = cur_time + 300;
			}
		}
		GPRS_CheckIfRecvClosed();									// 检查网络是否断开，并设置相关状态	//
	}
    #endif
    #if 1
    else if (gprs_state == NORMAL_COMM)
	{
		LED_State = GREEN;
		Led_SetState(LED_State);
        
        if (cur_time > heartbeat_lasttime + 60)                 // 超过一分钟 检测一次回复情况
        {
            if (heartbeat_SendCount > 0)                        // 没有回复
            {
                //EnterGuide_State = DoorClosed;
				EnterGuide_State = GetDoorState();
                send_heartbeat(EnterGuide_State);
                heartbeat_SendCount++;
                cur_time = (uint32)RTC_GetUnixTime();
                heartbeat_lasttime = cur_time;
                next_heartbeat_time = cur_time + 300;
                if (heartbeat_SendCount > 3)
    			{
    				gprs_state = NEED_INIT;
    			}
            }
                  
    		if (cur_time > next_heartbeat_time)
    		{
    			//EnterGuide_State = DoorClosed;
				EnterGuide_State = GetDoorState();
                send_heartbeat(EnterGuide_State);
    			heartbeat_SendCount++;
    			cur_time = (uint32)RTC_GetUnixTime();
                heartbeat_lasttime = cur_time;
    			next_heartbeat_time = cur_time + 300;
    		}
            
        }
		GPRS_CheckIfRecvClosed();									// 检查网络是否断开，并设置相关状态	//
	}
    #endif
    
	else if (gprs_state == NEED_INIT)
	{
		LED_State = RED;
		Led_SetState(LED_State);
		GPRS_ClosedConnect();										// 重启各设备之前 关闭连接 释放资源
        GPRSPowerDown();										    // 开 关机操作
       
        #if 0
        reboot_count = Read_RebootType();
        if ((reboot_count == 0xFF) || (reboot_count == 0x00))
        {
            GPRS_ClosedConnect();
            ResetGPRS();
            reboot_count = 1;
        }
        else if (reboot_count == 2)
        {
            GPRSPowerDown();										// 开 关机操作
            reboot_count = 0;
        }
        else
        {
            
            ResetGPRS();
            reboot_count = 2;
        }
        Write_RebootType(reboot_count);
        #endif
        gprs_state = INIT;
        flag_reboot = 1;
        Reboot_Check();
	}
    
	else if (gprs_state == NEED_UPDATE)
	{
		LED_State = RED;
		Led_SetState(LED_State);
		if (cur_time > next_gprs_operation_time)
		{
			if (gprs_operation_ErrCount > gprs_resetCount)
			{
				gprs_state = NEED_INIT;
			}
			else
			{
				next_gprs_operation_time = cur_time + 5;
				gprs_operation_ErrCount++;
				ret =  GPRS_ClosedConnect();
				if (ret == TRUE)
				{
					gprs_state = PRE_CLOSED;
					next_gprs_operation_time = cur_time;
					gprs_operation_ErrCount = 0;
				}
			}
		}
	}
	else if (gprs_state == PRE_CLOSED)
	{
		LED_State = RED;
		Led_SetState(LED_State);
		if (gprs_operation_ErrCount > 3)
		{
			gprs_state = INIT;
		}
		else if (cur_time > next_gprs_operation_time)
		{
			next_gprs_operation_time = cur_time + 10;
			gprs_operation_ErrCount++;
			ret = GPRS_ConnectUpdateServer();
			if (ret == TRUE)
			{
				gprs_state = UPDATE_CONNECT;
				next_gprs_operation_time = cur_time;
				gprs_operation_ErrCount = 0;
			}
		}
	}
	else if (gprs_state == UPDATE_CONNECT)
	{
		LED_State = BLUE;
		Led_SetState(LED_State);
		Cmd_Protocol_ParaInit();
		update_page = 1;
		send_update_req(update_page);
		update_timeout_time = 20000;
		update_timeout_count = 0;
		gprs_state = WAIT_UPDATE;
	}
	else if (gprs_state == WAIT_UPDATE)
	{
		if (update_timeout_time == 0)
		{
			update_timeout_count++;
			if (update_timeout_count >= gprs_resetCount)
			{
				update_timeout_count = 0;
				GPRS_ClosedConnect();							// 下载升级包超时 关闭连接退出升级
				gprs_state = INIT;
			}
			else
			{
				Cmd_Protocol_ParaInit();
				send_update_req(update_page);
				update_timeout_time = 10000;
			}
		}
	}
	else if (gprs_state == UPDATE_FINISH)
	{
		GPRS_ClosedConnect();									// 下载升级包超完成 关闭连接 释放资源
		stop_reboot = 0x01;
		Bootloadable_Load();
	}
}


//********************************************************************************************************
// Function name	: CARD_Tick
// Description		: 刷卡验证并处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#if card_test
void CARD_Tick(void)
{
	uint8 i;
	uint8 ret;
	uint8 card_id[8] = {0};
	uint8 arr_time[4];
	uint32 cur_time;
	user_info_t user_info;
    open_door_t openInfo_card;
	uint8 ic_buf[64] = {0x01,0x02,0x03,0x04,0x05};
	
	uint8_t statues;
    uint8_t picc_atqa[2];                                               
    //static uint8_t picc_uid[15];                                        
    //uint8_t picc_sak[3];                                                
    uint8_t key[6]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};       				// 不同的卡是不是不一样             
	uint8_t key_b04[6]= {0xff,0xff,0xfe,0xef,0xfa,0xaf};       			// block4-5 的加密密钥
    
	uint8_t blockAddr;
    uint8_t blockData[16];
    uint32_t rec_len;
    uint8_t pupi[4];
    uint8_t buff[12];
	
	
	//==if(RET_ERR == Read_nfc_card2(card_id))
	//==	return;
	
	FM175X_SoftReset( );  
	Set_Rf( 3 );                                                /* 打开双天线                  */
	Pcd_ConfigISOType( 0 );                                     /* ISO14443寄存器初始化        */
	statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );  /* 激活A卡片                   */
	if ( statues == TRUE ) 
	{
		DBG_SOFT_PutStr("\nCardUID:");
		DBG_SOFT_PrintfHex(picc_uid,8,16);
		memcpy(card_id, picc_uid, 4);

	}
	else 
	{
		//DBG_SOFT_PutStr("No Card!\r\n");
		TypeA_Halt( 0 );  	 					 				/* 卡片睡眠 					*/
		return;
	}
	
#if 0	
	for(i = 0; i < 16; i++)										// 先读取16扇区数据
	{
		// 先读0区块
		blockAddr = i;
		if((i >= 4)&&(i <= 7))
		{
			statues = Mifare_Auth(0x00,blockAddr>>2,key_b04,picc_uid);      /* 密钥认证                    */
		}
		else
			statues = Mifare_Auth(0x00,blockAddr>>2,key,picc_uid);      	/* 密钥认证                    */
		
		if(statues  == TRUE)
		{
			DBG_SOFT_PutStr("Auth OK!\r\n");
		}
		else
		{
			DBG_SOFT_PutStr("Auth Err!\r\n");
			return ;
		}	
		statues = Mifare_Blockread(blockAddr,blockData);            /* 读块               		   */
		if(statues  == TRUE)
		{
			DBG_SOFT_PutStr("Read OK @Block ");
			DBG_SOFT_PrintfHex(&blockAddr,1,16);
			DBG_SOFT_PrintfHex(blockData,16,16);
		}
		else
		{
			DBG_SOFT_PutStr("Read Err!\r\n");
			return ;
		}
		
		if(i == 0)
		{
			memcpy(card_id,blockData,4);
		}
	}	

	if(1 == IsCardLegality(card_id))
	{
		DBG_SOFT_PutStr("IsCardLegality OK!\n");
	}
	else 
		DBG_SOFT_PutStr("IsCardLegality ERR!\n");
	
	TypeA_Halt( 0 );  	 					 					// 卡片睡眠 					
#endif
	CardBlock_Rd(5,blockData);
	DBG_SOFT_PutStr("\nCardBlock_Rd data @ Block 5:\n");
	DBG_SOFT_PrintfHex(blockData,16,16);

	if(1 == IsCardLegality(card_id))
	{
		DBG_SOFT_PutStr("\nIsCardLegality ok!\n");				// 合法性验证
	}
	
	CardInfoUpdate();
	

	#if 0
	blockAddr = 0x04;
	statues = Mifare_Blockwrite(blockAddr,blockData);			/* 写块	 				   */
	if(statues  == TRUE)
	{
		DBG_SOFT_PutStr("Write OK!\r\n");
	}
	else
	{
		DBG_SOFT_PutStr("Write Err!\r\n");
		return ;
	}
	#endif
	#if 0
	// 写完后 再读出来
	statues = Mifare_Blockread(blockAddr,blockData);            /* 再读块  01             		   */
	if(statues  == TRUE)
	{
		DBG_SOFT_PutStr("Read OK!\r\n");
		DBG_SOFT_PrintfHex(blockData,16,16);
	}
	else
	{
		DBG_SOFT_PutStr("Read Err!\r\n");
		return ;
	}
	#endif
	
	#if 0
	for(i = 0; i < 64; i++)
	{
		ic_buf[i]  = i;
	}
	if(TRUE == Mifare_Blockwrite(3,ic_buf))
	{
		DBG_SOFT_PutStr("\nWrite card Block OK!\n");
	}
	else
	{
		DBG_SOFT_PutStr("\nWrite card Block Failed!\n");
	}
	
	//如果读到卡 取出卡片的64个块的数据 打印
	for(i = 0; i < 64; i++)
	{
		if(TRUE == Mifare_Blockread(i, ic_buf))
		{
			#if DBG_SOFT_ENNABLE
			//memset(cur_readuid, 0, 8);
			//memcpy(card_id,card_id,8);
			DBG_SOFT_PutStr("\nCur Read card Block @i =");
			DBG_SOFT_PrintfHex(&i,1,16);
			DBG_SOFT_PrintfHex(ic_buf,16,16);
			#endif	
		}
		else
		{
			DBG_SOFT_PutStr("\nRead card Block Failed!\n");
		}
	}
	#endif
	
	for (i = 0; i < 2; i++)
	{
	    Led_AllOff();
		delay_with_ble(100);
		Led_BlueON();
		delay_with_ble(100);
	}
	
	cur_time = (uint32)RTC_GetUnixTime();
	LONG2BYTE(cur_time, arr_time);
	memcpy(user_info.card_uid, card_id, CARD_UID_LEN);
	memcpy(user_info.valid_time, arr_time, 4);
	
	if (1 == UserData_Judge_If_Open(&user_info))
	{
		Led_GreenON();
		DoorOpen();
		VoicePlay((uint8)Sound_VERIFY_SUCCESS);
		
		memset(&openInfo_card, 0, sizeof(open_door_t));
		memcpy(openInfo_card.card_uid, card_id, CARD_UID_LEN);
		memcpy(openInfo_card.open_time, arr_time, 4);
		memset(arr_time, 0, 4);
		memcpy(openInfo_card.send_time, arr_time, 4);
		Write_OpenInfo(&openInfo_card, CARD_OPEN);
		
		if (gprs_state == NORMAL_COMM)
		{
			cur_time = (uint32)RTC_GetUnixTime();
			if(cur_time > next_allupload_time)
			{
				next_allupload_time = cur_time + 15;
				send_upload_open_info(&openInfo_card, CARD_UID_LEN, CARD_OPEN);				
			}						
		}
		
		DoorClose();
	}
	else
	{
		Card_ErrCount++;
		Led_RedON();
		VoicePlay(Sound_VERIFY_FAILURE);
		delay_with_ble(800);
	}
	Led_SetState(LED_State);										// LED的状态切回刷卡前的状态		//
	
Err_Check:
	if (Card_ErrCount >= 5)
	{
		Card_ErrCount = 0;
		if (gprs_state == NORMAL_COMM)
		{
			EnterGuide_State = ALARM_ErrOpenQty;
			send_alarm_info(EnterGuide_State);						// 开门错误5次报警					//
		}
	}
}
#endif


#if card_scan //扫描scan
//********************************************************************************************************
// Function name	: CARD_Tick
// Description		: 刷卡验证并处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info	
#if 0
time_tmp = (uint32)RTC_GetUnixTime();
	if(time_tmp >= last_readcardtime + 2)
	{	
		last_readcardtime = time_tmp;
		#endif
//********************************************************************************************************
void CARD_Tick(void)
{
	uint8 i;

	uint8 card_id[8] = {0};
	uint8 arr_time[4];
	uint32 time_tmp;
	user_info_t user_info;
    open_door_t openInfo_card;
	
    if (CardCheck_CountTime > 1500)
    {
		CardCheck_CountTime = 0;
		if(0 == CardScan(card_id))							// 先读A卡
		{
			if(0 == nfc_detect_type_b(card_id, 8))			// 未读到 再读B卡
			{
				return ;
			}	
		}
		
		for (i = 0; i < 2; i++)
		{
		    Led_AllOff();
			delay_with_ble(100);
			Led_BlueON();
			delay_with_ble(100);
		}

		DBG_SOFT_PutStr("\nGet Card UID:");	
		DBG_SOFT_PrintfHex(card_id,8,16);
		
		time_tmp = (uint32)RTC_GetUnixTime();
		LONG2BYTE(time_tmp, arr_time);
		memcpy(user_info.card_uid, card_id, CARD_UID_LEN);
		memcpy(user_info.valid_time, arr_time, 4);
		
		if (1 == UserData_Judge_If_Open(&user_info))
		{
			Led_GreenON();
			DoorOpen();
			VoicePlay((uint8)Sound_VERIFY_SUCCESS);
			
			time_tmp = (uint32)RTC_GetUnixTime();
			if (time_tmp >= 1483203600)
			{
				memset(&openInfo_card, 0, sizeof(open_door_t));
				memcpy(openInfo_card.card_uid, card_id, CARD_UID_LEN);
				memcpy(openInfo_card.open_time, arr_time, 4);
				memset(arr_time, 0, 4);
				memcpy(openInfo_card.send_time, arr_time, 4);
				Write_OpenInfo(&openInfo_card, CARD_OPEN);
				
				if (gprs_state == NORMAL_COMM)
				{
					time_tmp = (uint32)RTC_GetUnixTime();
					if(time_tmp > next_allupload_time)
					{
						next_allupload_time = time_tmp + 15;
						send_upload_open_info(&openInfo_card, CARD_UID_LEN, CARD_OPEN);				
					}
				}
			}
			delay_with_ble(1000);
			DoorClose();
		}
		else
		{
			Card_ErrCount++;
			Led_RedON();
			VoicePlay(Sound_VERIFY_FAILURE);
			delay_with_ble(800);
		}
		Led_SetState(LED_State);										// LED的状态切回刷卡前的状态		//
		
	Err_Check:
		if (Card_ErrCount >= 5)
		{
			Card_ErrCount = 0;
			if (gprs_state == NORMAL_COMM)
			{
				EnterGuide_State = ALARM_ErrOpenQty;
				send_alarm_info(EnterGuide_State);						// 开门错误5次报警					//
			}
		}
		
	}
}
#endif


#if card_normal //原功能
//********************************************************************************************************
// Function name	: CARD_Tick
// Description		: 刷卡验证并处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void CARD_Tick(void)
{
	uint8 i;
	uint8 ret;
	uint8 card_id[8];
	uint8 arr_time[4];
	uint32 cur_time;
	user_info_t user_info;
    open_door_t openInfo_card;
	
	#if 0
	if ((ret = detect_nfc_card(card_id, CARD_UID_LEN)) != RET_OK)
	{
		goto Err_Check;
	}
	
	// 滤除不准确的卡ID
	if((card_id[0] == 0) && (card_id[1] == 0) &&(card_id[2] == 0) && (card_id[3] == 0))
	{
		return;
	}
	#endif
	
	if(RET_ERR == Read_nfc_card2(card_id))
		return;
	
		#if DBG_SOFT_ENNABLE
		//memset(cur_readuid, 0, 8);
		//memcpy(card_id,card_id,8);
		DBG_SOFT_PutStr("\nCur Read card UID:\n");
		DBG_SOFT_PrintfHex(card_id,8,16);
		#endif	
	
	for (i = 0; i < 2; i++)
	{
	    Led_AllOff();
		delay_with_ble(100);
		Led_BlueON();
		delay_with_ble(100);
	}
	
	cur_time = (uint32)RTC_GetUnixTime();
	LONG2BYTE(cur_time, arr_time);
	memcpy(user_info.card_uid, card_id, CARD_UID_LEN);
	memcpy(user_info.valid_time, arr_time, 4);
	
	if (1 == UserData_Judge_If_Open(&user_info))
	{
		Led_GreenON();
		DoorOpen();
		VoicePlay((uint8)Sound_VERIFY_SUCCESS);
		
		memset(&openInfo_card, 0, sizeof(open_door_t));
		memcpy(openInfo_card.card_uid, card_id, CARD_UID_LEN);
		memcpy(openInfo_card.open_time, arr_time, 4);
		memset(arr_time, 0, 4);
		memcpy(openInfo_card.send_time, arr_time, 4);
		Write_OpenInfo(&openInfo_card, CARD_OPEN);
		
		if (gprs_state == NORMAL_COMM)
		{
			cur_time = (uint32)RTC_GetUnixTime();
			if(cur_time > next_allupload_time)
			{
				next_allupload_time = cur_time + 15;
				send_upload_open_info(&openInfo_card, CARD_UID_LEN, CARD_OPEN);				
			}						
		}
		
		DoorClose();
	}
	else
	{
		Card_ErrCount++;
		Led_RedON();
		VoicePlay(Sound_VERIFY_FAILURE);
		delay_with_ble(800);
	}
	Led_SetState(LED_State);										// LED的状态切回刷卡前的状态		//
	
Err_Check:
	if (Card_ErrCount >= 5)
	{
		Card_ErrCount = 0;
		if (gprs_state == NORMAL_COMM)
		{
			EnterGuide_State = ALARM_ErrOpenQty;
			send_alarm_info(EnterGuide_State);						// 开门错误5次报警					//
		}
	}
}
#endif

//********************************************************************************************************
// Function name	: KeyOpen_Tick
// Description		: 机械开门的处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void KeyOpen_Tick(void)
{
	uint8 card_id[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x02};
	uint8 arr_time[4];
	uint32 time_tmp;
    open_door_t openInfo_Key;
	
	
	//if ((0 == KEY_OPEN_Read()) && (cur_time > next_gprs_upload_time))
	if (0 == KEY_OPEN_Read())
	{
		delay_with_ble(5);
		if (0 == KEY_OPEN_Read())
		{
			Led_GreenON();
			DoorOpen();
			VoicePlay((uint8)Sound_VERIFY_SUCCESS);
			
			time_tmp = (uint32)RTC_GetUnixTime();
			if(time_tmp >= 1483203600)
			{
				LONG2BYTE(time_tmp, arr_time);
				memset(&openInfo_Key, 0, sizeof(open_door_t));
				memcpy(openInfo_Key.card_uid, card_id, CARD_UID_LEN);
				memcpy(openInfo_Key.open_time, arr_time, 4);
				memset(arr_time, 0, 4);
				memcpy(openInfo_Key.send_time, arr_time, 4);
				Write_OpenInfo(&openInfo_Key, HAND_OPEN);
				
				if (gprs_state == NORMAL_COMM)
				{
					time_tmp = (uint32)RTC_GetUnixTime();
					if (time_tmp > next_allupload_time)
					{
						next_allupload_time = time_tmp + 15;
						send_upload_open_info(&openInfo_Key, CARD_UID_LEN, HAND_OPEN);
					}						
				}
			}
			delay_with_ble(1000);
			DoorClose();
	        Led_SetState(LED_State);
		}
	}
}

#if 0
//********************************************************************************************************
// Function name	: FireOpen_Tick
// Description		: 火警开门的处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void FireOpen_Tick(void)
{
	uint8 card_id[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01};
	uint8 arr_time[4];
	uint32 time_tmp;
    open_door_t openInfo_FIRE;
	
	//if ((0 == FIRE_OPEN_Read()) && (cur_time > next_gprs_upload_time))
	if (0 == FIRE_OPEN_Read())
	{
		delay_with_ble(20);
		if (0 == FIRE_OPEN_Read())
		{
			Led_GreenON();
			DoorOpen();
			VoicePlay((uint8)Sound_VERIFY_SUCCESS);
	        	
			time_tmp = (uint32)RTC_GetUnixTime();
			if(time_tmp >= 1483203600)
			{
				memset(&openInfo_FIRE, 0, sizeof(open_door_t));
				memcpy(openInfo_FIRE.card_uid, card_id, CARD_UID_LEN);
				memcpy(openInfo_FIRE.open_time, arr_time, 4);
				memset(arr_time, 0, 4);
				memcpy(openInfo_FIRE.send_time, arr_time, 4);
				Write_OpenInfo(&openInfo_FIRE, FIRE_OPEN);
				
				if (gprs_state == NORMAL_COMM)
				{
					time_tmp = (uint32)RTC_GetUnixTime();
					if(time_tmp > next_allupload_time)
					{
						next_allupload_time = time_tmp + 15;
						send_upload_open_info(&openInfo_FIRE, CARD_UID_LEN, FIRE_OPEN);
						firealarm_flag = 1;												 // 开启1min一次火警上报功能 不清除
					}						
				}
			}
			delay_with_ble(1000);
			DoorClose();
			Led_SetState(LED_State);								// LED的状态切回刷卡前的状态		//
		}
	}
}
#endif

#if FireAlarm_Open
//********************************************************************************************************
// Function name	: FireAlarm_Tick
// Description		: 火警上报功能
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 火警上报处理1min一次
//********************************************************************************************************
void FireAlarm_Tick(void)
{
	uint8 card_id[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01};
	uint8 arr_time[4];
	uint32 time_tmp;
    open_door_t openInfo_FIRE;
	
	if (0 == FIRE_OPEN_Read())
	{
		delay_with_ble(20);
		if (0 == FIRE_OPEN_Read())
		{
			DoorOpen();
			
			if (((firealarm_flag == 0) && (FireCheck_time > 5000)) ||
				((firealarm_flag == 2) && (FireCheck_time > 60000)))
			{
				time_tmp = (uint32)RTC_GetUnixTime();
				if (time_tmp > 1483203600)
				{
					if (gprs_state == NORMAL_COMM)
					{
						EnterGuide_State = ALARM_Fire;			// 火警1min上报一次     					//
						send_alarm_info(EnterGuide_State);
						FireCheck_time = 0;
						if (firealarm_flag == 0)
						{
							firealarm_flag = 1;
						}
					}
				}
			}
			
			if ((FireCheck_time > 15000) && (firealarm_flag == 1))
			{
				time_tmp = (uint32)RTC_GetUnixTime();
				if (time_tmp > 1483203600)
				{
					if (gprs_state == NORMAL_COMM)
					{
						LONG2BYTE(time_tmp, arr_time);				
						memset(&openInfo_FIRE, 0, sizeof(open_door_t));
						memcpy(openInfo_FIRE.card_uid, card_id, CARD_UID_LEN);
						memcpy(openInfo_FIRE.open_time, arr_time, 4);
						memset(arr_time, 0, 4);
						memcpy(openInfo_FIRE.send_time, arr_time, 4);
						Write_OpenInfo(&openInfo_FIRE, FIRE_OPEN);

						send_upload_open_info(&openInfo_FIRE, CARD_UID_LEN, FIRE_OPEN);
						firealarm_flag = 2;
					}
				}
 			}
		}
		else
		{
			FireCheck_time = 0;
			firealarm_flag = 0;
			DoorClose();
		}
	}
	else
	{
		FireCheck_time = 0;
		firealarm_flag = 0;
		DoorClose();
	}
}
#endif

#if 0	// 之前版本
//********************************************************************************************************
// Function name	: DefendAlarm_Tick
// Description		: 防撬报警的处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void DefendAlarm_Tick(void)
{
	//if ((0 == DEFEND_ALARM_Read()) && (cur_time > next_gprs_upload_time))
	if (0 == DEFEND_ALARM_Read())
	{
		delay_with_ble(20);
		if (0 == DEFEND_ALARM_Read())
		{
			if (gprs_state == NORMAL_COMM)
			{

				EnterGuide_State = ALARM_Destroy;
				send_alarm_info(EnterGuide_State);
				next_gprs_upload_time = cur_time + 10;
			}
		}
	}
}
#endif

#if DefendAlarm_Open
//********************************************************************************************************
// Function name	: DefendAlarm_Tick
// Description		: 防撬报警的处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 只是上报一次，装回撤销报警
//********************************************************************************************************
void DefendAlarm_Tick(void)
{
	uint32 time_tmp= 0;
	
	if (0 == DEFEND_ALARM_Read())
	{
		delay_with_ble(20);
		if (0 == DEFEND_ALARM_Read())
		{
            if ((gprs_state != UPDATE_CONNECT) && (gprs_state != WAIT_UPDATE) && 
                (gprs_state != UPDATE_FINISH))
            {
                if (defendalarm_time > 2000)
    			{
    				defendalarm_time = 0;
    				VoicePlay((uint8)Sound_DEFENDALRM);				// 若拆锁 声音一直响                //
    			}
            }
            
			time_tmp = (uint32)RTC_GetUnixTime();
			if (time_tmp > 1483203600)
			{
				if ((next_defend_flag == 0) && (gprs_state == NORMAL_COMM))
				{
					// 报一次
					next_defend_flag = 1;						    // 上报服务器只是上报一次           //
					EnterGuide_State = ALARM_Destroy;
					send_alarm_info(EnterGuide_State);
				}
			}
		}
	}
	else
	{
		next_defend_flag = 0;								        // 正常了 说明锁装回 恢复标志位     //
	}
}

//********************************************************************************************************
// Function name	: Defend_Tick
// Description		: 防撬报警在未联网时的处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 断网期间不上报
//********************************************************************************************************
void Defend_Tick(void)
{
    if (Code_Action_flag == 0)
    {
        if (0 == DEFEND_ALARM_Read())
	    {
		    delay_with_ble(20);
		    if (0 == DEFEND_ALARM_Read())
            {
                if (defendalarm_time > 2000)
    			{
    				defendalarm_time = 0;
				    VoicePlay((uint8)Sound_DEFENDALRM);				// 若拆锁 声音一直响 
                }
            }
        }
    }
}
#endif

#if MgCheck_Open
//********************************************************************************************************
// Function name	: MgCheck_Tick
// Description		: 门磁检测报警的处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 如果门一直开着，每15分钟上报一次.
//********************************************************************************************************
void MgCheck_Tick(void)
{
	uint32 time_tmp = 0;
	
	if (DoorOpened == GetDoorState())
	{
		delay_with_ble(20);
		if (DoorOpened == GetDoorState())
		{
			if (MgCheck_time > 900000)
			{
				time_tmp = (uint32)RTC_GetUnixTime();
				if (time_tmp > 1483203600)
				{
					if (gprs_state == NORMAL_COMM)
					{
						EnterGuide_State = ALARM_ErrDoorState;		// 门状态异常     					//
						send_alarm_info(EnterGuide_State);
						MgCheck_time = 0;
					}
				}
			}
		}
		else
		{
			MgCheck_time = 0;
		}
	}
	else
	{
		MgCheck_time = 0;
	}
}

#endif

#if PowerOnCheck_Open
//********************************************************************************************************
// Function name	: PowerOnCheck_Tick
// Description		: 上电报警
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 联网上报一次
//********************************************************************************************************
void PowerOnCheck_Tick(void)
{
	uint32 time_tmp;
	
	if(1 == once_poweron_flag)
	{
		time_tmp = (uint32)RTC_GetUnixTime();
		if (time_tmp > 1483203600)								// 初次上电延迟1h上报
		{
			if (gprs_state == NORMAL_COMM)						// 上电 联网才上报
			{
				EnterGuide_State = ALARM_PowerUp;				// 上电     					
				send_alarm_info(EnterGuide_State);	
				once_poweron_flag = 2;							// 锁止上报						
				FireCheck_time = 0;								// 防止火警马上上报
			}
		}
	}
}	
#endif




//********************************************************************************************************
// Function name	: UploadOpenInfo_Tick
// Description		: 开门信息轮训上报处理
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#if 1
void UploadOpenInfo_Tick(void)
{
	uint16 OpenQty = 0;
	uint8  open_flag[256] = {0};
	open_door_t repet_info;
	uint16 i;
	
	//1.定时轮询
	if (gprs_state == NORMAL_COMM)
	{
		cur_time = (uint32)RTC_GetUnixTime();
		if((cur_time > next_gprs_upload_time) && (uploadpolling_stop != 1))
		{		

			OpenQty = Read_Open_Qty();
			OpenQty %= MAX_OPEN_NUM; 
			W25QXX_Read(open_flag, (OPEN_FLAG_BASE + openinfopage_offset*256), 256);
			for(i = 0; i < 256; i++)
			{

				if((0x80 != (0x80 & open_flag[i])) && (open_flag[i] != 0) && (open_flag[i] != 0Xff))
					break;
			}
			DBG_SOFT_PutStr("\n after find loop,i = ");
			DBG_SOFT_PrintfHex(&i,1,16);		
			
			if(i == 256)
			{
				if((openinfopage_offset + 1) < (OpenQty/256 + (OpenQty%256 ? 1:0)))
					openinfopage_offset++;								//非最后一页
				else
				{
					uploadpolling_stop = 1;
					DBG_SOFT_PutStr("\npolling_stop: ");
					DBG_SOFT_PrintfHex(&uploadpolling_stop,1,16);
				}
				DBG_SOFT_PutStr("\nsend_upload_open_info loop,page_offset= ");
			DBG_SOFT_PrintfHex(&openinfopage_offset,1,16);
			}
			else
			{	
				//==W25QXX_Read((uint8 *)&repet_info, (OPEN_INFO_BASE + i*sizeof(open_door_t)), sizeof(open_door_t));	
			#if 1
			if((i == last_index) && (i != 0))
			{
				uploadflag_cnt++;
				if(uploadflag_cnt >= 5)
				{
					DBG_SOFT_PutStr("\nset opendateflag：");
					DBG_SOFT_PrintfHex((uint8 *)&uploadflag_cnt,1,16);
					Write_Open_UploadFlag(i + openinfopage_offset*256, TRUE);		// 重复超过5次 置位该位 不再报 @4.11之前没有加上页偏移
					uploadflag_cnt = 0;
				}
			}
			else
			{
				last_index = i;
			}
			#endif
				Read_OpenInfo(i, (open_door_t *)&repet_info);
				DBG_SOFT_PutStr("\n repet_info :\n");
				DBG_SOFT_PrintfHex((uint8 *)&repet_info,sizeof(open_door_t),16);
				//2.条件上报
				if (gprs_state == NORMAL_COMM)
				{	
					send_upload_open_info(&repet_info, CARD_UID_LEN, (open_flag[i] & 0x7f));
					//next_gprs_upload_time = cur_time + 10;
				}
			}
			next_gprs_upload_time = cur_time + 60;					//轮训一次就加1min间隔 不管是否上报
		}	
	}
}
#endif

//********************************************************************************************************
// Function name	: Set_DeviceMacAndName
// Description		: 设置设备的Mac和Name
// Parameter		: mac	: mac地址
//					: name	: 广播名称
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void Set_DeviceMacAndName(char8 *mac, char8 *name)
{
	uint8 i;
	uint8 local_name[13] = {0x52u, 0x4Bu, 0x4Au, 0x30u, 0x36u,0x30u, 0x31u, 0x30u, 0x31u, 0x30u, 0x31u, 0x30u, 0x31u};	
	
	if (NULL != mac)
	{
		for (i = 0; i < 6; i++) 
		{
			cyBle_deviceAddress.bdAddr[i]   	= mac[5 - i];
			cyBle_discoveryParam.directAddr[i]	= mac[5 - i];
		}	
	}
	
	if (NULL != name)
	{
		for (i = 0; i < 10; i++)
		{	
			cyBle_discoveryData.advData[8 + i] 	 = name[i];
			cyBle_scanRspData.scanRspData[5 + i] = name[i];
			local_name[3 + i] = name[i];
		}
		CyBle_GapSetLocalName((char8 *)local_name);
	}
}

//********************************************************************************************************
// Function name	: mac_conver_name
// Description		: 根据mac生成name
// Parameter		: mac	: mac地址
//					: name	: 广播名称
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
#if 0
void mac_conver_name(uint8 mac[], uint8 name[])
{
	uint8 i;
	for (i = 0; i < 4; i++)
	{
		name[9 - i*2] 	 = (mac[5 - i] % 0x10) + 0x30;
		name[9 - i*2 -1] = (mac[5 - i] / 0x10) + 0x30;
	}
}
#endif

void mac_conver_name(uint8 mac[], uint8 name[])
{
	uint8 i;
	uint8 mac_inflash[6] = {0};
	uint8 flag = 0;
	uint8 tmpmac[8] = {0};
	
	#if 0
	//0.手动写mac信息入flash的方式  省去蓝牙配置--50套 **云端更新程序要去掉这段**  
	W25QXX_Write(mac, MAC_BASE, (uint16)sizeof(mac_inflash));
	flag = 1;
	W25QXX_Write(&flag, MACFLAG_BASE, 1);
	flag = 0;
	#endif
	
	//1.读flash 有没有生产初始化过
	W25QXX_Read(&flag, MACFLAG_BASE, 1);
	W25QXX_Read(mac_inflash, MAC_BASE, (uint16)sizeof(mac_inflash));
	if((mac_inflash[0] == 0x06) && (mac_inflash[1] == 0x01) && (flag == 1))
	{
		memcpy(mac, mac_inflash, sizeof(mac_inflash));
	}	

	//2.没有 用这里默认的 手动mac(08)
	#if  0
	for (i = 0; i < 4; i++)
	{
		if((mac[i] >= 0x41)&&(mac[i] <= 0x46))
		{
			name[9 - i*2] 	 = (mac[5 - i] % 0x10);
			name[9 - i*2 -1] = (mac[5 - i] / 0x10);
		}
		else
		{
			name[9 - i*2] 	 = (mac[5 - i] % 0x10) + 0x30;
			name[9 - i*2 -1] = (mac[5 - i] / 0x10) + 0x30;
		}
	}		
	#endif
	tmpmac[7] = mac[5] % 0x10;
	tmpmac[6] = mac[5] / 0x10;
	tmpmac[5] = mac[4] % 0x10;
	tmpmac[4] = mac[4] / 0x10;
	tmpmac[3] = mac[3] % 0x10;
	tmpmac[2] = mac[3] / 0x10;
	tmpmac[1] = mac[2] % 0x10;
	tmpmac[0] = mac[2] / 0x10;
	
	for (i = 0; i < 8; i++)
	{
		#if 1
		if((tmpmac[i] >= 0x0A)&&(tmpmac[i] <=0x0F))
		{
			name[i+2] = tmpmac[i] + 0x37;
		}
		else if((tmpmac[i] >= 0x00)&&(tmpmac[i] <= 0x09))	
		{
			name[i+2] = tmpmac[i] + 0x30;
		}
		#endif
	}
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
	uint8 ret,i,tmp,count;
	uint8 setmac[]= {0x06, 0x01, 0x88, 0x88, 0x88, 0x88};			// 手动设置mac						//
	uint8 setname[] = {0x30u, 0x36u, 
						0x30u, 0x31u,
						0x30u, 0x31u, 
		 				0x30u, 0x31u, 
						0x30u, 0x38u};
	
#if !defined(__ARMCC_VERSION)
    InitializeBootloaderSRAM();
#endif

	CyGlobalIntEnable;
	
	//提前检测GPRS模块状态 status 并开启						    @0321修复6311模块启动status电平不稳定
	//GPRS_SetON();
	
	LED_State = RED;
	Led_SetState(LED_State);										// 上电设置状态为红灯亮				// 
	DoorClose();													// 上电门关闭						//
	
	WDT_ISR_StartEx(WDT_Interrupt);									// 看门狗中断						//
	WDT_Start();													// 看门狗定时器初始化				//	
	
	UART_433_tx_Write(0);											// 初始GSM串口拉低

	Timer_INT_ISR_StartEx(Timer_InterruptHandler);					// 开启定时器中断					//
    Timer_100ms_Start();											// 定时器初始化						//
	
	W25QXX_Init();
	// 获取系统启动原因，检查是否是重新上电 <上电重启 = 0x00>	 										//
	if (0x10 != CySysGetResetReason(CY_SYS_RESET_WDT | CY_SYS_RESET_PROTFAULT | CY_SYS_RESET_SW))
	{
		SendMacFlag = 0;
		tmp = Read_CenterIpType();
		if(tmp == 5)
		{
			
			VoicePlay((uint8)Sound_SYSTEM_POWER_ON);
		}
		else
		{
			VoicePlay((uint8)Sound_SYSTEM_POWER_ON_TEST);
		}
		#if 1
		// 第一次 开启
		GPRS_PWRKEY_Write(1);
		count = 0;
		while (count <= 30)
		{
			CyDelay(100);
			count++;
			WDT_FeedDog();
		}
		GPRS_PWRKEY_Write(0);
		#endif
		once_poweron_flag = 1;						// 首次上电的情况 启动上电报警功能
	}
	else
	{
		SendMacFlag = 1;
	}
	
	cur_time = 1483200000;											// 默认将时间设置为2017-1-1 0:0:0	//
	RTC_Start();													// RTC时钟初始化					//
	RTC_SetUnixTime(cur_time);										// 将默认的时间写到RTC中			//
	
	nfc_init();														// 刷卡功能初始化					//
	mac_conver_name(setmac, setname);
	Set_DeviceMacAndName((char8 *)setmac, (char8 *)setname);		// 设置mac、name					//
	
	CyBle_Start(AppCallback);										// 开启蓝牙功能						//
	
	GPRSPowerOn_0321();												// GPRS开机并等初始化完成			//
	ret = GPRS_TestAndWaitCommonOK();								// 测试GPRS AT指令通讯是否正常		//
	if (ret == TRUE)
	{
		GPRS_OpenNetworkLED();
		
		ret = GPRS_CheckConnectState();
		if (ret == TRUE)
		{	
			GPRS_ClosedConnect();
		}
		else
		{
			GPRS_SetConnectMode();
		}
	}
	#if DBG_SOFT_ENNABLE
	DBG_SOFT_PutString("BLE SmartGuard Project.\n");					// 初始化完成后，发送Log信息		//
	DBG_SOFT_PutString("System Version:");								// 版本								//
	DBG_SOFT_PutHexInt(system_version);
	DBG_SOFT_PutString("\nProtocl Version:");							// 通讯协议版本，02加密				//
	DBG_SOFT_PutHexInt(PROTOCOL_VER);
	#if 1
		DBG_SOFT_PutString("\nDevice Mac:");
	for (i = 0; i < 6; i++)
	{
		DBG_SOFT_PutHexByte(Mac_Addr[i]);
		if(i < 5)
			DBG_SOFT_PutString("-");	
	}
	DBG_SOFT_PutString("\n");
	#endif
	#endif
    
	Code_Action_flag = 1;                                               // 程序即将进入主循环			   //
	
    while(1) 
	{
		BLE_Tick();
		GPRS_Tick();
		CARD_Tick();
		KeyOpen_Tick(); 
		
		DefendAlarm_Tick();												// 防止撬锁 先不开 2018-3-8
		MgCheck_Tick();													// 门状态一直开门检测
		PowerOnCheck_Tick();											// 上电报警一次
		FireAlarm_Tick();												// 火警触发一分钟报一次
		
		Reboot_Check();
		//--UploadOpenInfo_Tick();
		Led_SetState(LED_State);
	}
}

//******************************************** END OF FILE *********************************************//
