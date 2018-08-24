//********************************************************************************************************
// File name		: flash_spi.c
// Description		: 模拟flash_spi的相关函数
// Author			: Keno
// Date				: 2016.11.14
// Version			: V02
//********************************************************************************************************
#include "flash_spi.h"


//********************************************************************************************************
// Function name	: FLASH_FLASH_SPI_Init
// Description		: FLASH_FLASH_SPI的初始化
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
void FLASH_SPI_Init(void)
{
	FLASH_DISABLE();
	FLASH_SCK_RESET();
	FLASH_MOSI_RESET();
}

//********************************************************************************************************
// Function name	: FLASH_SPI_Send_Dat
// Description		: 发送一字节数据
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
static void FLASH_SPI_Send_Dat(INT8U dat)
{
	INT8U i;
	for (i = 0; i < 8; i++)
	{
		FLASH_SCK_RESET();
		CyDelayUs(1);
		if (dat & 0x80)
			FLASH_MOSI_SET();
		else
			FLASH_MOSI_RESET();
		CyDelayUs(1);
		dat <<= 1;
		FLASH_SCK_SET();
		CyDelayUs(2);
	}
	FLASH_SCK_RESET();
	CyDelayUs(10);
}

//********************************************************************************************************
// Function name	: FLASH_SPI_Receive_Dat
// Description		: 接收一字节数据
// Parameter		: none
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
static INT8U FLASH_SPI_Receive_Dat(void)
{
	INT8U i ,dat = 0;
	for (i = 0; i < 8; i++)
	{
		FLASH_SCK_RESET();
		CyDelayUs(1);
		dat <<= 1;
		if(FLASH_MISO_Read())
			dat |= 0x01;
		else
			dat &= 0xfe;
		CyDelayUs(1);
		FLASH_SCK_SET();
		CyDelayUs(2);
	}
	FLASH_SCK_RESET();
	CyDelayUs(10);
	return dat;
}

//********************************************************************************************************
// Function name	: flash_spi_SetReg
// Description		: 设置寄存器
// Parameter		: ucRegAddr	: 地址
//					: ucRegVal	: 数据
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t flash_spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal)
{
	FLASH_ENABLE();
	FLASH_SPI_Send_Dat(ucRegAddr << 1);
	FLASH_SPI_Send_Dat(ucRegVal);
	FLASH_DISABLE();
    return TRUE;
}

//********************************************************************************************************
// Function name	: flash_spi_SetReg
// Description		: 读取寄存器
// Parameter		: ucRegAddr	: 地址
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t flash_spi_GetReg(uint8_t ucRegAddr)
{
	uint8_t ucRegVal;
	FLASH_ENABLE();
	FLASH_SPI_Send_Dat(0x80 | (ucRegAddr << 1));
	//FLASH_SPI_Send_Dat(0x00);
	ucRegVal = FLASH_SPI_Receive_Dat();
	FLASH_DISABLE();
    return ucRegVal;
}

//********************************************************************************************************
// Function name	: flash_spi_SetReg
// Description		: 读取寄存器
// Parameter		: ucRegAddr	: 地址
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
//********************************************************************************************************
uint8_t flash_spi_GetReg2(uint8_t ucRegAddr, uint8_t *p)
{
	FLASH_ENABLE();
	FLASH_SPI_Send_Dat(0x80 | (ucRegAddr << 1));
	//FLASH_SPI_Send_Dat(0x00);
	*p = FLASH_SPI_Receive_Dat();
	FLASH_DISABLE();
    return TRUE;
}

/*********************************************************************************************************
// Function name	: FLASH_SPIRead_Sequence
// Description		: FLASH_SPI读FIFO寄存器的值
// Parameter		: sequence_length 	: 数据长度
//					: ucRegAddr			: 寄存器地址
//					: *reg_value		: 数据指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
*********************************************************************************************************/
void FLASH_SPIRead_Sequence(uint8_t sequence_length, uint8_t ucRegAddr, uint8_t *reg_value)
{
	uint8_t i;
	if (sequence_length == 0)
		return;
	for (i = 0; i < sequence_length; i++)
	{
		*(reg_value + i) = flash_spi_GetReg(ucRegAddr);
	}
	return;
}

/*********************************************************************************************************
// Function name	: FLASH_SPIWrite_Sequence
// Description		: FLASH_SPI写FIFO的值
// Parameter		: sequence_length 数据长度 
//					: ucRegAddr：寄存器地址  
//					: *reg_value 数据指针
// Return			: none
//--------------------------------------------------------------------------------------------------------
// Info				: 
*********************************************************************************************************/
void FLASH_SPIWrite_Sequence(uint8_t sequence_length, uint8_t ucRegAddr, uint8_t *reg_value)
{
	uint8_t i;
	if (sequence_length == 0)
		return;
	for (i = 0; i < sequence_length; i++)
	{
		flash_spi_SetReg(ucRegAddr, *(reg_value + i));
	}
	return;
}

/*********************************************************************************************************
// Function name	: FLASH_ReadWriteByte
// Description		: w25qxx 命令值
// Parameter		: uint8_t txdata 
//					:   
//					: 
// Return			: uint8_t
//--------------------------------------------------------------------------------------------------------
// Info				: 
*********************************************************************************************************/
#if 1
uint8_t FLASH_ReadWriteByte(uint8_t txdata, uint8 cmd_flag)
{
	if (cmd_flag == Flash_cmdRead)					
	{
		return FLASH_SPI_Receive_Dat();					// 如果cmd_flag 是1 表示读取指令 
	}
	else
	{
		FLASH_SPI_Send_Dat(txdata);						// 如果cmd_flag 是0 表示写入指令 
		return 0;
	}
}
#endif

//******************************************** END OF FILE *********************************************//
