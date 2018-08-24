#ifndef LPCD_API_H
#define LPCD_API_H

#include <stdint.h>

uint8_t  ReadLpcdADCResult(uint8_t  *ADCResult);
uint8_t  LpcdParamInit(void);
uint8_t  LpcdRegisterInit(void);
uint8_t  LpcdInitCalibra(uint8_t  *CalibraFlag);
uint8_t  ADCResultCenter;        //LPCDÂö¿íÖÐÐÄµã
void LpcdCardIn_IRQHandler(void);
void LpcdAutoWakeUp_IRQHandler(void);
uint8_t  LpcdAuxSelect(uint8_t  OpenClose);
//extern uint8_t  LpcdAutoTest(void);

#endif


