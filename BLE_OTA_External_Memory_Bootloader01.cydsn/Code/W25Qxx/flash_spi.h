//********************************************************************************************************
// File name		: flash_spi.h
// Description 		: flash_spi.c的头文件
// Author			: Keno
// Date				: 2016.11.14
// Version			: V02
//********************************************************************************************************
#if !defined(__FLASH_SPI_H__)
#define __FLASH_SPI_H__


//******************************************** 头文件声明 **********************************************//
#include "config.h"
#include "FLASH_MISO.h"
#include "FLASH_MOSI.h"
#include "FLASH_NCS.h"
#include "FLASH_SCK.h"



//********************************************* 引脚定义 ***********************************************//
#define	FLASH_ENABLE()					FLASH_NCS_Write(0)
#define	FLASH_DISABLE() 				FLASH_NCS_Write(1)

#define	FLASH_SCK_SET()					FLASH_SCK_Write(1)
#define	FLASH_SCK_RESET() 				FLASH_SCK_Write(0)

#define	FLASH_MOSI_SET()				FLASH_MOSI_Write(1)
#define	FLASH_MOSI_RESET() 				FLASH_MOSI_Write(0)

#define FLASH_MISO_Read()				FLASH_MISO_Read()



//********************************************* 函数声明 ***********************************************//
void 	FLASH_SPI_Init(void);
uint8_t flash_spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal);
uint8_t flash_spi_GetReg(uint8_t ucRegAddr);
uint8_t flash_spi_GetReg2(uint8_t ucRegAddr, uint8_t *p);
void 	FLASH_SPIRead_Sequence(uint8_t sequence_length, uint8_t ucRegAddr, uint8_t *reg_value);
void 	FLASH_SPIWrite_Sequence(uint8_t sequence_length, uint8_t ucRegAddr, uint8_t *reg_value);

uint8_t FLASH_ReadWriteByte(uint8_t txdata);

#endif

//******************************************** END OF FILE *********************************************//
