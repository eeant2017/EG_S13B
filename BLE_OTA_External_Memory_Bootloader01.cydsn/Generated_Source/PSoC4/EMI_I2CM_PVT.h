/***************************************************************************//**
* \file .h
* \version 3.20
*
* \brief
*  This private file provides constants and parameter values for the
*  SCB Component.
*  Please do not use this file or its content in your project.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PVT_EMI_I2CM_H)
#define CY_SCB_PVT_EMI_I2CM_H

#include "EMI_I2CM.h"


/***************************************
*     Private Function Prototypes
***************************************/

/* APIs to service INTR_I2C_EC register */
#define EMI_I2CM_SetI2CExtClkInterruptMode(interruptMask) EMI_I2CM_WRITE_INTR_I2C_EC_MASK(interruptMask)
#define EMI_I2CM_ClearI2CExtClkInterruptSource(interruptMask) EMI_I2CM_CLEAR_INTR_I2C_EC(interruptMask)
#define EMI_I2CM_GetI2CExtClkInterruptSource()                (EMI_I2CM_INTR_I2C_EC_REG)
#define EMI_I2CM_GetI2CExtClkInterruptMode()                  (EMI_I2CM_INTR_I2C_EC_MASK_REG)
#define EMI_I2CM_GetI2CExtClkInterruptSourceMasked()          (EMI_I2CM_INTR_I2C_EC_MASKED_REG)

#if (!EMI_I2CM_CY_SCBIP_V1)
    /* APIs to service INTR_SPI_EC register */
    #define EMI_I2CM_SetSpiExtClkInterruptMode(interruptMask) \
                                                                EMI_I2CM_WRITE_INTR_SPI_EC_MASK(interruptMask)
    #define EMI_I2CM_ClearSpiExtClkInterruptSource(interruptMask) \
                                                                EMI_I2CM_CLEAR_INTR_SPI_EC(interruptMask)
    #define EMI_I2CM_GetExtSpiClkInterruptSource()                 (EMI_I2CM_INTR_SPI_EC_REG)
    #define EMI_I2CM_GetExtSpiClkInterruptMode()                   (EMI_I2CM_INTR_SPI_EC_MASK_REG)
    #define EMI_I2CM_GetExtSpiClkInterruptSourceMasked()           (EMI_I2CM_INTR_SPI_EC_MASKED_REG)
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

#if(EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    extern void EMI_I2CM_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask);
#endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Vars with External Linkage
***************************************/

#if (EMI_I2CM_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_EMI_I2CM_CUSTOM_INTR_HANDLER)
    extern cyisraddress EMI_I2CM_customIntrHandler;
#endif /* !defined (CY_REMOVE_EMI_I2CM_CUSTOM_INTR_HANDLER) */
#endif /* (EMI_I2CM_SCB_IRQ_INTERNAL) */

extern EMI_I2CM_BACKUP_STRUCT EMI_I2CM_backup;

#if(EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    extern uint8 EMI_I2CM_scbMode;
    extern uint8 EMI_I2CM_scbEnableWake;
    extern uint8 EMI_I2CM_scbEnableIntr;

    /* I2C configuration variables */
    extern uint8 EMI_I2CM_mode;
    extern uint8 EMI_I2CM_acceptAddr;

    /* SPI/UART configuration variables */
    extern volatile uint8 * EMI_I2CM_rxBuffer;
    extern uint8   EMI_I2CM_rxDataBits;
    extern uint32  EMI_I2CM_rxBufferSize;

    extern volatile uint8 * EMI_I2CM_txBuffer;
    extern uint8   EMI_I2CM_txDataBits;
    extern uint32  EMI_I2CM_txBufferSize;

    /* EZI2C configuration variables */
    extern uint8 EMI_I2CM_numberOfAddr;
    extern uint8 EMI_I2CM_subAddrSize;
#endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */

#if (! (EMI_I2CM_SCB_MODE_I2C_CONST_CFG || \
        EMI_I2CM_SCB_MODE_EZI2C_CONST_CFG))
    extern uint16 EMI_I2CM_IntrTxMask;
#endif /* (! (EMI_I2CM_SCB_MODE_I2C_CONST_CFG || \
              EMI_I2CM_SCB_MODE_EZI2C_CONST_CFG)) */


/***************************************
*        Conditional Macro
****************************************/

#if(EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Defines run time operation mode */
    #define EMI_I2CM_SCB_MODE_I2C_RUNTM_CFG     (EMI_I2CM_SCB_MODE_I2C      == EMI_I2CM_scbMode)
    #define EMI_I2CM_SCB_MODE_SPI_RUNTM_CFG     (EMI_I2CM_SCB_MODE_SPI      == EMI_I2CM_scbMode)
    #define EMI_I2CM_SCB_MODE_UART_RUNTM_CFG    (EMI_I2CM_SCB_MODE_UART     == EMI_I2CM_scbMode)
    #define EMI_I2CM_SCB_MODE_EZI2C_RUNTM_CFG   (EMI_I2CM_SCB_MODE_EZI2C    == EMI_I2CM_scbMode)
    #define EMI_I2CM_SCB_MODE_UNCONFIG_RUNTM_CFG \
                                                        (EMI_I2CM_SCB_MODE_UNCONFIG == EMI_I2CM_scbMode)

    /* Defines wakeup enable */
    #define EMI_I2CM_SCB_WAKE_ENABLE_CHECK       (0u != EMI_I2CM_scbEnableWake)
#endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */

/* Defines maximum number of SCB pins */
#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_SCB_PINS_NUMBER    (7u)
#else
    #define EMI_I2CM_SCB_PINS_NUMBER    (2u)
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

#endif /* (CY_SCB_PVT_EMI_I2CM_H) */


/* [] END OF FILE */
