/*******************************************************************************
* File Name: DBG_SOFT.h
* Version 1.50
*
* Description:
*  This file provides constants and parameter values for the Software Transmit
*  UART Component.
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CY_SW_TX_UART_DBG_SOFT_H
#define CY_SW_TX_UART_DBG_SOFT_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"

#define DBG_SOFT_BAUD_RATE                      (115200u)
#define DBG_SOFT_PIN_STATIC_MODE                (1u)


/***************************************
*        Function Prototypes
***************************************/
#if(DBG_SOFT_PIN_STATIC_MODE == 1u)
    void DBG_SOFT_Start(void) ;
#else
    void DBG_SOFT_StartEx(uint8 port, uint8 pin) ;
#endif /* (DBG_SOFT_PIN_STATIC_MODE == 1u) */

void DBG_SOFT_Stop(void) ;
void DBG_SOFT_PutChar(uint8 txDataByte) ;
void DBG_SOFT_PutString(const char8 string[]) ;
void DBG_SOFT_PutArray(const uint8 array[], uint32 byteCount) ;
void DBG_SOFT_PutHexByte(uint8 txHexByte) ;
void DBG_SOFT_PutHexInt(uint16 txHexInt) ;
void DBG_SOFT_PutCRLF(void) ;

#endif /* CY_SW_TX_UART_DBG_SOFT_H */


/* [] END OF FILE */
