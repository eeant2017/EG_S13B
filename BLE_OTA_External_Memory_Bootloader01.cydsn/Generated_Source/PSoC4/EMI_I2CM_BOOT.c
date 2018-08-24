/***************************************************************************//**
* \file EMI_I2CM_BOOT.c
* \version 3.20
*
* \brief
*  This file provides the source code of the bootloader communication APIs
*  for the SCB Component Unconfigured mode.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "EMI_I2CM_BOOT.h"

#if defined(CYDEV_BOOTLOADER_IO_COMP) && (EMI_I2CM_BTLDR_COMM_ENABLED) && \
                                (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)

/*******************************************************************************
* Function Name: EMI_I2CM_CyBtldrCommStart
****************************************************************************//**
*
*  Starts EMI_I2CM component. After this function call the component is 
*  ready for communication.
*
*******************************************************************************/
void EMI_I2CM_CyBtldrCommStart(void)
{
    if (EMI_I2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        EMI_I2CM_I2CCyBtldrCommStart();
    }
    else if (EMI_I2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        EMI_I2CM_EzI2CCyBtldrCommStart();
    }
#if (!EMI_I2CM_CY_SCBIP_V1)
    else if (EMI_I2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        EMI_I2CM_SpiCyBtldrCommStart();
    }
    else if (EMI_I2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        EMI_I2CM_UartCyBtldrCommStart();
    }
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: EMI_I2CM_CyBtldrCommStop
****************************************************************************//**
*
*  Stops EMI_I2CM component.
*
*******************************************************************************/
void EMI_I2CM_CyBtldrCommStop(void)
{
    if (EMI_I2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        EMI_I2CM_I2CCyBtldrCommStop();
    }
    else if (EMI_I2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        EMI_I2CM_EzI2CCyBtldrCommStop();
    }
#if (!EMI_I2CM_CY_SCBIP_V1)
    else if (EMI_I2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        EMI_I2CM_SpiCyBtldrCommStop();
    }
    else if (EMI_I2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        EMI_I2CM_UartCyBtldrCommStop();
    }
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: EMI_I2CM_CyBtldrCommReset
****************************************************************************//**
*
*  Clears EMI_I2CM component buffers.
*
*******************************************************************************/
void EMI_I2CM_CyBtldrCommReset(void)
{
    if(EMI_I2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        EMI_I2CM_I2CCyBtldrCommReset();
    }
    else if(EMI_I2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        EMI_I2CM_EzI2CCyBtldrCommReset();
    }
#if (!EMI_I2CM_CY_SCBIP_V1)
    else if(EMI_I2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        EMI_I2CM_SpiCyBtldrCommReset();
    }
    else if(EMI_I2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        EMI_I2CM_UartCyBtldrCommReset();
    }
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
}


/*******************************************************************************
* Function Name: EMI_I2CM_CyBtldrCommRead
****************************************************************************//**
*
*  Allows the caller to read data from the bootloader host (the host writes the 
*  data). The function handles polling to allow a block of data to be completely
*  received from the host device.
*
*  \param pData: Pointer to storage for the block of data to be read from the
*   bootloader host.
*  \param size: Number of bytes to be read.
*  \param count: Pointer to the variable to write the number of bytes actually
*   read.
*  \param timeOut: Number of units in 10 ms to wait before returning because of a
*   timeout.
*
* \return
*  \return
*  cystatus: Returns CYRET_SUCCESS if no problem was encountered or returns the
*  value that best describes the problem. For more information refer to 
*  the ¡°Return Codes¡± section of the System Reference Guide.
*
*******************************************************************************/
cystatus EMI_I2CM_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(EMI_I2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = EMI_I2CM_I2CCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(EMI_I2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = EMI_I2CM_EzI2CCyBtldrCommRead(pData, size, count, timeOut);
    }
#if (!EMI_I2CM_CY_SCBIP_V1)
    else if(EMI_I2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = EMI_I2CM_SpiCyBtldrCommRead(pData, size, count, timeOut);
    }
    else if(EMI_I2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        status = EMI_I2CM_UartCyBtldrCommRead(pData, size, count, timeOut);
    }
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}


/*******************************************************************************
* Function Name: EMI_I2CM_CyBtldrCommWrite
****************************************************************************//**
*
*  Allows the caller to write data to the bootloader host (the host reads the 
*  data). The function does not use timeout and returns after data has been copied
*  into the slave read buffer. This data available to be read by the bootloader
*  host until following host data write.
*
*  \param pData: Pointer to the block of data to be written to the bootloader host.
*  \param size: Number of bytes to be written.
*  \param count: Pointer to the variable to write the number of bytes actually
*   written.
*  \param timeOut: Number of units in 10 ms to wait before returning because of a
*   timeout.
*
*  \return
*  cystatus: Returns CYRET_SUCCESS if no problem was encountered or returns the
*  value that best describes the problem. For more information refer to 
*  the ¡°Return Codes¡± section of the System Reference Guide.
*
*******************************************************************************/
cystatus EMI_I2CM_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut)
{
    cystatus status;

    if(EMI_I2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        status = EMI_I2CM_I2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(EMI_I2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        status = EMI_I2CM_EzI2CCyBtldrCommWrite(pData, size, count, timeOut);
    }
#if (!EMI_I2CM_CY_SCBIP_V1)
    else if(EMI_I2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        status = EMI_I2CM_SpiCyBtldrCommWrite(pData, size, count, timeOut);
    }
    else if(EMI_I2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        status = EMI_I2CM_UartCyBtldrCommWrite(pData, size, count, timeOut);
    }
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */
    else
    {
        status = CYRET_INVALID_STATE; /* Unknown mode: return invalid status */
    }

    return(status);
}

#endif /* defined(CYDEV_BOOTLOADER_IO_COMP) && (EMI_I2CM_BTLDR_COMM_MODE_ENABLED) */


/* [] END OF FILE */
