/*******************************************************************************
* File Name: FLASH_MOSI.c  
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
#include "FLASH_MOSI.h"

static FLASH_MOSI_BACKUP_STRUCT  FLASH_MOSI_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: FLASH_MOSI_Sleep
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
*  \snippet FLASH_MOSI_SUT.c usage_FLASH_MOSI_Sleep_Wakeup
*******************************************************************************/
void FLASH_MOSI_Sleep(void)
{
    #if defined(FLASH_MOSI__PC)
        FLASH_MOSI_backup.pcState = FLASH_MOSI_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            FLASH_MOSI_backup.usbState = FLASH_MOSI_CR1_REG;
            FLASH_MOSI_USB_POWER_REG |= FLASH_MOSI_USBIO_ENTER_SLEEP;
            FLASH_MOSI_CR1_REG &= FLASH_MOSI_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(FLASH_MOSI__SIO)
        FLASH_MOSI_backup.sioState = FLASH_MOSI_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        FLASH_MOSI_SIO_REG &= (uint32)(~FLASH_MOSI_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: FLASH_MOSI_Wakeup
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
*  Refer to FLASH_MOSI_Sleep() for an example usage.
*******************************************************************************/
void FLASH_MOSI_Wakeup(void)
{
    #if defined(FLASH_MOSI__PC)
        FLASH_MOSI_PC = FLASH_MOSI_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            FLASH_MOSI_USB_POWER_REG &= FLASH_MOSI_USBIO_EXIT_SLEEP_PH1;
            FLASH_MOSI_CR1_REG = FLASH_MOSI_backup.usbState;
            FLASH_MOSI_USB_POWER_REG &= FLASH_MOSI_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(FLASH_MOSI__SIO)
        FLASH_MOSI_SIO_REG = FLASH_MOSI_backup.sioState;
    #endif
}


/* [] END OF FILE */
