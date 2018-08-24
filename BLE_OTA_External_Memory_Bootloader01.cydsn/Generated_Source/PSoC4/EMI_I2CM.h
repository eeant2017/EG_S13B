/***************************************************************************//**
* \file EMI_I2CM.h
* \version 3.20
*
* \brief
*  This file provides constants and parameter values for the SCB Component.
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

#if !defined(CY_SCB_EMI_I2CM_H)
#define CY_SCB_EMI_I2CM_H

#include <cydevice_trm.h>
#include <cyfitter.h>
#include <cytypes.h>
#include <CyLib.h>

/* SCB IP block v0 is available in PSoC 4100/PSoC 4200 */
#define EMI_I2CM_CY_SCBIP_V0    (CYIPBLOCK_m0s8scb_VERSION == 0u)
/* SCB IP block v1 is available in PSoC 4000 */
#define EMI_I2CM_CY_SCBIP_V1    (CYIPBLOCK_m0s8scb_VERSION == 1u)
/* SCB IP block v2 is available in all other devices */
#define EMI_I2CM_CY_SCBIP_V2    (CYIPBLOCK_m0s8scb_VERSION >= 2u)

#define EMI_I2CM_SCB_MODE                     (1u)

/* SCB modes enum */
#define EMI_I2CM_SCB_MODE_I2C                 (0x01u)
#define EMI_I2CM_SCB_MODE_SPI                 (0x02u)
#define EMI_I2CM_SCB_MODE_UART                (0x04u)
#define EMI_I2CM_SCB_MODE_EZI2C               (0x08u)
#define EMI_I2CM_SCB_MODE_UNCONFIG            (0xFFu)

/* Condition compilation depends on operation mode: Unconfigured implies apply to all modes */
#define EMI_I2CM_SCB_MODE_I2C_CONST_CFG       (EMI_I2CM_SCB_MODE_I2C       == EMI_I2CM_SCB_MODE)
#define EMI_I2CM_SCB_MODE_SPI_CONST_CFG       (EMI_I2CM_SCB_MODE_SPI       == EMI_I2CM_SCB_MODE)
#define EMI_I2CM_SCB_MODE_UART_CONST_CFG      (EMI_I2CM_SCB_MODE_UART      == EMI_I2CM_SCB_MODE)
#define EMI_I2CM_SCB_MODE_EZI2C_CONST_CFG     (EMI_I2CM_SCB_MODE_EZI2C     == EMI_I2CM_SCB_MODE)
#define EMI_I2CM_SCB_MODE_UNCONFIG_CONST_CFG  (EMI_I2CM_SCB_MODE_UNCONFIG  == EMI_I2CM_SCB_MODE)

/* Condition compilation for includes */
#define EMI_I2CM_SCB_MODE_I2C_INC      (0u !=(EMI_I2CM_SCB_MODE_I2C   & EMI_I2CM_SCB_MODE))
#define EMI_I2CM_SCB_MODE_EZI2C_INC    (0u !=(EMI_I2CM_SCB_MODE_EZI2C & EMI_I2CM_SCB_MODE))
#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_SCB_MODE_SPI_INC  (0u !=(EMI_I2CM_SCB_MODE_SPI   & EMI_I2CM_SCB_MODE))
    #define EMI_I2CM_SCB_MODE_UART_INC (0u !=(EMI_I2CM_SCB_MODE_UART  & EMI_I2CM_SCB_MODE))
#else
    #define EMI_I2CM_SCB_MODE_SPI_INC  (0u)
    #define EMI_I2CM_SCB_MODE_UART_INC (0u)
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

/* Interrupts remove options */
#define EMI_I2CM_REMOVE_SCB_IRQ             (0u)
#define EMI_I2CM_SCB_IRQ_INTERNAL           (0u == EMI_I2CM_REMOVE_SCB_IRQ)

#define EMI_I2CM_REMOVE_UART_RX_WAKEUP_IRQ  (1u)
#define EMI_I2CM_UART_RX_WAKEUP_IRQ         (0u == EMI_I2CM_REMOVE_UART_RX_WAKEUP_IRQ)

/* SCB interrupt enum */
#define EMI_I2CM_SCB_INTR_MODE_NONE     (0u)
#define EMI_I2CM_SCB_INTR_MODE_INTERNAL (1u)
#define EMI_I2CM_SCB_INTR_MODE_EXTERNAL (2u)

/* Internal clock remove option */
#define EMI_I2CM_REMOVE_SCB_CLK     (0u)
#define EMI_I2CM_SCB_CLK_INTERNAL   (0u == EMI_I2CM_REMOVE_SCB_CLK)


/***************************************
*       Includes
****************************************/

#include "EMI_I2CM_PINS.h"

#if (EMI_I2CM_SCB_CLK_INTERNAL)
    #include "EMI_I2CM_SCBCLK.h"
#endif /* (EMI_I2CM_SCB_CLK_INTERNAL) */


/***************************************
*       Type Definitions
***************************************/

typedef struct
{
    uint8 enableState;
} EMI_I2CM_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

/**
* \addtogroup group_general
* @{
*/

/* Start and Stop APIs */
void EMI_I2CM_Init(void);
void EMI_I2CM_Enable(void);
void EMI_I2CM_Start(void);
void EMI_I2CM_Stop(void);

/** @} general */

/**
* \addtogroup group_power
* @{
*/
/* Sleep and Wakeup APis */
void EMI_I2CM_Sleep(void);
void EMI_I2CM_Wakeup(void);
/** @} power */ 

/**
* \addtogroup group_interrupt
* @{
*/
#if (EMI_I2CM_SCB_IRQ_INTERNAL)
    /* Custom interrupt handler */
    void EMI_I2CM_SetCustomInterruptHandler(void (*func)(void));
#endif /* (EMI_I2CM_SCB_IRQ_INTERNAL) */
/** @} interrupt */

/* Interface to internal interrupt component */
#if (EMI_I2CM_SCB_IRQ_INTERNAL)
    /**
    * \addtogroup group_interrupt
    * @{
    */    
    /*******************************************************************************
    * Function Name: EMI_I2CM_EnableInt
    ****************************************************************************//**
    *
    *  When using an Internal interrupt, this enables the interrupt in the NVIC. 
    *  When using an external interrupt the API for the interrupt component must 
    *  be used to enable the interrupt.
    *
    *******************************************************************************/
    #define EMI_I2CM_EnableInt()    CyIntEnable(EMI_I2CM_ISR_NUMBER)
    
    
    /*******************************************************************************
    * Function Name: EMI_I2CM_DisableInt
    ****************************************************************************//**
    *
    *  When using an Internal interrupt, this disables the interrupt in the NVIC. 
    *  When using an external interrupt the API for the interrupt component must 
    *  be used to disable the interrupt.
    *
    *******************************************************************************/    
    #define EMI_I2CM_DisableInt()   CyIntDisable(EMI_I2CM_ISR_NUMBER)
    /** @} interrupt */

    /*******************************************************************************
    * Function Name: EMI_I2CM_ClearPendingInt
    ****************************************************************************//**
    *
    *  This function clears the interrupt pending status in the NVIC. 
    *
    *******************************************************************************/
    #define EMI_I2CM_ClearPendingInt()  CyIntClearPending(EMI_I2CM_ISR_NUMBER)
#endif /* (EMI_I2CM_SCB_IRQ_INTERNAL) */

#if (EMI_I2CM_UART_RX_WAKEUP_IRQ)
    /*******************************************************************************
    * Function Name: EMI_I2CM_RxWakeEnableInt
    ****************************************************************************//**
    *
    *  This function enables the interrupt (RX_WAKE) pending status in the NVIC. 
    *
    *******************************************************************************/    
    #define EMI_I2CM_RxWakeEnableInt()  CyIntEnable(EMI_I2CM_RX_WAKE_ISR_NUMBER)
    

    /*******************************************************************************
    * Function Name: EMI_I2CM_RxWakeDisableInt
    ****************************************************************************//**
    *
    *  This function disables the interrupt (RX_WAKE) pending status in the NVIC.  
    *
    *******************************************************************************/
    #define EMI_I2CM_RxWakeDisableInt() CyIntDisable(EMI_I2CM_RX_WAKE_ISR_NUMBER)
    
    
    /*******************************************************************************
    * Function Name: EMI_I2CM_RxWakeClearPendingInt
    ****************************************************************************//**
    *
    *  This function clears the interrupt (RX_WAKE) pending status in the NVIC. 
    *
    *******************************************************************************/    
    #define EMI_I2CM_RxWakeClearPendingInt()  CyIntClearPending(EMI_I2CM_RX_WAKE_ISR_NUMBER)
#endif /* (EMI_I2CM_UART_RX_WAKEUP_IRQ) */

/**
* \addtogroup group_interrupt
* @{
*/
/* Get interrupt cause */
/*******************************************************************************
* Function Name: EMI_I2CM_GetInterruptCause
****************************************************************************//**
*
*  Returns a mask of bits showing the source of the current triggered interrupt. 
*  This is useful for modes of operation where an interrupt can be generated by 
*  conditions in multiple interrupt source registers.
*
*  \return
*   Mask with the OR of the following conditions that have been triggered.
*    - EMI_I2CM_INTR_CAUSE_MASTER - Interrupt from Master
*    - EMI_I2CM_INTR_CAUSE_SLAVE - Interrupt from Slave
*    - EMI_I2CM_INTR_CAUSE_TX - Interrupt from TX
*    - EMI_I2CM_INTR_CAUSE_RX - Interrupt from RX
*
*******************************************************************************/
#define EMI_I2CM_GetInterruptCause()    (EMI_I2CM_INTR_CAUSE_REG)


/* APIs to service INTR_RX register */
/*******************************************************************************
* Function Name: EMI_I2CM_GetRxInterruptSource
****************************************************************************//**
*
*  Returns RX interrupt request register. This register contains current status 
*  of RX interrupt sources.
*
*  \return
*   Current status of RX interrupt sources.
*   Each constant is a bit field value. The value returned may have multiple 
*   bits set to indicate the current status.
*   - EMI_I2CM_INTR_RX_FIFO_LEVEL - The number of data elements in the 
      RX FIFO is greater than the value of RX FIFO level.
*   - EMI_I2CM_INTR_RX_NOT_EMPTY - Receiver FIFO is not empty.
*   - EMI_I2CM_INTR_RX_FULL - Receiver FIFO is full.
*   - EMI_I2CM_INTR_RX_OVERFLOW - Attempt to write to a full 
*     receiver FIFO.
*   - EMI_I2CM_INTR_RX_UNDERFLOW - Attempt to read from an empty 
*     receiver FIFO.
*   - EMI_I2CM_INTR_RX_FRAME_ERROR - UART framing error detected.
*   - EMI_I2CM_INTR_RX_PARITY_ERROR - UART parity error detected.
*
*******************************************************************************/
#define EMI_I2CM_GetRxInterruptSource() (EMI_I2CM_INTR_RX_REG)


/*******************************************************************************
* Function Name: EMI_I2CM_SetRxInterruptMode
****************************************************************************//**
*
*  Writes RX interrupt mask register. This register configures which bits from 
*  RX interrupt request register will trigger an interrupt event.
*
*  \param interruptMask: RX interrupt sources to be enabled (refer to 
*   EMI_I2CM_GetRxInterruptSource() function for bit fields values).
*
*******************************************************************************/
#define EMI_I2CM_SetRxInterruptMode(interruptMask)     EMI_I2CM_WRITE_INTR_RX_MASK(interruptMask)


/*******************************************************************************
* Function Name: EMI_I2CM_GetRxInterruptMode
****************************************************************************//**
*
*  Returns RX interrupt mask register This register specifies which bits from 
*  RX interrupt request register will trigger an interrupt event.
*
*  \return 
*   RX interrupt sources to be enabled (refer to 
*   EMI_I2CM_GetRxInterruptSource() function for bit fields values).
*
*******************************************************************************/
#define EMI_I2CM_GetRxInterruptMode()   (EMI_I2CM_INTR_RX_MASK_REG)


/*******************************************************************************
* Function Name: EMI_I2CM_GetRxInterruptSourceMasked
****************************************************************************//**
*
*  Returns RX interrupt masked request register. This register contains logical
*  AND of corresponding bits from RX interrupt request and mask registers.
*  This function is intended to be used in the interrupt service routine to 
*  identify which of enabled RX interrupt sources cause interrupt event.
*
*  \return 
*   Current status of enabled RX interrupt sources (refer to 
*   EMI_I2CM_GetRxInterruptSource() function for bit fields values).
*
*******************************************************************************/
#define EMI_I2CM_GetRxInterruptSourceMasked()   (EMI_I2CM_INTR_RX_MASKED_REG)


/*******************************************************************************
* Function Name: EMI_I2CM_ClearRxInterruptSource
****************************************************************************//**
*
*  Clears RX interrupt sources in the interrupt request register.
*
*  \param interruptMask: RX interrupt sources to be cleared (refer to 
*   EMI_I2CM_GetRxInterruptSource() function for bit fields values).
*
*  \sideeffects 
*   The side effects are listed in the table below for each 
*   affected interrupt source. Refer to section RX FIFO interrupt sources for 
*   detailed description.
*   - EMI_I2CM_INTR_RX_FIFO_LEVEL Interrupt source is not cleared when 
*     the receiver FIFO has more entries than level.
*   - EMI_I2CM_INTR_RX_NOT_EMPTY Interrupt source is not cleared when
*     receiver FIFO is not empty.
*   - EMI_I2CM_INTR_RX_FULL Interrupt source is not cleared when 
*      receiver FIFO is full.
*
*******************************************************************************/
#define EMI_I2CM_ClearRxInterruptSource(interruptMask)  EMI_I2CM_CLEAR_INTR_RX(interruptMask)


/*******************************************************************************
* Function Name: EMI_I2CM_SetRxInterrupt
****************************************************************************//**
*
*  Sets RX interrupt sources in the interrupt request register.
*
*  \param interruptMask: RX interrupt sources to set in the RX interrupt request 
*   register (refer to EMI_I2CM_GetRxInterruptSource() function for bit 
*   fields values).
*
*******************************************************************************/
#define EMI_I2CM_SetRxInterrupt(interruptMask)  EMI_I2CM_SET_INTR_RX(interruptMask)

void EMI_I2CM_SetRxFifoLevel(uint32 level);


/* APIs to service INTR_TX register */
/*******************************************************************************
* Function Name: EMI_I2CM_GetTxInterruptSource
****************************************************************************//**
*
*  Returns TX interrupt request register. This register contains current status 
*  of TX interrupt sources.
* 
*  \return 
*   Current status of TX interrupt sources.
*   Each constant is a bit field value. The value returned may have multiple 
*   bits set to indicate the current status.
*   - EMI_I2CM_INTR_TX_FIFO_LEVEL - The number of data elements in the 
*     TX FIFO is less than the value of TX FIFO level.
*   - EMI_I2CM_INTR_TX_NOT_FULL - Transmitter FIFO is not full.
*   - EMI_I2CM_INTR_TX_EMPTY - Transmitter FIFO is empty.
*   - EMI_I2CM_INTR_TX_OVERFLOW - Attempt to write to a full 
*     transmitter FIFO.
*   - EMI_I2CM_INTR_TX_UNDERFLOW - Attempt to read from an empty 
*     transmitter FIFO.
*   - EMI_I2CM_INTR_TX_UART_NACK - UART received a NACK in SmartCard 
*   mode.
*   - EMI_I2CM_INTR_TX_UART_DONE - UART transfer is complete. 
*     All data elements from the TX FIFO are sent.
*   - EMI_I2CM_INTR_TX_UART_ARB_LOST - Value on the TX line of the UART
*     does not match the value on the RX line.
*
*******************************************************************************/
#define EMI_I2CM_GetTxInterruptSource() (EMI_I2CM_INTR_TX_REG)


/*******************************************************************************
* Function Name: EMI_I2CM_SetTxInterruptMode
****************************************************************************//**
*
*  Writes TX interrupt mask register. This register configures which bits from 
*  TX interrupt request register will trigger an interrupt event.
*
*  \param interruptMask: TX interrupt sources to be enabled (refer to 
*   EMI_I2CM_GetTxInterruptSource() function for bit field values).
*
*******************************************************************************/
#define EMI_I2CM_SetTxInterruptMode(interruptMask)  EMI_I2CM_WRITE_INTR_TX_MASK(interruptMask)


/*******************************************************************************
* Function Name: EMI_I2CM_GetTxInterruptMode
****************************************************************************//**
*
*  Returns TX interrupt mask register This register specifies which bits from 
*  TX interrupt request register will trigger an interrupt event.
*
*  \return 
*   Enabled TX interrupt sources (refer to 
*   EMI_I2CM_GetTxInterruptSource() function for bit field values).
*   
*******************************************************************************/
#define EMI_I2CM_GetTxInterruptMode()   (EMI_I2CM_INTR_TX_MASK_REG)


/*******************************************************************************
* Function Name: EMI_I2CM_GetTxInterruptSourceMasked
****************************************************************************//**
*
*  Returns TX interrupt masked request register. This register contains logical
*  AND of corresponding bits from TX interrupt request and mask registers.
*  This function is intended to be used in the interrupt service routine to identify 
*  which of enabled TX interrupt sources cause interrupt event.
*
*  \return 
*   Current status of enabled TX interrupt sources (refer to 
*   EMI_I2CM_GetTxInterruptSource() function for bit field values).
*
*******************************************************************************/
#define EMI_I2CM_GetTxInterruptSourceMasked()   (EMI_I2CM_INTR_TX_MASKED_REG)


/*******************************************************************************
* Function Name: EMI_I2CM_ClearTxInterruptSource
****************************************************************************//**
*
*  Clears TX interrupt sources in the interrupt request register.
*
*  \param interruptMask: TX interrupt sources to be cleared (refer to 
*   EMI_I2CM_GetTxInterruptSource() function for bit field values).
*
*  \sideeffects 
*   The side effects are listed in the table below for each affected interrupt 
*   source. Refer to section TX FIFO interrupt sources for detailed description.
*   - EMI_I2CM_INTR_TX_FIFO_LEVEL - Interrupt source is not cleared when 
*     transmitter FIFO has less entries than level.
*   - EMI_I2CM_INTR_TX_NOT_FULL - Interrupt source is not cleared when
*     transmitter FIFO has empty entries.
*   - EMI_I2CM_INTR_TX_EMPTY - Interrupt source is not cleared when 
*     transmitter FIFO is empty.
*   - EMI_I2CM_INTR_TX_UNDERFLOW - Interrupt source is not cleared when 
*     transmitter FIFO is empty and I2C mode with clock stretching is selected. 
*     Put data into the transmitter FIFO before clearing it. This behavior only 
*     applicable for PSoC 4100/PSoC 4200 devices.
*
*******************************************************************************/
#define EMI_I2CM_ClearTxInterruptSource(interruptMask)  EMI_I2CM_CLEAR_INTR_TX(interruptMask)


/*******************************************************************************
* Function Name: EMI_I2CM_SetTxInterrupt
****************************************************************************//**
*
*  Sets RX interrupt sources in the interrupt request register.
*
*  \param interruptMask: RX interrupt sources to set in the RX interrupt request 
*   register (refer to EMI_I2CM_GetRxInterruptSource() function for bit 
*   fields values).
*
*******************************************************************************/
#define EMI_I2CM_SetTxInterrupt(interruptMask)  EMI_I2CM_SET_INTR_TX(interruptMask)

void EMI_I2CM_SetTxFifoLevel(uint32 level);


/* APIs to service INTR_MASTER register */
/*******************************************************************************
* Function Name: EMI_I2CM_GetMasterInterruptSource
****************************************************************************//**
*
*  Returns Master interrupt request register. This register contains current 
*  status of Master interrupt sources.
*
*  \return 
*   Current status of Master interrupt sources. 
*   Each constant is a bit field value. The value returned may have multiple 
*   bits set to indicate the current status.
*   - EMI_I2CM_INTR_MASTER_SPI_DONE - SPI master transfer is complete.
*     Refer to Interrupt sources section for detailed description.
*   - EMI_I2CM_INTR_MASTER_I2C_ARB_LOST - I2C master lost arbitration.
*   - EMI_I2CM_INTR_MASTER_I2C_NACK - I2C master received negative 
*    acknowledgement (NAK).
*   - EMI_I2CM_INTR_MASTER_I2C_ACK - I2C master received acknowledgement.
*   - EMI_I2CM_INTR_MASTER_I2C_STOP - I2C master generated STOP.
*   - EMI_I2CM_INTR_MASTER_I2C_BUS_ERROR - I2C master bus error 
*     (detection of unexpected START or STOP condition).
*
*******************************************************************************/
#define EMI_I2CM_GetMasterInterruptSource() (EMI_I2CM_INTR_MASTER_REG)

/*******************************************************************************
* Function Name: EMI_I2CM_SetMasterInterruptMode
****************************************************************************//**
*
*  Writes Master interrupt mask register. This register configures which bits 
*  from Master interrupt request register will trigger an interrupt event.
*
*  \param interruptMask: Master interrupt sources to be enabled (refer to 
*   EMI_I2CM_GetMasterInterruptSource() function for bit field values).
*
*******************************************************************************/
#define EMI_I2CM_SetMasterInterruptMode(interruptMask)  EMI_I2CM_WRITE_INTR_MASTER_MASK(interruptMask)

/*******************************************************************************
* Function Name: EMI_I2CM_GetMasterInterruptMode
****************************************************************************//**
*
*  Returns Master interrupt mask register This register specifies which bits 
*  from Master interrupt request register will trigger an interrupt event.
*
*  \return 
*   Enabled Master interrupt sources (refer to 
*   EMI_I2CM_GetMasterInterruptSource() function for return values).
*
*******************************************************************************/
#define EMI_I2CM_GetMasterInterruptMode()   (EMI_I2CM_INTR_MASTER_MASK_REG)

/*******************************************************************************
* Function Name: EMI_I2CM_GetMasterInterruptSourceMasked
****************************************************************************//**
*
*  Returns Master interrupt masked request register. This register contains 
*  logical AND of corresponding bits from Master interrupt request and mask 
*  registers.
*  This function is intended to be used in the interrupt service routine to 
*  identify which of enabled Master interrupt sources cause interrupt event.
*
*  \return 
*   Current status of enabled Master interrupt sources (refer to 
*   EMI_I2CM_GetMasterInterruptSource() function for return values).
*
*******************************************************************************/
#define EMI_I2CM_GetMasterInterruptSourceMasked()   (EMI_I2CM_INTR_MASTER_MASKED_REG)

/*******************************************************************************
* Function Name: EMI_I2CM_ClearMasterInterruptSource
****************************************************************************//**
*
*  Clears Master interrupt sources in the interrupt request register.
*
*  \param interruptMask: Master interrupt sources to be cleared (refer to 
*   EMI_I2CM_GetMasterInterruptSource() function for bit field values).
*
*******************************************************************************/
#define EMI_I2CM_ClearMasterInterruptSource(interruptMask)  EMI_I2CM_CLEAR_INTR_MASTER(interruptMask)

/*******************************************************************************
* Function Name: EMI_I2CM_SetMasterInterrupt
****************************************************************************//**
*
*  Sets Master interrupt sources in the interrupt request register.
*
*  \param interruptMask: Master interrupt sources to set in the Master interrupt
*   request register (refer to EMI_I2CM_GetMasterInterruptSource() 
*   function for bit field values).
*
*******************************************************************************/
#define EMI_I2CM_SetMasterInterrupt(interruptMask)  EMI_I2CM_SET_INTR_MASTER(interruptMask)


/* APIs to service INTR_SLAVE register */
/*******************************************************************************
* Function Name: EMI_I2CM_GetSlaveInterruptSource
****************************************************************************//**
*
*  Returns Slave interrupt request register. This register contains current 
*  status of Slave interrupt sources.
*
*  \return 
*   Current status of Slave interrupt sources.
*   Each constant is a bit field value. The value returned may have multiple 
*   bits set to indicate the current status.
*   - EMI_I2CM_INTR_SLAVE_I2C_ARB_LOST - I2C slave lost arbitration: 
*     the value driven on the SDA line is not the same as the value observed 
*     on the SDA line.
*   - EMI_I2CM_INTR_SLAVE_I2C_NACK - I2C slave received negative 
*     acknowledgement (NAK).
*   - EMI_I2CM_INTR_SLAVE_I2C_ACK - I2C slave received 
*     acknowledgement (ACK).
*   - EMI_I2CM_INTR_SLAVE_I2C_WRITE_STOP - Stop or Repeated Start 
*     event for write transfer intended for this slave (address matching 
*     is performed).
*   - EMI_I2CM_INTR_SLAVE_I2C_STOP - Stop or Repeated Start event 
*     for (read or write) transfer intended for this slave (address matching 
*     is performed).
*   - EMI_I2CM_INTR_SLAVE_I2C_START - I2C slave received Start 
*     condition.
*   - EMI_I2CM_INTR_SLAVE_I2C_ADDR_MATCH - I2C slave received matching 
*     address.
*   - EMI_I2CM_INTR_SLAVE_I2C_GENERAL - I2C Slave received general 
*     call address.
*   - EMI_I2CM_INTR_SLAVE_I2C_BUS_ERROR - I2C slave bus error (detection 
*      of unexpected Start or Stop condition).
*   - EMI_I2CM_INTR_SLAVE_SPI_BUS_ERROR - SPI slave select line is 
*      deselected at an expected time while the SPI transfer.
*
*******************************************************************************/
#define EMI_I2CM_GetSlaveInterruptSource()  (EMI_I2CM_INTR_SLAVE_REG)

/*******************************************************************************
* Function Name: EMI_I2CM_SetSlaveInterruptMode
****************************************************************************//**
*
*  Writes Slave interrupt mask register. 
*  This register configures which bits from Slave interrupt request register 
*  will trigger an interrupt event.
*
*  \param interruptMask: Slave interrupt sources to be enabled (refer to 
*   EMI_I2CM_GetSlaveInterruptSource() function for bit field values).
*
*******************************************************************************/
#define EMI_I2CM_SetSlaveInterruptMode(interruptMask)   EMI_I2CM_WRITE_INTR_SLAVE_MASK(interruptMask)

/*******************************************************************************
* Function Name: EMI_I2CM_GetSlaveInterruptMode
****************************************************************************//**
*
*  Returns Slave interrupt mask register.
*  This register specifies which bits from Slave interrupt request register 
*  will trigger an interrupt event.
*
*  \return 
*   Enabled Slave interrupt sources(refer to 
*   EMI_I2CM_GetSlaveInterruptSource() function for bit field values).
*
*******************************************************************************/
#define EMI_I2CM_GetSlaveInterruptMode()    (EMI_I2CM_INTR_SLAVE_MASK_REG)

/*******************************************************************************
* Function Name: EMI_I2CM_GetSlaveInterruptSourceMasked
****************************************************************************//**
*
*  Returns Slave interrupt masked request register. This register contains 
*  logical AND of corresponding bits from Slave interrupt request and mask 
*  registers.
*  This function is intended to be used in the interrupt service routine to 
*  identify which of enabled Slave interrupt sources cause interrupt event.
*
*  \return 
*   Current status of enabled Slave interrupt sources (refer to 
*   EMI_I2CM_GetSlaveInterruptSource() function for return values).
*
*******************************************************************************/
#define EMI_I2CM_GetSlaveInterruptSourceMasked()    (EMI_I2CM_INTR_SLAVE_MASKED_REG)

/*******************************************************************************
* Function Name: EMI_I2CM_ClearSlaveInterruptSource
****************************************************************************//**
*
*  Clears Slave interrupt sources in the interrupt request register.
*
*  \param interruptMask: Slave interrupt sources to be cleared (refer to 
*   EMI_I2CM_GetSlaveInterruptSource() function for return values).
*
*******************************************************************************/
#define EMI_I2CM_ClearSlaveInterruptSource(interruptMask)   EMI_I2CM_CLEAR_INTR_SLAVE(interruptMask)

/*******************************************************************************
* Function Name: EMI_I2CM_SetSlaveInterrupt
****************************************************************************//**
*
*  Sets Slave interrupt sources in the interrupt request register.
*
*  \param interruptMask: Slave interrupt sources to set in the Slave interrupt 
*   request register (refer to EMI_I2CM_GetSlaveInterruptSource() 
*   function for return values).
*
*******************************************************************************/
#define EMI_I2CM_SetSlaveInterrupt(interruptMask)   EMI_I2CM_SET_INTR_SLAVE(interruptMask)

/** @} interrupt */ 


/***************************************
*     Vars with External Linkage
***************************************/

/**
* \addtogroup group_globals
* @{
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
extern uint8 EMI_I2CM_initVar;
/** @} globals */

/***************************************
*              Registers
***************************************/

#define EMI_I2CM_CTRL_REG               (*(reg32 *) EMI_I2CM_SCB__CTRL)
#define EMI_I2CM_CTRL_PTR               ( (reg32 *) EMI_I2CM_SCB__CTRL)

#define EMI_I2CM_STATUS_REG             (*(reg32 *) EMI_I2CM_SCB__STATUS)
#define EMI_I2CM_STATUS_PTR             ( (reg32 *) EMI_I2CM_SCB__STATUS)

#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_SPI_CTRL_REG           (*(reg32 *) EMI_I2CM_SCB__SPI_CTRL)
    #define EMI_I2CM_SPI_CTRL_PTR           ( (reg32 *) EMI_I2CM_SCB__SPI_CTRL)

    #define EMI_I2CM_SPI_STATUS_REG         (*(reg32 *) EMI_I2CM_SCB__SPI_STATUS)
    #define EMI_I2CM_SPI_STATUS_PTR         ( (reg32 *) EMI_I2CM_SCB__SPI_STATUS)

    #define EMI_I2CM_UART_CTRL_REG          (*(reg32 *) EMI_I2CM_SCB__UART_CTRL)
    #define EMI_I2CM_UART_CTRL_PTR          ( (reg32 *) EMI_I2CM_SCB__UART_CTRL)

    #define EMI_I2CM_UART_TX_CTRL_REG       (*(reg32 *) EMI_I2CM_SCB__UART_TX_CTRL)
    #define EMI_I2CM_UART_TX_CTRL_PTR       ( (reg32 *) EMI_I2CM_SCB__UART_TX_CTRL)

    #define EMI_I2CM_UART_RX_CTRL_REG       (*(reg32 *) EMI_I2CM_SCB__UART_RX_CTRL)
    #define EMI_I2CM_UART_RX_CTRL_PTR       ( (reg32 *) EMI_I2CM_SCB__UART_RX_CTRL)

    #define EMI_I2CM_UART_RX_STATUS_REG     (*(reg32 *) EMI_I2CM_SCB__UART_RX_STATUS)
    #define EMI_I2CM_UART_RX_STATUS_PTR     ( (reg32 *) EMI_I2CM_SCB__UART_RX_STATUS)
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

#if !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_UART_FLOW_CTRL_REG     (*(reg32 *) EMI_I2CM_SCB__UART_FLOW_CTRL)
    #define EMI_I2CM_UART_FLOW_CTRL_PTR     ( (reg32 *) EMI_I2CM_SCB__UART_FLOW_CTRL)
#endif /* !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */

#define EMI_I2CM_I2C_CTRL_REG           (*(reg32 *) EMI_I2CM_SCB__I2C_CTRL)
#define EMI_I2CM_I2C_CTRL_PTR           ( (reg32 *) EMI_I2CM_SCB__I2C_CTRL)

#define EMI_I2CM_I2C_STATUS_REG         (*(reg32 *) EMI_I2CM_SCB__I2C_STATUS)
#define EMI_I2CM_I2C_STATUS_PTR         ( (reg32 *) EMI_I2CM_SCB__I2C_STATUS)

#define EMI_I2CM_I2C_MASTER_CMD_REG     (*(reg32 *) EMI_I2CM_SCB__I2C_M_CMD)
#define EMI_I2CM_I2C_MASTER_CMD_PTR     ( (reg32 *) EMI_I2CM_SCB__I2C_M_CMD)

#define EMI_I2CM_I2C_SLAVE_CMD_REG      (*(reg32 *) EMI_I2CM_SCB__I2C_S_CMD)
#define EMI_I2CM_I2C_SLAVE_CMD_PTR      ( (reg32 *) EMI_I2CM_SCB__I2C_S_CMD)

#define EMI_I2CM_I2C_CFG_REG            (*(reg32 *) EMI_I2CM_SCB__I2C_CFG)
#define EMI_I2CM_I2C_CFG_PTR            ( (reg32 *) EMI_I2CM_SCB__I2C_CFG)

#define EMI_I2CM_TX_CTRL_REG            (*(reg32 *) EMI_I2CM_SCB__TX_CTRL)
#define EMI_I2CM_TX_CTRL_PTR            ( (reg32 *) EMI_I2CM_SCB__TX_CTRL)

#define EMI_I2CM_TX_FIFO_CTRL_REG       (*(reg32 *) EMI_I2CM_SCB__TX_FIFO_CTRL)
#define EMI_I2CM_TX_FIFO_CTRL_PTR       ( (reg32 *) EMI_I2CM_SCB__TX_FIFO_CTRL)

#define EMI_I2CM_TX_FIFO_STATUS_REG     (*(reg32 *) EMI_I2CM_SCB__TX_FIFO_STATUS)
#define EMI_I2CM_TX_FIFO_STATUS_PTR     ( (reg32 *) EMI_I2CM_SCB__TX_FIFO_STATUS)

#define EMI_I2CM_TX_FIFO_WR_REG         (*(reg32 *) EMI_I2CM_SCB__TX_FIFO_WR)
#define EMI_I2CM_TX_FIFO_WR_PTR         ( (reg32 *) EMI_I2CM_SCB__TX_FIFO_WR)

#define EMI_I2CM_RX_CTRL_REG            (*(reg32 *) EMI_I2CM_SCB__RX_CTRL)
#define EMI_I2CM_RX_CTRL_PTR            ( (reg32 *) EMI_I2CM_SCB__RX_CTRL)

#define EMI_I2CM_RX_FIFO_CTRL_REG       (*(reg32 *) EMI_I2CM_SCB__RX_FIFO_CTRL)
#define EMI_I2CM_RX_FIFO_CTRL_PTR       ( (reg32 *) EMI_I2CM_SCB__RX_FIFO_CTRL)

#define EMI_I2CM_RX_FIFO_STATUS_REG     (*(reg32 *) EMI_I2CM_SCB__RX_FIFO_STATUS)
#define EMI_I2CM_RX_FIFO_STATUS_PTR     ( (reg32 *) EMI_I2CM_SCB__RX_FIFO_STATUS)

#define EMI_I2CM_RX_MATCH_REG           (*(reg32 *) EMI_I2CM_SCB__RX_MATCH)
#define EMI_I2CM_RX_MATCH_PTR           ( (reg32 *) EMI_I2CM_SCB__RX_MATCH)

#define EMI_I2CM_RX_FIFO_RD_REG         (*(reg32 *) EMI_I2CM_SCB__RX_FIFO_RD)
#define EMI_I2CM_RX_FIFO_RD_PTR         ( (reg32 *) EMI_I2CM_SCB__RX_FIFO_RD)

#define EMI_I2CM_RX_FIFO_RD_SILENT_REG  (*(reg32 *) EMI_I2CM_SCB__RX_FIFO_RD_SILENT)
#define EMI_I2CM_RX_FIFO_RD_SILENT_PTR  ( (reg32 *) EMI_I2CM_SCB__RX_FIFO_RD_SILENT)

#ifdef EMI_I2CM_SCB__EZ_DATA0
    #define EMI_I2CM_EZBUF_DATA0_REG    (*(reg32 *) EMI_I2CM_SCB__EZ_DATA0)
    #define EMI_I2CM_EZBUF_DATA0_PTR    ( (reg32 *) EMI_I2CM_SCB__EZ_DATA0)
#else
    #define EMI_I2CM_EZBUF_DATA0_REG    (*(reg32 *) EMI_I2CM_SCB__EZ_DATA00)
    #define EMI_I2CM_EZBUF_DATA0_PTR    ( (reg32 *) EMI_I2CM_SCB__EZ_DATA00)
#endif /* EMI_I2CM_SCB__EZ_DATA00 */

#define EMI_I2CM_INTR_CAUSE_REG         (*(reg32 *) EMI_I2CM_SCB__INTR_CAUSE)
#define EMI_I2CM_INTR_CAUSE_PTR         ( (reg32 *) EMI_I2CM_SCB__INTR_CAUSE)

#define EMI_I2CM_INTR_I2C_EC_REG        (*(reg32 *) EMI_I2CM_SCB__INTR_I2C_EC)
#define EMI_I2CM_INTR_I2C_EC_PTR        ( (reg32 *) EMI_I2CM_SCB__INTR_I2C_EC)

#define EMI_I2CM_INTR_I2C_EC_MASK_REG   (*(reg32 *) EMI_I2CM_SCB__INTR_I2C_EC_MASK)
#define EMI_I2CM_INTR_I2C_EC_MASK_PTR   ( (reg32 *) EMI_I2CM_SCB__INTR_I2C_EC_MASK)

#define EMI_I2CM_INTR_I2C_EC_MASKED_REG (*(reg32 *) EMI_I2CM_SCB__INTR_I2C_EC_MASKED)
#define EMI_I2CM_INTR_I2C_EC_MASKED_PTR ( (reg32 *) EMI_I2CM_SCB__INTR_I2C_EC_MASKED)

#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_INTR_SPI_EC_REG        (*(reg32 *) EMI_I2CM_SCB__INTR_SPI_EC)
    #define EMI_I2CM_INTR_SPI_EC_PTR        ( (reg32 *) EMI_I2CM_SCB__INTR_SPI_EC)

    #define EMI_I2CM_INTR_SPI_EC_MASK_REG   (*(reg32 *) EMI_I2CM_SCB__INTR_SPI_EC_MASK)
    #define EMI_I2CM_INTR_SPI_EC_MASK_PTR   ( (reg32 *) EMI_I2CM_SCB__INTR_SPI_EC_MASK)

    #define EMI_I2CM_INTR_SPI_EC_MASKED_REG (*(reg32 *) EMI_I2CM_SCB__INTR_SPI_EC_MASKED)
    #define EMI_I2CM_INTR_SPI_EC_MASKED_PTR ( (reg32 *) EMI_I2CM_SCB__INTR_SPI_EC_MASKED)
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

#define EMI_I2CM_INTR_MASTER_REG        (*(reg32 *) EMI_I2CM_SCB__INTR_M)
#define EMI_I2CM_INTR_MASTER_PTR        ( (reg32 *) EMI_I2CM_SCB__INTR_M)

#define EMI_I2CM_INTR_MASTER_SET_REG    (*(reg32 *) EMI_I2CM_SCB__INTR_M_SET)
#define EMI_I2CM_INTR_MASTER_SET_PTR    ( (reg32 *) EMI_I2CM_SCB__INTR_M_SET)

#define EMI_I2CM_INTR_MASTER_MASK_REG   (*(reg32 *) EMI_I2CM_SCB__INTR_M_MASK)
#define EMI_I2CM_INTR_MASTER_MASK_PTR   ( (reg32 *) EMI_I2CM_SCB__INTR_M_MASK)

#define EMI_I2CM_INTR_MASTER_MASKED_REG (*(reg32 *) EMI_I2CM_SCB__INTR_M_MASKED)
#define EMI_I2CM_INTR_MASTER_MASKED_PTR ( (reg32 *) EMI_I2CM_SCB__INTR_M_MASKED)

#define EMI_I2CM_INTR_SLAVE_REG         (*(reg32 *) EMI_I2CM_SCB__INTR_S)
#define EMI_I2CM_INTR_SLAVE_PTR         ( (reg32 *) EMI_I2CM_SCB__INTR_S)

#define EMI_I2CM_INTR_SLAVE_SET_REG     (*(reg32 *) EMI_I2CM_SCB__INTR_S_SET)
#define EMI_I2CM_INTR_SLAVE_SET_PTR     ( (reg32 *) EMI_I2CM_SCB__INTR_S_SET)

#define EMI_I2CM_INTR_SLAVE_MASK_REG    (*(reg32 *) EMI_I2CM_SCB__INTR_S_MASK)
#define EMI_I2CM_INTR_SLAVE_MASK_PTR    ( (reg32 *) EMI_I2CM_SCB__INTR_S_MASK)

#define EMI_I2CM_INTR_SLAVE_MASKED_REG  (*(reg32 *) EMI_I2CM_SCB__INTR_S_MASKED)
#define EMI_I2CM_INTR_SLAVE_MASKED_PTR  ( (reg32 *) EMI_I2CM_SCB__INTR_S_MASKED)

#define EMI_I2CM_INTR_TX_REG            (*(reg32 *) EMI_I2CM_SCB__INTR_TX)
#define EMI_I2CM_INTR_TX_PTR            ( (reg32 *) EMI_I2CM_SCB__INTR_TX)

#define EMI_I2CM_INTR_TX_SET_REG        (*(reg32 *) EMI_I2CM_SCB__INTR_TX_SET)
#define EMI_I2CM_INTR_TX_SET_PTR        ( (reg32 *) EMI_I2CM_SCB__INTR_TX_SET)

#define EMI_I2CM_INTR_TX_MASK_REG       (*(reg32 *) EMI_I2CM_SCB__INTR_TX_MASK)
#define EMI_I2CM_INTR_TX_MASK_PTR       ( (reg32 *) EMI_I2CM_SCB__INTR_TX_MASK)

#define EMI_I2CM_INTR_TX_MASKED_REG     (*(reg32 *) EMI_I2CM_SCB__INTR_TX_MASKED)
#define EMI_I2CM_INTR_TX_MASKED_PTR     ( (reg32 *) EMI_I2CM_SCB__INTR_TX_MASKED)

#define EMI_I2CM_INTR_RX_REG            (*(reg32 *) EMI_I2CM_SCB__INTR_RX)
#define EMI_I2CM_INTR_RX_PTR            ( (reg32 *) EMI_I2CM_SCB__INTR_RX)

#define EMI_I2CM_INTR_RX_SET_REG        (*(reg32 *) EMI_I2CM_SCB__INTR_RX_SET)
#define EMI_I2CM_INTR_RX_SET_PTR        ( (reg32 *) EMI_I2CM_SCB__INTR_RX_SET)

#define EMI_I2CM_INTR_RX_MASK_REG       (*(reg32 *) EMI_I2CM_SCB__INTR_RX_MASK)
#define EMI_I2CM_INTR_RX_MASK_PTR       ( (reg32 *) EMI_I2CM_SCB__INTR_RX_MASK)

#define EMI_I2CM_INTR_RX_MASKED_REG     (*(reg32 *) EMI_I2CM_SCB__INTR_RX_MASKED)
#define EMI_I2CM_INTR_RX_MASKED_PTR     ( (reg32 *) EMI_I2CM_SCB__INTR_RX_MASKED)

/* Defines get from SCB IP parameters. */
#define EMI_I2CM_FIFO_SIZE      (8u)  /* TX or RX FIFO size. */
#define EMI_I2CM_EZ_DATA_NR     (32u)  /* Number of words in EZ memory. */ 
#define EMI_I2CM_ONE_BYTE_WIDTH (8u)            /* Number of bits in one byte. */
#define EMI_I2CM_FF_DATA_NR_LOG2_MASK       (0x0Fu)      /* Number of bits to represent a FIFO address. */
#define EMI_I2CM_FF_DATA_NR_LOG2_PLUS1_MASK (0x1Fu) /* Number of bits to represent #bytes in FIFO. */


/***************************************
*        Registers Constants
***************************************/

#if (EMI_I2CM_SCB_IRQ_INTERNAL)
    #define EMI_I2CM_ISR_NUMBER     ((uint8) EMI_I2CM_SCB_IRQ__INTC_NUMBER)
    #define EMI_I2CM_ISR_PRIORITY   ((uint8) EMI_I2CM_SCB_IRQ__INTC_PRIOR_NUM)
#endif /* (EMI_I2CM_SCB_IRQ_INTERNAL) */

#if (EMI_I2CM_UART_RX_WAKEUP_IRQ)
    #define EMI_I2CM_RX_WAKE_ISR_NUMBER     ((uint8) EMI_I2CM_RX_WAKEUP_IRQ__INTC_NUMBER)
    #define EMI_I2CM_RX_WAKE_ISR_PRIORITY   ((uint8) EMI_I2CM_RX_WAKEUP_IRQ__INTC_PRIOR_NUM)
#endif /* (EMI_I2CM_UART_RX_WAKEUP_IRQ) */

/* EMI_I2CM_CTRL_REG */
#define EMI_I2CM_CTRL_OVS_POS           (0u)  /* [3:0]   Oversampling factor                 */
#define EMI_I2CM_CTRL_EC_AM_MODE_POS    (8u)  /* [8]     Externally clocked address match    */
#define EMI_I2CM_CTRL_EC_OP_MODE_POS    (9u)  /* [9]     Externally clocked operation mode   */
#define EMI_I2CM_CTRL_EZBUF_MODE_POS    (10u) /* [10]    EZ buffer is enabled                */
#if !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_CTRL_BYTE_MODE_POS (11u) /* [11]    Determines the number of bits per FIFO data element */
#endif /* !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */
#define EMI_I2CM_CTRL_ADDR_ACCEPT_POS   (16u) /* [16]    Put matched address in RX FIFO       */
#define EMI_I2CM_CTRL_BLOCK_POS         (17u) /* [17]    Ext and Int logic to resolve collide */
#define EMI_I2CM_CTRL_MODE_POS          (24u) /* [25:24] Operation mode                       */
#define EMI_I2CM_CTRL_ENABLED_POS       (31u) /* [31]    Enable SCB block                     */
#define EMI_I2CM_CTRL_OVS_MASK          ((uint32) 0x0Fu)
#define EMI_I2CM_CTRL_EC_AM_MODE        ((uint32) 0x01u << EMI_I2CM_CTRL_EC_AM_MODE_POS)
#define EMI_I2CM_CTRL_EC_OP_MODE        ((uint32) 0x01u << EMI_I2CM_CTRL_EC_OP_MODE_POS)
#define EMI_I2CM_CTRL_EZBUF_MODE        ((uint32) 0x01u << EMI_I2CM_CTRL_EZBUF_MODE_POS)
#if !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_CTRL_BYTE_MODE ((uint32) 0x01u << EMI_I2CM_CTRL_BYTE_MODE_POS)
#endif /* !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */
#define EMI_I2CM_CTRL_ADDR_ACCEPT       ((uint32) 0x01u << EMI_I2CM_CTRL_ADDR_ACCEPT_POS)
#define EMI_I2CM_CTRL_BLOCK             ((uint32) 0x01u << EMI_I2CM_CTRL_BLOCK_POS)
#define EMI_I2CM_CTRL_MODE_MASK         ((uint32) 0x03u << EMI_I2CM_CTRL_MODE_POS)
#define EMI_I2CM_CTRL_MODE_I2C          ((uint32) 0x00u)
#define EMI_I2CM_CTRL_MODE_SPI          ((uint32) 0x01u << EMI_I2CM_CTRL_MODE_POS)
#define EMI_I2CM_CTRL_MODE_UART         ((uint32) 0x02u << EMI_I2CM_CTRL_MODE_POS)
#define EMI_I2CM_CTRL_ENABLED           ((uint32) 0x01u << EMI_I2CM_CTRL_ENABLED_POS)

/* EMI_I2CM_STATUS_REG */
#define EMI_I2CM_STATUS_EC_BUSY_POS     (0u)  /* [0] Bus busy. Externally clocked logic access to EZ memory */
#define EMI_I2CM_STATUS_EC_BUSY         ((uint32) 0x0Fu)

/* EMI_I2CM_SPI_CTRL_REG  */
#define EMI_I2CM_SPI_CTRL_CONTINUOUS_POS        (0u)  /* [0]     Continuous or Separated SPI data transfers */
#define EMI_I2CM_SPI_CTRL_SELECT_PRECEDE_POS    (1u)  /* [1]     Precedes or coincides start of data frame  */
#define EMI_I2CM_SPI_CTRL_CPHA_POS              (2u)  /* [2]     SCLK phase                                 */
#define EMI_I2CM_SPI_CTRL_CPOL_POS              (3u)  /* [3]     SCLK polarity                              */
#define EMI_I2CM_SPI_CTRL_LATE_MISO_SAMPLE_POS  (4u)  /* [4]     Late MISO sample enabled                   */
#if !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_SPI_CTRL_SCLK_CONTINUOUS_POS   (5u)  /* [5]     Enable continuous SCLK generation */
    #define EMI_I2CM_SPI_CTRL_SSEL0_POLARITY_POS    (8u)  /* [8]     SS0 polarity                      */
    #define EMI_I2CM_SPI_CTRL_SSEL1_POLARITY_POS    (9u)  /* [9]     SS1 polarity                      */
    #define EMI_I2CM_SPI_CTRL_SSEL2_POLARITY_POS    (10u) /* [10]    SS2 polarity                      */
    #define EMI_I2CM_SPI_CTRL_SSEL3_POLARITY_POS    (11u) /* [11]    SS3 polarity                      */
#endif /* !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */
#define EMI_I2CM_SPI_CTRL_LOOPBACK_POS          (16u) /* [16]    Local loop-back control enabled            */
#define EMI_I2CM_SPI_CTRL_MODE_POS              (24u) /* [25:24] Submode of SPI operation                   */
#define EMI_I2CM_SPI_CTRL_SLAVE_SELECT_POS      (26u) /* [27:26] Selects SPI SS signal                      */
#define EMI_I2CM_SPI_CTRL_MASTER_MODE_POS       (31u) /* [31]    Master mode enabled                        */
#define EMI_I2CM_SPI_CTRL_CONTINUOUS            ((uint32) 0x01u)
#define EMI_I2CM_SPI_CTRL_SELECT_PRECEDE        ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_SELECT_PRECEDE_POS)
#define EMI_I2CM_SPI_CTRL_SCLK_MODE_MASK        ((uint32) 0x03u << EMI_I2CM_SPI_CTRL_CPHA_POS)
#define EMI_I2CM_SPI_CTRL_CPHA                  ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_CPHA_POS)
#define EMI_I2CM_SPI_CTRL_CPOL                  ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_CPOL_POS)
#define EMI_I2CM_SPI_CTRL_LATE_MISO_SAMPLE      ((uint32) 0x01u << \
                                                                    EMI_I2CM_SPI_CTRL_LATE_MISO_SAMPLE_POS)
#if !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_SPI_CTRL_SCLK_CONTINUOUS  ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_SCLK_CONTINUOUS_POS)
    #define EMI_I2CM_SPI_CTRL_SSEL0_POLARITY   ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_SSEL0_POLARITY_POS)
    #define EMI_I2CM_SPI_CTRL_SSEL1_POLARITY   ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_SSEL1_POLARITY_POS)
    #define EMI_I2CM_SPI_CTRL_SSEL2_POLARITY   ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_SSEL2_POLARITY_POS)
    #define EMI_I2CM_SPI_CTRL_SSEL3_POLARITY   ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_SSEL3_POLARITY_POS)
    #define EMI_I2CM_SPI_CTRL_SSEL_POLARITY_MASK ((uint32)0x0Fu << EMI_I2CM_SPI_CTRL_SSEL0_POLARITY_POS)
#endif /* !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */

#define EMI_I2CM_SPI_CTRL_LOOPBACK              ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_LOOPBACK_POS)
#define EMI_I2CM_SPI_CTRL_MODE_MASK             ((uint32) 0x03u << EMI_I2CM_SPI_CTRL_MODE_POS)
#define EMI_I2CM_SPI_CTRL_MODE_MOTOROLA         ((uint32) 0x00u)
#define EMI_I2CM_SPI_CTRL_MODE_TI               ((uint32) 0x01u << EMI_I2CM_CTRL_MODE_POS)
#define EMI_I2CM_SPI_CTRL_MODE_NS               ((uint32) 0x02u << EMI_I2CM_CTRL_MODE_POS)
#define EMI_I2CM_SPI_CTRL_SLAVE_SELECT_MASK     ((uint32) 0x03u << EMI_I2CM_SPI_CTRL_SLAVE_SELECT_POS)
#define EMI_I2CM_SPI_CTRL_SLAVE_SELECT0         ((uint32) 0x00u)
#define EMI_I2CM_SPI_CTRL_SLAVE_SELECT1         ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_SLAVE_SELECT_POS)
#define EMI_I2CM_SPI_CTRL_SLAVE_SELECT2         ((uint32) 0x02u << EMI_I2CM_SPI_CTRL_SLAVE_SELECT_POS)
#define EMI_I2CM_SPI_CTRL_SLAVE_SELECT3         ((uint32) 0x03u << EMI_I2CM_SPI_CTRL_SLAVE_SELECT_POS)
#define EMI_I2CM_SPI_CTRL_MASTER                ((uint32) 0x01u << EMI_I2CM_SPI_CTRL_MASTER_MODE_POS)
#define EMI_I2CM_SPI_CTRL_SLAVE                 ((uint32) 0x00u)

/* EMI_I2CM_SPI_STATUS_REG  */
#define EMI_I2CM_SPI_STATUS_BUS_BUSY_POS    (0u)  /* [0]    Bus busy - slave selected */
#define EMI_I2CM_SPI_STATUS_EZBUF_ADDR_POS  (8u)  /* [15:8] EzAddress                 */
#define EMI_I2CM_SPI_STATUS_BUS_BUSY        ((uint32) 0x01u)
#define EMI_I2CM_SPI_STATUS_EZBUF_ADDR_MASK ((uint32) 0xFFu << EMI_I2CM_I2C_STATUS_EZBUF_ADDR_POS)

/* EMI_I2CM_UART_CTRL */
#define EMI_I2CM_UART_CTRL_LOOPBACK_POS         (16u) /* [16] Loop-back    */
#define EMI_I2CM_UART_CTRL_MODE_POS             (24u) /* [24] UART subMode */
#define EMI_I2CM_UART_CTRL_LOOPBACK             ((uint32) 0x01u << EMI_I2CM_UART_CTRL_LOOPBACK_POS)
#define EMI_I2CM_UART_CTRL_MODE_UART_STD        ((uint32) 0x00u)
#define EMI_I2CM_UART_CTRL_MODE_UART_SMARTCARD  ((uint32) 0x01u << EMI_I2CM_UART_CTRL_MODE_POS)
#define EMI_I2CM_UART_CTRL_MODE_UART_IRDA       ((uint32) 0x02u << EMI_I2CM_UART_CTRL_MODE_POS)
#define EMI_I2CM_UART_CTRL_MODE_MASK            ((uint32) 0x03u << EMI_I2CM_UART_CTRL_MODE_POS)

/* EMI_I2CM_UART_TX_CTRL */
#define EMI_I2CM_UART_TX_CTRL_STOP_BITS_POS         (0u)  /* [2:0] Stop bits: (Stop bits + 1) * 0.5 period */
#define EMI_I2CM_UART_TX_CTRL_PARITY_POS            (4u)  /* [4]   Parity bit                              */
#define EMI_I2CM_UART_TX_CTRL_PARITY_ENABLED_POS    (5u)  /* [5]   Parity enable                           */
#define EMI_I2CM_UART_TX_CTRL_RETRY_ON_NACK_POS     (8u)  /* [8]   Smart Card: re-send frame on NACK       */
#define EMI_I2CM_UART_TX_CTRL_ONE_STOP_BIT          ((uint32) 0x01u)
#define EMI_I2CM_UART_TX_CTRL_ONE_HALF_STOP_BITS    ((uint32) 0x02u)
#define EMI_I2CM_UART_TX_CTRL_TWO_STOP_BITS         ((uint32) 0x03u)
#define EMI_I2CM_UART_TX_CTRL_STOP_BITS_MASK        ((uint32) 0x07u)
#define EMI_I2CM_UART_TX_CTRL_PARITY                ((uint32) 0x01u << \
                                                                    EMI_I2CM_UART_TX_CTRL_PARITY_POS)
#define EMI_I2CM_UART_TX_CTRL_PARITY_ENABLED        ((uint32) 0x01u << \
                                                                    EMI_I2CM_UART_TX_CTRL_PARITY_ENABLED_POS)
#define EMI_I2CM_UART_TX_CTRL_RETRY_ON_NACK         ((uint32) 0x01u << \
                                                                    EMI_I2CM_UART_TX_CTRL_RETRY_ON_NACK_POS)

/* EMI_I2CM_UART_RX_CTRL */
#define EMI_I2CM_UART_RX_CTRL_STOP_BITS_POS             (0u)  /* [2:0] Stop bits: (Stop bits + 1) * 0.5 period*/
#define EMI_I2CM_UART_RX_CTRL_PARITY_POS                (4u)  /* [4]   Parity bit                             */
#define EMI_I2CM_UART_RX_CTRL_PARITY_ENABLED_POS        (5u)  /* [5]   Parity enable                          */
#define EMI_I2CM_UART_RX_CTRL_POLARITY_POS              (6u)  /* [6]   IrDA: inverts polarity of RX signal    */
#define EMI_I2CM_UART_RX_CTRL_DROP_ON_PARITY_ERR_POS    (8u)  /* [8]   Drop and lost RX FIFO on parity error  */
#define EMI_I2CM_UART_RX_CTRL_DROP_ON_FRAME_ERR_POS     (9u)  /* [9]   Drop and lost RX FIFO on frame error   */
#define EMI_I2CM_UART_RX_CTRL_MP_MODE_POS               (10u) /* [10]  Multi-processor mode                   */
#define EMI_I2CM_UART_RX_CTRL_LIN_MODE_POS              (12u) /* [12]  Lin mode: applicable for UART Standard */
#define EMI_I2CM_UART_RX_CTRL_SKIP_START_POS            (13u) /* [13]  Skip start not: only for UART Standard */
#define EMI_I2CM_UART_RX_CTRL_BREAK_WIDTH_POS           (16u) /* [19:16]  Break width: (Break width + 1)      */
#define EMI_I2CM_UART_TX_CTRL_ONE_STOP_BIT              ((uint32) 0x01u)
#define EMI_I2CM_UART_TX_CTRL_ONE_HALF_STOP_BITS        ((uint32) 0x02u)
#define EMI_I2CM_UART_TX_CTRL_TWO_STOP_BITS             ((uint32) 0x03u)
#define EMI_I2CM_UART_RX_CTRL_STOP_BITS_MASK            ((uint32) 0x07u)
#define EMI_I2CM_UART_RX_CTRL_PARITY                    ((uint32) 0x01u << \
                                                                    EMI_I2CM_UART_RX_CTRL_PARITY_POS)
#define EMI_I2CM_UART_RX_CTRL_PARITY_ENABLED            ((uint32) 0x01u << \
                                                                    EMI_I2CM_UART_RX_CTRL_PARITY_ENABLED_POS)
#define EMI_I2CM_UART_RX_CTRL_POLARITY                  ((uint32) 0x01u << \
                                                                    EMI_I2CM_UART_RX_CTRL_POLARITY_POS)
#define EMI_I2CM_UART_RX_CTRL_DROP_ON_PARITY_ERR        ((uint32) 0x01u << \
                                                                   EMI_I2CM_UART_RX_CTRL_DROP_ON_PARITY_ERR_POS)
#define EMI_I2CM_UART_RX_CTRL_DROP_ON_FRAME_ERR         ((uint32) 0x01u << \
                                                                    EMI_I2CM_UART_RX_CTRL_DROP_ON_FRAME_ERR_POS)
#define EMI_I2CM_UART_RX_CTRL_MP_MODE                   ((uint32) 0x01u << \
                                                                    EMI_I2CM_UART_RX_CTRL_MP_MODE_POS)
#define EMI_I2CM_UART_RX_CTRL_LIN_MODE                  ((uint32) 0x01u << \
                                                                    EMI_I2CM_UART_RX_CTRL_LIN_MODE_POS)
#define EMI_I2CM_UART_RX_CTRL_SKIP_START                ((uint32) 0x01u << \
                                                                    EMI_I2CM_UART_RX_CTRL_SKIP_START_POS)
#define EMI_I2CM_UART_RX_CTRL_BREAK_WIDTH_MASK          ((uint32) 0x0Fu << \
                                                                    EMI_I2CM_UART_RX_CTRL_BREAK_WIDTH_POS)
/* EMI_I2CM_UART_RX_STATUS_REG */
#define EMI_I2CM_UART_RX_STATUS_BR_COUNTER_POS     (0u)  /* [11:0] Baud Rate counter */
#define EMI_I2CM_UART_RX_STATUS_BR_COUNTER_MASK    ((uint32) 0xFFFu)

#if !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    /* EMI_I2CM_UART_FLOW_CTRL_REG */
    #define EMI_I2CM_UART_FLOW_CTRL_TRIGGER_LEVEL_POS    (0u)  /* [7:0] RTS RX FIFO trigger level         */
    #define EMI_I2CM_UART_FLOW_CTRL_RTS_POLARITY_POS     (16u) /* [16]  Polarity of the RTS output signal */
    #define EMI_I2CM_UART_FLOW_CTRL_CTS_POLARITY_POS     (24u) /* [24]  Polarity of the CTS input signal  */
    #define EMI_I2CM_UART_FLOW_CTRL_CTS_ENABLED_POS      (25u) /* [25]  Enable CTS signal                 */
    #define EMI_I2CM_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK   ((uint32) EMI_I2CM_FF_DATA_NR_LOG2_MASK)
    #define EMI_I2CM_UART_FLOW_CTRL_RTS_POLARITY         ((uint32) 0x01u << \
                                                                       EMI_I2CM_UART_FLOW_CTRL_RTS_POLARITY_POS)
    #define EMI_I2CM_UART_FLOW_CTRL_CTS_POLARITY         ((uint32) 0x01u << \
                                                                       EMI_I2CM_UART_FLOW_CTRL_CTS_POLARITY_POS)
    #define EMI_I2CM_UART_FLOW_CTRL_CTS_ENABLE           ((uint32) 0x01u << \
                                                                       EMI_I2CM_UART_FLOW_CTRL_CTS_ENABLED_POS)
#endif /* !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */

/* EMI_I2CM_I2C_CTRL */
#define EMI_I2CM_I2C_CTRL_HIGH_PHASE_OVS_POS           (0u)   /* [3:0] Oversampling factor high: master only */
#define EMI_I2CM_I2C_CTRL_LOW_PHASE_OVS_POS            (4u)   /* [7:4] Oversampling factor low:  master only */
#define EMI_I2CM_I2C_CTRL_M_READY_DATA_ACK_POS         (8u)   /* [8]   Master ACKs data while RX FIFO != FULL*/
#define EMI_I2CM_I2C_CTRL_M_NOT_READY_DATA_NACK_POS    (9u)   /* [9]   Master NACKs data if RX FIFO ==  FULL */
#define EMI_I2CM_I2C_CTRL_S_GENERAL_IGNORE_POS         (11u)  /* [11]  Slave ignores General call            */
#define EMI_I2CM_I2C_CTRL_S_READY_ADDR_ACK_POS         (12u)  /* [12]  Slave ACKs Address if RX FIFO != FULL */
#define EMI_I2CM_I2C_CTRL_S_READY_DATA_ACK_POS         (13u)  /* [13]  Slave ACKs data while RX FIFO == FULL */
#define EMI_I2CM_I2C_CTRL_S_NOT_READY_ADDR_NACK_POS    (14u)  /* [14]  Slave NACKs address if RX FIFO == FULL*/
#define EMI_I2CM_I2C_CTRL_S_NOT_READY_DATA_NACK_POS    (15u)  /* [15]  Slave NACKs data if RX FIFO is  FULL  */
#define EMI_I2CM_I2C_CTRL_LOOPBACK_POS                 (16u)  /* [16]  Loop-back                             */
#define EMI_I2CM_I2C_CTRL_SLAVE_MODE_POS               (30u)  /* [30]  Slave mode enabled                    */
#define EMI_I2CM_I2C_CTRL_MASTER_MODE_POS              (31u)  /* [31]  Master mode enabled                   */
#define EMI_I2CM_I2C_CTRL_HIGH_PHASE_OVS_MASK  ((uint32) 0x0Fu)
#define EMI_I2CM_I2C_CTRL_LOW_PHASE_OVS_MASK   ((uint32) 0x0Fu << \
                                                                EMI_I2CM_I2C_CTRL_LOW_PHASE_OVS_POS)
#define EMI_I2CM_I2C_CTRL_M_READY_DATA_ACK      ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CTRL_M_READY_DATA_ACK_POS)
#define EMI_I2CM_I2C_CTRL_M_NOT_READY_DATA_NACK ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CTRL_M_NOT_READY_DATA_NACK_POS)
#define EMI_I2CM_I2C_CTRL_S_GENERAL_IGNORE      ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CTRL_S_GENERAL_IGNORE_POS)
#define EMI_I2CM_I2C_CTRL_S_READY_ADDR_ACK      ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CTRL_S_READY_ADDR_ACK_POS)
#define EMI_I2CM_I2C_CTRL_S_READY_DATA_ACK      ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CTRL_S_READY_DATA_ACK_POS)
#define EMI_I2CM_I2C_CTRL_S_NOT_READY_ADDR_NACK ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CTRL_S_NOT_READY_ADDR_NACK_POS)
#define EMI_I2CM_I2C_CTRL_S_NOT_READY_DATA_NACK ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CTRL_S_NOT_READY_DATA_NACK_POS)
#define EMI_I2CM_I2C_CTRL_LOOPBACK              ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CTRL_LOOPBACK_POS)
#define EMI_I2CM_I2C_CTRL_SLAVE_MODE            ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CTRL_SLAVE_MODE_POS)
#define EMI_I2CM_I2C_CTRL_MASTER_MODE           ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CTRL_MASTER_MODE_POS)
#define EMI_I2CM_I2C_CTRL_SLAVE_MASTER_MODE_MASK    ((uint32) 0x03u << \
                                                                EMI_I2CM_I2C_CTRL_SLAVE_MODE_POS)

/* EMI_I2CM_I2C_STATUS_REG  */
#define EMI_I2CM_I2C_STATUS_BUS_BUSY_POS    (0u)  /* [0]    Bus busy: internally clocked */
#define EMI_I2CM_I2C_STATUS_S_READ_POS      (4u)  /* [4]    Slave is read by master      */
#define EMI_I2CM_I2C_STATUS_M_READ_POS      (5u)  /* [5]    Master reads Slave           */
#define EMI_I2CM_I2C_STATUS_EZBUF_ADDR_POS  (8u)  /* [15:8] EZAddress                    */
#define EMI_I2CM_I2C_STATUS_BUS_BUSY        ((uint32) 0x01u)
#define EMI_I2CM_I2C_STATUS_S_READ          ((uint32) 0x01u << EMI_I2CM_I2C_STATUS_S_READ_POS)
#define EMI_I2CM_I2C_STATUS_M_READ          ((uint32) 0x01u << EMI_I2CM_I2C_STATUS_M_READ_POS)
#define EMI_I2CM_I2C_STATUS_EZBUF_ADDR_MASK ((uint32) 0xFFu << EMI_I2CM_I2C_STATUS_EZBUF_ADDR_POS)

/* EMI_I2CM_I2C_MASTER_CMD_REG */
#define EMI_I2CM_I2C_MASTER_CMD_M_START_POS             (0u)  /* [0] Master generate Start                */
#define EMI_I2CM_I2C_MASTER_CMD_M_START_ON_IDLE_POS     (1u)  /* [1] Master generate Start if bus is free */
#define EMI_I2CM_I2C_MASTER_CMD_M_ACK_POS               (2u)  /* [2] Master generate ACK                  */
#define EMI_I2CM_I2C_MASTER_CMD_M_NACK_POS              (3u)  /* [3] Master generate NACK                 */
#define EMI_I2CM_I2C_MASTER_CMD_M_STOP_POS              (4u)  /* [4] Master generate Stop                 */
#define EMI_I2CM_I2C_MASTER_CMD_M_START         ((uint32) 0x01u)
#define EMI_I2CM_I2C_MASTER_CMD_M_START_ON_IDLE ((uint32) 0x01u << \
                                                                   EMI_I2CM_I2C_MASTER_CMD_M_START_ON_IDLE_POS)
#define EMI_I2CM_I2C_MASTER_CMD_M_ACK           ((uint32) 0x01u << \
                                                                   EMI_I2CM_I2C_MASTER_CMD_M_ACK_POS)
#define EMI_I2CM_I2C_MASTER_CMD_M_NACK          ((uint32) 0x01u << \
                                                                    EMI_I2CM_I2C_MASTER_CMD_M_NACK_POS)
#define EMI_I2CM_I2C_MASTER_CMD_M_STOP          ((uint32) 0x01u << \
                                                                    EMI_I2CM_I2C_MASTER_CMD_M_STOP_POS)

/* EMI_I2CM_I2C_SLAVE_CMD_REG  */
#define EMI_I2CM_I2C_SLAVE_CMD_S_ACK_POS    (0u)  /* [0] Slave generate ACK  */
#define EMI_I2CM_I2C_SLAVE_CMD_S_NACK_POS   (1u)  /* [1] Slave generate NACK */
#define EMI_I2CM_I2C_SLAVE_CMD_S_ACK        ((uint32) 0x01u)
#define EMI_I2CM_I2C_SLAVE_CMD_S_NACK       ((uint32) 0x01u << EMI_I2CM_I2C_SLAVE_CMD_S_NACK_POS)

#define EMI_I2CM_I2C_SLAVE_CMD_S_ACK_POS    (0u)  /* [0] Slave generate ACK  */
#define EMI_I2CM_I2C_SLAVE_CMD_S_NACK_POS   (1u)  /* [1] Slave generate NACK */
#define EMI_I2CM_I2C_SLAVE_CMD_S_ACK        ((uint32) 0x01u)
#define EMI_I2CM_I2C_SLAVE_CMD_S_NACK       ((uint32) 0x01u << EMI_I2CM_I2C_SLAVE_CMD_S_NACK_POS)

/* EMI_I2CM_I2C_CFG_REG */
#if (EMI_I2CM_CY_SCBIP_V0)
#define EMI_I2CM_I2C_CFG_SDA_FILT_HYS_POS           (0u)  /* [1:0]   Trim bits for the I2C SDA filter         */
#define EMI_I2CM_I2C_CFG_SDA_FILT_TRIM_POS          (2u)  /* [3:2]   Trim bits for the I2C SDA filter         */
#define EMI_I2CM_I2C_CFG_SCL_FILT_HYS_POS           (4u)  /* [5:4]   Trim bits for the I2C SCL filter         */
#define EMI_I2CM_I2C_CFG_SCL_FILT_TRIM_POS          (6u)  /* [7:6]   Trim bits for the I2C SCL filter         */
#define EMI_I2CM_I2C_CFG_SDA_FILT_OUT_HYS_POS       (8u)  /* [9:8]   Trim bits for I2C SDA filter output path */
#define EMI_I2CM_I2C_CFG_SDA_FILT_OUT_TRIM_POS      (10u) /* [11:10] Trim bits for I2C SDA filter output path */
#define EMI_I2CM_I2C_CFG_SDA_FILT_HS_POS            (16u) /* [16]    '0': 50 ns filter, '1': 10 ns filter     */
#define EMI_I2CM_I2C_CFG_SDA_FILT_ENABLED_POS       (17u) /* [17]    I2C SDA filter enabled                   */
#define EMI_I2CM_I2C_CFG_SCL_FILT_HS_POS            (24u) /* [24]    '0': 50 ns filter, '1': 10 ns filter     */
#define EMI_I2CM_I2C_CFG_SCL_FILT_ENABLED_POS       (25u) /* [25]    I2C SCL filter enabled                   */
#define EMI_I2CM_I2C_CFG_SDA_FILT_OUT_HS_POS        (26u) /* [26]    '0': 50 ns filter, '1': 10 ns filter     */
#define EMI_I2CM_I2C_CFG_SDA_FILT_OUT_ENABLED_POS   (27u) /* [27]    I2C SDA output delay filter enabled      */
#define EMI_I2CM_I2C_CFG_SDA_FILT_HYS_MASK          ((uint32) 0x03u)
#define EMI_I2CM_I2C_CFG_SDA_FILT_TRIM_MASK         ((uint32) 0x03u << \
                                                                EMI_I2CM_I2C_CFG_SDA_FILT_TRIM_POS)
#define EMI_I2CM_I2C_CFG_SCL_FILT_HYS_MASK          ((uint32) 0x03u << \
                                                                EMI_I2CM_I2C_CFG_SCL_FILT_HYS_POS)
#define EMI_I2CM_I2C_CFG_SCL_FILT_TRIM_MASK         ((uint32) 0x03u << \
                                                                EMI_I2CM_I2C_CFG_SCL_FILT_TRIM_POS)
#define EMI_I2CM_I2C_CFG_SDA_FILT_OUT_HYS_MASK      ((uint32) 0x03u << \
                                                                EMI_I2CM_I2C_CFG_SDA_FILT_OUT_HYS_POS)
#define EMI_I2CM_I2C_CFG_SDA_FILT_OUT_TRIM_MASK     ((uint32) 0x03u << \
                                                                EMI_I2CM_I2C_CFG_SDA_FILT_OUT_TRIM_POS)
#define EMI_I2CM_I2C_CFG_SDA_FILT_HS                ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CFG_SDA_FILT_HS_POS)
#define EMI_I2CM_I2C_CFG_SDA_FILT_ENABLED           ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CFG_SDA_FILT_ENABLED_POS)
#define EMI_I2CM_I2C_CFG_SCL_FILT_HS                ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CFG_SCL_FILT_HS_POS)
#define EMI_I2CM_I2C_CFG_SCL_FILT_ENABLED           ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CFG_SCL_FILT_ENABLED_POS)
#define EMI_I2CM_I2C_CFG_SDA_FILT_OUT_HS            ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CFG_SDA_FILT_OUT_HS_POS)
#define EMI_I2CM_I2C_CFG_SDA_FILT_OUT_ENABLED       ((uint32) 0x01u << \
                                                                EMI_I2CM_I2C_CFG_SDA_FILT_OUT_ENABLED_POS)
#else
#define EMI_I2CM_I2C_CFG_SDA_IN_FILT_TRIM_POS   (0u)  /* [1:0] Trim bits for "i2c_sda_in" 50 ns filter */
#define EMI_I2CM_I2C_CFG_SDA_IN_FILT_SEL_POS    (4u)  /* [4]   "i2c_sda_in" filter delay: 0 ns and 50 ns */
#define EMI_I2CM_I2C_CFG_SCL_IN_FILT_TRIM_POS   (8u)  /* [9:8] Trim bits for "i2c_scl_in" 50 ns filter */
#define EMI_I2CM_I2C_CFG_SCL_IN_FILT_SEL_POS    (12u) /* [12]  "i2c_scl_in" filter delay: 0 ns and 50 ns */
#define EMI_I2CM_I2C_CFG_SDA_OUT_FILT0_TRIM_POS (16u) /* [17:16] Trim bits for "i2c_sda_out" 50 ns filter 0 */
#define EMI_I2CM_I2C_CFG_SDA_OUT_FILT1_TRIM_POS (18u) /* [19:18] Trim bits for "i2c_sda_out" 50 ns filter 1 */
#define EMI_I2CM_I2C_CFG_SDA_OUT_FILT2_TRIM_POS (20u) /* [21:20] Trim bits for "i2c_sda_out" 50 ns filter 2 */
#define EMI_I2CM_I2C_CFG_SDA_OUT_FILT_SEL_POS   (28u) /* [29:28] Cumulative "i2c_sda_out" filter delay: */

#define EMI_I2CM_I2C_CFG_SDA_IN_FILT_TRIM_MASK  ((uint32) 0x03u)
#define EMI_I2CM_I2C_CFG_SDA_IN_FILT_SEL        ((uint32) 0x01u << EMI_I2CM_I2C_CFG_SDA_IN_FILT_SEL_POS)
#define EMI_I2CM_I2C_CFG_SCL_IN_FILT_TRIM_MASK  ((uint32) 0x03u << \
                                                            EMI_I2CM_I2C_CFG_SCL_IN_FILT_TRIM_POS)
#define EMI_I2CM_I2C_CFG_SCL_IN_FILT_SEL        ((uint32) 0x01u << EMI_I2CM_I2C_CFG_SCL_IN_FILT_SEL_POS)
#define EMI_I2CM_I2C_CFG_SDA_OUT_FILT0_TRIM_MASK ((uint32) 0x03u << \
                                                            EMI_I2CM_I2C_CFG_SDA_OUT_FILT0_TRIM_POS)
#define EMI_I2CM_I2C_CFG_SDA_OUT_FILT1_TRIM_MASK ((uint32) 0x03u << \
                                                            EMI_I2CM_I2C_CFG_SDA_OUT_FILT1_TRIM_POS)
#define EMI_I2CM_I2C_CFG_SDA_OUT_FILT2_TRIM_MASK ((uint32) 0x03u << \
                                                            EMI_I2CM_I2C_CFG_SDA_OUT_FILT2_TRIM_POS)
#define EMI_I2CM_I2C_CFG_SDA_OUT_FILT_SEL_MASK   ((uint32) 0x03u << \
                                                            EMI_I2CM_I2C_CFG_SDA_OUT_FILT_SEL_POS)
#endif /* (EMI_I2CM_CY_SCBIP_V0) */


/* EMI_I2CM_TX_CTRL_REG */
#define EMI_I2CM_TX_CTRL_DATA_WIDTH_POS     (0u)  /* [3:0] Data frame width: (Data width - 1) */
#define EMI_I2CM_TX_CTRL_MSB_FIRST_POS      (8u)  /* [8]   MSB first shifter-out             */
#define EMI_I2CM_TX_CTRL_ENABLED_POS        (31u) /* [31]  Transmitter enabled               */
#define EMI_I2CM_TX_CTRL_DATA_WIDTH_MASK    ((uint32) 0x0Fu)
#define EMI_I2CM_TX_CTRL_MSB_FIRST          ((uint32) 0x01u << EMI_I2CM_TX_CTRL_MSB_FIRST_POS)
#define EMI_I2CM_TX_CTRL_LSB_FIRST          ((uint32) 0x00u)
#define EMI_I2CM_TX_CTRL_ENABLED            ((uint32) 0x01u << EMI_I2CM_TX_CTRL_ENABLED_POS)

/* EMI_I2CM_TX_CTRL_FIFO_REG */
#define EMI_I2CM_TX_FIFO_CTRL_TRIGGER_LEVEL_POS     (0u)  /* [2:0] Trigger level                              */
#define EMI_I2CM_TX_FIFO_CTRL_CLEAR_POS             (16u) /* [16]  Clear TX FIFO: cleared after set           */
#define EMI_I2CM_TX_FIFO_CTRL_FREEZE_POS            (17u) /* [17]  Freeze TX FIFO: HW do not inc read pointer */
#define EMI_I2CM_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK    ((uint32) EMI_I2CM_FF_DATA_NR_LOG2_MASK)
#define EMI_I2CM_TX_FIFO_CTRL_CLEAR                 ((uint32) 0x01u << EMI_I2CM_TX_FIFO_CTRL_CLEAR_POS)
#define EMI_I2CM_TX_FIFO_CTRL_FREEZE                ((uint32) 0x01u << EMI_I2CM_TX_FIFO_CTRL_FREEZE_POS)

/* EMI_I2CM_TX_FIFO_STATUS_REG */
#define EMI_I2CM_TX_FIFO_STATUS_USED_POS    (0u)  /* [3:0]   Amount of entries in TX FIFO */
#define EMI_I2CM_TX_FIFO_SR_VALID_POS       (15u) /* [15]    Shifter status of TX FIFO    */
#define EMI_I2CM_TX_FIFO_STATUS_RD_PTR_POS  (16u) /* [18:16] TX FIFO read pointer         */
#define EMI_I2CM_TX_FIFO_STATUS_WR_PTR_POS  (24u) /* [26:24] TX FIFO write pointer        */
#define EMI_I2CM_TX_FIFO_STATUS_USED_MASK   ((uint32) EMI_I2CM_FF_DATA_NR_LOG2_PLUS1_MASK)
#define EMI_I2CM_TX_FIFO_SR_VALID           ((uint32) 0x01u << EMI_I2CM_TX_FIFO_SR_VALID_POS)
#define EMI_I2CM_TX_FIFO_STATUS_RD_PTR_MASK ((uint32) EMI_I2CM_FF_DATA_NR_LOG2_MASK << \
                                                                    EMI_I2CM_TX_FIFO_STATUS_RD_PTR_POS)
#define EMI_I2CM_TX_FIFO_STATUS_WR_PTR_MASK ((uint32) EMI_I2CM_FF_DATA_NR_LOG2_MASK << \
                                                                    EMI_I2CM_TX_FIFO_STATUS_WR_PTR_POS)

/* EMI_I2CM_TX_FIFO_WR_REG */
#define EMI_I2CM_TX_FIFO_WR_POS    (0u)  /* [15:0] Data written into TX FIFO */
#define EMI_I2CM_TX_FIFO_WR_MASK   ((uint32) 0xFFu)

/* EMI_I2CM_RX_CTRL_REG */
#define EMI_I2CM_RX_CTRL_DATA_WIDTH_POS     (0u)  /* [3:0] Data frame width: (Data width - 1) */
#define EMI_I2CM_RX_CTRL_MSB_FIRST_POS      (8u)  /* [8]   MSB first shifter-out             */
#define EMI_I2CM_RX_CTRL_MEDIAN_POS         (9u)  /* [9]   Median filter                     */
#define EMI_I2CM_RX_CTRL_ENABLED_POS        (31u) /* [31]  Receiver enabled                  */
#define EMI_I2CM_RX_CTRL_DATA_WIDTH_MASK    ((uint32) 0x0Fu)
#define EMI_I2CM_RX_CTRL_MSB_FIRST          ((uint32) 0x01u << EMI_I2CM_RX_CTRL_MSB_FIRST_POS)
#define EMI_I2CM_RX_CTRL_LSB_FIRST          ((uint32) 0x00u)
#define EMI_I2CM_RX_CTRL_MEDIAN             ((uint32) 0x01u << EMI_I2CM_RX_CTRL_MEDIAN_POS)
#define EMI_I2CM_RX_CTRL_ENABLED            ((uint32) 0x01u << EMI_I2CM_RX_CTRL_ENABLED_POS)


/* EMI_I2CM_RX_FIFO_CTRL_REG */
#define EMI_I2CM_RX_FIFO_CTRL_TRIGGER_LEVEL_POS     (0u)   /* [2:0] Trigger level                            */
#define EMI_I2CM_RX_FIFO_CTRL_CLEAR_POS             (16u)  /* [16]  Clear RX FIFO: clear after set           */
#define EMI_I2CM_RX_FIFO_CTRL_FREEZE_POS            (17u)  /* [17]  Freeze RX FIFO: HW writes has not effect */
#define EMI_I2CM_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK    ((uint32) EMI_I2CM_FF_DATA_NR_LOG2_MASK)
#define EMI_I2CM_RX_FIFO_CTRL_CLEAR                 ((uint32) 0x01u << EMI_I2CM_RX_FIFO_CTRL_CLEAR_POS)
#define EMI_I2CM_RX_FIFO_CTRL_FREEZE                ((uint32) 0x01u << EMI_I2CM_RX_FIFO_CTRL_FREEZE_POS)

/* EMI_I2CM_RX_FIFO_STATUS_REG */
#define EMI_I2CM_RX_FIFO_STATUS_USED_POS    (0u)   /* [3:0]   Amount of entries in RX FIFO */
#define EMI_I2CM_RX_FIFO_SR_VALID_POS       (15u)  /* [15]    Shifter status of RX FIFO    */
#define EMI_I2CM_RX_FIFO_STATUS_RD_PTR_POS  (16u)  /* [18:16] RX FIFO read pointer         */
#define EMI_I2CM_RX_FIFO_STATUS_WR_PTR_POS  (24u)  /* [26:24] RX FIFO write pointer        */
#define EMI_I2CM_RX_FIFO_STATUS_USED_MASK   ((uint32) EMI_I2CM_FF_DATA_NR_LOG2_PLUS1_MASK)
#define EMI_I2CM_RX_FIFO_SR_VALID           ((uint32) 0x01u << EMI_I2CM_RX_FIFO_SR_VALID_POS)
#define EMI_I2CM_RX_FIFO_STATUS_RD_PTR_MASK ((uint32) EMI_I2CM_FF_DATA_NR_LOG2_MASK << \
                                                                    EMI_I2CM_RX_FIFO_STATUS_RD_PTR_POS)
#define EMI_I2CM_RX_FIFO_STATUS_WR_PTR_MASK ((uint32) EMI_I2CM_FF_DATA_NR_LOG2_MASK << \
                                                                    EMI_I2CM_RX_FIFO_STATUS_WR_PTR_POS)

/* EMI_I2CM_RX_MATCH_REG */
#define EMI_I2CM_RX_MATCH_ADDR_POS     (0u)  /* [7:0]   Slave address                        */
#define EMI_I2CM_RX_MATCH_MASK_POS     (16u) /* [23:16] Slave address mask: 0 - doesn't care */
#define EMI_I2CM_RX_MATCH_ADDR_MASK    ((uint32) 0xFFu)
#define EMI_I2CM_RX_MATCH_MASK_MASK    ((uint32) 0xFFu << EMI_I2CM_RX_MATCH_MASK_POS)

/* EMI_I2CM_RX_FIFO_WR_REG */
#define EMI_I2CM_RX_FIFO_RD_POS    (0u)  /* [15:0] Data read from RX FIFO */
#define EMI_I2CM_RX_FIFO_RD_MASK   ((uint32) 0xFFu)

/* EMI_I2CM_RX_FIFO_RD_SILENT_REG */
#define EMI_I2CM_RX_FIFO_RD_SILENT_POS     (0u)  /* [15:0] Data read from RX FIFO: not remove data from FIFO */
#define EMI_I2CM_RX_FIFO_RD_SILENT_MASK    ((uint32) 0xFFu)

/* EMI_I2CM_RX_FIFO_RD_SILENT_REG */
#define EMI_I2CM_RX_FIFO_RD_SILENT_POS     (0u)  /* [15:0] Data read from RX FIFO: not remove data from FIFO */
#define EMI_I2CM_RX_FIFO_RD_SILENT_MASK    ((uint32) 0xFFu)

/* EMI_I2CM_EZBUF_DATA_REG */
#define EMI_I2CM_EZBUF_DATA_POS   (0u)  /* [7:0] Data from EZ Memory */
#define EMI_I2CM_EZBUF_DATA_MASK  ((uint32) 0xFFu)

/*  EMI_I2CM_INTR_CAUSE_REG */
#define EMI_I2CM_INTR_CAUSE_MASTER_POS  (0u)  /* [0] Master interrupt active                 */
#define EMI_I2CM_INTR_CAUSE_SLAVE_POS   (1u)  /* [1] Slave interrupt active                  */
#define EMI_I2CM_INTR_CAUSE_TX_POS      (2u)  /* [2] Transmitter interrupt active            */
#define EMI_I2CM_INTR_CAUSE_RX_POS      (3u)  /* [3] Receiver interrupt active               */
#define EMI_I2CM_INTR_CAUSE_I2C_EC_POS  (4u)  /* [4] Externally clock I2C interrupt active   */
#define EMI_I2CM_INTR_CAUSE_SPI_EC_POS  (5u)  /* [5] Externally clocked SPI interrupt active */
#define EMI_I2CM_INTR_CAUSE_MASTER      ((uint32) 0x01u)
#define EMI_I2CM_INTR_CAUSE_SLAVE       ((uint32) 0x01u << EMI_I2CM_INTR_CAUSE_SLAVE_POS)
#define EMI_I2CM_INTR_CAUSE_TX          ((uint32) 0x01u << EMI_I2CM_INTR_CAUSE_TX_POS)
#define EMI_I2CM_INTR_CAUSE_RX          ((uint32) 0x01u << EMI_I2CM_INTR_CAUSE_RX_POS)
#define EMI_I2CM_INTR_CAUSE_I2C_EC      ((uint32) 0x01u << EMI_I2CM_INTR_CAUSE_I2C_EC_POS)
#define EMI_I2CM_INTR_CAUSE_SPI_EC      ((uint32) 0x01u << EMI_I2CM_INTR_CAUSE_SPI_EC_POS)

/* EMI_I2CM_INTR_SPI_EC_REG, EMI_I2CM_INTR_SPI_EC_MASK_REG, EMI_I2CM_INTR_SPI_EC_MASKED_REG */
#define EMI_I2CM_INTR_SPI_EC_WAKE_UP_POS          (0u)  /* [0] Address match: triggers wakeup of chip */
#define EMI_I2CM_INTR_SPI_EC_EZBUF_STOP_POS       (1u)  /* [1] Externally clocked Stop detected       */
#define EMI_I2CM_INTR_SPI_EC_EZBUF_WRITE_STOP_POS (2u)  /* [2] Externally clocked Write Stop detected */
#define EMI_I2CM_INTR_SPI_EC_WAKE_UP              ((uint32) 0x01u)
#define EMI_I2CM_INTR_SPI_EC_EZBUF_STOP           ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SPI_EC_EZBUF_STOP_POS)
#define EMI_I2CM_INTR_SPI_EC_EZBUF_WRITE_STOP     ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SPI_EC_EZBUF_WRITE_STOP_POS)

/* EMI_I2CM_INTR_I2C_EC, EMI_I2CM_INTR_I2C_EC_MASK, EMI_I2CM_INTR_I2C_EC_MASKED */
#define EMI_I2CM_INTR_I2C_EC_WAKE_UP_POS          (0u)  /* [0] Address match: triggers wakeup of chip */
#define EMI_I2CM_INTR_I2C_EC_EZBUF_STOP_POS       (1u)  /* [1] Externally clocked Stop detected       */
#define EMI_I2CM_INTR_I2C_EC_EZBUF_WRITE_STOP_POS (2u)  /* [2] Externally clocked Write Stop detected */
#define EMI_I2CM_INTR_I2C_EC_WAKE_UP              ((uint32) 0x01u)
#define EMI_I2CM_INTR_I2C_EC_EZBUF_STOP           ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_I2C_EC_EZBUF_STOP_POS)
#define EMI_I2CM_INTR_I2C_EC_EZBUF_WRITE_STOP     ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_I2C_EC_EZBUF_WRITE_STOP_POS)

/* EMI_I2CM_INTR_MASTER, EMI_I2CM_INTR_MASTER_SET,
   EMI_I2CM_INTR_MASTER_MASK, EMI_I2CM_INTR_MASTER_MASKED */
#define EMI_I2CM_INTR_MASTER_I2C_ARB_LOST_POS   (0u)  /* [0] Master lost arbitration                          */
#define EMI_I2CM_INTR_MASTER_I2C_NACK_POS       (1u)  /* [1] Master receives NACK: address or write to slave  */
#define EMI_I2CM_INTR_MASTER_I2C_ACK_POS        (2u)  /* [2] Master receives NACK: address or write to slave  */
#define EMI_I2CM_INTR_MASTER_I2C_STOP_POS       (4u)  /* [4] Master detects the Stop: only self generated Stop*/
#define EMI_I2CM_INTR_MASTER_I2C_BUS_ERROR_POS  (8u)  /* [8] Master detects bus error: misplaced Start or Stop*/
#define EMI_I2CM_INTR_MASTER_SPI_DONE_POS       (9u)  /* [9] Master complete transfer: Only for SPI           */
#define EMI_I2CM_INTR_MASTER_I2C_ARB_LOST       ((uint32) 0x01u)
#define EMI_I2CM_INTR_MASTER_I2C_NACK           ((uint32) 0x01u << EMI_I2CM_INTR_MASTER_I2C_NACK_POS)
#define EMI_I2CM_INTR_MASTER_I2C_ACK            ((uint32) 0x01u << EMI_I2CM_INTR_MASTER_I2C_ACK_POS)
#define EMI_I2CM_INTR_MASTER_I2C_STOP           ((uint32) 0x01u << EMI_I2CM_INTR_MASTER_I2C_STOP_POS)
#define EMI_I2CM_INTR_MASTER_I2C_BUS_ERROR      ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_MASTER_I2C_BUS_ERROR_POS)
#define EMI_I2CM_INTR_MASTER_SPI_DONE           ((uint32) 0x01u << EMI_I2CM_INTR_MASTER_SPI_DONE_POS)

/*
* EMI_I2CM_INTR_SLAVE, EMI_I2CM_INTR_SLAVE_SET,
* EMI_I2CM_INTR_SLAVE_MASK, EMI_I2CM_INTR_SLAVE_MASKED
*/
#define EMI_I2CM_INTR_SLAVE_I2C_ARB_LOST_POS         (0u)  /* [0]  Slave lost arbitration                   */
#define EMI_I2CM_INTR_SLAVE_I2C_NACK_POS             (1u)  /* [1]  Slave receives NACK: master reads data   */
#define EMI_I2CM_INTR_SLAVE_I2C_ACK_POS              (2u)  /* [2]  Slave receives ACK: master reads data    */
#define EMI_I2CM_INTR_SLAVE_I2C_WRITE_STOP_POS       (3u)  /* [3]  Slave detects end of write transaction   */
#define EMI_I2CM_INTR_SLAVE_I2C_STOP_POS             (4u)  /* [4]  Slave detects end of transaction intended */
#define EMI_I2CM_INTR_SLAVE_I2C_START_POS            (5u)  /* [5]  Slave detects Start                      */
#define EMI_I2CM_INTR_SLAVE_I2C_ADDR_MATCH_POS       (6u)  /* [6]  Slave address matches                    */
#define EMI_I2CM_INTR_SLAVE_I2C_GENERAL_POS          (7u)  /* [7]  General call received                    */
#define EMI_I2CM_INTR_SLAVE_I2C_BUS_ERROR_POS        (8u)  /* [8]  Slave detects bus error                  */
#define EMI_I2CM_INTR_SLAVE_SPI_EZBUF_WRITE_STOP_POS (9u)  /* [9]  Slave write complete: Only for SPI       */
#define EMI_I2CM_INTR_SLAVE_SPI_EZBUF_STOP_POS       (10u) /* [10] Slave end of transaction: Only for SPI   */
#define EMI_I2CM_INTR_SLAVE_SPI_BUS_ERROR_POS        (11u) /* [11] Slave detects bus error: Only for SPI    */
#define EMI_I2CM_INTR_SLAVE_I2C_ARB_LOST             ((uint32) 0x01u)
#define EMI_I2CM_INTR_SLAVE_I2C_NACK                 ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SLAVE_I2C_NACK_POS)
#define EMI_I2CM_INTR_SLAVE_I2C_ACK                  ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SLAVE_I2C_ACK_POS)
#define EMI_I2CM_INTR_SLAVE_I2C_WRITE_STOP           ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SLAVE_I2C_WRITE_STOP_POS)
#define EMI_I2CM_INTR_SLAVE_I2C_STOP                 ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SLAVE_I2C_STOP_POS)
#define EMI_I2CM_INTR_SLAVE_I2C_START                ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SLAVE_I2C_START_POS)
#define EMI_I2CM_INTR_SLAVE_I2C_ADDR_MATCH           ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SLAVE_I2C_ADDR_MATCH_POS)
#define EMI_I2CM_INTR_SLAVE_I2C_GENERAL              ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SLAVE_I2C_GENERAL_POS)
#define EMI_I2CM_INTR_SLAVE_I2C_BUS_ERROR            ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SLAVE_I2C_BUS_ERROR_POS)
#define EMI_I2CM_INTR_SLAVE_SPI_EZBUF_WRITE_STOP     ((uint32) 0x01u << \
                                                                   EMI_I2CM_INTR_SLAVE_SPI_EZBUF_WRITE_STOP_POS)
#define EMI_I2CM_INTR_SLAVE_SPI_EZBUF_STOP           ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SLAVE_SPI_EZBUF_STOP_POS)
#define EMI_I2CM_INTR_SLAVE_SPI_BUS_ERROR           ((uint32) 0x01u << \
                                                                    EMI_I2CM_INTR_SLAVE_SPI_BUS_ERROR_POS)

/*
* EMI_I2CM_INTR_TX, EMI_I2CM_INTR_TX_SET,
* EMI_I2CM_INTR_TX_MASK, EMI_I2CM_INTR_TX_MASKED
*/
#define EMI_I2CM_INTR_TX_TRIGGER_POS        (0u)  /* [0]  Trigger on TX FIFO entires                       */
#define EMI_I2CM_INTR_TX_NOT_FULL_POS       (1u)  /* [1]  TX FIFO is not full                              */
#define EMI_I2CM_INTR_TX_EMPTY_POS          (4u)  /* [4]  TX FIFO is empty                                 */
#define EMI_I2CM_INTR_TX_OVERFLOW_POS       (5u)  /* [5]  Attempt to write to a full TX FIFO               */
#define EMI_I2CM_INTR_TX_UNDERFLOW_POS      (6u)  /* [6]  Attempt to read from an empty TX FIFO            */
#define EMI_I2CM_INTR_TX_BLOCKED_POS        (7u)  /* [7]  No access to the EZ memory                       */
#define EMI_I2CM_INTR_TX_UART_NACK_POS      (8u)  /* [8]  UART transmitter received a NACK: SmartCard mode */
#define EMI_I2CM_INTR_TX_UART_DONE_POS      (9u)  /* [9]  UART transmitter done even                       */
#define EMI_I2CM_INTR_TX_UART_ARB_LOST_POS  (10u) /* [10] UART lost arbitration: LIN or SmartCard          */
#define EMI_I2CM_INTR_TX_TRIGGER            ((uint32) 0x01u)
#define EMI_I2CM_INTR_TX_FIFO_LEVEL         (EMI_I2CM_INTR_TX_TRIGGER)
#define EMI_I2CM_INTR_TX_NOT_FULL           ((uint32) 0x01u << EMI_I2CM_INTR_TX_NOT_FULL_POS)
#define EMI_I2CM_INTR_TX_EMPTY              ((uint32) 0x01u << EMI_I2CM_INTR_TX_EMPTY_POS)
#define EMI_I2CM_INTR_TX_OVERFLOW           ((uint32) 0x01u << EMI_I2CM_INTR_TX_OVERFLOW_POS)
#define EMI_I2CM_INTR_TX_UNDERFLOW          ((uint32) 0x01u << EMI_I2CM_INTR_TX_UNDERFLOW_POS)
#define EMI_I2CM_INTR_TX_BLOCKED            ((uint32) 0x01u << EMI_I2CM_INTR_TX_BLOCKED_POS)
#define EMI_I2CM_INTR_TX_UART_NACK          ((uint32) 0x01u << EMI_I2CM_INTR_TX_UART_NACK_POS)
#define EMI_I2CM_INTR_TX_UART_DONE          ((uint32) 0x01u << EMI_I2CM_INTR_TX_UART_DONE_POS)
#define EMI_I2CM_INTR_TX_UART_ARB_LOST      ((uint32) 0x01u << EMI_I2CM_INTR_TX_UART_ARB_LOST_POS)

/*
* EMI_I2CM_INTR_RX, EMI_I2CM_INTR_RX_SET,
* EMI_I2CM_INTR_RX_MASK, EMI_I2CM_INTR_RX_MASKED
*/
#define EMI_I2CM_INTR_RX_TRIGGER_POS        (0u)   /* [0]  Trigger on RX FIFO entires            */
#define EMI_I2CM_INTR_RX_NOT_EMPTY_POS      (2u)   /* [2]  RX FIFO is not empty                  */
#define EMI_I2CM_INTR_RX_FULL_POS           (3u)   /* [3]  RX FIFO is full                       */
#define EMI_I2CM_INTR_RX_OVERFLOW_POS       (5u)   /* [5]  Attempt to write to a full RX FIFO    */
#define EMI_I2CM_INTR_RX_UNDERFLOW_POS      (6u)   /* [6]  Attempt to read from an empty RX FIFO */
#define EMI_I2CM_INTR_RX_BLOCKED_POS        (7u)   /* [7]  No access to the EZ memory            */
#define EMI_I2CM_INTR_RX_FRAME_ERROR_POS    (8u)   /* [8]  Frame error in received data frame    */
#define EMI_I2CM_INTR_RX_PARITY_ERROR_POS   (9u)   /* [9]  Parity error in received data frame   */
#define EMI_I2CM_INTR_RX_BAUD_DETECT_POS    (10u)  /* [10] LIN baud rate detection is completed   */
#define EMI_I2CM_INTR_RX_BREAK_DETECT_POS   (11u)  /* [11] Break detection is successful         */
#define EMI_I2CM_INTR_RX_TRIGGER            ((uint32) 0x01u)
#define EMI_I2CM_INTR_RX_FIFO_LEVEL         (EMI_I2CM_INTR_RX_TRIGGER)
#define EMI_I2CM_INTR_RX_NOT_EMPTY          ((uint32) 0x01u << EMI_I2CM_INTR_RX_NOT_EMPTY_POS)
#define EMI_I2CM_INTR_RX_FULL               ((uint32) 0x01u << EMI_I2CM_INTR_RX_FULL_POS)
#define EMI_I2CM_INTR_RX_OVERFLOW           ((uint32) 0x01u << EMI_I2CM_INTR_RX_OVERFLOW_POS)
#define EMI_I2CM_INTR_RX_UNDERFLOW          ((uint32) 0x01u << EMI_I2CM_INTR_RX_UNDERFLOW_POS)
#define EMI_I2CM_INTR_RX_BLOCKED            ((uint32) 0x01u << EMI_I2CM_INTR_RX_BLOCKED_POS)
#define EMI_I2CM_INTR_RX_FRAME_ERROR        ((uint32) 0x01u << EMI_I2CM_INTR_RX_FRAME_ERROR_POS)
#define EMI_I2CM_INTR_RX_PARITY_ERROR       ((uint32) 0x01u << EMI_I2CM_INTR_RX_PARITY_ERROR_POS)
#define EMI_I2CM_INTR_RX_BAUD_DETECT        ((uint32) 0x01u << EMI_I2CM_INTR_RX_BAUD_DETECT_POS)
#define EMI_I2CM_INTR_RX_BREAK_DETECT       ((uint32) 0x01u << EMI_I2CM_INTR_RX_BREAK_DETECT_POS)

/* Define all interrupt sources */
#define EMI_I2CM_INTR_I2C_EC_ALL    (EMI_I2CM_INTR_I2C_EC_WAKE_UP    | \
                                             EMI_I2CM_INTR_I2C_EC_EZBUF_STOP | \
                                             EMI_I2CM_INTR_I2C_EC_EZBUF_WRITE_STOP)

#define EMI_I2CM_INTR_SPI_EC_ALL    (EMI_I2CM_INTR_SPI_EC_WAKE_UP    | \
                                             EMI_I2CM_INTR_SPI_EC_EZBUF_STOP | \
                                             EMI_I2CM_INTR_SPI_EC_EZBUF_WRITE_STOP)

#define EMI_I2CM_INTR_MASTER_ALL    (EMI_I2CM_INTR_MASTER_I2C_ARB_LOST  | \
                                             EMI_I2CM_INTR_MASTER_I2C_NACK      | \
                                             EMI_I2CM_INTR_MASTER_I2C_ACK       | \
                                             EMI_I2CM_INTR_MASTER_I2C_STOP      | \
                                             EMI_I2CM_INTR_MASTER_I2C_BUS_ERROR | \
                                             EMI_I2CM_INTR_MASTER_SPI_DONE)

#define EMI_I2CM_INTR_SLAVE_ALL     (EMI_I2CM_INTR_SLAVE_I2C_ARB_LOST      | \
                                             EMI_I2CM_INTR_SLAVE_I2C_NACK          | \
                                             EMI_I2CM_INTR_SLAVE_I2C_ACK           | \
                                             EMI_I2CM_INTR_SLAVE_I2C_WRITE_STOP    | \
                                             EMI_I2CM_INTR_SLAVE_I2C_STOP          | \
                                             EMI_I2CM_INTR_SLAVE_I2C_START         | \
                                             EMI_I2CM_INTR_SLAVE_I2C_ADDR_MATCH    | \
                                             EMI_I2CM_INTR_SLAVE_I2C_GENERAL       | \
                                             EMI_I2CM_INTR_SLAVE_I2C_BUS_ERROR     | \
                                             EMI_I2CM_INTR_SLAVE_SPI_EZBUF_WRITE_STOP | \
                                             EMI_I2CM_INTR_SLAVE_SPI_EZBUF_STOP       | \
                                             EMI_I2CM_INTR_SLAVE_SPI_BUS_ERROR)

#define EMI_I2CM_INTR_TX_ALL        (EMI_I2CM_INTR_TX_TRIGGER   | \
                                             EMI_I2CM_INTR_TX_NOT_FULL  | \
                                             EMI_I2CM_INTR_TX_EMPTY     | \
                                             EMI_I2CM_INTR_TX_OVERFLOW  | \
                                             EMI_I2CM_INTR_TX_UNDERFLOW | \
                                             EMI_I2CM_INTR_TX_BLOCKED   | \
                                             EMI_I2CM_INTR_TX_UART_NACK | \
                                             EMI_I2CM_INTR_TX_UART_DONE | \
                                             EMI_I2CM_INTR_TX_UART_ARB_LOST)

#define EMI_I2CM_INTR_RX_ALL        (EMI_I2CM_INTR_RX_TRIGGER      | \
                                             EMI_I2CM_INTR_RX_NOT_EMPTY    | \
                                             EMI_I2CM_INTR_RX_FULL         | \
                                             EMI_I2CM_INTR_RX_OVERFLOW     | \
                                             EMI_I2CM_INTR_RX_UNDERFLOW    | \
                                             EMI_I2CM_INTR_RX_BLOCKED      | \
                                             EMI_I2CM_INTR_RX_FRAME_ERROR  | \
                                             EMI_I2CM_INTR_RX_PARITY_ERROR | \
                                             EMI_I2CM_INTR_RX_BAUD_DETECT  | \
                                             EMI_I2CM_INTR_RX_BREAK_DETECT)

/* I2C and EZI2C slave address defines */
#define EMI_I2CM_I2C_SLAVE_ADDR_POS    (0x01u)    /* 7-bit address shift */
#define EMI_I2CM_I2C_SLAVE_ADDR_MASK   (0xFEu)    /* 8-bit address mask */

/* OVS constants for IrDA Low Power operation */
#define EMI_I2CM_CTRL_OVS_IRDA_LP_OVS16     (0x00u)
#define EMI_I2CM_CTRL_OVS_IRDA_LP_OVS32     (0x01u)
#define EMI_I2CM_CTRL_OVS_IRDA_LP_OVS48     (0x02u)
#define EMI_I2CM_CTRL_OVS_IRDA_LP_OVS96     (0x03u)
#define EMI_I2CM_CTRL_OVS_IRDA_LP_OVS192    (0x04u)
#define EMI_I2CM_CTRL_OVS_IRDA_LP_OVS768    (0x05u)
#define EMI_I2CM_CTRL_OVS_IRDA_LP_OVS1536   (0x06u)

/* OVS constant for IrDA */
#define EMI_I2CM_CTRL_OVS_IRDA_OVS16        (EMI_I2CM_UART_IRDA_LP_OVS16)


/***************************************
*    Common Macro Definitions
***************************************/

/* Re-enables the SCB IP. A clear enable bit has a different effect
* on the scb IP depending on the version:
*  CY_SCBIP_V0: resets state, status, TX and RX FIFOs.
*  CY_SCBIP_V1 or later: resets state, status, TX and RX FIFOs and interrupt sources.
*/
#define EMI_I2CM_SCB_SW_RESET \
                        do{           \
                            EMI_I2CM_CTRL_REG &= ((uint32) ~EMI_I2CM_CTRL_ENABLED); \
                            EMI_I2CM_CTRL_REG |= ((uint32)  EMI_I2CM_CTRL_ENABLED); \
                        }while(0)

/* TX FIFO macro */
#define EMI_I2CM_CLEAR_TX_FIFO \
                            do{        \
                                EMI_I2CM_TX_FIFO_CTRL_REG |= ((uint32)  EMI_I2CM_TX_FIFO_CTRL_CLEAR); \
                                EMI_I2CM_TX_FIFO_CTRL_REG &= ((uint32) ~EMI_I2CM_TX_FIFO_CTRL_CLEAR); \
                            }while(0)

#define EMI_I2CM_GET_TX_FIFO_ENTRIES    (EMI_I2CM_TX_FIFO_STATUS_REG & \
                                                 EMI_I2CM_TX_FIFO_STATUS_USED_MASK)

#define EMI_I2CM_GET_TX_FIFO_SR_VALID   ((0u != (EMI_I2CM_TX_FIFO_STATUS_REG & \
                                                         EMI_I2CM_TX_FIFO_SR_VALID)) ? (1u) : (0u))

/* RX FIFO macro */
#define EMI_I2CM_CLEAR_RX_FIFO \
                            do{        \
                                EMI_I2CM_RX_FIFO_CTRL_REG |= ((uint32)  EMI_I2CM_RX_FIFO_CTRL_CLEAR); \
                                EMI_I2CM_RX_FIFO_CTRL_REG &= ((uint32) ~EMI_I2CM_RX_FIFO_CTRL_CLEAR); \
                            }while(0)

#define EMI_I2CM_GET_RX_FIFO_ENTRIES    (EMI_I2CM_RX_FIFO_STATUS_REG & \
                                                    EMI_I2CM_RX_FIFO_STATUS_USED_MASK)

#define EMI_I2CM_GET_RX_FIFO_SR_VALID   ((0u != (EMI_I2CM_RX_FIFO_STATUS_REG & \
                                                         EMI_I2CM_RX_FIFO_SR_VALID)) ? (1u) : (0u))

/* Write interrupt source: set sourceMask bits in EMI_I2CM_INTR_X_MASK_REG */
#define EMI_I2CM_WRITE_INTR_I2C_EC_MASK(sourceMask) \
                                                do{         \
                                                    EMI_I2CM_INTR_I2C_EC_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)

#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_WRITE_INTR_SPI_EC_MASK(sourceMask) \
                                                do{         \
                                                    EMI_I2CM_INTR_SPI_EC_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

#define EMI_I2CM_WRITE_INTR_MASTER_MASK(sourceMask) \
                                                do{         \
                                                    EMI_I2CM_INTR_MASTER_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_WRITE_INTR_SLAVE_MASK(sourceMask)  \
                                                do{         \
                                                    EMI_I2CM_INTR_SLAVE_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_WRITE_INTR_TX_MASK(sourceMask)     \
                                                do{         \
                                                    EMI_I2CM_INTR_TX_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_WRITE_INTR_RX_MASK(sourceMask)     \
                                                do{         \
                                                    EMI_I2CM_INTR_RX_MASK_REG = (uint32) (sourceMask); \
                                                }while(0)

/* Enable interrupt source: set sourceMask bits in EMI_I2CM_INTR_X_MASK_REG */
#define EMI_I2CM_ENABLE_INTR_I2C_EC(sourceMask) \
                                                do{     \
                                                    EMI_I2CM_INTR_I2C_EC_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)
#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_ENABLE_INTR_SPI_EC(sourceMask) \
                                                do{     \
                                                    EMI_I2CM_INTR_SPI_EC_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

#define EMI_I2CM_ENABLE_INTR_MASTER(sourceMask) \
                                                do{     \
                                                    EMI_I2CM_INTR_MASTER_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_ENABLE_INTR_SLAVE(sourceMask)  \
                                                do{     \
                                                    EMI_I2CM_INTR_SLAVE_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_ENABLE_INTR_TX(sourceMask)     \
                                                do{     \
                                                    EMI_I2CM_INTR_TX_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_ENABLE_INTR_RX(sourceMask)     \
                                                do{     \
                                                    EMI_I2CM_INTR_RX_MASK_REG |= (uint32) (sourceMask); \
                                                }while(0)

/* Disable interrupt source: clear sourceMask bits in EMI_I2CM_INTR_X_MASK_REG */
#define EMI_I2CM_DISABLE_INTR_I2C_EC(sourceMask) \
                                do{                      \
                                    EMI_I2CM_INTR_I2C_EC_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                }while(0)

#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_DISABLE_INTR_SPI_EC(sourceMask) \
                                do{                      \
                                    EMI_I2CM_INTR_SPI_EC_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                 }while(0)
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

#define EMI_I2CM_DISABLE_INTR_MASTER(sourceMask) \
                                do{                      \
                                EMI_I2CM_INTR_MASTER_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                }while(0)

#define EMI_I2CM_DISABLE_INTR_SLAVE(sourceMask) \
                                do{                     \
                                    EMI_I2CM_INTR_SLAVE_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                }while(0)

#define EMI_I2CM_DISABLE_INTR_TX(sourceMask)    \
                                do{                     \
                                    EMI_I2CM_INTR_TX_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                 }while(0)

#define EMI_I2CM_DISABLE_INTR_RX(sourceMask)    \
                                do{                     \
                                    EMI_I2CM_INTR_RX_MASK_REG &= ((uint32) ~((uint32) (sourceMask))); \
                                }while(0)

/* Set interrupt sources: write sourceMask bits in EMI_I2CM_INTR_X_SET_REG */
#define EMI_I2CM_SET_INTR_MASTER(sourceMask)    \
                                                do{     \
                                                    EMI_I2CM_INTR_MASTER_SET_REG = (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_SET_INTR_SLAVE(sourceMask) \
                                                do{ \
                                                    EMI_I2CM_INTR_SLAVE_SET_REG = (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_SET_INTR_TX(sourceMask)    \
                                                do{ \
                                                    EMI_I2CM_INTR_TX_SET_REG = (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_SET_INTR_RX(sourceMask)    \
                                                do{ \
                                                    EMI_I2CM_INTR_RX_SET_REG = (uint32) (sourceMask); \
                                                }while(0)

/* Clear interrupt sources: write sourceMask bits in EMI_I2CM_INTR_X_REG */
#define EMI_I2CM_CLEAR_INTR_I2C_EC(sourceMask)  \
                                                do{     \
                                                    EMI_I2CM_INTR_I2C_EC_REG = (uint32) (sourceMask); \
                                                }while(0)

#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_CLEAR_INTR_SPI_EC(sourceMask)  \
                                                do{     \
                                                    EMI_I2CM_INTR_SPI_EC_REG = (uint32) (sourceMask); \
                                                }while(0)
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

#define EMI_I2CM_CLEAR_INTR_MASTER(sourceMask)  \
                                                do{     \
                                                    EMI_I2CM_INTR_MASTER_REG = (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_CLEAR_INTR_SLAVE(sourceMask)   \
                                                do{     \
                                                    EMI_I2CM_INTR_SLAVE_REG  = (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_CLEAR_INTR_TX(sourceMask)      \
                                                do{     \
                                                    EMI_I2CM_INTR_TX_REG     = (uint32) (sourceMask); \
                                                }while(0)

#define EMI_I2CM_CLEAR_INTR_RX(sourceMask)      \
                                                do{     \
                                                    EMI_I2CM_INTR_RX_REG     = (uint32) (sourceMask); \
                                                }while(0)

/* Return true if sourceMask is set in EMI_I2CM_INTR_CAUSE_REG */
#define EMI_I2CM_CHECK_CAUSE_INTR(sourceMask)    (0u != (EMI_I2CM_INTR_CAUSE_REG & (sourceMask)))

/* Return true if sourceMask is set in INTR_X_MASKED_REG */
#define EMI_I2CM_CHECK_INTR_I2C_EC(sourceMask)  (0u != (EMI_I2CM_INTR_I2C_EC_REG & (sourceMask)))
#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_CHECK_INTR_SPI_EC(sourceMask)  (0u != (EMI_I2CM_INTR_SPI_EC_REG & (sourceMask)))
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */
#define EMI_I2CM_CHECK_INTR_MASTER(sourceMask)  (0u != (EMI_I2CM_INTR_MASTER_REG & (sourceMask)))
#define EMI_I2CM_CHECK_INTR_SLAVE(sourceMask)   (0u != (EMI_I2CM_INTR_SLAVE_REG  & (sourceMask)))
#define EMI_I2CM_CHECK_INTR_TX(sourceMask)      (0u != (EMI_I2CM_INTR_TX_REG     & (sourceMask)))
#define EMI_I2CM_CHECK_INTR_RX(sourceMask)      (0u != (EMI_I2CM_INTR_RX_REG     & (sourceMask)))

/* Return true if sourceMask is set in EMI_I2CM_INTR_X_MASKED_REG */
#define EMI_I2CM_CHECK_INTR_I2C_EC_MASKED(sourceMask)   (0u != (EMI_I2CM_INTR_I2C_EC_MASKED_REG & \
                                                                       (sourceMask)))
#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_CHECK_INTR_SPI_EC_MASKED(sourceMask)   (0u != (EMI_I2CM_INTR_SPI_EC_MASKED_REG & \
                                                                       (sourceMask)))
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */
#define EMI_I2CM_CHECK_INTR_MASTER_MASKED(sourceMask)   (0u != (EMI_I2CM_INTR_MASTER_MASKED_REG & \
                                                                       (sourceMask)))
#define EMI_I2CM_CHECK_INTR_SLAVE_MASKED(sourceMask)    (0u != (EMI_I2CM_INTR_SLAVE_MASKED_REG  & \
                                                                       (sourceMask)))
#define EMI_I2CM_CHECK_INTR_TX_MASKED(sourceMask)       (0u != (EMI_I2CM_INTR_TX_MASKED_REG     & \
                                                                       (sourceMask)))
#define EMI_I2CM_CHECK_INTR_RX_MASKED(sourceMask)       (0u != (EMI_I2CM_INTR_RX_MASKED_REG     & \
                                                                       (sourceMask)))

/* Return true if sourceMask is set in EMI_I2CM_CTRL_REG: generally is used to check enable bit */
#define EMI_I2CM_GET_CTRL_ENABLED    (0u != (EMI_I2CM_CTRL_REG & EMI_I2CM_CTRL_ENABLED))

#define EMI_I2CM_CHECK_SLAVE_AUTO_ADDR_NACK     (0u != (EMI_I2CM_I2C_CTRL_REG & \
                                                                EMI_I2CM_I2C_CTRL_S_NOT_READY_DATA_NACK))


/***************************************
*      I2C Macro Definitions
***************************************/

/* Enable auto ACK/NACK */
#define EMI_I2CM_ENABLE_SLAVE_AUTO_ADDR_NACK \
                            do{                      \
                                EMI_I2CM_I2C_CTRL_REG |= EMI_I2CM_I2C_CTRL_S_NOT_READY_DATA_NACK; \
                            }while(0)

#define EMI_I2CM_ENABLE_SLAVE_AUTO_DATA_ACK \
                            do{                     \
                                EMI_I2CM_I2C_CTRL_REG |= EMI_I2CM_I2C_CTRL_S_READY_DATA_ACK; \
                            }while(0)

#define EMI_I2CM_ENABLE_SLAVE_AUTO_DATA_NACK \
                            do{                      \
                                EMI_I2CM_I2C_CTRL_REG |= EMI_I2CM_I2C_CTRL_S_NOT_READY_DATA_NACK; \
                            }while(0)

#define EMI_I2CM_ENABLE_MASTER_AUTO_DATA_ACK \
                            do{                      \
                                EMI_I2CM_I2C_CTRL_REG |= EMI_I2CM_I2C_CTRL_M_READY_DATA_ACK; \
                            }while(0)

#define EMI_I2CM_ENABLE_MASTER_AUTO_DATA_NACK \
                            do{                       \
                                EMI_I2CM_I2C_CTRL_REG |= EMI_I2CM_I2C_CTRL_M_NOT_READY_DATA_NACK; \
                            }while(0)

/* Disable auto ACK/NACK */
#define EMI_I2CM_DISABLE_SLAVE_AUTO_ADDR_NACK \
                            do{                       \
                                EMI_I2CM_I2C_CTRL_REG &= ~EMI_I2CM_I2C_CTRL_S_NOT_READY_DATA_NACK; \
                            }while(0)

#define EMI_I2CM_DISABLE_SLAVE_AUTO_DATA_ACK \
                            do{                      \
                                EMI_I2CM_I2C_CTRL_REG &= ~EMI_I2CM_I2C_CTRL_S_READY_DATA_ACK; \
                            }while(0)

#define EMI_I2CM_DISABLE_SLAVE_AUTO_DATA_NACK \
                            do{                       \
                                EMI_I2CM_I2C_CTRL_REG &= ~EMI_I2CM_I2C_CTRL_S_NOT_READY_DATA_NACK; \
                            }while(0)

#define EMI_I2CM_DISABLE_MASTER_AUTO_DATA_ACK \
                            do{                       \
                                EMI_I2CM_I2C_CTRL_REG &= ~EMI_I2CM_I2C_CTRL_M_READY_DATA_ACK; \
                            }while(0)

#define EMI_I2CM_DISABLE_MASTER_AUTO_DATA_NACK \
                            do{                        \
                                EMI_I2CM_I2C_CTRL_REG &= ~EMI_I2CM_I2C_CTRL_M_NOT_READY_DATA_NACK; \
                            }while(0)

/* Enable Slave autoACK/NACK Data */
#define EMI_I2CM_ENABLE_SLAVE_AUTO_DATA \
                            do{                 \
                                EMI_I2CM_I2C_CTRL_REG |= (EMI_I2CM_I2C_CTRL_S_READY_DATA_ACK |      \
                                                                  EMI_I2CM_I2C_CTRL_S_NOT_READY_DATA_NACK); \
                            }while(0)

/* Disable Slave autoACK/NACK Data */
#define EMI_I2CM_DISABLE_SLAVE_AUTO_DATA \
                            do{                  \
                                EMI_I2CM_I2C_CTRL_REG &= ((uint32) \
                                                                  ~(EMI_I2CM_I2C_CTRL_S_READY_DATA_ACK |       \
                                                                    EMI_I2CM_I2C_CTRL_S_NOT_READY_DATA_NACK)); \
                            }while(0)

/* Disable Master autoACK/NACK Data */
#define EMI_I2CM_DISABLE_MASTER_AUTO_DATA \
                            do{                   \
                                EMI_I2CM_I2C_CTRL_REG &= ((uint32) \
                                                                  ~(EMI_I2CM_I2C_CTRL_M_READY_DATA_ACK |       \
                                                                    EMI_I2CM_I2C_CTRL_M_NOT_READY_DATA_NACK)); \
                            }while(0)

/* Master commands */
#define EMI_I2CM_I2C_MASTER_GENERATE_START \
                            do{                    \
                                EMI_I2CM_I2C_MASTER_CMD_REG = EMI_I2CM_I2C_MASTER_CMD_M_START_ON_IDLE; \
                            }while(0)

#define EMI_I2CM_I2C_MASTER_CLEAR_START \
                            do{                 \
                                EMI_I2CM_I2C_MASTER_CMD_REG =  ((uint32) 0u); \
                            }while(0)

#define EMI_I2CM_I2C_MASTER_GENERATE_RESTART EMI_I2CM_I2CReStartGeneration()

#define EMI_I2CM_I2C_MASTER_GENERATE_STOP \
                            do{                   \
                                EMI_I2CM_I2C_MASTER_CMD_REG =                                            \
                                    (EMI_I2CM_I2C_MASTER_CMD_M_STOP |                                    \
                                        (EMI_I2CM_CHECK_I2C_STATUS(EMI_I2CM_I2C_STATUS_M_READ) ? \
                                            (EMI_I2CM_I2C_MASTER_CMD_M_NACK) : (0u)));                   \
                            }while(0)

#define EMI_I2CM_I2C_MASTER_GENERATE_ACK \
                            do{                  \
                                EMI_I2CM_I2C_MASTER_CMD_REG = EMI_I2CM_I2C_MASTER_CMD_M_ACK; \
                            }while(0)

#define EMI_I2CM_I2C_MASTER_GENERATE_NACK \
                            do{                   \
                                EMI_I2CM_I2C_MASTER_CMD_REG = EMI_I2CM_I2C_MASTER_CMD_M_NACK; \
                            }while(0)

/* Slave commands */
#define EMI_I2CM_I2C_SLAVE_GENERATE_ACK \
                            do{                 \
                                EMI_I2CM_I2C_SLAVE_CMD_REG = EMI_I2CM_I2C_SLAVE_CMD_S_ACK; \
                            }while(0)

#if (EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    /* Slave NACK generation for EC_AM logic on address phase. Ticket ID #183902 */
    void EMI_I2CM_I2CSlaveNackGeneration(void);
    #define EMI_I2CM_I2C_SLAVE_GENERATE_NACK EMI_I2CM_I2CSlaveNackGeneration()

#else
    #define EMI_I2CM_I2C_SLAVE_GENERATE_NACK \
                            do{                      \
                                EMI_I2CM_I2C_SLAVE_CMD_REG = EMI_I2CM_I2C_SLAVE_CMD_S_NACK; \
                            }while(0)
#endif /* (EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */

#define EMI_I2CM_I2C_SLAVE_CLEAR_NACK \
                            do{               \
                                EMI_I2CM_I2C_SLAVE_CMD_REG = 0u; \
                            }while(0)

/* Return 8-bit address. The input address should be 7-bits */
#define EMI_I2CM_GET_I2C_8BIT_ADDRESS(addr) (((uint32) ((uint32) (addr) << \
                                                                    EMI_I2CM_I2C_SLAVE_ADDR_POS)) & \
                                                                        EMI_I2CM_I2C_SLAVE_ADDR_MASK)

#define EMI_I2CM_GET_I2C_7BIT_ADDRESS(addr) ((uint32) (addr) >> EMI_I2CM_I2C_SLAVE_ADDR_POS)

/* Adjust SDA filter Trim settings */
#define EMI_I2CM_DEFAULT_I2C_CFG_SDA_FILT_TRIM  (0x02u)
#define EMI_I2CM_EC_AM_I2C_CFG_SDA_FILT_TRIM    (0x03u)

#if (EMI_I2CM_CY_SCBIP_V0)
    #define EMI_I2CM_SET_I2C_CFG_SDA_FILT_TRIM(sdaTrim) \
        do{                                                 \
            EMI_I2CM_I2C_CFG_REG =                  \
                            ((EMI_I2CM_I2C_CFG_REG & (uint32) ~EMI_I2CM_I2C_CFG_SDA_FILT_TRIM_MASK) | \
                             ((uint32) ((uint32) (sdaTrim) <<EMI_I2CM_I2C_CFG_SDA_FILT_TRIM_POS)));           \
        }while(0)
#endif /* (EMI_I2CM_CY_SCBIP_V0) */

/* Enable/Disable analog and digital filter */
#define EMI_I2CM_DIGITAL_FILTER_DISABLE    (0u)
#define EMI_I2CM_DIGITAL_FILTER_ENABLE     (1u)
#define EMI_I2CM_I2C_DATA_RATE_FS_MODE_MAX (400u)
#if (EMI_I2CM_CY_SCBIP_V0)
    /* EMI_I2CM_I2C_CFG_SDA_FILT_OUT_ENABLED is disabled by default */
    #define EMI_I2CM_I2C_CFG_FILT_MASK  (EMI_I2CM_I2C_CFG_SDA_FILT_ENABLED | \
                                                 EMI_I2CM_I2C_CFG_SCL_FILT_ENABLED)
#else
    /* EMI_I2CM_I2C_CFG_SDA_OUT_FILT_SEL_MASK is disabled by default */
    #define EMI_I2CM_I2C_CFG_FILT_MASK  (EMI_I2CM_I2C_CFG_SDA_IN_FILT_SEL | \
                                                 EMI_I2CM_I2C_CFG_SCL_IN_FILT_SEL)
#endif /* (EMI_I2CM_CY_SCBIP_V0) */

#define EMI_I2CM_I2C_CFG_ANALOG_FITER_DISABLE \
        do{                                           \
            EMI_I2CM_I2C_CFG_REG &= (uint32) ~EMI_I2CM_I2C_CFG_FILT_MASK; \
        }while(0)

#define EMI_I2CM_I2C_CFG_ANALOG_FITER_ENABLE \
        do{                                          \
            EMI_I2CM_I2C_CFG_REG |= (uint32)  EMI_I2CM_I2C_CFG_FILT_MASK; \
        }while(0)

/* Return slave select number from SPI_CTRL register */
#define EMI_I2CM_GET_SPI_CTRL_SS(activeSelect) (((uint32) ((uint32) (activeSelect) << \
                                                                    EMI_I2CM_SPI_CTRL_SLAVE_SELECT_POS)) & \
                                                                        EMI_I2CM_SPI_CTRL_SLAVE_SELECT_MASK)

/* Return true if bit is set in EMI_I2CM_I2C_STATUS_REG */
#define EMI_I2CM_CHECK_I2C_STATUS(sourceMask)   (0u != (EMI_I2CM_I2C_STATUS_REG & (sourceMask)))

/* Return true if bit is set in EMI_I2CM_SPI_STATUS_REG */
#define EMI_I2CM_CHECK_SPI_STATUS(sourceMask)   (0u != (EMI_I2CM_SPI_STATUS_REG & (sourceMask)))

/* Return FIFO size depends on EMI_I2CM_CTRL_BYTE_MODE bit */
#define EMI_I2CM_GET_FIFO_SIZE(condition) ((0u != (condition)) ? \
                                                    (2u * EMI_I2CM_FIFO_SIZE) : (EMI_I2CM_FIFO_SIZE))


/***************************************
*       Get Macros Definitions
***************************************/

/* EMI_I2CM_CTRL */
#define EMI_I2CM_GET_CTRL_OVS(oversample)       (((uint32) (oversample) - 1u) & EMI_I2CM_CTRL_OVS_MASK)

#define EMI_I2CM_GET_CTRL_EC_OP_MODE(opMode)        ((0u != (opMode)) ? \
                                                                (EMI_I2CM_CTRL_EC_OP_MODE)  : (0u))

#define EMI_I2CM_GET_CTRL_EC_AM_MODE(amMode)        ((0u != (amMode)) ? \
                                                                (EMI_I2CM_CTRL_EC_AM_MODE)  : (0u))

#define EMI_I2CM_GET_CTRL_BLOCK(block)              ((0u != (block))  ? \
                                                                (EMI_I2CM_CTRL_BLOCK)       : (0u))

#define EMI_I2CM_GET_CTRL_ADDR_ACCEPT(acceptAddr)   ((0u != (acceptAddr)) ? \
                                                                (EMI_I2CM_CTRL_ADDR_ACCEPT) : (0u))

#if (EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_GET_CTRL_BYTE_MODE(mode)   (0u)
#else
    #define EMI_I2CM_GET_CTRL_BYTE_MODE(mode)   ((0u != (mode)) ? \
                                                            (EMI_I2CM_CTRL_BYTE_MODE) : (0u))
#endif /* (EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */

/* EMI_I2CM_I2C_CTRL */
#define EMI_I2CM_GET_I2C_CTRL_HIGH_PHASE_OVS(oversampleHigh) (((uint32) (oversampleHigh) - 1u) & \
                                                                        EMI_I2CM_I2C_CTRL_HIGH_PHASE_OVS_MASK)

#define EMI_I2CM_GET_I2C_CTRL_LOW_PHASE_OVS(oversampleLow)  ((((uint32) (oversampleLow) - 1u) << \
                                                                    EMI_I2CM_I2C_CTRL_LOW_PHASE_OVS_POS) &  \
                                                                    EMI_I2CM_I2C_CTRL_LOW_PHASE_OVS_MASK)

#define EMI_I2CM_GET_I2C_CTRL_S_NOT_READY_ADDR_NACK(wakeNack) ((0u != (wakeNack)) ? \
                                                            (EMI_I2CM_I2C_CTRL_S_NOT_READY_ADDR_NACK) : (0u))

#define EMI_I2CM_GET_I2C_CTRL_S_GENERAL_IGNORE(genCall) ((0u != (genCall)) ? \
                                                                    (EMI_I2CM_I2C_CTRL_S_GENERAL_IGNORE) : (0u))

#define EMI_I2CM_GET_I2C_CTRL_SL_MSTR_MODE(mode)    ((uint32)(mode) << EMI_I2CM_I2C_CTRL_SLAVE_MODE_POS)

/* EMI_I2CM_SPI_CTRL */
#define EMI_I2CM_GET_SPI_CTRL_CONTINUOUS(separate)  ((0u != (separate)) ? \
                                                                (EMI_I2CM_SPI_CTRL_CONTINUOUS) : (0u))

#define EMI_I2CM_GET_SPI_CTRL_SELECT_PRECEDE(mode)  ((0u != (mode)) ? \
                                                                      (EMI_I2CM_SPI_CTRL_SELECT_PRECEDE) : (0u))

#define EMI_I2CM_GET_SPI_CTRL_SCLK_MODE(mode)       (((uint32) (mode) << \
                                                                        EMI_I2CM_SPI_CTRL_CPHA_POS) & \
                                                                        EMI_I2CM_SPI_CTRL_SCLK_MODE_MASK)

#define EMI_I2CM_GET_SPI_CTRL_LATE_MISO_SAMPLE(lateMiso) ((0u != (lateMiso)) ? \
                                                                    (EMI_I2CM_SPI_CTRL_LATE_MISO_SAMPLE) : (0u))

#if (EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_GET_SPI_CTRL_SCLK_CONTINUOUS(sclkType) (0u)
    #define EMI_I2CM_GET_SPI_CTRL_SSEL_POLARITY(polarity)   (0u)
#else
    #define EMI_I2CM_GET_SPI_CTRL_SCLK_CONTINUOUS(sclkType) ((0u != (sclkType)) ? \
                                                                    (EMI_I2CM_SPI_CTRL_SCLK_CONTINUOUS) : (0u))

    #define EMI_I2CM_GET_SPI_CTRL_SSEL_POLARITY(polarity)   (((uint32) (polarity) << \
                                                                     EMI_I2CM_SPI_CTRL_SSEL0_POLARITY_POS) & \
                                                                     EMI_I2CM_SPI_CTRL_SSEL_POLARITY_MASK)
#endif /* ((EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */

#define EMI_I2CM_GET_SPI_CTRL_SUB_MODE(mode)        (((uint32) (mode) << EMI_I2CM_SPI_CTRL_MODE_POS) & \
                                                                                 EMI_I2CM_SPI_CTRL_MODE_MASK)

#define EMI_I2CM_GET_SPI_CTRL_SLAVE_SELECT(select)  (((uint32) (select) << \
                                                                      EMI_I2CM_SPI_CTRL_SLAVE_SELECT_POS) & \
                                                                      EMI_I2CM_SPI_CTRL_SLAVE_SELECT_MASK)

#define EMI_I2CM_GET_SPI_CTRL_MASTER_MODE(mode)     ((0u != (mode)) ? \
                                                                (EMI_I2CM_SPI_CTRL_MASTER) : (0u))

/* EMI_I2CM_UART_CTRL */
#define EMI_I2CM_GET_UART_CTRL_MODE(mode)           (((uint32) (mode) << \
                                                                            EMI_I2CM_UART_CTRL_MODE_POS) & \
                                                                            EMI_I2CM_UART_CTRL_MODE_MASK)

/* EMI_I2CM_UART_RX_CTRL */
#define EMI_I2CM_GET_UART_RX_CTRL_MODE(stopBits)    (((uint32) (stopBits) - 1u) & \
                                                                        EMI_I2CM_UART_RX_CTRL_STOP_BITS_MASK)

#define EMI_I2CM_GET_UART_RX_CTRL_PARITY(parity)    ((0u != (parity)) ? \
                                                                    (EMI_I2CM_UART_RX_CTRL_PARITY) : (0u))

#define EMI_I2CM_GET_UART_RX_CTRL_POLARITY(polarity)    ((0u != (polarity)) ? \
                                                                    (EMI_I2CM_UART_RX_CTRL_POLARITY) : (0u))

#define EMI_I2CM_GET_UART_RX_CTRL_DROP_ON_PARITY_ERR(dropErr) ((0u != (dropErr)) ? \
                                                        (EMI_I2CM_UART_RX_CTRL_DROP_ON_PARITY_ERR) : (0u))

#define EMI_I2CM_GET_UART_RX_CTRL_DROP_ON_FRAME_ERR(dropErr) ((0u != (dropErr)) ? \
                                                        (EMI_I2CM_UART_RX_CTRL_DROP_ON_FRAME_ERR) : (0u))

#define EMI_I2CM_GET_UART_RX_CTRL_MP_MODE(mpMode)   ((0u != (mpMode)) ? \
                                                        (EMI_I2CM_UART_RX_CTRL_MP_MODE) : (0u))

/* EMI_I2CM_UART_TX_CTRL */
#define EMI_I2CM_GET_UART_TX_CTRL_MODE(stopBits)    (((uint32) (stopBits) - 1u) & \
                                                                EMI_I2CM_UART_RX_CTRL_STOP_BITS_MASK)

#define EMI_I2CM_GET_UART_TX_CTRL_PARITY(parity)    ((0u != (parity)) ? \
                                                               (EMI_I2CM_UART_TX_CTRL_PARITY) : (0u))

#define EMI_I2CM_GET_UART_TX_CTRL_RETRY_NACK(nack)  ((0u != (nack)) ? \
                                                               (EMI_I2CM_UART_TX_CTRL_RETRY_ON_NACK) : (0u))

/* EMI_I2CM_UART_FLOW_CTRL */
#if !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_GET_UART_FLOW_CTRL_TRIGGER_LEVEL(level)   ( (uint32) (level) & \
                                                                 EMI_I2CM_UART_FLOW_CTRL_TRIGGER_LEVEL_MASK)

    #define EMI_I2CM_GET_UART_FLOW_CTRL_RTS_POLARITY(polarity) ((0u != (polarity)) ? \
                                                                (EMI_I2CM_UART_FLOW_CTRL_RTS_POLARITY) : (0u))

    #define EMI_I2CM_GET_UART_FLOW_CTRL_CTS_POLARITY(polarity) ((0u != (polarity)) ? \
                                                                (EMI_I2CM_UART_FLOW_CTRL_CTS_POLARITY) : (0u))

    #define EMI_I2CM_GET_UART_FLOW_CTRL_CTS_ENABLE(ctsEn)      ((0u != (ctsEn)) ? \
                                                                (EMI_I2CM_UART_FLOW_CTRL_CTS_ENABLE) : (0u))
#endif /* !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */

/* EMI_I2CM_RX_CTRL */
#define EMI_I2CM_GET_RX_CTRL_DATA_WIDTH(dataWidth)  (((uint32) (dataWidth) - 1u) & \
                                                                EMI_I2CM_RX_CTRL_DATA_WIDTH_MASK)

#define EMI_I2CM_GET_RX_CTRL_BIT_ORDER(bitOrder)    ((0u != (bitOrder)) ? \
                                                                (EMI_I2CM_RX_CTRL_MSB_FIRST) : (0u))

#define EMI_I2CM_GET_RX_CTRL_MEDIAN(filterEn)       ((0u != (filterEn)) ? \
                                                                (EMI_I2CM_RX_CTRL_MEDIAN) : (0u))

/* EMI_I2CM_RX_MATCH */
#define EMI_I2CM_GET_RX_MATCH_ADDR(addr)    ((uint32) (addr) & EMI_I2CM_RX_MATCH_ADDR_MASK)
#define EMI_I2CM_GET_RX_MATCH_MASK(mask)    (((uint32) (mask) << \
                                                            EMI_I2CM_RX_MATCH_MASK_POS) & \
                                                            EMI_I2CM_RX_MATCH_MASK_MASK)

/* EMI_I2CM_RX_FIFO_CTRL */
#define EMI_I2CM_GET_RX_FIFO_CTRL_TRIGGER_LEVEL(level)  ((uint32) (level) & \
                                                                    EMI_I2CM_RX_FIFO_CTRL_TRIGGER_LEVEL_MASK)

/* EMI_I2CM_TX_CTRL */
#define EMI_I2CM_GET_TX_CTRL_DATA_WIDTH(dataWidth)  (((uint32) (dataWidth) - 1u) & \
                                                                EMI_I2CM_RX_CTRL_DATA_WIDTH_MASK)

#define EMI_I2CM_GET_TX_CTRL_BIT_ORDER(bitOrder)    ((0u != (bitOrder)) ? \
                                                                (EMI_I2CM_TX_CTRL_MSB_FIRST) : (0u))

/* EMI_I2CM_TX_FIFO_CTRL */
#define EMI_I2CM_GET_TX_FIFO_CTRL_TRIGGER_LEVEL(level)  ((uint32) (level) & \
                                                                    EMI_I2CM_TX_FIFO_CTRL_TRIGGER_LEVEL_MASK)

/* EMI_I2CM_INTR_SLAVE_I2C_GENERAL */
#define EMI_I2CM_GET_INTR_SLAVE_I2C_GENERAL(genCall)  ((0u != (genCall)) ? \
                                                                (EMI_I2CM_INTR_SLAVE_I2C_GENERAL) : (0u))

/* Return true if master mode is enabled EMI_I2CM_SPI_CTRL_REG */
#define EMI_I2CM_CHECK_SPI_MASTER   (0u != (EMI_I2CM_SPI_CTRL_REG & EMI_I2CM_SPI_CTRL_MASTER))

/* Return inactive state of SPI SCLK line depends on CPOL */
#define EMI_I2CM_GET_SPI_SCLK_INACTIVE \
            ((0u == (EMI_I2CM_SPI_CTRL_REG & EMI_I2CM_SPI_CTRL_CPOL)) ? (0u) : (1u))

/* Get output pin inactive state */
#if (EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
#define EMI_I2CM_GET_SPI_SS0_INACTIVE       (1u)
#define EMI_I2CM_GET_SPI_SS1_INACTIVE       (1u)
#define EMI_I2CM_GET_SPI_SS2_INACTIVE       (1u)
#define EMI_I2CM_GET_SPI_SS3_INACTIVE       (1u)
#define EMI_I2CM_GET_UART_RTS_INACTIVE      (1u)

#else
#define EMI_I2CM_GET_SPI_SS0_INACTIVE  \
        ((0u != (EMI_I2CM_SPI_CTRL_REG & EMI_I2CM_SPI_CTRL_SSEL0_POLARITY)) ? (0u) : (1u))

#define EMI_I2CM_GET_SPI_SS1_INACTIVE  \
        ((0u != (EMI_I2CM_SPI_CTRL_REG & EMI_I2CM_SPI_CTRL_SSEL1_POLARITY)) ? (0u) : (1u))

#define EMI_I2CM_GET_SPI_SS2_INACTIVE  \
        ((0u != (EMI_I2CM_SPI_CTRL_REG & EMI_I2CM_SPI_CTRL_SSEL2_POLARITY)) ? (0u) : (1u))

#define EMI_I2CM_GET_SPI_SS3_INACTIVE  \
        ((0u != (EMI_I2CM_SPI_CTRL_REG & EMI_I2CM_SPI_CTRL_SSEL3_POLARITY)) ? (0u) : (1u))

#define EMI_I2CM_GET_UART_RTS_INACTIVE \
        ((0u == (EMI_I2CM_UART_FLOW_CTRL_REG & EMI_I2CM_UART_FLOW_CTRL_RTS_POLARITY)) ? (0u) : (1u))

#endif /*(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */

/* Clear register constants for configuration and interrupt mask */
#define EMI_I2CM_CLEAR_REG          ((uint32) (0u))
#define EMI_I2CM_NO_INTR_SOURCES    ((uint32) (0u))
#define EMI_I2CM_DUMMY_PARAM        ((uint32) (0u))
#define EMI_I2CM_SUBMODE_SPI_SLAVE  ((uint32) (0u))

/* Return in case of I2C read error */
#define EMI_I2CM_I2C_INVALID_BYTE   ((uint32) 0xFFFFFFFFu)
#define EMI_I2CM_CHECK_VALID_BYTE   ((uint32) 0x80000000u)


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

#define EMI_I2CM_CHECK_INTR_EC_I2C(sourceMask)  EMI_I2CM_CHECK_INTR_I2C_EC(sourceMask)
#if (!EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_CHECK_INTR_EC_SPI(sourceMask)  EMI_I2CM_CHECK_INTR_SPI_EC(sourceMask)
#endif /* (!EMI_I2CM_CY_SCBIP_V1) */

#define EMI_I2CM_CY_SCBIP_V1_I2C_ONLY   (EMI_I2CM_CY_SCBIP_V1)
#define EMI_I2CM_EZBUFFER_SIZE          (EMI_I2CM_EZ_DATA_NR)

#define EMI_I2CM_EZBUF_DATA00_REG   EMI_I2CM_EZBUF_DATA0_REG
#define EMI_I2CM_EZBUF_DATA00_PTR   EMI_I2CM_EZBUF_DATA0_PTR

#endif /* (CY_SCB_EMI_I2CM_H) */


/* [] END OF FILE */
