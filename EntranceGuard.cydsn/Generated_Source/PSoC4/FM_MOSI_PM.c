/*******************************************************************************
* File Name: FM_MOSI.c  
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
#include "FM_MOSI.h"

static FM_MOSI_BACKUP_STRUCT  FM_MOSI_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: FM_MOSI_Sleep
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
*  \snippet FM_MOSI_SUT.c usage_FM_MOSI_Sleep_Wakeup
*******************************************************************************/
void FM_MOSI_Sleep(void)
{
    #if defined(FM_MOSI__PC)
        FM_MOSI_backup.pcState = FM_MOSI_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            FM_MOSI_backup.usbState = FM_MOSI_CR1_REG;
            FM_MOSI_USB_POWER_REG |= FM_MOSI_USBIO_ENTER_SLEEP;
            FM_MOSI_CR1_REG &= FM_MOSI_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(FM_MOSI__SIO)
        FM_MOSI_backup.sioState = FM_MOSI_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        FM_MOSI_SIO_REG &= (uint32)(~FM_MOSI_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: FM_MOSI_Wakeup
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
*  Refer to FM_MOSI_Sleep() for an example usage.
*******************************************************************************/
void FM_MOSI_Wakeup(void)
{
    #if defined(FM_MOSI__PC)
        FM_MOSI_PC = FM_MOSI_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            FM_MOSI_USB_POWER_REG &= FM_MOSI_USBIO_EXIT_SLEEP_PH1;
            FM_MOSI_CR1_REG = FM_MOSI_backup.usbState;
            FM_MOSI_USB_POWER_REG &= FM_MOSI_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(FM_MOSI__SIO)
        FM_MOSI_SIO_REG = FM_MOSI_backup.sioState;
    #endif
}


/* [] END OF FILE */
