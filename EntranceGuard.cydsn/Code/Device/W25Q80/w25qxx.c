//********************************************************************************************************
// File name		: w25qxx.c
// Description		: W25Qxx����������
// Author			: Keno
// Date				: 2016.11.14
// Version			: V02
// -------------------------------------------------------------------------------------------------------
// W25Q80��Ϣ		: 4K ByteΪһ������(Sector)������2048������
//					: 16������Ϊһ����(Block)������128����
//********************************************************************************************************
#include "w25qxx.h"


//******************************************* �ⲿ�������� *********************************************//
extern void ble_heartbeat(void);


//********************************************************************************************************
// Function name	: SPI2_Init
// Description		: ��ʼ��SPI��IO��
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void SPI2_Init(void)
{
	FLASH_SCK_RESET();
	FLASH_MOSI_RESET();
}
		
//********************************************************************************************************
// Function name	: W25QXX_Init
// Description		: ��ʼ��SPI��IO��
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void W25QXX_Init(void)
{ 
    uint8_t temp;
    uint16_t id;
	
	FLASH_DISABLE();			            						// ȡ��Ƭѡ							//
	SPI2_Init();		   			        						// ��ʼ��SPI��IO��					//
	id = W25QXX_ReadID();	        								// ��ȡFLASH ID						//
    if (id == W25Q80)												// SPI FLASHΪW25Q80				//
    {
        temp = W25QXX_ReadSR(3);              						// ��ȡ״̬�Ĵ���3���жϵ�ַģʽ	//
        if ((temp & 0X01) == 0)			        					// �ж��Ƿ�Ϊ4�ֽڵ�ַģʽ			//
		{
			FLASH_ENABLE(); 			        					// ʹ��Ƭѡ							//
			FLASH_ReadWriteByte(W25X_Enable4ByteAddr,Flash_cmdWrite);				// ���ͽ���4�ֽڵ�ַģʽָ�� 		//  
			FLASH_DISABLE();										// ȡ��Ƭѡ							//
		}
    }
}

//********************************************************************************************************
// Function name	: W25QXX_ReadSR
// Description		: ��W25QXX״̬�Ĵ���
// Parameter		: regno	: �Ĵ�����
// Return			: ״̬�Ĵ���ֵ
//--------------------------------------------------------------------------------------------------------
// Info				: 
//--------------------------------------------------------------------------------------------------------
//��ȡW25QXX��״̬�Ĵ�����W25QXXһ����3��״̬�Ĵ���
//״̬�Ĵ���1��
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR: Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB, BP2, BP1, BP0: FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ; 0,����)
//Ĭ��:0x00
//
//״̬�Ĵ���2��
//BIT7  6   5   4   3   2   1    0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//
//״̬�Ĵ���3��
//BIT7       6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//********************************************************************************************************
uint8_t W25QXX_ReadSR(uint8_t reg_num)
{  
	uint8_t byte =0, command = 0;
    switch (reg_num)
    {
        case 1:
            command = W25X_ReadStatusReg1;
            break;
        case 2:
            command = W25X_ReadStatusReg2;
            break;
        case 3:
            command = W25X_ReadStatusReg3;
            break;
        default:
            command = W25X_ReadStatusReg1;
            break;
    }    
	FLASH_ENABLE();
	FLASH_ReadWriteByte(command,Flash_cmdWrite);									// ���Ͷ�ȡ״̬�Ĵ�������  			//  
	byte = FLASH_ReadWriteByte(0xFF,Flash_cmdRead);          						// ��ȡһ���ֽ�  					//
	FLASH_DISABLE();
	return byte;
}

//********************************************************************************************************
// Function name	: W25QXX_Write_SR
// Description		: дW25QXX״̬�Ĵ���
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void W25QXX_Write_SR(uint8_t reg_num, uint8_t sr)
{   
    uint8_t command = 0;
    switch (reg_num)
    {
        case 1:
            command = W25X_WriteStatusReg1;
            break;
        case 2:
            command  =W25X_WriteStatusReg2;
            break;
        case 3:
            command = W25X_WriteStatusReg3;
            break;
        default:
            command = W25X_WriteStatusReg1;
            break;
    }   
	FLASH_ENABLE();
	FLASH_ReadWriteByte(command,Flash_cmdWrite);									// ����дȡ״̬�Ĵ�������    		//
	FLASH_ReadWriteByte(sr,Flash_cmdWrite);											// д��һ���ֽ�						//
	FLASH_DISABLE();
}

//********************************************************************************************************
// Function name	: W25QXX_Write_Enable
// Description		: W25QXXдʹ��	
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: ��WEL��λ
//********************************************************************************************************
void W25QXX_Write_Enable(void)   
{
	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_WriteEnable,Flash_cmdWrite);   						// ����дʹ��  						//
	FLASH_DISABLE();
}

//********************************************************************************************************
// Function name	: W25QXX_Write_Disable
// Description		: W25QXXд��ֹ
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: ��WEL����
//********************************************************************************************************
void W25QXX_Write_Disable(void)   
{  
	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_WriteDisable,Flash_cmdWrite);							// ����д��ָֹ��    				//
	FLASH_DISABLE();
}

//********************************************************************************************************
// Function name	: W25QXX_Write_Disable
// Description		: ��ȡоƬID
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;
	
	FLASH_ENABLE();				    
	FLASH_ReadWriteByte(W25X_ManufactDeviceID,Flash_cmdWrite);						// ���Ͷ�ȡID����					//
	FLASH_ReadWriteByte(0x00,Flash_cmdWrite);
	FLASH_ReadWriteByte(0x00,Flash_cmdWrite);
	FLASH_ReadWriteByte(0x00,Flash_cmdWrite);
	Temp |= FLASH_ReadWriteByte(0xFF,Flash_cmdRead) << 8;
	Temp |= FLASH_ReadWriteByte(0xFF,Flash_cmdRead);
	FLASH_DISABLE();
	return Temp;
}

//********************************************************************************************************
// Function name	: W25QXX_Read
// Description		: ��ָ����ַ��ʼ��ȡָ�����ȵ�����
// Parameter		: pBuffer		: ���ݴ洢��
//					: ReadAddr		: ��ʼ��ȡ�ĵ�ַ(24bit)
//					: NumByteToRead	: Ҫ��ȡ���ֽ���(���65535)
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{ 
 	uint16_t i;
	
	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_ReadData,Flash_cmdWrite);      						// ���Ͷ�ȡ����  					//
	// �����W25Q256�Ļ�����ַΪ4�ֽڵģ�Ҫ�������8λ													//
    if (W25QXX_TYPE == W25Q256)
    {
        FLASH_ReadWriteByte((uint8_t)((ReadAddr) >> 24),Flash_cmdWrite);
    }
    FLASH_ReadWriteByte((uint8_t)((ReadAddr) >> 16),Flash_cmdWrite);
    FLASH_ReadWriteByte((uint8_t)((ReadAddr) >> 8),Flash_cmdWrite);
    FLASH_ReadWriteByte((uint8_t)ReadAddr,Flash_cmdWrite);
    for (i = 0; i < NumByteToRead; i++)
	{
		pBuffer[i] = FLASH_ReadWriteByte(0xFF, Flash_cmdRead);
    }
	FLASH_DISABLE();
}

//********************************************************************************************************
// Function name	: W25QXX_Write_Page
// Description		: ҳ��д����
// Parameter		: pBuffer		: ���ݴ洢��
//					: ReadAddr		: ��ʼд��ĵ�ַ(24bit)
//					: NumByteToWrite: Ҫд����ֽ���(���256),��Ӧ�ó�����ҳ��ʣ���ֽ���
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void W25QXX_Write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
 	uint16_t i;
	CyGlobalIntDisable;
    W25QXX_Write_Enable();
	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_PageProgram,Flash_cmdWrite);							// ����дҳ����   					//
	// �����W25Q256�Ļ���ַΪ4�ֽڵģ�Ҫ�������8λ													//
    if (W25QXX_TYPE == W25Q256)
    {
        FLASH_ReadWriteByte((uint8_t)((WriteAddr) >> 24),Flash_cmdWrite); 
    }
    FLASH_ReadWriteByte((uint8_t)((WriteAddr) >> 16),Flash_cmdWrite);
    FLASH_ReadWriteByte((uint8_t)((WriteAddr) >> 8),Flash_cmdWrite);   
    FLASH_ReadWriteByte((uint8_t)WriteAddr,Flash_cmdWrite);
    for (i = 0; i < NumByteToWrite; i++)
	{
		FLASH_ReadWriteByte(pBuffer[i], Flash_cmdWrite);
	}
	FLASH_DISABLE();
	W25QXX_Wait_Idle();					   							// �ȴ�д�����						//
	CyGlobalIntEnable;
}

//********************************************************************************************************
// Function name	: W25QXX_Write_Page
// Description		: ��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
// Parameter		: pBuffer		: ���ݴ洢��
//					: WriteAddr		: ��ʼд��ĵ�ַ(24bit)
//					: NumByteToWrite: Ҫд����ֽ���(���65535)
// Return			: none
//--------------------------------------------------------------------------------------------------------
//					: �����Զ���ҳ����
// Info				: ����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//********************************************************************************************************
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	
	
	ble_heartbeat();
	pageremain = 256 - WriteAddr % 256;
	if (NumByteToWrite <= pageremain)
	{
		pageremain = NumByteToWrite;
	}
	while(1)
	{
		ble_heartbeat();
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if (NumByteToWrite == pageremain)
		{
			break;
	 	}
		else
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;	
			NumByteToWrite -= pageremain;			  				// ��ȥ�Ѿ�д���˵��ֽ���			//
			if (NumByteToWrite > 256)
			{
				pageremain = 256; 									// һ�ο���д��256���ֽ�			//
			}
			else
			{
				pageremain = NumByteToWrite; 	  					// ����256���ֽ�					//
			}
		}
	};	    
}

//********************************************************************************************************
// Function name	: W25QXX_Write
// Description		: ��ָ����ַ��ʼд��ָ�����ȵ�����
// Parameter		: pBuffer		: ���ݴ洢��
//					: WriteAddr		: ��ʼд��ĵ�ַ(24bit)
//					: NumByteToWrite: Ҫд����ֽ���(���65535)
// Return			: none
//--------------------------------------------------------------------------------------------------------
//					: �����Զ���ҳ����
// Info				: ����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//********************************************************************************************************
void W25QXX_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t W25QXX_BUFFER[4096] = {0};
	uint8_t *W25QXX_BUF = W25QXX_BUFFER;
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
 	uint16_t i;
	
 	secpos = WriteAddr / 4096;										// �������  						//
	secoff = WriteAddr % 4096;										// �������ڵ�ƫ��					//
	secremain = 4096 - secoff;										// ����ʣ��ռ��С   				//
 	
 	if (NumByteToWrite <= secremain)								// �ж��Ƿ�����ڱ�����д��			//
	{
		secremain = NumByteToWrite;									// ������4096���ֽ�					//
	}
	while(1)
	{	
		W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096);				// ������������������				//
		for (i = 0; i < secremain; i++)								// ��д�ĳ������ݽ���У��			//
		{
			if (W25QXX_BUF[secoff + i] != 0xFF)						// �ж��Ƿ���Ҫ����					// 
				break;
		}
		if (i < secremain)											// ��Ҫ����							//
		{
			W25QXX_Erase_Sector(secpos);							// �����������						//
			for (i = 0; i < secremain; i++)
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096);	// д����������						//
		}
		else
		{
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain);	// ��д�����Ѳ�����ֱ��д			//
		}
		
		if (NumByteToWrite == secremain)							// д�����							//
		{
			break;
		}
		else
		{
			secpos++;												// ���������1						//
			secoff = 0;												// ƫ��λ��Ϊ0 	 					//
			
		   	pBuffer += secremain;									// ָ��ƫ��							//
			WriteAddr += secremain;									// д��ַƫ��						//
		   	NumByteToWrite -= secremain;							// �ֽ����ݼ�						//
			if (NumByteToWrite > 4096)								// ��һ����������д����				//
			{
				secremain = 4096;
			}
			else
			{
				secremain = NumByteToWrite;							// ��һ����������д��				//
			}
		}	 
	}
}

//********************************************************************************************************
// Function name	: W25QXX_Erase_Chip
// Description		: ��������оƬ
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: �ȴ�ʱ�䳬��
//********************************************************************************************************
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();
    W25QXX_Wait_Idle();
  	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_ChipErase,Flash_cmdWrite);							// ����Ƭ��������					//
	FLASH_DISABLE();
	W25QXX_Wait_Idle();
}

//********************************************************************************************************
// Function name	: W25QXX_Erase_Sector
// Description		: ����һ������
// Parameter		: Dst_Addr	: ������ַ ����ʵ����������
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: ����һ������������ʱ��:150ms
//********************************************************************************************************
void W25QXX_Erase_Sector(uint32_t Dst_Addr)
{  
 	Dst_Addr *= 4096;
    
	W25QXX_Write_Enable();
    W25QXX_Wait_Idle();
  	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_SectorErase,Flash_cmdWrite);
    if (W25QXX_TYPE == W25Q256)
    {
        FLASH_ReadWriteByte((uint8_t)((Dst_Addr) >> 24),Flash_cmdWrite); 
    }
    FLASH_ReadWriteByte((uint8_t)((Dst_Addr) >> 16),Flash_cmdWrite);
    FLASH_ReadWriteByte((uint8_t)((Dst_Addr) >> 8),Flash_cmdWrite);   
    FLASH_ReadWriteByte((uint8_t)Dst_Addr,Flash_cmdWrite);  
	FLASH_DISABLE();
    W25QXX_Wait_Idle();
}

//********************************************************************************************************
// Function name	: W25QXX_Wait_Idle
// Description		: �ȴ�����
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void W25QXX_Wait_Idle(void)
{   
	while((W25QXX_ReadSR(1)&0x01) == 0x01);   						// �ȴ�BUSYλ���					//
}

//********************************************************************************************************
// Function name	: W25QXX_PowerDown
// Description		: �������ģʽ
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void W25QXX_PowerDown(void)   
{ 
  	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_PowerDown,Flash_cmdWrite);
	FLASH_DISABLE();
	CyDelayUs(3);
}

//********************************************************************************************************
// Function name	: W25QXX_WAKEUP
// Description		: ����
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void W25QXX_WAKEUP(void)   
{  
  	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_ReleasePowerDown,Flash_cmdWrite);
	FLASH_DISABLE();
    CyDelayUs(3);
}

//******************************************** END OF FILE *********************************************//
