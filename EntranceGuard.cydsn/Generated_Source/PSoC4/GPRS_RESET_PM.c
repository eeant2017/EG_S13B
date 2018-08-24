/*******************************************************************************
* File Name: GPRS_RESET.c  
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
#include "GPRS_RESET.h"

static GPRS_RESET_BACKUP_STRUCT  GPRS_RESET_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: GPRS_RESET_Sleep
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
*  \snippet GPRS_RESET_SUT.c usage_GPRS_RESET_Sleep_Wakeup
*******************************************************************************/
void GPRS_RESET_Sleep(void)
{
    #if defined(GPRS_RESET__PC)
        GPRS_RESET_backup.pcState = GPRS_RESET_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            GPRS_RESET_backup.usbState = GPRS_RESET_CR1_REG;
            GPRS_RESET_USB_POWER_REG |= GPRS_RESET_USBIO_ENTER_SLEEP;
            GPRS_RESET_CR1_REG &= GPRS_RESET_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(GPRS_RESET__SIO)
        GPRS_RESET_backup.sioState = GPRS_RESET_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        GPRS_RESET_SIO_REG &= (uint32)(~GPRS_RESET_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: GPRS_RESET_Wakeup
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
*  Refer to GPRS_RESET_Sleep() for an example usage.
*******************************************************************************/
void GPRS_RESET_Wakeup(void)
{
    #if defined(GPRS_RESET__PC)
        GPRS_RESET_PC = GPRS_RESET_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            GPRS_RESET_USB_POWER_REG &= GPRS_RESET_USBIO_EXIT_SLEEP_PH1;
            GPRS_RESET_CR1_REG = GPRS_RESET_backup.usbState;
            GPRS_RESET_USB_POWER_REG &= GPRS_RESET_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(GPRS_RESET__SIO)
        GPRS_RESET_SIO_REG = GPRS_RESET_backup.sioState;
    #endif
}


/* [] END OF FILE */
