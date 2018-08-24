//********************************************************************************************************
// File name		: spi.c
// Description		: 模拟spi的相关函数
// Author			: Keno
// Date				: 2016.11.14
// Version			: V02
//********************************************************************************************************
#include "spi.h"


//********************************************************************************************************
// Function name	: SPI_Init
// Description		: SPI的初始化
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void SPI_Init(void)
{
	SPI_DISABLE();
	SPI_SCK_RESET();
	SPI_MOSI_RESET();
}

//********************************************************************************************************
// Function name	: SPI_Send_Dat
// Description		: 发送一字节数据
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
static void SPI_Send_Dat(INT8U dat)
{
	INT8U i;
	for (i = 0; i < 8; i++)
	{
		SPI_SCK_RESET();
		CyDelayUs(1);
		if (dat & 0x80)
			SPI_MOSI_SET();
		else
			SPI_MOSI_RESET();
		CyDelayUs(1);
		dat <<= 1;
		SPI_SCK_SET();
		CyDelayUs(2);
	}
	SPI_SCK_RESET();
	CyDelayUs(10);
}

//********************************************************************************************************
// Function name	: SPI_Receive_Dat
// Description		: 接收一字节数据
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
static INT8U SPI_Receive_Dat(void)
{
	INT8U i ,dat = 0;
	for (i = 0; i < 8; i++)
	{
		SPI_SCK_RESET();
		CyDelayUs(1);
		dat <<= 1;
		if(SPI_MISO_Read())
			dat |= 0x01;
		else
			dat &= 0xfe;
		CyDelayUs(1);
		SPI_SCK_SET();
		CyDelayUs(2);
	}
	SPI_SCK_RESET();
	CyDelayUs(10);
	return dat;
}

//********************************************************************************************************
// Function name	: spi_SetReg
// Description		: 设置寄存器
// Parameter		: ucRegAddr	: 地址
//					: ucRegVal	: 数据
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal)
{
	SPI_ENABLE();
	SPI_Send_Dat(ucRegAddr << 1);
	SPI_Send_Dat(ucRegVal);
	SPI_DISABLE();
    return TRUE;
}

//********************************************************************************************************
// Function name	: spi_SetReg
// Description		: 读取寄存器
// Parameter		: ucRegAddr	: 地址
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t spi_GetReg(uint8_t ucRegAddr)
{
	uint8_t ucRegVal;
	SPI_ENABLE();
	SPI_Send_Dat(0x80 | (ucRegAddr << 1));
	//SPI_Send_Dat(0x00);
	ucRegVal = SPI_Receive_Dat();
	SPI_DISABLE();
    return ucRegVal;
}

//********************************************************************************************************
// Function name	: spi_SetReg
// Description		: 读取寄存器
// Parameter		: ucRegAddr	: 地址
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t spi_GetReg2(uint8_t ucRegAddr, uint8_t *p)
{
	SPI_ENABLE();
	SPI_Send_Dat(0x80 | (ucRegAddr << 1));
	//SPI_Send_Dat(0x00);
	*p = SPI_Receive_Dat();
	SPI_DISABLE();
    return TRUE;
}

/*********************************************************************************************************
// Function name	: SPIRead_Sequence
// Description		: SPI读FIFO寄存器的值
// Parameter		: sequence_length 	: 数据长度
//					: ucRegAddr			: 寄存器地址
//					: *reg_value		: 数据指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
*********************************************************************************************************/
void SPIRead_Sequence(uint8_t sequence_length, uint8_t ucRegAddr, uint8_t *reg_value)
{
	uint8_t i;
	if (sequence_length == 0)
		return;
	for (i = 0; i < sequence_length; i++)
	{
		*(reg_value + i) = spi_GetReg(ucRegAddr);
	}
	return;
}

/*********************************************************************************************************
// Function name	: SPIWrite_Sequence
// Description		: SPI写FIFO的值
// Parameter		: sequence_length 数据长度 
//					: ucRegAddr：寄存器地址  
//					: *reg_value 数据指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
*********************************************************************************************************/
void SPIWrite_Sequence(uint8_t sequence_length, uint8_t ucRegAddr, uint8_t *reg_value)
{
	uint8_t i;
	if (sequence_length == 0)
		return;
	for (i = 0; i < sequence_length; i++)
	{
		spi_SetReg(ucRegAddr, *(reg_value + i));
	}
	return;
}

