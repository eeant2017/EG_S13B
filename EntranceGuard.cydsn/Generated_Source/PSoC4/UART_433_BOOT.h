/***************************************************************************//**
* \file UART_433_BOOT.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values of the bootloader
*  communication APIs for the SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2014-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_BOOT_UART_433_H)
#define CY_SCB_BOOT_UART_433_H

#include "UART_433_PVT.h"

#if (UART_433_SCB_MODE_I2C_INC)
    #include "UART_433_I2C.h"
#endif /* (UART_433_SCB_MODE_I2C_INC) */

#if (UART_433_SCB_MODE_EZI2C_INC)
    #include "UART_433_EZI2C.h"
#endif /* (UART_433_SCB_MODE_EZI2C_INC) */

#if (UART_433_SCB_MODE_SPI_INC || UART_433_SCB_MODE_UART_INC)
    #include "UART_433_SPI_UART.h"
#endif /* (UART_433_SCB_MODE_SPI_INC || UART_433_SCB_MODE_UART_INC) */


/***************************************
*  Conditional Compilation Parameters
****************************************/

/* Bootloader communication interface enable */
#define UART_433_BTLDR_COMM_ENABLED ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UART_433) || \
                                             (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))

/* Enable I2C bootloader communication */
#if (UART_433_SCB_MODE_I2C_INC)
    #define UART_433_I2C_BTLDR_COMM_ENABLED     (UART_433_BTLDR_COMM_ENABLED && \
                                                            (UART_433_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             UART_433_I2C_SLAVE_CONST))
#else
     #define UART_433_I2C_BTLDR_COMM_ENABLED    (0u)
#endif /* (UART_433_SCB_MODE_I2C_INC) */

/* EZI2C does not support bootloader communication. Provide empty APIs */
#if (UART_433_SCB_MODE_EZI2C_INC)
    #define UART_433_EZI2C_BTLDR_COMM_ENABLED   (UART_433_BTLDR_COMM_ENABLED && \
                                                         UART_433_SCB_MODE_UNCONFIG_CONST_CFG)
#else
    #define UART_433_EZI2C_BTLDR_COMM_ENABLED   (0u)
#endif /* (UART_433_EZI2C_BTLDR_COMM_ENABLED) */

/* Enable SPI bootloader communication */
#if (UART_433_SCB_MODE_SPI_INC)
    #define UART_433_SPI_BTLDR_COMM_ENABLED     (UART_433_BTLDR_COMM_ENABLED && \
                                                            (UART_433_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             UART_433_SPI_SLAVE_CONST))
#else
        #define UART_433_SPI_BTLDR_COMM_ENABLED (0u)
#endif /* (UART_433_SPI_BTLDR_COMM_ENABLED) */

/* Enable UART bootloader communication */
#if (UART_433_SCB_MODE_UART_INC)
       #define UART_433_UART_BTLDR_COMM_ENABLED    (UART_433_BTLDR_COMM_ENABLED && \
                                                            (UART_433_SCB_MODE_UNCONFIG_CONST_CFG || \
                                                             (UART_433_UART_RX_DIRECTION && \
                                                              UART_433_UART_TX_DIRECTION)))
#else
     #define UART_433_UART_BTLDR_COMM_ENABLED   (0u)
#endif /* (UART_433_UART_BTLDR_COMM_ENABLED) */

/* Enable bootloader communication */
#define UART_433_BTLDR_COMM_MODE_ENABLED    (UART_433_I2C_BTLDR_COMM_ENABLED   || \
                                                     UART_433_SPI_BTLDR_COMM_ENABLED   || \
                                                     UART_433_EZI2C_BTLDR_COMM_ENABLED || \
                                                     UART_433_UART_BTLDR_COMM_ENABLED)


/***************************************
*        Function Prototypes
***************************************/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (UART_433_I2C_BTLDR_COMM_ENABLED)
    /* I2C Bootloader physical layer functions */
    void UART_433_I2CCyBtldrCommStart(void);
    void UART_433_I2CCyBtldrCommStop (void);
    void UART_433_I2CCyBtldrCommReset(void);
    cystatus UART_433_I2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus UART_433_I2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map I2C specific bootloader communication APIs to SCB specific APIs */
    #if (UART_433_SCB_MODE_I2C_CONST_CFG)
        #define UART_433_CyBtldrCommStart   UART_433_I2CCyBtldrCommStart
        #define UART_433_CyBtldrCommStop    UART_433_I2CCyBtldrCommStop
        #define UART_433_CyBtldrCommReset   UART_433_I2CCyBtldrCommReset
        #define UART_433_CyBtldrCommRead    UART_433_I2CCyBtldrCommRead
        #define UART_433_CyBtldrCommWrite   UART_433_I2CCyBtldrCommWrite
    #endif /* (UART_433_SCB_MODE_I2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (UART_433_I2C_BTLDR_COMM_ENABLED) */


#if defined(CYDEV_BOOTLOADER_IO_COMP) && (UART_433_EZI2C_BTLDR_COMM_ENABLED)
    /* Bootloader physical layer functions */
    void UART_433_EzI2CCyBtldrCommStart(void);
    void UART_433_EzI2CCyBtldrCommStop (void);
    void UART_433_EzI2CCyBtldrCommReset(void);
    cystatus UART_433_EzI2CCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus UART_433_EzI2CCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map EZI2C specific bootloader communication APIs to SCB specific APIs */
    #if (UART_433_SCB_MODE_EZI2C_CONST_CFG)
        #define UART_433_CyBtldrCommStart   UART_433_EzI2CCyBtldrCommStart
        #define UART_433_CyBtldrCommStop    UART_433_EzI2CCyBtldrCommStop
        #define UART_433_CyBtldrCommReset   UART_433_EzI2CCyBtldrCommReset
        #define UART_433_CyBtldrCommRead    UART_433_EzI2CCyBtldrCommRead
        #define UART_433_CyBtldrCommWrite   UART_433_EzI2CCyBtldrCommWrite
    #endif /* (UART_433_SCB_MODE_EZI2C_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (UART_433_EZI2C_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (UART_433_SPI_BTLDR_COMM_ENABLED)
    /* SPI Bootloader physical layer functions */
    void UART_433_SpiCyBtldrCommStart(void);
    void UART_433_SpiCyBtldrCommStop (void);
    void UART_433_SpiCyBtldrCommReset(void);
    cystatus UART_433_SpiCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus UART_433_SpiCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map SPI specific bootloader communication APIs to SCB specific APIs */
    #if (UART_433_SCB_MODE_SPI_CONST_CFG)
        #define UART_433_CyBtldrCommStart   UART_433_SpiCyBtldrCommStart
        #define UART_433_CyBtldrCommStop    UART_433_SpiCyBtldrCommStop
        #define UART_433_CyBtldrCommReset   UART_433_SpiCyBtldrCommReset
        #define UART_433_CyBtldrCommRead    UART_433_SpiCyBtldrCommRead
        #define UART_433_CyBtldrCommWrite   UART_433_SpiCyBtldrCommWrite
    #endif /* (UART_433_SCB_MODE_SPI_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (UART_433_SPI_BTLDR_COMM_ENABLED) */

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (UART_433_UART_BTLDR_COMM_ENABLED)
    /* UART Bootloader physical layer functions */
    void UART_433_UartCyBtldrCommStart(void);
    void UART_433_UartCyBtldrCommStop (void);
    void UART_433_UartCyBtldrCommReset(void);
    cystatus UART_433_UartCyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    cystatus UART_433_UartCyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);

    /* Map UART specific bootloader communication APIs to SCB specific APIs */
    #if (UART_433_SCB_MODE_UART_CONST_CFG)
        #define UART_433_CyBtldrCommStart   UART_433_UartCyBtldrCommStart
        #define UART_433_CyBtldrCommStop    UART_433_UartCyBtldrCommStop
        #define UART_433_CyBtldrCommReset   UART_433_UartCyBtldrCommReset
        #define UART_433_CyBtldrCommRead    UART_433_UartCyBtldrCommRead
        #define UART_433_CyBtldrCommWrite   UART_433_UartCyBtldrCommWrite
    #endif /* (UART_433_SCB_MODE_UART_CONST_CFG) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (UART_433_UART_BTLDR_COMM_ENABLED) */

/**
* \addtogroup group_bootloader
* @{
*/

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (UART_433_BTLDR_COMM_ENABLED)
    #if (UART_433_SCB_MODE_UNCONFIG_CONST_CFG)
        /* Bootloader physical layer functions */
        void UART_433_CyBtldrCommStart(void);
        void UART_433_CyBtldrCommStop (void);
        void UART_433_CyBtldrCommReset(void);
        cystatus UART_433_CyBtldrCommRead       (uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
        cystatus UART_433_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut);
    #endif /* (UART_433_SCB_MODE_UNCONFIG_CONST_CFG) */

    /* Map SCB specific bootloader communication APIs to common APIs */
    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UART_433)
        #define CyBtldrCommStart    UART_433_CyBtldrCommStart
        #define CyBtldrCommStop     UART_433_CyBtldrCommStop
        #define CyBtldrCommReset    UART_433_CyBtldrCommReset
        #define CyBtldrCommWrite    UART_433_CyBtldrCommWrite
        #define CyBtldrCommRead     UART_433_CyBtldrCommRead
    #endif /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_UART_433) */

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (UART_433_BTLDR_COMM_ENABLED) */

/** @} group_bootloader */

/***************************************
*           API Constants
***************************************/

/* Timeout unit in milliseconds */
#define UART_433_WAIT_1_MS  (1u)

/* Return number of bytes to copy into bootloader buffer */
#define UART_433_BYTES_TO_COPY(actBufSize, bufSize) \
                            ( ((uint32)(actBufSize) < (uint32)(bufSize)) ? \
                                ((uint32) (actBufSize)) : ((uint32) (bufSize)) )

/* Size of Read/Write buffers for I2C bootloader  */
#define UART_433_I2C_BTLDR_SIZEOF_READ_BUFFER   (64u)
#define UART_433_I2C_BTLDR_SIZEOF_WRITE_BUFFER  (64u)

/* Byte to byte time interval: calculated basing on current component
* data rate configuration, can be defined in project if required.
*/
#ifndef UART_433_SPI_BYTE_TO_BYTE
    #define UART_433_SPI_BYTE_TO_BYTE   (160u)
#endif

/* Byte to byte time interval: calculated basing on current component
* baud rate configuration, can be defined in the project if required.
*/
#ifndef UART_433_UART_BYTE_TO_BYTE
    #define UART_433_UART_BYTE_TO_BYTE  (175u)
#endif /* UART_433_UART_BYTE_TO_BYTE */

#endif /* (CY_SCB_BOOT_UART_433_H) */


/* [] END OF FILE */
