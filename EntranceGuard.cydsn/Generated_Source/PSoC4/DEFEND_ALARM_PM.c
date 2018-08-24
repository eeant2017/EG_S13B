/*******************************************************************************
* File Name: DEFEND_ALARM.c  
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
#include "DEFEND_ALARM.h"

static DEFEND_ALARM_BACKUP_STRUCT  DEFEND_ALARM_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: DEFEND_ALARM_Sleep
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
*  \snippet DEFEND_ALARM_SUT.c usage_DEFEND_ALARM_Sleep_Wakeup
*******************************************************************************/
void DEFEND_ALARM_Sleep(void)
{
    #if defined(DEFEND_ALARM__PC)
        DEFEND_ALARM_backup.pcState = DEFEND_ALARM_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            DEFEND_ALARM_backup.usbState = DEFEND_ALARM_CR1_REG;
            DEFEND_ALARM_USB_POWER_REG |= DEFEND_ALARM_USBIO_ENTER_SLEEP;
            DEFEND_ALARM_CR1_REG &= DEFEND_ALARM_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(DEFEND_ALARM__SIO)
        DEFEND_ALARM_backup.sioState = DEFEND_ALARM_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        DEFEND_ALARM_SIO_REG &= (uint32)(~DEFEND_ALARM_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: DEFEND_ALARM_Wakeup
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
*  Refer to DEFEND_ALARM_Sleep() for an example usage.
*******************************************************************************/
void DEFEND_ALARM_Wakeup(void)
{
    #if defined(DEFEND_ALARM__PC)
        DEFEND_ALARM_PC = DEFEND_ALARM_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            DEFEND_ALARM_USB_POWER_REG &= DEFEND_ALARM_USBIO_EXIT_SLEEP_PH1;
            DEFEND_ALARM_CR1_REG = DEFEND_ALARM_backup.usbState;
            DEFEND_ALARM_USB_POWER_REG &= DEFEND_ALARM_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(DEFEND_ALARM__SIO)
        DEFEND_ALARM_SIO_REG = DEFEND_ALARM_backup.sioState;
    #endif
}


/* [] END OF FILE */
