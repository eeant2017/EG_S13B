/***************************************************************************//**
* \file EMI_I2CM.c
* \version 3.20
*
* \brief
*  This file provides the source code to the API for the SCB Component.
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

#if (EMI_I2CM_SCB_MODE_I2C_INC)
    #include "EMI_I2CM_I2C_PVT.h"
#endif /* (EMI_I2CM_SCB_MODE_I2C_INC) */

#if (EMI_I2CM_SCB_MODE_EZI2C_INC)
    #include "EMI_I2CM_EZI2C_PVT.h"
#endif /* (EMI_I2CM_SCB_MODE_EZI2C_INC) */

#if (EMI_I2CM_SCB_MODE_SPI_INC || EMI_I2CM_SCB_MODE_UART_INC)
    #include "EMI_I2CM_SPI_UART_PVT.h"
#endif /* (EMI_I2CM_SCB_MODE_SPI_INC || EMI_I2CM_SCB_MODE_UART_INC) */


/***************************************
*    Run Time Configuration Vars
***************************************/

/* Stores internal component configuration for Unconfigured mode */
#if (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Common configuration variables */
    uint8 EMI_I2CM_scbMode = EMI_I2CM_SCB_MODE_UNCONFIG;
    uint8 EMI_I2CM_scbEnableWake;
    uint8 EMI_I2CM_scbEnableIntr;

    /* I2C configuration variables */
    uint8 EMI_I2CM_mode;
    uint8 EMI_I2CM_acceptAddr;

    /* SPI/UART configuration variables */
    volatile uint8 * EMI_I2CM_rxBuffer;
    uint8  EMI_I2CM_rxDataBits;
    uint32 EMI_I2CM_rxBufferSize;

    volatile uint8 * EMI_I2CM_txBuffer;
    uint8  EMI_I2CM_txDataBits;
    uint32 EMI_I2CM_txBufferSize;

    /* EZI2C configuration variables */
    uint8 EMI_I2CM_numberOfAddr;
    uint8 EMI_I2CM_subAddrSize;
#endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */


/***************************************
*     Common SCB Vars
***************************************/
/**
* \addtogroup group_general
* \{
*/

/** EMI_I2CM_initVar indicates whether the EMI_I2CM 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time SCB_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the EMI_I2CM_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  EMI_I2CM_Init() function can be called before the 
*  EMI_I2CM_Start() or EMI_I2CM_Enable() function.
*/
uint8 EMI_I2CM_initVar = 0u;


#if (! (EMI_I2CM_SCB_MODE_I2C_CONST_CFG || \
        EMI_I2CM_SCB_MODE_EZI2C_CONST_CFG))
    /** This global variable stores TX interrupt sources after 
    * EMI_I2CM_Stop() is called. Only these TX interrupt sources 
    * will be restored on a subsequent EMI_I2CM_Enable() call.
    */
    uint16 EMI_I2CM_IntrTxMask = 0u;
#endif /* (! (EMI_I2CM_SCB_MODE_I2C_CONST_CFG || \
              EMI_I2CM_SCB_MODE_EZI2C_CONST_CFG)) */
/** \} globals */

#if (EMI_I2CM_SCB_IRQ_INTERNAL)
#if !defined (CY_REMOVE_EMI_I2CM_CUSTOM_INTR_HANDLER)
    void (*EMI_I2CM_customIntrHandler)(void) = NULL;
#endif /* !defined (CY_REMOVE_EMI_I2CM_CUSTOM_INTR_HANDLER) */
#endif /* (EMI_I2CM_SCB_IRQ_INTERNAL) */


/***************************************
*    Private Function Prototypes
***************************************/

static void EMI_I2CM_ScbEnableIntr(void);
static void EMI_I2CM_ScbModeStop(void);
static void EMI_I2CM_ScbModePostEnable(void);


/*******************************************************************************
* Function Name: EMI_I2CM_Init
****************************************************************************//**
*
*  Initializes the EMI_I2CM component to operate in one of the selected
*  configurations: I2C, SPI, UART or EZI2C.
*  When the configuration is set to "Unconfigured SCB", this function does
*  not do any initialization. Use mode-specific initialization APIs instead:
*  EMI_I2CM_I2CInit, EMI_I2CM_SpiInit, 
*  EMI_I2CM_UartInit or EMI_I2CM_EzI2CInit.
*
*******************************************************************************/
void EMI_I2CM_Init(void)
{
#if (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    if (EMI_I2CM_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        EMI_I2CM_initVar = 0u;
    }
    else
    {
        /* Initialization was done before this function call */
    }

#elif (EMI_I2CM_SCB_MODE_I2C_CONST_CFG)
    EMI_I2CM_I2CInit();

#elif (EMI_I2CM_SCB_MODE_SPI_CONST_CFG)
    EMI_I2CM_SpiInit();

#elif (EMI_I2CM_SCB_MODE_UART_CONST_CFG)
    EMI_I2CM_UartInit();

#elif (EMI_I2CM_SCB_MODE_EZI2C_CONST_CFG)
    EMI_I2CM_EzI2CInit();

#endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: EMI_I2CM_Enable
****************************************************************************//**
*
*  Enables EMI_I2CM component operation: activates the hardware and 
*  internal interrupt. It also restores TX interrupt sources disabled after the 
*  EMI_I2CM_Stop() function was called (note that level-triggered TX 
*  interrupt sources remain disabled to not cause code lock-up).
*  For I2C and EZI2C modes the interrupt is internal and mandatory for 
*  operation. For SPI and UART modes the interrupt can be configured as none, 
*  internal or external.
*  The EMI_I2CM configuration should be not changed when the component
*  is enabled. Any configuration changes should be made after disabling the 
*  component.
*  When configuration is set to ��Unconfigured EMI_I2CM��, the component 
*  must first be initialized to operate in one of the following configurations: 
*  I2C, SPI, UART or EZ I2C, using the mode-specific initialization API. 
*  Otherwise this function does not enable the component.
*
*******************************************************************************/
void EMI_I2CM_Enable(void)
{
#if (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    /* Enable SCB block, only if it is already configured */
    if (!EMI_I2CM_SCB_MODE_UNCONFIG_RUNTM_CFG)
    {
        EMI_I2CM_CTRL_REG |= EMI_I2CM_CTRL_ENABLED;

        EMI_I2CM_ScbEnableIntr();

        /* Call PostEnable function specific to current operation mode */
        EMI_I2CM_ScbModePostEnable();
    }
#else
    EMI_I2CM_CTRL_REG |= EMI_I2CM_CTRL_ENABLED;

    EMI_I2CM_ScbEnableIntr();

    /* Call PostEnable function specific to current operation mode */
    EMI_I2CM_ScbModePostEnable();
#endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: EMI_I2CM_Start
****************************************************************************//**
*
*  Invokes EMI_I2CM_Init() and EMI_I2CM_Enable().
*  After this function call, the component is enabled and ready for operation.
*  When configuration is set to "Unconfigured SCB", the component must first be
*  initialized to operate in one of the following configurations: I2C, SPI, UART
*  or EZI2C. Otherwise this function does not enable the component.
*
* \globalvars
*  EMI_I2CM_initVar - used to check initial configuration, modified
*  on first function call.
*
*******************************************************************************/
void EMI_I2CM_Start(void)
{
    if (0u == EMI_I2CM_initVar)
    {
        EMI_I2CM_Init();
        EMI_I2CM_initVar = 1u; /* Component was initialized */
    }

    EMI_I2CM_Enable();
}


/*******************************************************************************
* Function Name: EMI_I2CM_Stop
****************************************************************************//**
*
*  Disables the EMI_I2CM component: disable the hardware and internal 
*  interrupt. It also disables all TX interrupt sources so as not to cause an 
*  unexpected interrupt trigger because after the component is enabled, the 
*  TX FIFO is empty.
*  Refer to the function EMI_I2CM_Enable() for the interrupt 
*  configuration details.
*  This function disables the SCB component without checking to see if 
*  communication is in progress. Before calling this function it may be 
*  necessary to check the status of communication to make sure communication 
*  is complete. If this is not done then communication could be stopped mid 
*  byte and corrupted data could result.
*
*******************************************************************************/
void EMI_I2CM_Stop(void)
{
#if (EMI_I2CM_SCB_IRQ_INTERNAL)
    EMI_I2CM_DisableInt();
#endif /* (EMI_I2CM_SCB_IRQ_INTERNAL) */

    /* Call Stop function specific to current operation mode */
    EMI_I2CM_ScbModeStop();

    /* Disable SCB IP */
    EMI_I2CM_CTRL_REG &= (uint32) ~EMI_I2CM_CTRL_ENABLED;

    /* Disable all TX interrupt sources so as not to cause an unexpected
    * interrupt trigger after the component will be enabled because the 
    * TX FIFO is empty.
    * For SCB IP v0, it is critical as it does not mask-out interrupt
    * sources when it is disabled. This can cause a code lock-up in the
    * interrupt handler because TX FIFO cannot be loaded after the block
    * is disabled.
    */
    EMI_I2CM_SetTxInterruptMode(EMI_I2CM_NO_INTR_SOURCES);

#if (EMI_I2CM_SCB_IRQ_INTERNAL)
    EMI_I2CM_ClearPendingInt();
#endif /* (EMI_I2CM_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: EMI_I2CM_SetRxFifoLevel
****************************************************************************//**
*
*  Sets level in the RX FIFO to generate a RX level interrupt.
*  When the RX FIFO has more entries than the RX FIFO level an RX level
*  interrupt request is generated.
*
*  \param level: Level in the RX FIFO to generate RX level interrupt.
*   The range of valid level values is between 0 and RX FIFO depth - 1.
*
*******************************************************************************/
void EMI_I2CM_SetRxFifoLevel(uint32 level)
{
    uint32 rxFifoCtrl;

    rxFifoCtrl = EMI_I2CM_RX_FIFO_CTRL_REG;

    rxFifoCtrl &= ((uint32) ~EMI_I2CM_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    rxFifoCtrl |= ((uint32) (EMI_I2CM_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    EMI_I2CM_RX_FIFO_CTRL_REG = rxFifoCtrl;
}


/*******************************************************************************
* Function Name: EMI_I2CM_SetTxFifoLevel
****************************************************************************//**
*
*  Sets level in the TX FIFO to generate a TX level interrupt.
*  When the TX FIFO has less entries than the TX FIFO level an TX level
*  interrupt request is generated.
*
*  \param level: Level in the TX FIFO to generate TX level interrupt.
*   The range of valid level values is between 0 and TX FIFO depth - 1.
*
*******************************************************************************/
void EMI_I2CM_SetTxFifoLevel(uint32 level)
{
    uint32 txFifoCtrl;

    txFifoCtrl = EMI_I2CM_TX_FIFO_CTRL_REG;

    txFifoCtrl &= ((uint32) ~EMI_I2CM_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK); /* Clear level mask bits */
    txFifoCtrl |= ((uint32) (EMI_I2CM_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK & level));

    EMI_I2CM_TX_FIFO_CTRL_REG = txFifoCtrl;
}


#if (EMI_I2CM_SCB_IRQ_INTERNAL)
    /*******************************************************************************
    * Function Name: EMI_I2CM_SetCustomInterruptHandler
    ****************************************************************************//**
    *
    *  Registers a function to be called by the internal interrupt handler.
    *  First the function that is registered is called, then the internal interrupt
    *  handler performs any operation such as software buffer management functions
    *  before the interrupt returns.  It is the user's responsibility not to break
    *  the software buffer operations. Only one custom handler is supported, which
    *  is the function provided by the most recent call.
    *  At the initialization time no custom handler is registered.
    *
    *  \param func: Pointer to the function to register.
    *        The value NULL indicates to remove the current custom interrupt
    *        handler.
    *
    *******************************************************************************/
    void EMI_I2CM_SetCustomInterruptHandler(void (*func)(void))
    {
    #if !defined (CY_REMOVE_EMI_I2CM_CUSTOM_INTR_HANDLER)
        EMI_I2CM_customIntrHandler = func; /* Register interrupt handler */
    #else
        if (NULL != func)
        {
            /* Suppress compiler warning */
        }
    #endif /* !defined (CY_REMOVE_EMI_I2CM_CUSTOM_INTR_HANDLER) */
    }
#endif /* (EMI_I2CM_SCB_IRQ_INTERNAL) */


/*******************************************************************************
* Function Name: EMI_I2CM_ScbModeEnableIntr
****************************************************************************//**
*
*  Enables an interrupt for a specific mode.
*
*******************************************************************************/
static void EMI_I2CM_ScbEnableIntr(void)
{
#if (EMI_I2CM_SCB_IRQ_INTERNAL)
    /* Enable interrupt in NVIC */
    #if (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
        if (0u != EMI_I2CM_scbEnableIntr)
        {
            EMI_I2CM_EnableInt();
        }

    #else
        EMI_I2CM_EnableInt();

    #endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */
#endif /* (EMI_I2CM_SCB_IRQ_INTERNAL) */
}


/*******************************************************************************
* Function Name: EMI_I2CM_ScbModePostEnable
****************************************************************************//**
*
*  Calls the PostEnable function for a specific operation mode.
*
*******************************************************************************/
static void EMI_I2CM_ScbModePostEnable(void)
{
#if (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
#if (!EMI_I2CM_CY_SCBIP_V1)
    if (EMI_I2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        EMI_I2CM_SpiPostEnable();
    }
    else if (EMI_I2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        EMI_I2CM_UartPostEnable();
    }
    else
    {
        /* Unknown mode: do nothing */
    }
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

#elif (EMI_I2CM_SCB_MODE_SPI_CONST_CFG)
    EMI_I2CM_SpiPostEnable();

#elif (EMI_I2CM_SCB_MODE_UART_CONST_CFG)
    EMI_I2CM_UartPostEnable();

#else
    /* Unknown mode: do nothing */
#endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


/*******************************************************************************
* Function Name: EMI_I2CM_ScbModeStop
****************************************************************************//**
*
*  Calls the Stop function for a specific operation mode.
*
*******************************************************************************/
static void EMI_I2CM_ScbModeStop(void)
{
#if (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    if (EMI_I2CM_SCB_MODE_I2C_RUNTM_CFG)
    {
        EMI_I2CM_I2CStop();
    }
    else if (EMI_I2CM_SCB_MODE_EZI2C_RUNTM_CFG)
    {
        EMI_I2CM_EzI2CStop();
    }
#if (!EMI_I2CM_CY_SCBIP_V1)
    else if (EMI_I2CM_SCB_MODE_SPI_RUNTM_CFG)
    {
        EMI_I2CM_SpiStop();
    }
    else if (EMI_I2CM_SCB_MODE_UART_RUNTM_CFG)
    {
        EMI_I2CM_UartStop();
    }
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */
    else
    {
        /* Unknown mode: do nothing */
    }
#elif (EMI_I2CM_SCB_MODE_I2C_CONST_CFG)
    EMI_I2CM_I2CStop();

#elif (EMI_I2CM_SCB_MODE_EZI2C_CONST_CFG)
    EMI_I2CM_EzI2CStop();

#elif (EMI_I2CM_SCB_MODE_SPI_CONST_CFG)
    EMI_I2CM_SpiStop();

#elif (EMI_I2CM_SCB_MODE_UART_CONST_CFG)
    EMI_I2CM_UartStop();

#else
    /* Unknown mode: do nothing */
#endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */
}


#if (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG)
    /*******************************************************************************
    * Function Name: EMI_I2CM_SetPins
    ****************************************************************************//**
    *
    *  Sets the pins settings accordingly to the selected operation mode.
    *  Only available in the Unconfigured operation mode. The mode specific
    *  initialization function calls it.
    *  Pins configuration is set by PSoC Creator when a specific mode of operation
    *  is selected in design time.
    *
    *  \param mode:      Mode of SCB operation.
    *  \param subMode:   Sub-mode of SCB operation. It is only required for SPI and UART
    *             modes.
    *  \param uartEnableMask: enables TX or RX direction and RTS and CTS signals.
    *
    *******************************************************************************/
    void EMI_I2CM_SetPins(uint32 mode, uint32 subMode, uint32 uartEnableMask)
    {
        uint32 pinsDm[EMI_I2CM_SCB_PINS_NUMBER];
        uint32 i;
        
    #if (!EMI_I2CM_CY_SCBIP_V1)
        uint32 pinsInBuf = 0u;
    #endif /* (!EMI_I2CM_CY_SCBIP_V1) */
        
        uint32 hsiomSel[EMI_I2CM_SCB_PINS_NUMBER] = 
        {
            EMI_I2CM_RX_SDA_MOSI_HSIOM_SEL_GPIO,
            EMI_I2CM_TX_SCL_MISO_HSIOM_SEL_GPIO,
            0u,
            0u,
            0u,
            0u,
            0u,
        };

    #if (EMI_I2CM_CY_SCBIP_V1)
        /* Supress compiler warning. */
        if ((0u == subMode) || (0u == uartEnableMask))
        {
        }
    #endif /* (EMI_I2CM_CY_SCBIP_V1) */

        /* Set default HSIOM to GPIO and Drive Mode to Analog Hi-Z */
        for (i = 0u; i < EMI_I2CM_SCB_PINS_NUMBER; i++)
        {
            pinsDm[i] = EMI_I2CM_PIN_DM_ALG_HIZ;
        }

        if ((EMI_I2CM_SCB_MODE_I2C   == mode) ||
            (EMI_I2CM_SCB_MODE_EZI2C == mode))
        {
        #if (EMI_I2CM_RX_SDA_MOSI_PIN)
            hsiomSel[EMI_I2CM_RX_SDA_MOSI_PIN_INDEX] = EMI_I2CM_RX_SDA_MOSI_HSIOM_SEL_I2C;
            pinsDm  [EMI_I2CM_RX_SDA_MOSI_PIN_INDEX] = EMI_I2CM_PIN_DM_OD_LO;
        #elif (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN)
            hsiomSel[EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX] = EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_SEL_I2C;
            pinsDm  [EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX] = EMI_I2CM_PIN_DM_OD_LO;
        #else
        #endif /* (EMI_I2CM_RX_SDA_MOSI_PIN) */
        
        #if (EMI_I2CM_TX_SCL_MISO_PIN)
            hsiomSel[EMI_I2CM_TX_SCL_MISO_PIN_INDEX] = EMI_I2CM_TX_SCL_MISO_HSIOM_SEL_I2C;
            pinsDm  [EMI_I2CM_TX_SCL_MISO_PIN_INDEX] = EMI_I2CM_PIN_DM_OD_LO;
        #endif /* (EMI_I2CM_TX_SCL_MISO_PIN) */
        }
    #if (!EMI_I2CM_CY_SCBIP_V1)
        else if (EMI_I2CM_SCB_MODE_SPI == mode)
        {
        #if (EMI_I2CM_RX_SDA_MOSI_PIN)
            hsiomSel[EMI_I2CM_RX_SDA_MOSI_PIN_INDEX] = EMI_I2CM_RX_SDA_MOSI_HSIOM_SEL_SPI;
        #elif (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN)
            hsiomSel[EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX] = EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_SEL_SPI;
        #else
        #endif /* (EMI_I2CM_RX_SDA_MOSI_PIN) */
        
        #if (EMI_I2CM_TX_SCL_MISO_PIN)
            hsiomSel[EMI_I2CM_TX_SCL_MISO_PIN_INDEX] = EMI_I2CM_TX_SCL_MISO_HSIOM_SEL_SPI;
        #endif /* (EMI_I2CM_TX_SCL_MISO_PIN) */
        
        #if (EMI_I2CM_CTS_SCLK_PIN)
            hsiomSel[EMI_I2CM_CTS_SCLK_PIN_INDEX] = EMI_I2CM_CTS_SCLK_HSIOM_SEL_SPI;
        #endif /* (EMI_I2CM_CTS_SCLK_PIN) */

            if (EMI_I2CM_SPI_SLAVE == subMode)
            {
                /* Slave */
                pinsDm[EMI_I2CM_RX_SDA_MOSI_PIN_INDEX] = EMI_I2CM_PIN_DM_DIG_HIZ;
                pinsDm[EMI_I2CM_TX_SCL_MISO_PIN_INDEX] = EMI_I2CM_PIN_DM_STRONG;
                pinsDm[EMI_I2CM_CTS_SCLK_PIN_INDEX] = EMI_I2CM_PIN_DM_DIG_HIZ;

            #if (EMI_I2CM_RTS_SS0_PIN)
                /* Only SS0 is valid choice for Slave */
                hsiomSel[EMI_I2CM_RTS_SS0_PIN_INDEX] = EMI_I2CM_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm  [EMI_I2CM_RTS_SS0_PIN_INDEX] = EMI_I2CM_PIN_DM_DIG_HIZ;
            #endif /* (EMI_I2CM_RTS_SS0_PIN) */

            #if (EMI_I2CM_TX_SCL_MISO_PIN)
                /* Disable input buffer */
                 pinsInBuf |= EMI_I2CM_TX_SCL_MISO_PIN_MASK;
            #endif /* (EMI_I2CM_TX_SCL_MISO_PIN) */
            }
            else 
            {
                /* (Master) */
                pinsDm[EMI_I2CM_RX_SDA_MOSI_PIN_INDEX] = EMI_I2CM_PIN_DM_STRONG;
                pinsDm[EMI_I2CM_TX_SCL_MISO_PIN_INDEX] = EMI_I2CM_PIN_DM_DIG_HIZ;
                pinsDm[EMI_I2CM_CTS_SCLK_PIN_INDEX] = EMI_I2CM_PIN_DM_STRONG;

            #if (EMI_I2CM_RTS_SS0_PIN)
                hsiomSel [EMI_I2CM_RTS_SS0_PIN_INDEX] = EMI_I2CM_RTS_SS0_HSIOM_SEL_SPI;
                pinsDm   [EMI_I2CM_RTS_SS0_PIN_INDEX] = EMI_I2CM_PIN_DM_STRONG;
                pinsInBuf |= EMI_I2CM_RTS_SS0_PIN_MASK;
            #endif /* (EMI_I2CM_RTS_SS0_PIN) */

            #if (EMI_I2CM_SS1_PIN)
                hsiomSel [EMI_I2CM_SS1_PIN_INDEX] = EMI_I2CM_SS1_HSIOM_SEL_SPI;
                pinsDm   [EMI_I2CM_SS1_PIN_INDEX] = EMI_I2CM_PIN_DM_STRONG;
                pinsInBuf |= EMI_I2CM_SS1_PIN_MASK;
            #endif /* (EMI_I2CM_SS1_PIN) */

            #if (EMI_I2CM_SS2_PIN)
                hsiomSel [EMI_I2CM_SS2_PIN_INDEX] = EMI_I2CM_SS2_HSIOM_SEL_SPI;
                pinsDm   [EMI_I2CM_SS2_PIN_INDEX] = EMI_I2CM_PIN_DM_STRONG;
                pinsInBuf |= EMI_I2CM_SS2_PIN_MASK;
            #endif /* (EMI_I2CM_SS2_PIN) */

            #if (EMI_I2CM_SS3_PIN)
                hsiomSel [EMI_I2CM_SS3_PIN_INDEX] = EMI_I2CM_SS3_HSIOM_SEL_SPI;
                pinsDm   [EMI_I2CM_SS3_PIN_INDEX] = EMI_I2CM_PIN_DM_STRONG;
                pinsInBuf |= EMI_I2CM_SS3_PIN_MASK;
            #endif /* (EMI_I2CM_SS3_PIN) */

                /* Disable input buffers */
            #if (EMI_I2CM_RX_SDA_MOSI_PIN)
                pinsInBuf |= EMI_I2CM_RX_SDA_MOSI_PIN_MASK;
            #elif (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN)
                pinsInBuf |= EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_MASK;
            #else
            #endif /* (EMI_I2CM_RX_SDA_MOSI_PIN) */

            #if (EMI_I2CM_CTS_SCLK_PIN)
                pinsInBuf |= EMI_I2CM_CTS_SCLK_PIN_MASK;
            #endif /* (EMI_I2CM_CTS_SCLK_PIN) */
            }
        }
        else /* UART */
        {
            if (EMI_I2CM_UART_MODE_SMARTCARD == subMode)
            {
                /* SmartCard */
            #if (EMI_I2CM_TX_SCL_MISO_PIN)
                hsiomSel[EMI_I2CM_TX_SCL_MISO_PIN_INDEX] = EMI_I2CM_TX_SCL_MISO_HSIOM_SEL_UART;
                pinsDm  [EMI_I2CM_TX_SCL_MISO_PIN_INDEX] = EMI_I2CM_PIN_DM_OD_LO;
            #endif /* (EMI_I2CM_TX_SCL_MISO_PIN) */
            }
            else /* Standard or IrDA */
            {
                if (0u != (EMI_I2CM_UART_RX_PIN_ENABLE & uartEnableMask))
                {
                #if (EMI_I2CM_RX_SDA_MOSI_PIN)
                    hsiomSel[EMI_I2CM_RX_SDA_MOSI_PIN_INDEX] = EMI_I2CM_RX_SDA_MOSI_HSIOM_SEL_UART;
                    pinsDm  [EMI_I2CM_RX_SDA_MOSI_PIN_INDEX] = EMI_I2CM_PIN_DM_DIG_HIZ;
                #elif (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN)
                    hsiomSel[EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX] = EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_SEL_UART;
                    pinsDm  [EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX] = EMI_I2CM_PIN_DM_DIG_HIZ;
                #else
                #endif /* (EMI_I2CM_RX_SDA_MOSI_PIN) */
                }

                if (0u != (EMI_I2CM_UART_TX_PIN_ENABLE & uartEnableMask))
                {
                #if (EMI_I2CM_TX_SCL_MISO_PIN)
                    hsiomSel[EMI_I2CM_TX_SCL_MISO_PIN_INDEX] = EMI_I2CM_TX_SCL_MISO_HSIOM_SEL_UART;
                    pinsDm  [EMI_I2CM_TX_SCL_MISO_PIN_INDEX] = EMI_I2CM_PIN_DM_STRONG;
                    
                    /* Disable input buffer */
                    pinsInBuf |= EMI_I2CM_TX_SCL_MISO_PIN_MASK;
                #endif /* (EMI_I2CM_TX_SCL_MISO_PIN) */
                }

            #if !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
                if (EMI_I2CM_UART_MODE_STD == subMode)
                {
                    if (0u != (EMI_I2CM_UART_CTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* CTS input is multiplexed with SCLK */
                    #if (EMI_I2CM_CTS_SCLK_PIN)
                        hsiomSel[EMI_I2CM_CTS_SCLK_PIN_INDEX] = EMI_I2CM_CTS_SCLK_HSIOM_SEL_UART;
                        pinsDm  [EMI_I2CM_CTS_SCLK_PIN_INDEX] = EMI_I2CM_PIN_DM_DIG_HIZ;
                    #endif /* (EMI_I2CM_CTS_SCLK_PIN) */
                    }

                    if (0u != (EMI_I2CM_UART_RTS_PIN_ENABLE & uartEnableMask))
                    {
                        /* RTS output is multiplexed with SS0 */
                    #if (EMI_I2CM_RTS_SS0_PIN)
                        hsiomSel[EMI_I2CM_RTS_SS0_PIN_INDEX] = EMI_I2CM_RTS_SS0_HSIOM_SEL_UART;
                        pinsDm  [EMI_I2CM_RTS_SS0_PIN_INDEX] = EMI_I2CM_PIN_DM_STRONG;
                        
                        /* Disable input buffer */
                        pinsInBuf |= EMI_I2CM_RTS_SS0_PIN_MASK;
                    #endif /* (EMI_I2CM_RTS_SS0_PIN) */
                    }
                }
            #endif /* !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */
            }
        }
    #endif /* (!EMI_I2CM_CY_SCBIP_V1) */

    /* Configure pins: set HSIOM, DM and InputBufEnable */
    /* Note: the DR register settings do not effect the pin output if HSIOM is other than GPIO */

    #if (EMI_I2CM_RX_SDA_MOSI_PIN)
        EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_RX_SDA_MOSI_HSIOM_REG,
                                       EMI_I2CM_RX_SDA_MOSI_HSIOM_MASK,
                                       EMI_I2CM_RX_SDA_MOSI_HSIOM_POS,
                                        hsiomSel[EMI_I2CM_RX_SDA_MOSI_PIN_INDEX]);

        EMI_I2CM_uart_rx_i2c_sda_spi_mosi_SetDriveMode((uint8) pinsDm[EMI_I2CM_RX_SDA_MOSI_PIN_INDEX]);

        #if (!EMI_I2CM_CY_SCBIP_V1)
            EMI_I2CM_SET_INP_DIS(EMI_I2CM_uart_rx_i2c_sda_spi_mosi_INP_DIS,
                                         EMI_I2CM_uart_rx_i2c_sda_spi_mosi_MASK,
                                         (0u != (pinsInBuf & EMI_I2CM_RX_SDA_MOSI_PIN_MASK)));
        #endif /* (!EMI_I2CM_CY_SCBIP_V1) */
    
    #elif (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN)
        EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG,
                                       EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_MASK,
                                       EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_POS,
                                       hsiomSel[EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX]);

        EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi_SetDriveMode((uint8)
                                                               pinsDm[EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX]);

        EMI_I2CM_SET_INP_DIS(EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi_INP_DIS,
                                     EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi_MASK,
                                     (0u != (pinsInBuf & EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_MASK)));

         /* Set interrupt on falling edge */
        EMI_I2CM_SET_INCFG_TYPE(EMI_I2CM_RX_WAKE_SDA_MOSI_INTCFG_REG,
                                        EMI_I2CM_RX_WAKE_SDA_MOSI_INTCFG_TYPE_MASK,
                                        EMI_I2CM_RX_WAKE_SDA_MOSI_INTCFG_TYPE_POS,
                                        EMI_I2CM_INTCFG_TYPE_FALLING_EDGE);
    #else
    #endif /* (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN) */

    #if (EMI_I2CM_TX_SCL_MISO_PIN)
        EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_TX_SCL_MISO_HSIOM_REG,
                                       EMI_I2CM_TX_SCL_MISO_HSIOM_MASK,
                                       EMI_I2CM_TX_SCL_MISO_HSIOM_POS,
                                        hsiomSel[EMI_I2CM_TX_SCL_MISO_PIN_INDEX]);

        EMI_I2CM_uart_tx_i2c_scl_spi_miso_SetDriveMode((uint8) pinsDm[EMI_I2CM_TX_SCL_MISO_PIN_INDEX]);

    #if (!EMI_I2CM_CY_SCBIP_V1)
        EMI_I2CM_SET_INP_DIS(EMI_I2CM_uart_tx_i2c_scl_spi_miso_INP_DIS,
                                     EMI_I2CM_uart_tx_i2c_scl_spi_miso_MASK,
                                    (0u != (pinsInBuf & EMI_I2CM_TX_SCL_MISO_PIN_MASK)));
    #endif /* (!EMI_I2CM_CY_SCBIP_V1) */
    #endif /* (EMI_I2CM_RX_SDA_MOSI_PIN) */

    #if (EMI_I2CM_CTS_SCLK_PIN)
        EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_CTS_SCLK_HSIOM_REG,
                                       EMI_I2CM_CTS_SCLK_HSIOM_MASK,
                                       EMI_I2CM_CTS_SCLK_HSIOM_POS,
                                       hsiomSel[EMI_I2CM_CTS_SCLK_PIN_INDEX]);

        EMI_I2CM_uart_cts_spi_sclk_SetDriveMode((uint8) pinsDm[EMI_I2CM_CTS_SCLK_PIN_INDEX]);

        EMI_I2CM_SET_INP_DIS(EMI_I2CM_uart_cts_spi_sclk_INP_DIS,
                                     EMI_I2CM_uart_cts_spi_sclk_MASK,
                                     (0u != (pinsInBuf & EMI_I2CM_CTS_SCLK_PIN_MASK)));
    #endif /* (EMI_I2CM_CTS_SCLK_PIN) */

    #if (EMI_I2CM_RTS_SS0_PIN)
        EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_RTS_SS0_HSIOM_REG,
                                       EMI_I2CM_RTS_SS0_HSIOM_MASK,
                                       EMI_I2CM_RTS_SS0_HSIOM_POS,
                                       hsiomSel[EMI_I2CM_RTS_SS0_PIN_INDEX]);

        EMI_I2CM_uart_rts_spi_ss0_SetDriveMode((uint8) pinsDm[EMI_I2CM_RTS_SS0_PIN_INDEX]);

        EMI_I2CM_SET_INP_DIS(EMI_I2CM_uart_rts_spi_ss0_INP_DIS,
                                     EMI_I2CM_uart_rts_spi_ss0_MASK,
                                     (0u != (pinsInBuf & EMI_I2CM_RTS_SS0_PIN_MASK)));
    #endif /* (EMI_I2CM_RTS_SS0_PIN) */

    #if (EMI_I2CM_SS1_PIN)
        EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_SS1_HSIOM_REG,
                                       EMI_I2CM_SS1_HSIOM_MASK,
                                       EMI_I2CM_SS1_HSIOM_POS,
                                       hsiomSel[EMI_I2CM_SS1_PIN_INDEX]);

        EMI_I2CM_spi_ss1_SetDriveMode((uint8) pinsDm[EMI_I2CM_SS1_PIN_INDEX]);

        EMI_I2CM_SET_INP_DIS(EMI_I2CM_spi_ss1_INP_DIS,
                                     EMI_I2CM_spi_ss1_MASK,
                                     (0u != (pinsInBuf & EMI_I2CM_SS1_PIN_MASK)));
    #endif /* (EMI_I2CM_SS1_PIN) */

    #if (EMI_I2CM_SS2_PIN)
        EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_SS2_HSIOM_REG,
                                       EMI_I2CM_SS2_HSIOM_MASK,
                                       EMI_I2CM_SS2_HSIOM_POS,
                                       hsiomSel[EMI_I2CM_SS2_PIN_INDEX]);

        EMI_I2CM_spi_ss2_SetDriveMode((uint8) pinsDm[EMI_I2CM_SS2_PIN_INDEX]);

        EMI_I2CM_SET_INP_DIS(EMI_I2CM_spi_ss2_INP_DIS,
                                     EMI_I2CM_spi_ss2_MASK,
                                     (0u != (pinsInBuf & EMI_I2CM_SS2_PIN_MASK)));
    #endif /* (EMI_I2CM_SS2_PIN) */

    #if (EMI_I2CM_SS3_PIN)
        EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_SS3_HSIOM_REG,
                                       EMI_I2CM_SS3_HSIOM_MASK,
                                       EMI_I2CM_SS3_HSIOM_POS,
                                       hsiomSel[EMI_I2CM_SS3_PIN_INDEX]);

        EMI_I2CM_spi_ss3_SetDriveMode((uint8) pinsDm[EMI_I2CM_SS3_PIN_INDEX]);

        EMI_I2CM_SET_INP_DIS(EMI_I2CM_spi_ss3_INP_DIS,
                                     EMI_I2CM_spi_ss3_MASK,
                                     (0u != (pinsInBuf & EMI_I2CM_SS3_PIN_MASK)));
    #endif /* (EMI_I2CM_SS3_PIN) */
    }

#endif /* (EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG) */


#if (EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    /*******************************************************************************
    * Function Name: EMI_I2CM_I2CSlaveNackGeneration
    ****************************************************************************//**
    *
    *  Sets command to generate NACK to the address or data.
    *
    *******************************************************************************/
    void EMI_I2CM_I2CSlaveNackGeneration(void)
    {
        /* Check for EC_AM toggle condition: EC_AM and clock stretching for address are enabled */
        if ((0u != (EMI_I2CM_CTRL_REG & EMI_I2CM_CTRL_EC_AM_MODE)) &&
            (0u == (EMI_I2CM_I2C_CTRL_REG & EMI_I2CM_I2C_CTRL_S_NOT_READY_ADDR_NACK)))
        {
            /* Toggle EC_AM before NACK generation */
            EMI_I2CM_CTRL_REG &= ~EMI_I2CM_CTRL_EC_AM_MODE;
            EMI_I2CM_CTRL_REG |=  EMI_I2CM_CTRL_EC_AM_MODE;
        }

        EMI_I2CM_I2C_SLAVE_CMD_REG = EMI_I2CM_I2C_SLAVE_CMD_S_NACK;
    }
#endif /* (EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */


/* [] END OF FILE */
