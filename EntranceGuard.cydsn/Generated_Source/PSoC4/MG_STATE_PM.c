/*******************************************************************************
* File Name: MG_STATE.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "MG_STATE.h"

static MG_STATE_BACKUP_STRUCT  MG_STATE_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: MG_STATE_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet MG_STATE_SUT.c usage_MG_STATE_Sleep_Wakeup
*******************************************************************************/
void MG_STATE_Sleep(void)
{
    #if defined(MG_STATE__PC)
        MG_STATE_backup.pcState = MG_STATE_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            MG_STATE_backup.usbState = MG_STATE_CR1_REG;
            MG_STATE_USB_POWER_REG |= MG_STATE_USBIO_ENTER_SLEEP;
            MG_STATE_CR1_REG &= MG_STATE_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(MG_STATE__SIO)
        MG_STATE_backup.sioState = MG_STATE_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        MG_STATE_SIO_REG &= (uint32)(~MG_STATE_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: MG_STATE_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to MG_STATE_Sleep() for an example usage.
*******************************************************************************/
void MG_STATE_Wakeup(void)
{
    #if defined(MG_STATE__PC)
        MG_STATE_PC = MG_STATE_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            MG_STATE_USB_POWER_REG &= MG_STATE_USBIO_EXIT_SLEEP_PH1;
            MG_STATE_CR1_REG = MG_STATE_backup.usbState;
            MG_STATE_USB_POWER_REG &= MG_STATE_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(MG_STATE__SIO)
        MG_STATE_SIO_REG = MG_STATE_backup.sioState;
    #endif
}


/* [] END OF FILE */
