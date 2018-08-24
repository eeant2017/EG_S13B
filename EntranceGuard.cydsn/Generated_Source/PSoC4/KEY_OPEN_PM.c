/*******************************************************************************
* File Name: KEY_OPEN.c  
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
#include "KEY_OPEN.h"

static KEY_OPEN_BACKUP_STRUCT  KEY_OPEN_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: KEY_OPEN_Sleep
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
*  \snippet KEY_OPEN_SUT.c usage_KEY_OPEN_Sleep_Wakeup
*******************************************************************************/
void KEY_OPEN_Sleep(void)
{
    #if defined(KEY_OPEN__PC)
        KEY_OPEN_backup.pcState = KEY_OPEN_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            KEY_OPEN_backup.usbState = KEY_OPEN_CR1_REG;
            KEY_OPEN_USB_POWER_REG |= KEY_OPEN_USBIO_ENTER_SLEEP;
            KEY_OPEN_CR1_REG &= KEY_OPEN_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(KEY_OPEN__SIO)
        KEY_OPEN_backup.sioState = KEY_OPEN_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        KEY_OPEN_SIO_REG &= (uint32)(~KEY_OPEN_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: KEY_OPEN_Wakeup
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
*  Refer to KEY_OPEN_Sleep() for an example usage.
*******************************************************************************/
void KEY_OPEN_Wakeup(void)
{
    #if defined(KEY_OPEN__PC)
        KEY_OPEN_PC = KEY_OPEN_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            KEY_OPEN_USB_POWER_REG &= KEY_OPEN_USBIO_EXIT_SLEEP_PH1;
            KEY_OPEN_CR1_REG = KEY_OPEN_backup.usbState;
            KEY_OPEN_USB_POWER_REG &= KEY_OPEN_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(KEY_OPEN__SIO)
        KEY_OPEN_SIO_REG = KEY_OPEN_backup.sioState;
    #endif
}


/* [] END OF FILE */
