//********************************************************************************************************
// File name		: w25qxx.h
// Description 		: w25qxx.c的头文件
// Author			: Keno
// Date				: 2016.11.14
// Version			: V02
//********************************************************************************************************
#ifndef __W25QXX_H__
#define __W25QXX_H__


//******************************************** 头文件声明 **********************************************//
#include <project.h>
#include "flash_spi.h"
	

//********************************************* 型号信息 ***********************************************//
//W25X系列/Q系列芯片列表
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16
//W25Q128 ID  0XEF17
//W25Q256 ID  0XEF18
//******************************************************************************************************//
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

#define W25QXX_TYPE						W25Q80
	
	
//********************************************** 指令表 ************************************************//
#define W25X_WriteEnable				0x06 
#define W25X_WriteDisable				0x04 
#define W25X_ReadStatusReg1				0x05 
#define W25X_ReadStatusReg2				0x35 
#define W25X_ReadStatusReg3				0x15 
#define W25X_WriteStatusReg1    		0x01 
#define W25X_WriteStatusReg2    		0x31 
#define W25X_WriteStatusReg3    		0x11 
#define W25X_ReadData					0x03 
#define W25X_FastReadData				0x0B 
#define W25X_FastReadDual				0x3B 
#define W25X_PageProgram				0x02 
#define W25X_BlockErase					0xD8 
#define W25X_SectorErase				0x20 
#define W25X_ChipErase					0xC7 
#define W25X_PowerDown					0xB9 
#define W25X_ReleasePowerDown			0xAB 
#define W25X_DeviceID					0xAB 
#define W25X_ManufactDeviceID			0x90 
#define W25X_JedecDeviceID				0x9F 
#define W25X_Enable4ByteAddr    		0xB7
#define W25X_Exit4ByteAddr      		0xE9
	
	
//********************************************* 基本信息 ***********************************************//
#define	W25X_PAGE_SIZE					(0x100)		
#define W25X_SECTOR_SIZE				(16 * W25X_PAGE_SIZE)
#define W25X_BLOCK_SIZE					(16 * W25X_SECTOR_SIZE)

#define W25X_START_ADDR					(0x0)							// 起始地址						//
#define W25X_END_ADDR					(0x100000)						// 结束地址						//


//********************************************* 基本信息 ***********************************************//
void 		W25QXX_Init(void);
uint16_t	W25QXX_ReadID(void);  	    							// 读取FLASH ID						//
uint8_t		W25QXX_ReadSR(uint8_t regno);							// 读取状态寄存器 					//
void W25QXX_4ByteAddr_Enable(void);     							// 使能4字节地址模式				//
void W25QXX_Write_SR(uint8_t regno,uint8_t sr);						// 写状态寄存器						//
void W25QXX_Write_Enable(void);  									// 写使能 							//
void W25QXX_Write_Disable(void);									// 写保护							//
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25QXX_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   		//读取flash			//
void W25QXX_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);		//写入flash			//
void W25QXX_Erase_Chip(void);										// 整片擦除							//
void W25QXX_Erase_Sector(uint32_t Dst_Addr);						// 扇区擦除							//
void W25QXX_Wait_Idle(void);										// 等待空闲							//
void W25QXX_PowerDown(void);										// 进入掉电模式						//
void W25QXX_WAKEUP(void);											// 唤醒								//


#endif


//******************************************** END OF FILE *********************************************//
