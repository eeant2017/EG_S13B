#ifndef FM175X_H
#define FM175X_H

#include <stdint.h>
#include "lpcd_reg.h"
#include "lpcd_api.h"
#include "lpcd_regctrl.h"


//********************************************* 引脚定义 ***********************************************//
#define CD_SetTPD()			FM_RST_Write(1)
#define CD_ClrTPD()			FM_RST_Write(0)

#define SPI_INT_Read()		FM_INT_Read()
	
	
#define Anticollision		0x02

#define CommandReg			0x01
#define ComIEnReg			0x02
#define DivIEnReg			0x03
#define ComIrqReg			0x04
#define DivIrqReg			0x05
#define ErrorReg			0x06
#define Status1Reg			0x07
#define Status2Reg			0x08
#define FIFODataReg			0x09
#define FIFOLevelReg		0x0A
#define WaterLevelReg		0x0B
#define ControlReg			0x0C
#define BitFramingReg		0x0D
#define CollReg				0x0E
#define ModeReg				0x11
#define TxModeReg			0x12
#define RxModeReg			0x13
#define TxControlReg		0x14
#define TxAutoReg			0x15
#define TxSelReg			0x16
#define RxSelReg			0x17
#define RxThresholdReg		0x18
#define DemodReg			0x19
#define MfTxReg				0x1C
#define MfRxReg				0x1D
#define SerialSpeedReg		0x1F
#define CRCMSBReg			0x21
#define CRCLSBReg			0x22
#define ModWidthReg			0x24
#define GsNOffReg			0x23
#define TxBitPhaseReg		0x25
#define RFCfgReg			0x26
#define GsNReg				0x27
#define CWGsPReg			0x28
#define ModGsPReg			0x29
#define TModeReg			0x2A
#define TPrescalerReg		0x2B
#define TReloadMSBReg		0x2C
#define TReloadLSBReg		0x2D
#define TCounterValMSBReg	0x2E
#define TCounterValLSBReg	0x2F
#define TestSel1Reg			0x31
#define TestSel2Reg			0x32
#define TestPinEnReg		0x33
#define TestPinValueReg		0x34
#define TestBusReg			0x35
#define AutoTestReg			0x36
#define VersionReg			0x37
#define AnalogTestReg		0x38
#define TestDAC1Reg			0x39
#define TestDAC2Reg			0x3A
#define TestADCReg			0x3B

#define Idle				0x00
#define Mem					0x01
#define RandomID			0x02
#define CalcCRC				0x03
#define Transmit			0x04
#define NoCmdChange			0x07
#define Receive				0x08
#define Transceive			0x0C
#define MFAuthent			0x0E
#define SoftReset			0x0F
	

extern uint8_t  FM175XX_IRQ;
	

void	 pcd_Init(void);
void	 pcd_RST(void);
uint8_t  MCU_TO_PCD_TEST(void);
uint8_t  Pcd_Comm( uint8_t  Command, uint8_t  *pInData, uint8_t  InLenByte, uint8_t  *pOutData, uint32_t *pOutLenBit);
uint8_t  Set_Rf(uint8_t  mode);
uint8_t  Read_Reg(uint8_t  reg_add);
uint8_t  Read_Reg_All(uint8_t  *reg_value);
uint8_t  Write_Reg(uint8_t  reg_add,uint8_t  reg_value);
void	 Write_FIFO(uint8_t  length,uint8_t  *fifo_data);
void	 Read_FIFO(uint8_t  length,uint8_t  *fifo_data);
uint8_t  Clear_FIFO(void);
uint8_t  Set_BitMask(uint8_t  reg_add,uint8_t  mask);
uint8_t  Clear_BitMask(uint8_t  reg_add,uint8_t  mask);
uint8_t  Pcd_ConfigISOType(uint8_t  type);
uint8_t  FM175X_SoftReset(void);
uint8_t  FM175X_HardReset(void);
uint8_t  FM175X_SoftPowerdown(void);
uint8_t  FM175X_HardPowerdown(void);
uint8_t  Pcd_SetTimer(unsigned long delaytime);
uint8_t	 Read_Ext_Reg(uint8_t  reg_add);
uint8_t	 Write_Ext_Reg(uint8_t  reg_add,uint8_t  reg_value);
void	 NRSTPD_CTRL(uint8_t  ctrltype);
uint8_t  FM175XX_Initial(void);                         

void 	 nfc_init(void);

uint8_t  FM175XX_LPCD_InitConfig(void);
uint8_t  FM175XX_LPCD_WakeUpHandler(uint8_t *p);
uint8_t  FM175XX_ReadCardUid(uint8_t *p);

uint8_t nfc_detect_type_a(uint8_t *card_id, uint16 sz);
uint8_t nfc_detect_type_b(uint8_t *card_id, uint16 sz);


#endif
