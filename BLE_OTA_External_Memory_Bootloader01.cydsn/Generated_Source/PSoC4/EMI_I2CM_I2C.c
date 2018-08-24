/***************************************************************************//**
* \file EMI_I2CM_I2C.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component in
*  I2C mode.
*
* Note:
*
*******************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "EMI_I2CM_PVT.h"
#include "EMI_I2CM_I2C_PVT.h"


/***************************************
*      I2C Private Vars
***************************************/

volatile uint8 EMI_I2CM_state;  /* Current state of I2C FSM */

#if(EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)

    /***************************************
    *  Configuration Structure Initialization
    ***************************************/

    /* Constant configuration of I2C */
    const EMI_I2CM_I2C_INIT_STRUCT EMI_I2CM_configI2C =
    {
        EMI_I2CM_I2C_MODE,
        EMI_I2CM_I2C_OVS_FACTOR_LOW,
        EMI_I2CM_I2C_OVS_FACTOR_HIGH,
        EMI_I2CM_I2C_MEDIAN_FILTER_ENABLE,
        EMI_I2CM_I2C_SLAVE_ADDRESS,
        EMI_I2CM_I2C_SLAVE_ADDRESS_MASK,
        EMI_I2CM_I2C_ACCEPT_ADDRESS,
        EMI_I2CM_I2C_WAKE_ENABLE,
        EMI_I2CM_I2C_BYTE_MODE_ENABLE,
        EMI_I2CM_I2C_DATA_RATE,
        EMI_I2CM_I2C_ACCEPT_GENERAL_CALL,
    };

    /*******************************************************************************
    * Function Name: EMI_I2CM_I2CInit
    ****************************************************************************//**
    *
    *
    *  Configures the EMI_I2CM for I2C operation.
    *
    *  This function is intended specifically to be used when the EMI_I2CM 
    *  configuration is set to ¡°Unconfigured EMI_I2CM¡± in the customizer. 
    *  After initializing the EMI_I2CM in I2C mode using this function, 
    *  the component can be enabled using the EMI_I2CM_Start() or 
    * EMI_I2CM_Enable() function.
    *  This function uses a pointer to a structure that provides the configuration 
    *  settings. This structure contains the same information that would otherwise 
    *  be provided by the customizer settings.
    *
    *  \param config: pointer to a structure that contains the following list of 
    *   fields. These fields match the selections available in the customizer. 
    *   Refer to the customizer for further description of the settings.
    *
    *******************************************************************************/
    void EMI_I2CM_I2CInit(const EMI_I2CM_I2C_INIT_STRUCT *config)
    {
        uint32 medianFilter;
        uint32 locEnableWake;

        if(NULL == config)
        {
            CYASSERT(0u != 0u); /* Halt execution due to bad function parameter */
        }
        else
        {
            /* Configure pins */
            EMI_I2CM_SetPins(EMI_I2CM_SCB_MODE_I2C, EMI_I2CM_DUMMY_PARAM,
                                     EMI_I2CM_DUMMY_PARAM);

            /* Store internal configuration */
            EMI_I2CM_scbMode       = (uint8) EMI_I2CM_SCB_MODE_I2C;
            EMI_I2CM_scbEnableWake = (uint8) config->enableWake;
            EMI_I2CM_scbEnableIntr = (uint8) EMI_I2CM_SCB_IRQ_INTERNAL;

            EMI_I2CM_mode          = (uint8) config->mode;
            EMI_I2CM_acceptAddr    = (uint8) config->acceptAddr;

        #if (EMI_I2CM_CY_SCBIP_V0)
            /* Adjust SDA filter settings. Ticket ID#150521 */
            EMI_I2CM_SET_I2C_CFG_SDA_FILT_TRIM(EMI_I2CM_EC_AM_I2C_CFG_SDA_FILT_TRIM);
        #endif /* (EMI_I2CM_CY_SCBIP_V0) */

            /* Adjust AF and DF filter settings. Ticket ID#176179 */
            if (((EMI_I2CM_I2C_MODE_SLAVE != config->mode) &&
                 (config->dataRate <= EMI_I2CM_I2C_DATA_RATE_FS_MODE_MAX)) ||
                 (EMI_I2CM_I2C_MODE_SLAVE == config->mode))
            {
                /* AF = 1, DF = 0 */
                EMI_I2CM_I2C_CFG_ANALOG_FITER_ENABLE;
                medianFilter = EMI_I2CM_DIGITAL_FILTER_DISABLE;
            }
            else
            {
                /* AF = 0, DF = 1 */
                EMI_I2CM_I2C_CFG_ANALOG_FITER_DISABLE;
                medianFilter = EMI_I2CM_DIGITAL_FILTER_ENABLE;
            }

        #if (!EMI_I2CM_CY_SCBIP_V0)
            locEnableWake = (EMI_I2CM_I2C_MULTI_MASTER_SLAVE) ? (0u) : (config->enableWake);
        #else
            locEnableWake = config->enableWake;
        #endif /* (!EMI_I2CM_CY_SCBIP_V0) */

            /* Configure I2C interface */
            EMI_I2CM_CTRL_REG     = EMI_I2CM_GET_CTRL_BYTE_MODE  (config->enableByteMode) |
                                            EMI_I2CM_GET_CTRL_ADDR_ACCEPT(config->acceptAddr)     |
                                            EMI_I2CM_GET_CTRL_EC_AM_MODE (locEnableWake);

            EMI_I2CM_I2C_CTRL_REG = EMI_I2CM_GET_I2C_CTRL_HIGH_PHASE_OVS(config->oversampleHigh) |
                    EMI_I2CM_GET_I2C_CTRL_LOW_PHASE_OVS (config->oversampleLow)                          |
                    EMI_I2CM_GET_I2C_CTRL_S_GENERAL_IGNORE((uint32)(0u == config->acceptGeneralAddr))    |
                    EMI_I2CM_GET_I2C_CTRL_SL_MSTR_MODE  (config->mode);

            /* Configure RX direction */
            EMI_I2CM_RX_CTRL_REG      = EMI_I2CM_GET_RX_CTRL_MEDIAN(medianFilter) |
                                                EMI_I2CM_I2C_RX_CTRL;
            EMI_I2CM_RX_FIFO_CTRL_REG = EMI_I2CM_CLEAR_REG;

            /* Set default address and mask */
            EMI_I2CM_RX_MATCH_REG    = ((EMI_I2CM_I2C_SLAVE) ?
                                                (EMI_I2CM_GET_I2C_8BIT_ADDRESS(config->slaveAddr) |
                                                 EMI_I2CM_GET_RX_MATCH_MASK(config->slaveAddrMask)) :
                                                (EMI_I2CM_CLEAR_REG));


            /* Configure TX direction */
            EMI_I2CM_TX_CTRL_REG      = EMI_I2CM_I2C_TX_CTRL;
            EMI_I2CM_TX_FIFO_CTRL_REG = EMI_I2CM_CLEAR_REG;

            /* Configure interrupt with I2C handler but do not enable it */
            CyIntDisable    (EMI_I2CM_ISR_NUMBER);
            CyIntSetPriority(EMI_I2CM_ISR_NUMBER, EMI_I2CM_ISR_PRIORITY);
            (void) CyIntSetVector(EMI_I2CM_ISR_NUMBER, &EMI_I2CM_I2C_ISR);

            /* Configure interrupt sources */
        #if(!EMI_I2CM_CY_SCBIP_V1)
            EMI_I2CM_INTR_SPI_EC_MASK_REG = EMI_I2CM_NO_INTR_SOURCES;
        #endif /* (!EMI_I2CM_CY_SCBIP_V1) */

            EMI_I2CM_INTR_I2C_EC_MASK_REG = EMI_I2CM_NO_INTR_SOURCES;
            EMI_I2CM_INTR_RX_MASK_REG     = EMI_I2CM_NO_INTR_SOURCES;
            EMI_I2CM_INTR_TX_MASK_REG     = EMI_I2CM_NO_INTR_SOURCES;

            EMI_I2CM_INTR_SLAVE_MASK_REG  = ((EMI_I2CM_I2C_SLAVE) ?
                            (EMI_I2CM_GET_INTR_SLAVE_I2C_GENERAL(config->acceptGeneralAddr) |
                             EMI_I2CM_I2C_INTR_SLAVE_MASK) : (EMI_I2CM_CLEAR_REG));

            EMI_I2CM_INTR_MASTER_MASK_REG = ((EMI_I2CM_I2C_MASTER) ?
                                                     (EMI_I2CM_I2C_INTR_MASTER_MASK) :
                                                     (EMI_I2CM_CLEAR_REG));

            /* Configure global variables */
            EMI_I2CM_state = EMI_I2CM_I2C_FSM_IDLE;

            /* Internal slave variables */
            EMI_I2CM_slStatus        = 0u;
            EMI_I2CM_slRdBufIndex    = 0u;
            EMI_I2CM_slWrBufIndex    = 0u;
            EMI_I2CM_slOverFlowCount = 0u;

            /* Internal master variables */
            EMI_I2CM_mstrStatus     = 0u;
            EMI_I2CM_mstrRdBufIndex = 0u;
            EMI_I2CM_mstrWrBufIndex = 0u;
        }
    }

#else

    /*******************************************************************************
    * Function Name: EMI_I2CM_I2CInit
    ****************************************************************************//**
    *
    *  Configures the SCB for the I2C operation.
    *
    *******************************************************************************/
    void EMI_I2CM_I2CInit(void)
    {
    #if(EMI_I2CM_CY_SCBIP_V0)
        /* Adjust SDA filter settings. Ticket ID#150521 */
        EMI_I2CM_SET_I2C_CFG_SDA_FILT_TRIM(EMI_I2CM_EC_AM_I2C_CFG_SDA_FILT_TRIM);
    #endif /* (EMI_I2CM_CY_SCBIP_V0) */

        /* Adjust AF and DF filter settings. Ticket ID#176179 */
        EMI_I2CM_I2C_CFG_ANALOG_FITER_ENABLE_ADJ;

        /* Configure I2C interface */
        EMI_I2CM_CTRL_REG     = EMI_I2CM_I2C_DEFAULT_CTRL;
        EMI_I2CM_I2C_CTRL_REG = EMI_I2CM_I2C_DEFAULT_I2C_CTRL;

        /* Configure RX direction */
        EMI_I2CM_RX_CTRL_REG      = EMI_I2CM_I2C_DEFAULT_RX_CTRL;
        EMI_I2CM_RX_FIFO_CTRL_REG = EMI_I2CM_I2C_DEFAULT_RX_FIFO_CTRL;

        /* Set default address and mask */
        EMI_I2CM_RX_MATCH_REG     = EMI_I2CM_I2C_DEFAULT_RX_MATCH;

        /* Configure TX direction */
        EMI_I2CM_TX_CTRL_REG      = EMI_I2CM_I2C_DEFAULT_TX_CTRL;
        EMI_I2CM_TX_FIFO_CTRL_REG = EMI_I2CM_I2C_DEFAULT_TX_FIFO_CTRL;

        /* Configure interrupt with I2C handler but do not enable it */
        CyIntDisable    (EMI_I2CM_ISR_NUMBER);
        CyIntSetPriority(EMI_I2CM_ISR_NUMBER, EMI_I2CM_ISR_PRIORITY);
    #if(!EMI_I2CM_I2C_EXTERN_INTR_HANDLER)
        (void) CyIntSetVector(EMI_I2CM_ISR_NUMBER, &EMI_I2CM_I2C_ISR);
    #endif /* (EMI_I2CM_I2C_EXTERN_INTR_HANDLER) */

        /* Configure interrupt sources */
    #if(!EMI_I2CM_CY_SCBIP_V1)
        EMI_I2CM_INTR_SPI_EC_MASK_REG = EMI_I2CM_I2C_DEFAULT_INTR_SPI_EC_MASK;
    #endif /* (!EMI_I2CM_CY_SCBIP_V1) */

        EMI_I2CM_INTR_I2C_EC_MASK_REG = EMI_I2CM_I2C_DEFAULT_INTR_I2C_EC_MASK;
        EMI_I2CM_INTR_SLAVE_MASK_REG  = EMI_I2CM_I2C_DEFAULT_INTR_SLAVE_MASK;
        EMI_I2CM_INTR_MASTER_MASK_REG = EMI_I2CM_I2C_DEFAULT_INTR_MASTER_MASK;
        EMI_I2CM_INTR_RX_MASK_REG     = EMI_I2CM_I2C_DEFAULT_INTR_RX_MASK;
        EMI_I2CM_INTR_TX_MASK_REG     = EMI_I2CM_I2C_DEFAULT_INTR_TX_MASK;

        /* Configure global variables */
        EMI_I2CM_state = EMI_I2CM_I2C_FSM_IDLE;

    #if(EMI_I2CM_I2C_SLAVE)
        /* Internal slave variable */
        EMI_I2CM_slStatus        = 0u;
        EMI_I2CM_slRdBufIndex    = 0u;
        EMI_I2CM_slWrBufIndex    = 0u;
        EMI_I2CM_slOverFlowCount = 0u;
    #endif /* (EMI_I2CM_I2C_SLAVE) */

    #if(EMI_I2CM_I2C_MASTER)
    /* Internal master variable */
        EMI_I2CM_mstrStatus     = 0u;
        EMI_I2CM_mstrRdBufIndex = 0u;
        EMI_I2CM_mstrWrBufIndex = 0u;
    #endif /* (EMI_I2CM_I2C_MASTER) */
    }
#endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */


/*******************************************************************************
* Function Name: EMI_I2CM_I2CStop
****************************************************************************//**
*
*  Resets the I2C FSM into the default state.
*
*******************************************************************************/
void EMI_I2CM_I2CStop(void)
{
    EMI_I2CM_state = EMI_I2CM_I2C_FSM_IDLE;
}


#if(EMI_I2CM_I2C_WAKE_ENABLE_CONST)
    /*******************************************************************************
    * Function Name: EMI_I2CM_I2CSaveConfig
    ****************************************************************************//**
    *
    *  Enables EMI_I2CM_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void EMI_I2CM_I2CSaveConfig(void)
    {
    #if (!EMI_I2CM_CY_SCBIP_V0)
        #if (EMI_I2CM_I2C_MULTI_MASTER_SLAVE_CONST && EMI_I2CM_I2C_WAKE_ENABLE_CONST)
            /* Enable externally clocked address match if it was not enabled before.
            * This applicable only for Multi-Master-Slave. Ticket ID#192742 */
            if (0u == (EMI_I2CM_CTRL_REG & EMI_I2CM_CTRL_EC_AM_MODE))
            {
                /* Enable external address match logic */
                EMI_I2CM_Stop();
                EMI_I2CM_CTRL_REG |= EMI_I2CM_CTRL_EC_AM_MODE;
                EMI_I2CM_Enable();
            }
        #endif /* (EMI_I2CM_I2C_MULTI_MASTER_SLAVE_CONST) */

        #if (EMI_I2CM_SCB_CLK_INTERNAL)
            /* Disable clock to internal address match logic. Ticket ID#187931 */
            EMI_I2CM_SCBCLK_Stop();
        #endif /* (EMI_I2CM_SCB_CLK_INTERNAL) */
    #endif /* (!EMI_I2CM_CY_SCBIP_V0) */

        EMI_I2CM_SetI2CExtClkInterruptMode(EMI_I2CM_INTR_I2C_EC_WAKE_UP);
    }


    /*******************************************************************************
    * Function Name: EMI_I2CM_I2CRestoreConfig
    ****************************************************************************//**
    *
    *  Disables EMI_I2CM_INTR_I2C_EC_WAKE_UP interrupt source. This interrupt
    *  triggers on address match and wakes up device.
    *
    *******************************************************************************/
    void EMI_I2CM_I2CRestoreConfig(void)
    {
        /* Disable wakeup interrupt on address match */
        EMI_I2CM_SetI2CExtClkInterruptMode(EMI_I2CM_NO_INTR_SOURCES);

    #if (!EMI_I2CM_CY_SCBIP_V0)
        #if (EMI_I2CM_SCB_CLK_INTERNAL)
            /* Enable clock to internal address match logic. Ticket ID#187931 */
            EMI_I2CM_SCBCLK_Start();
        #endif /* (EMI_I2CM_SCB_CLK_INTERNAL) */
    #endif /* (!EMI_I2CM_CY_SCBIP_V0) */
    }
#endif /* (EMI_I2CM_I2C_WAKE_ENABLE_CONST) */


/* [] END OF FILE */
