/* /////////////////////////////////////////////////////////////////////////////////////////////////
//                     Copyright (c) FMSH
///////////////////////////////////////////////////////////////////////////////////////////////// */

#ifndef LPCD_REGCTRL_H
#define LPCD_REGCTRL_H

#include <stdint.h>

uint8_t  GetReg_Ext(uint8_t  ExtRegAddr,uint8_t * ExtRegData);
uint8_t  SetReg_Ext(uint8_t  ExtRegAddr,uint8_t  ExtRegData);
uint8_t  ModifyReg_Ext(uint8_t  ExtRegAddr,uint8_t  mask,uint8_t  set);
uint8_t  GetReg_Ext(uint8_t  ExtRegAddr,uint8_t * ExtRegData);


#endif

