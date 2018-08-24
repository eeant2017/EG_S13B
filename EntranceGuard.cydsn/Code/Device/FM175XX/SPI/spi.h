//********************************************************************************************************
// File name		: spi.h
// Description 		: spi.c的头文件
// Author			: Keno
// Date				: 2016.11.14
// Version			: V02
//********************************************************************************************************
#if !defined(__SPI_H__)
#define __SPI_H__


//******************************************** 头文件声明 **********************************************//
#include "common.h"
#include "config.h"
#include "FM_MISO.h"
#include "FM_MOSI.h"
#include "FM_NCS.h"
#include "FM_SCK.h"



//********************************************* 引脚定义 ***********************************************//
#define	SPI_ENABLE()					FM_NCS_Write(0)
#define	SPI_DISABLE() 					FM_NCS_Write(1)

#define	SPI_SCK_SET()					FM_SCK_Write(1)
#define	SPI_SCK_RESET() 				FM_SCK_Write(0)

#define	SPI_MOSI_SET()					FM_MOSI_Write(1)
#define	SPI_MOSI_RESET() 				FM_MOSI_Write(0)

#define SPI_MISO_Read()					FM_MISO_Read()



//********************************************* 函数声明 ***********************************************//
void SPI_Init(void);
uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal);
uint8_t spi_GetReg(uint8_t ucRegAddr);
uint8_t spi_GetReg2(uint8_t ucRegAddr, uint8_t *p);
void SPIRead_Sequence(uint8_t sequence_length, uint8_t ucRegAddr, uint8_t *reg_value);
void SPIWrite_Sequence(uint8_t sequence_length, uint8_t ucRegAddr, uint8_t *reg_value);


#endif

//******************************************** END OF FILE *********************************************//
