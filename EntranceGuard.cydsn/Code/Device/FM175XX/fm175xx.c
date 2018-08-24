/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Technology Co., LTD
**
**                                 http://www.zlgmcu.com
**
**      ������������Ƭ���Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        ����������������Ƭ���Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           type_a.c
** Last modified Date:  2016-3-14
** Last Version:        V1.00
** Descriptions:        ISO/IEC144443A����ز���
**
**--------------------------------------------------------------------------------------------------------
*/
#include <stdint.h>
#include "fm175xx.h"
#include "Utility.h"
#include <string.h>
#include "spi.h"
#include "type_a.h"
#include "type_b.h"


#define MI_NOTAGERR 0xEE
#define MAXRLEN 	64


extern uint8_t  TyteA_Test(uint8_t *p);
extern uint8_t  TyteB_Test(uint8_t *p);
extern void 	MifareCard_Test (uint8_t command);
extern void 	delay_with_ble(uint32 ms);
/*********************************************************************************************************
** Function name:       pcd_Init
** Descriptions:        MCU��ʼ������������SPI��UART�ĳ�ʼ��
** input parameters:    N/A
** output parameters:   N/A
** Returned value:      
*********************************************************************************************************/
void pcd_Init(void)
{
	SPI_Init();
}

/*********************************************************************************************************
** Function name:       MCU_TO_PCD_TEST
** Descriptions:        MCU�����оƬͨ�Ų��Ժ���
**                      ��ͬ�Ķ���оƬ��ѡȡ���ڲ��ԵļĴ�����һ������Ҫ���߾���оƬ��ȷ��Ŀ��Ĵ���
** input parameters:    N/A
** output parameters:   N/A
** Returned value:      TRUE---����ͨ��    FALSE----����ʧ��
*********************************************************************************************************/
uint8_t MCU_TO_PCD_TEST(void)
{
    volatile uint8_t ucRegVal;
    pcd_RST();                                                          /* Ӳ����λ                     */
	ucRegVal = spi_GetReg(ControlReg);
    spi_SetReg(ControlReg, 0x10);                                       /* ������д��ģʽ               */
	
	ucRegVal = spi_GetReg(GsNReg);
	DBG_PRINTF("MCU_TO_PCD_TEST  GsNReg=0x%x !\r\n",ucRegVal);
	ucRegVal = spi_GetReg(CWGsPReg);
	DBG_PRINTF("MCU_TO_PCD_TEST  CWGsPReg=0x%x !\r\n",ucRegVal);
	
    ucRegVal = spi_GetReg(ControlReg);
    spi_SetReg(GsNReg, 0xF0 | 0x04);                                    /* CWGsN = 0xF; ModGsN = 0x4    */
    ucRegVal = spi_GetReg(GsNReg);
    if(ucRegVal != 0xF4)                                                /* ��֤�ӿ���ȷ                 */
        return FALSE;
    return TRUE;
}


/*********************************************************************************************************
** Function name:       pcd_RST
** Descriptions:        PCD����PICCӲ����λ����
** input parameters:    N/A
** output parameters:   N/A
** Returned value:      �Ĵ�����ֵ
*********************************************************************************************************/
void pcd_RST(void)
{
    CD_SetTPD();
    CyDelayUs(300);
    CD_ClrTPD();                                                        /* ��λ�ܽ�����                 */
    CyDelayUs(500);
    CD_SetTPD();                                                        /* ������                       */
    CyDelay(1);
}

/*********************************************************************************************************
** Function name:       NRSTPD_CTRL
** Descriptions:        PCD����PICCӲ����λ����
** input parameters:    ctrltype ͨ����������NRSTPD���������
** output parameters:   N/A
** Returned value:      �Ĵ�����ֵ
*********************************************************************************************************/
void NRSTPD_CTRL(uint8_t  ctrltype)
{
    if(ctrltype == 0x0)
		CD_ClrTPD();
    else
		CD_SetTPD();
}

/*********************************************************************************************************
** Function name:       Read_Reg
** Descriptions:        ��ȡ�Ĵ���        
** input parameters:    reg_add:�Ĵ�����ֵ
** output parameters:   N/A
** Returned value:      �Ĵ�����ֵ
*********************************************************************************************************/
uint8_t  Read_Reg(uint8_t  reg_add)
{
    uint8_t   reg_value;       
    reg_value=spi_GetReg(reg_add);
    return reg_value;
}

/*********************************************************************************************************
** Function name:       Read_Reg_All
** Descriptions:        ��ȡȫ���Ĵ���        
** input parameters:    reg_value:�Ĵ�����ֵ
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Read_Reg_All(uint8_t  *reg_value)
{
    uint8_t  i;
    for (i=0;i<64;i++)       
        *(reg_value+i) = spi_GetReg(i);
    return TRUE;
}


/*********************************************************************************************************
** Function name:       Write_Reg
** Descriptions:        д�Ĵ�������        
** input parameters:    reg_add:�Ĵ�����ַ
**                      reg_value:�Ĵ�����ֵ
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Write_Reg(uint8_t  reg_add,uint8_t  reg_value)
{
    spi_SetReg(reg_add,reg_value);
    return TRUE;
}


/*********************************************************************************************************
** Function name:       Read_FIFO
** Descriptions:        ����FIFO������     
** input parameters:    length:��ȡ���ݳ���
**                      *fifo_data:���ݴ��ָ��
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
void Read_FIFO(uint8_t  length,uint8_t  *fifo_data)
{     
    SPIRead_Sequence(length,FIFODataReg,fifo_data);
    return;
}


/*********************************************************************************************************
** Function name:       Write_FIFO
** Descriptions:        д��FIFO     
** input parameters:    length:��ȡ���ݳ���
**                      *fifo_data:���ݴ��ָ��
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
void Write_FIFO(uint8_t  length,uint8_t  *fifo_data)
{
    SPIWrite_Sequence(length,FIFODataReg,fifo_data);
    return;
}


/*********************************************************************************************************
** Function name:       Clear_FIFO
** Descriptions:        ���FIFO          
** input parameters:   
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Clear_FIFO(void)
{
    Set_BitMask(FIFOLevelReg,0x80);                                     /* ���FIFO����                 */
    if( spi_GetReg(FIFOLevelReg) == 0 )
        return TRUE;
    else
        return FALSE;
}


/*********************************************************************************************************
** Function name:       Set_BitMask
** Descriptions:        ��λ�Ĵ�������    
** input parameters:    reg_add���Ĵ�����ַ
**                      mask���Ĵ������λ
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Set_BitMask(uint8_t  reg_add,uint8_t  mask)
{
    uint8_t  result;
    result=spi_SetReg(reg_add,Read_Reg(reg_add) | mask);                /* set bit mask                 */
    return result;
}

/*********************************************************************************************************
** Function name:       Clear_BitMask
** Descriptions:        ���λ�Ĵ�������
** input parameters:    reg_add���Ĵ�����ַ
**                      mask���Ĵ������λ
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Clear_BitMask(uint8_t  reg_add,uint8_t  mask)
{
    uint8_t  result;
    result=Write_Reg(reg_add,Read_Reg(reg_add) & ~mask);                /* clear bit mask               */
    return result;
}


/*********************************************************************************************************
** Function name:       Set_RF
** Descriptions:        ������Ƶ���
** input parameters:    mode����Ƶ���ģʽ
**                      0���ر����
**                      1������TX1���
**                      2������TX2���
**                      3��TX1��TX2�������TX2Ϊ�������
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Set_Rf(uint8_t  mode)
{
	uint8_t result = 0;
	if( (Read_Reg(TxControlReg)&0x03) == mode )
	    return TRUE;
	if( mode == 0 )
	    result = Clear_BitMask(TxControlReg,0x03);                      /* �ر�TX1��TX2���             */
	if( mode== 1 )
	    result = Clear_BitMask(TxControlReg,0x01);                      /* ����TX1���                */
	if( mode == 2)
	    result = Clear_BitMask(TxControlReg,0x02);                      /* ����TX2���                */
	if (mode==3)
	    result = Set_BitMask(TxControlReg,0x03);                        /* ��TX1��TX2���             */
	delay_with_ble(100);
	return result;
}
 
/*********************************************************************************************************
** Function name:       Pcd_Comm
** Descriptions:        ������ͨ�� ������IRQ�ܽŵ����
** input parameters:    Command:ͨ�Ų�������
**                      pInData:������������
**                      InLenByte:�������������ֽڳ���
**                      pOutData:������������
**                      pOutLenBit:�������ݵ�λ����
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Pcd_Comm (uint8_t  Command, 
                        uint8_t  *pInData, 
                        uint8_t  InLenByte,
                        uint8_t  *pOutData, 
                        uint32_t *pOutLenBit)
{
    uint8_t status  = FALSE;
    uint8_t irqEn   = 0x00;                                             /* ʹ�ܵ��ж�                   */
    uint8_t waitFor = 0x00;                                             /* �ȴ����ж�                   */
    uint8_t lastBits;
    uint8_t n;
    uint32_t i;
    Write_Reg(ComIrqReg, 0x7F);                                         /* ���IRQ���                  */
    Write_Reg(TModeReg,0x80);                                           /* ����TIMER�Զ�����            */
    switch (Command) {
    case MFAuthent:                                                     /* Mifare��֤                   */
        irqEn   = 0x12;
        waitFor = 0x10;
        break;
    case Transceive:                                       /* ����FIFO�е����ݵ����ߣ�����󼤻���յ�·*/
        irqEn   = 0x77;
        waitFor = 0x30;
        break;
    default:
        break;
    }
   
    Write_Reg(ComIEnReg, irqEn | 0x80);
    //Clear_BitMask(ComIrqReg, 0x80);
    Write_Reg(CommandReg, Idle);
    Set_BitMask(FIFOLevelReg, 0x80);
    
    for (i=0; i < InLenByte; i++) {
        Write_Reg(FIFODataReg, pInData[i]);
    }
    Write_Reg(CommandReg, Command);

    if (Command == Transceive) {
        Set_BitMask(BitFramingReg, 0x80);
    }

    i = 3000;                                              /* ����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms*/

    do {
        n = Read_Reg(ComIrqReg);
        i--;                                                            /* i==0��ʾ��ʱ����             */
    } while ((i != 0) && !(n & 0x01) && !(n & waitFor));            /* n&0x01!=1��ʾPCDsettimerʱ��δ�� */
                                                                        /* n&waitFor!=1��ʾָ��ִ����� */
    Clear_BitMask(BitFramingReg, 0x80);
    if (i != 0) {
        if(!(Read_Reg(ErrorReg) & 0x1B)) {
            status = TRUE;
            if (n & irqEn & 0x01) {
                status = MI_NOTAGERR;
            }
            if (Command == Transceive) {
                n = Read_Reg(FIFOLevelReg);
                lastBits = Read_Reg(ControlReg) & 0x07;
                if (lastBits) {
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                } else {
                    *pOutLenBit = n * 8;
                }
                if (n == 0) {
                    n = 1;
                }
                if (n > MAXRLEN) {
                    n = MAXRLEN;
                }
                for (i = 0; i < n; i++) {
                    pOutData[i] = Read_Reg(FIFODataReg);
                }
            }
        } else {
            status = FALSE;
        }
    }
    Clear_BitMask(BitFramingReg,0x80);                                  /* �رշ���                     */
    return status;
}


/*********************************************************************************************************
** Function name:       Pcd_SetTimer
** Descriptions:        ���ý�����ʱ
** input parameters:    delaytime����ʱʱ�䣨��λΪ���룩  
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Pcd_SetTimer(unsigned long delaytime)
{
    unsigned long TimeReload;
    uint32_t Prescaler;

    Prescaler=0;
    TimeReload=0;
    while(Prescaler<0xfff) {
        TimeReload = ((delaytime*(long)13560)-1)/(Prescaler*2+1);
        if( TimeReload<0xffff)
            break;
        Prescaler++;
    }
    TimeReload=TimeReload&0xFFFF;
    Set_BitMask(TModeReg,Prescaler>>8);
    Write_Reg(TPrescalerReg,Prescaler&0xFF);            
    Write_Reg(TReloadMSBReg,TimeReload>>8);
    Write_Reg(TReloadLSBReg,TimeReload&0xFF);
    return TRUE;
}

/*********************************************************************************************************
** Function name:       Pcd_ConfigISOType
** Descriptions:        ����ISO14443A/BЭ��
** input parameters:    type = 0��ISO14443AЭ�飻
**                         type = 1��ISO14443BЭ�飻   
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Pcd_ConfigISOType(uint8_t  type)
{
    if (type == 0)   {                                                 /* ����ΪISO14443_A              */
        Set_BitMask(ControlReg, 0x10);                                 /* ControlReg 0x0C ����readerģʽ*/
        Set_BitMask(TxAutoReg, 0x40);                                  /* TxASKReg 0x15 ����100%ASK��Ч */
        Write_Reg(TxModeReg, 0x00);                /* TxModeReg 0x12 ����TX CRC��Ч��TX FRAMING =TYPE A */
        Write_Reg(RxModeReg, 0x00);                /* RxModeReg 0x13 ����RX CRC��Ч��RX FRAMING =TYPE A */
    }
    if (type == 1) {                                                   /* ����ΪISO14443_B              */
        Write_Reg(ControlReg,0x10);
        Write_Reg(TxModeReg,0x83);                                     /* BIT1~0 = 2'b11:ISO/IEC 14443B */
        Write_Reg(RxModeReg,0x83);                                     /* BIT1~0 = 2'b11:ISO/IEC 14443B */
        Write_Reg(TxAutoReg,0x00);
        Write_Reg(RxThresholdReg,0x55);
        Write_Reg(RFCfgReg,0x48);
        Write_Reg(TxBitPhaseReg,0x87);                                 /* Ĭ��ֵ                         */
        Write_Reg(GsNReg,0x83);                                        /* 0x83                           */
        Write_Reg(CWGsPReg,0x30);                                      /*  0x30                          */
        Write_Reg(GsNOffReg,0x38);
        Write_Reg(ModGsPReg,0x20);										/*  0x20                          */
    }
    CyDelay(3);
    return TRUE;
}


/*********************************************************************************************************
** Function name:       FM175X_SoftReset
** Descriptions:        FM175xx�����λ
** input parameters:    N/A       
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t   FM175X_SoftReset(void)
{    
    Write_Reg(CommandReg,SoftReset);
    return Set_BitMask(ControlReg,0x10);                                /* 17520��ʼֵ����              */
}

/*********************************************************************************************************
** Function name:       FM175X_HardReset
** Descriptions:        FM175xxӲ����λ
** input parameters:    N/A       
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  FM175X_HardReset(void)
{    
    CD_ClrTPD();
	CyDelayUs(100);
    CD_SetTPD();
	CyDelayUs(100);
    return TRUE;
}
    
/*********************************************************************************************************
** Function name:       FM175X_SoftPowerdown
** Descriptions:        Ӳ���͹��Ĳ���
** input parameters:    N/A       
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  FM175X_SoftPowerdown(void)
{
    if(Read_Reg(CommandReg)&0x10) {
        Clear_BitMask(CommandReg,0x10);									/* �˳��͹���ģʽ               */
        return FALSE;
    }
    else
    	Set_BitMask(CommandReg,0x10);									/* ����͹���ģʽ               */
    return TRUE;
}

/*********************************************************************************************************
** Function name:       FM175X_HardPowerdown
** Descriptions:        Ӳ���͹��Ĳ���
** input parameters:    N/A       
** output parameters:   N/A
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  FM175X_HardPowerdown(void)
{    
//	NPD=~NPD;
//	if(NPD==1)                                                          /* ����͹���ģʽ				*/
//		return TRUE;
//	else
//		return FALSE;													/* �˳��͹���ģʽ               */
	return TRUE;
}

/*********************************************************************************************************
** Function name:       Read_Ext_Reg
** Descriptions:        ��ȡ��չ�Ĵ���
** input parameters:    reg_add���Ĵ�����ַ��         
** output parameters:   reg_value���Ĵ�����ֵ
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Read_Ext_Reg(uint8_t  reg_add)
{
    Write_Reg(0x0F,0x80+reg_add);
    return Read_Reg(0x0F);
}

/*********************************************************************************************************
** Function name:       Write_Ext_Reg
** Descriptions:        д����չ�Ĵ���
** input parameters:    reg_add���Ĵ�����ַ��
**                      reg_value���Ĵ�����ֵ
** output parameters:   
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  Write_Ext_Reg(uint8_t  reg_add,uint8_t  reg_value)
{
    Write_Reg(0x0F,0x40+reg_add);
    return (Write_Reg(0x0F,0xC0+reg_value));
}

/*********************************************************************************************************
** Function name:       FM175XX_Initial
** Descriptions:        FM175XX�ĳ�ʼ��������FIFO�����ж�
** input parameters:    none
** output parameters:   none
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  FM175XX_Initial(void)
{
    uint8_t  regdata,res;
    
    regdata = 0x20;         //WaterLevel���յ�һ������ʱ�����ж�
    res = spi_SetReg(WaterLevelReg,regdata);
    if(res != TRUE)
        return FALSE;
    return TRUE;
}

/*********************************************************************************************************
** Function name:       FM175XX_LPCD_InitConfig
** Descriptions:        FM175XX��LPCD��ʼ�����ã�У׼������LPCDģʽ
** input parameters:    none
** output parameters:   none
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  FM175XX_LPCD_InitConfig(void)
{
    uint8_t CalibraFlag;
    
    pcd_Init();												            /* SPI��ʼ��������оƬ��λ	    */
    pcd_RST();                                                          /* Ӳ����λ                     */
    spi_SetReg(SerialSpeedReg,0x3A);
    //ucRegVal = spi_GetReg(SerialSpeedReg);
	//DBG_PRINTF("FM17550 SerialSpeedReg :%x.\r\n", ucRegVal);
	CyDelay(10);
    FM175XX_Initial();
    LpcdParamInit();                                                    /* LPCD������ʼ��				*/
    LpcdRegisterInit();                                                 /* LPCD�Ĵ�����ʼ��				*/
    //LpcdAuxSelect(ON);                                           	    /* ����AUX�۲�ͨ��				*/
	//DBG_PRINTF("FM17550 Lpcd Init Calibra Start!\r\n");
    LpcdInitCalibra(&CalibraFlag);                                      /* LPCD��ʼ������				*/
	//DBG_PRINTF("FM17550 Lpcd Init Calibra stop!\r\n");
    if (CalibraFlag == TRUE)                                            /* ���̳ɹ�			            */
	{
        //DBG_PRINTF("FM17550 LpcdInitCalibra  OK.\r\n");               /* ��ʾLPCD��У�ɹ�				*/
        CyDelay(1000);
        NRSTPD_CTRL(0);                                                 /* ����LPCDģʽ					*/
        return TRUE;
    } 
    else
	{
        //DBG_PRINTF("FM17550 LpcdInitCalibra  FALSE.\r\n");            /* LPCD��Уʧ��					*/
        return FALSE;
    }
}

/*********************************************************************************************************
** Function name:       FM175XX_LPCD_WakeUpHandler
** Descriptions:        FM175XX��LPCD�жϻ��Ѻ�Ĵ������������˶������ܡ�
** input parameters:    none
** output parameters:   none
** Returned value:      TRUE�������ɹ� ERROR������ʧ��    
*********************************************************************************************************/
uint8_t  FM175XX_LPCD_WakeUpHandler(uint8_t *p)
{
    uint8_t lpcd_irq;
    uint8_t cardFlag = 0;
    uint8_t static noCardTime;
    
    if (SPI_INT_Read() == 0)
	{                                   						        /* �ж��жϱ�־�Ƿ���λ			*/
		spi_GetReg(FIFOLevelReg);
		NRSTPD_CTRL(1);                                                 /* ����TPD���ţ�ʹ�ܶ���������	*/
		CyDelay(2);                                        	            /* ��ʱ2ms���þ����ȶ�			*/
        GetReg_Ext(JREG_LPCD_IRQ,&lpcd_irq);
        if ( (lpcd_irq & JBIT_AUTO_WUP_IRQ) == JBIT_AUTO_WUP_IRQ )      /* ��ʱ���ѣ��Զ���У	        */
        {
            LpcdAutoWakeUp_IRQHandler();
        }
        if ((lpcd_irq & JBIT_CARD_IN_IRQ) == JBIT_CARD_IN_IRQ )         /* ��ʾ��⵽��Ƭ			    */
        {
            //DBG_PRINTF("\nFM17550 start read card.\r\n");
            cardFlag = TyteB_Test(p);                                   /* ��ISO14443B��				*/
            if (cardFlag == 0)
            {
                cardFlag = TyteA_Test(p);                               /* ��ISO14443A��				*/
            }
            if(cardFlag == 0)
			{                             					            /* ���û�ж���������־һ��		*/
                noCardTime++;
            }
            else
            {
                noCardTime = 0;                                         /* �����������ʧ�ܴ���Ϊ0		*/
            }
            CyDelay(100);
            if(noCardTime > 30)
			{                           					            /* ��ϵ����ʧ�ܣ����µ�У		*/
                LpcdAutoWakeUp_IRQHandler();
                noCardTime = 0;
            }
            else
			{
                LpcdCardIn_IRQHandler();                                /* FM17550���½���LPCDģʽ		*/
            }
        }
    }
    CyDelayUs(100);
    return cardFlag;
}

/*********************************************************************************************************
** Function name:       FM175XX_ReadCardUid
** Descriptions:        FM175XX��ȡ��uid������cardFlagΪ�������͡�
** input parameters:    none
** output parameters:   none
** Returned value:      TRUE�������ɹ� ERROR������ʧ��
---------------------------------------------------------------------------------------------------------
** Info: �˺���ʹ����Ϻ��������½���LPCDģʽ�������µ���FM175XX_LPCD_InitConfig������
*********************************************************************************************************/
uint8_t  FM175XX_ReadCardUid(uint8_t *p)
{
    uint8_t cardFlag = 0;
    
    NRSTPD_CTRL(1);                                                     /* ����TPD���ţ�ʹ�ܶ���������	*/
    cardFlag = TyteB_Test(p);                                           /* ��ISO14443B��				*/
    if (cardFlag == 0)
    {
        cardFlag = TyteA_Test(p);                                       /* ��ISO14443A��				*/
    }
    return cardFlag;
}

void nfc_init(void)
{
	pcd_Init();												            /* SPI��ʼ��������оƬ��λ	    */
	pcd_RST();                                                          /* Ӳ����λ                     */
	spi_SetReg(SerialSpeedReg,0x3A);
	//ucRegVal = spi_GetReg(SerialSpeedReg);
	//DBG_PRINTF("FM17550 SerialSpeedReg :%x.\r\n", ucRegVal);
	CyDelay(10);
	FM175XX_Initial();
}

uint8_t nfc_detect_type_a(uint8_t *card_id, uint16 sz)
{	
    uint8_t statues = TRUE;
    uint8_t num = 0;
    uint8_t cardFlag = 0;
    uint8_t picc_atqa[2];                                               /* ����Ѱ�����ؿ�Ƭ������Ϣ     */
    static uint8_t picc_uid[15];                                        /* ���濨ƬUID��Ϣ              */
    uint8_t picc_sak[3];                                                /* ���濨ƬӦ����Ϣ             */
    
    memset (card_id, 0, sz);
    FM175X_SoftReset( );                                                /* FM175xx�����λ              */
    Set_Rf( 3 );                                                        /* ��˫����                   */
    Pcd_ConfigISOType( 0 );                                             /* ISO14443�Ĵ�����ʼ��         */
    while (num < 1)
    {
        statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );      /* ���Ƭ                     */
        if ( statues == TRUE )
        {
            num = 0;
            TypeA_Halt( 0 );                                            /* ˯�߿�Ƭ                     */
            if( ( picc_atqa[0] & 0xc0 ) == 0x00 )                       /* 4���ֽ�UID                   */
            {
                cardFlag = 2;
                memcpy(card_id, picc_uid, 4);
            }
            if( ( picc_atqa[0] & 0xc0 ) == 0x40 )                       /* 7�ֽ�UID                     */
            {
                cardFlag = 3;
                memcpy(card_id, picc_uid + 1, 3);
                memcpy(card_id + 3, picc_uid + 5, 4);
            }
            if( ( picc_atqa[0] & 0xc0 ) == 0x80 )                       /* 10�ֽ�UID                    */
            {
                cardFlag = 4;
                memcpy(card_id, picc_uid + 1, 3);
                memcpy(card_id + 3, picc_uid + 5, 3);
                memcpy(card_id + 6, picc_uid + 10, 4);
            }
            memset(picc_uid,0x00,15);
        }
        else
        {
            num++;
        }
    }
    return cardFlag;
}

uint8_t nfc_detect_type_b(uint8_t *card_id, uint16 sz)
{
    uint8_t statues;
    uint32_t rec_len;
    uint8_t cardFlag = 0;
    uint8_t pupi[4];
    uint8_t buff[12];

	memset(card_id, 0, 8);
    FM175X_SoftReset( );                                                /* FM175xx�����λ              */
    Pcd_ConfigISOType( 1 );                                             /* ISO14443�Ĵ�����ʼ��         */
    Set_Rf( 3 );                                                        /* ��˫����                   */ 
    statues = TypeB_WUP(&rec_len,buff,pupi);                            /* Ѱ��                         */
    if ( statues == TRUE )
    {
        statues = TypeB_Select(pupi,&rec_len,buff);
    }
    if ( statues == TRUE )
    {
        cardFlag = 1;
        statues = TypeB_GetUID(&rec_len,&buff[0]);
        if(statues == TRUE)
        {
            memcpy(card_id, buff, 8);
        }
    }
    Set_Rf( 0 );                                                        /* �ر�����                     */    
    return cardFlag;
}

