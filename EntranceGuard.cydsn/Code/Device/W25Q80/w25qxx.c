//********************************************************************************************************
// File name		: w25qxx.c
// Description		: W25Qxx的驱动函数
// Author			: Keno
// Date				: 2016.11.14
// Version			: V02
// -------------------------------------------------------------------------------------------------------
// W25Q80信息		: 4K Byte为一个扇区(Sector)，共有2048个扇区
//					: 16个扇区为一个块(Block)，共有128个块
//********************************************************************************************************
#include "w25qxx.h"


//******************************************* 外部函数声明 *********************************************//
extern void ble_heartbeat(void);


//********************************************************************************************************
// Function name	: SPI2_Init
// Description		: 初始化SPI的IO口
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
// Description		: 初始化SPI的IO口
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void W25QXX_Init(void)
{ 
    uint8_t temp;
    uint16_t id;
	
	FLASH_DISABLE();			            						// 取消片选							//
	SPI2_Init();		   			        						// 初始化SPI的IO口					//
	id = W25QXX_ReadID();	        								// 读取FLASH ID						//
    if (id == W25Q80)												// SPI FLASH为W25Q80				//
    {
        temp = W25QXX_ReadSR(3);              						// 读取状态寄存器3，判断地址模式	//
        if ((temp & 0X01) == 0)			        					// 判断是否为4字节地址模式			//
		{
			FLASH_ENABLE(); 			        					// 使能片选							//
			FLASH_ReadWriteByte(W25X_Enable4ByteAddr,Flash_cmdWrite);				// 发送进入4字节地址模式指令 		//  
			FLASH_DISABLE();										// 取消片选							//
		}
    }
}

//********************************************************************************************************
// Function name	: W25QXX_ReadSR
// Description		: 读W25QXX状态寄存器
// Parameter		: regno	: 寄存器号
// Return			: 状态寄存器值
//--------------------------------------------------------------------------------------------------------
// Info				: 
//--------------------------------------------------------------------------------------------------------
//读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
//状态寄存器1：
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR: 默认0,状态寄存器保护位,配合WP使用
//TB, BP2, BP1, BP0: FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙; 0,空闲)
//默认:0x00
//
//状态寄存器2：
//BIT7  6   5   4   3   2   1    0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//
//状态寄存器3：
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
	FLASH_ReadWriteByte(command,Flash_cmdWrite);									// 发送读取状态寄存器命令  			//  
	byte = FLASH_ReadWriteByte(0xFF,Flash_cmdRead);          						// 读取一个字节  					//
	FLASH_DISABLE();
	return byte;
}

//********************************************************************************************************
// Function name	: W25QXX_Write_SR
// Description		: 写W25QXX状态寄存器
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
	FLASH_ReadWriteByte(command,Flash_cmdWrite);									// 发送写取状态寄存器命令    		//
	FLASH_ReadWriteByte(sr,Flash_cmdWrite);											// 写入一个字节						//
	FLASH_DISABLE();
}

//********************************************************************************************************
// Function name	: W25QXX_Write_Enable
// Description		: W25QXX写使能	
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 将WEL置位
//********************************************************************************************************
void W25QXX_Write_Enable(void)   
{
	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_WriteEnable,Flash_cmdWrite);   						// 发送写使能  						//
	FLASH_DISABLE();
}

//********************************************************************************************************
// Function name	: W25QXX_Write_Disable
// Description		: W25QXX写禁止
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 将WEL清零
//********************************************************************************************************
void W25QXX_Write_Disable(void)   
{  
	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_WriteDisable,Flash_cmdWrite);							// 发送写禁止指令    				//
	FLASH_DISABLE();
}

//********************************************************************************************************
// Function name	: W25QXX_Write_Disable
// Description		: 读取芯片ID
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint16_t W25QXX_ReadID(void)
{
	uint16_t Temp = 0;
	
	FLASH_ENABLE();				    
	FLASH_ReadWriteByte(W25X_ManufactDeviceID,Flash_cmdWrite);						// 发送读取ID命令					//
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
// Description		: 在指定地址开始读取指定长度的数据
// Parameter		: pBuffer		: 数据存储区
//					: ReadAddr		: 开始读取的地址(24bit)
//					: NumByteToRead	: 要读取的字节数(最大65535)
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{ 
 	uint16_t i;
	
	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_ReadData,Flash_cmdWrite);      						// 发送读取命令  					//
	// 如果是W25Q256的话，地址为4字节的，要发送最高8位													//
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
// Description		: 页内写数据
// Parameter		: pBuffer		: 数据存储区
//					: ReadAddr		: 开始写入的地址(24bit)
//					: NumByteToWrite: 要写入的字节数(最大256),不应该超过该页的剩余字节数
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
    FLASH_ReadWriteByte(W25X_PageProgram,Flash_cmdWrite);							// 发送写页命令   					//
	// 如果是W25Q256的话地址为4字节的，要发送最高8位													//
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
	W25QXX_Wait_Idle();					   							// 等待写入结束						//
	CyGlobalIntEnable;
}

//********************************************************************************************************
// Function name	: W25QXX_Write_Page
// Description		: 在指定地址开始写入指定长度的数据,但是要确保地址不越界!
// Parameter		: pBuffer		: 数据存储区
//					: WriteAddr		: 开始写入的地址(24bit)
//					: NumByteToWrite: 要写入的字节数(最大65535)
// Return			: none
//--------------------------------------------------------------------------------------------------------
//					: 具有自动换页功能
// Info				: 必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
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
			NumByteToWrite -= pageremain;			  				// 减去已经写入了的字节数			//
			if (NumByteToWrite > 256)
			{
				pageremain = 256; 									// 一次可以写入256个字节			//
			}
			else
			{
				pageremain = NumByteToWrite; 	  					// 不够256个字节					//
			}
		}
	};	    
}

//********************************************************************************************************
// Function name	: W25QXX_Write
// Description		: 在指定地址开始写入指定长度的数据
// Parameter		: pBuffer		: 数据存储区
//					: WriteAddr		: 开始写入的地址(24bit)
//					: NumByteToWrite: 要写入的字节数(最大65535)
// Return			: none
//--------------------------------------------------------------------------------------------------------
//					: 具有自动换页功能
// Info				: 必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//********************************************************************************************************
void W25QXX_Write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t W25QXX_BUFFER[4096] = {0};
	uint8_t *W25QXX_BUF = W25QXX_BUFFER;
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
 	uint16_t i;
	
 	secpos = WriteAddr / 4096;										// 扇区编号  						//
	secoff = WriteAddr % 4096;										// 在扇区内的偏移					//
	secremain = 4096 - secoff;										// 扇区剩余空间大小   				//
 	
 	if (NumByteToWrite <= secremain)								// 判断是否可以在本扇区写完			//
	{
		secremain = NumByteToWrite;									// 不大于4096个字节					//
	}
	while(1)
	{	
		W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096);				// 读出整个扇区的内容				//
		for (i = 0; i < secremain; i++)								// 待写的长度数据进行校验			//
		{
			if (W25QXX_BUF[secoff + i] != 0xFF)						// 判断是否需要擦除					// 
				break;
		}
		if (i < secremain)											// 需要擦除							//
		{
			W25QXX_Erase_Sector(secpos);							// 擦除这个扇区						//
			for (i = 0; i < secremain; i++)
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096);	// 写入整个扇区						//
		}
		else
		{
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain);	// 待写区域已擦除，直接写			//
		}
		
		if (NumByteToWrite == secremain)							// 写入结束							//
		{
			break;
		}
		else
		{
			secpos++;												// 扇区编号增1						//
			secoff = 0;												// 偏移位置为0 	 					//
			
		   	pBuffer += secremain;									// 指针偏移							//
			WriteAddr += secremain;									// 写地址偏移						//
		   	NumByteToWrite -= secremain;							// 字节数递减						//
			if (NumByteToWrite > 4096)								// 下一个扇区还是写不完				//
			{
				secremain = 4096;
			}
			else
			{
				secremain = NumByteToWrite;							// 下一个扇区可以写完				//
			}
		}	 
	}
}

//********************************************************************************************************
// Function name	: W25QXX_Erase_Chip
// Description		: 擦除整个芯片
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 等待时间超长
//********************************************************************************************************
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();
    W25QXX_Wait_Idle();
  	FLASH_ENABLE();
    FLASH_ReadWriteByte(W25X_ChipErase,Flash_cmdWrite);							// 发送片擦除命令					//
	FLASH_DISABLE();
	W25QXX_Wait_Idle();
}

//********************************************************************************************************
// Function name	: W25QXX_Erase_Sector
// Description		: 擦除一个扇区
// Parameter		: Dst_Addr	: 扇区地址 根据实际容量设置
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 擦除一个扇区的最少时间:150ms
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
// Description		: 等待空闲
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void W25QXX_Wait_Idle(void)
{   
	while((W25QXX_ReadSR(1)&0x01) == 0x01);   						// 等待BUSY位清空					//
}

//********************************************************************************************************
// Function name	: W25QXX_PowerDown
// Description		: 进入掉电模式
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
// Description		: 唤醒
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
