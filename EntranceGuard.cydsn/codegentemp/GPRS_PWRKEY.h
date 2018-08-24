/*******************************************************************************
* File Name: GPRS_PWRKEY.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_GPRS_PWRKEY_H) /* Pins GPRS_PWRKEY_H */
#define CY_PINS_GPRS_PWRKEY_H

#include "cytypes.h"
#include "cyfitter.h"
#include "GPRS_PWRKEY_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} GPRS_PWRKEY_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   GPRS_PWRKEY_Read(void);
void    GPRS_PWRKEY_Write(uint8 value);
uint8   GPRS_PWRKEY_ReadDataReg(void);
#if defined(GPRS_PWRKEY__PC) || (CY_PSOC4_4200L) 
    void    GPRS_PWRKEY_SetDriveMode(uint8 mode);
#endif
void    GPRS_PWRKEY_SetInterruptMode(uint16 position, uint16 mode);
uint8   GPRS_PWRKEY_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void GPRS_PWRKEY_Sleep(void); 
void GPRS_PWRKEY_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(GPRS_PWRKEY__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define GPRS_PWRKEY_DRIVE_MODE_BITS        (3)
    #define GPRS_PWRKEY_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - GPRS_PWRKEY_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the GPRS_PWRKEY_SetDriveMode() function.
         *  @{
         */
        #define GPRS_PWRKEY_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define GPRS_PWRKEY_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define GPRS_PWRKEY_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define GPRS_PWRKEY_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define GPRS_PWRKEY_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define GPRS_PWRKEY_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define GPRS_PWRKEY_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define GPRS_PWRKEY_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define GPRS_PWRKEY_MASK               GPRS_PWRKEY__MASK
#define GPRS_PWRKEY_SHIFT              GPRS_PWRKEY__SHIFT
#define GPRS_PWRKEY_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in GPRS_PWRKEY_SetInterruptMode() function.
     *  @{
     */
        #define GPRS_PWRKEY_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define GPRS_PWRKEY_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define GPRS_PWRKEY_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define GPRS_PWRKEY_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(GPRS_PWRKEY__SIO)
    #define GPRS_PWRKEY_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(GPRS_PWRKEY__PC) && (CY_PSOC4_4200L)
    #define GPRS_PWRKEY_USBIO_ENABLE               ((uint32)0x80000000u)
    #define GPRS_PWRKEY_USBIO_DISABLE              ((uint32)(~GPRS_PWRKEY_USBIO_ENABLE))
    #define GPRS_PWRKEY_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define GPRS_PWRKEY_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define GPRS_PWRKEY_USBIO_ENTER_SLEEP          ((uint32)((1u << GPRS_PWRKEY_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << GPRS_PWRKEY_USBIO_SUSPEND_DEL_SHIFT)))
    #define GPRS_PWRKEY_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << GPRS_PWRKEY_USBIO_SUSPEND_SHIFT)))
    #define GPRS_PWRKEY_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << GPRS_PWRKEY_USBIO_SUSPEND_DEL_SHIFT)))
    #define GPRS_PWRKEY_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(GPRS_PWRKEY__PC)
    /* Port Configuration */
    #define GPRS_PWRKEY_PC                 (* (reg32 *) GPRS_PWRKEY__PC)
#endif
/* Pin State */
#define GPRS_PWRKEY_PS                     (* (reg32 *) GPRS_PWRKEY__PS)
/* Data Register */
#define GPRS_PWRKEY_DR                     (* (reg32 *) GPRS_PWRKEY__DR)
/* Input Buffer Disable Override */
#define GPRS_PWRKEY_INP_DIS                (* (reg32 *) GPRS_PWRKEY__PC2)

/* Interrupt configuration Registers */
#define GPRS_PWRKEY_INTCFG                 (* (reg32 *) GPRS_PWRKEY__INTCFG)
#define GPRS_PWRKEY_INTSTAT                (* (reg32 *) GPRS_PWRKEY__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define GPRS_PWRKEY_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(GPRS_PWRKEY__SIO)
    #define GPRS_PWRKEY_SIO_REG            (* (reg32 *) GPRS_PWRKEY__SIO)
#endif /* (GPRS_PWRKEY__SIO_CFG) */

/* USBIO registers */
#if !defined(GPRS_PWRKEY__PC) && (CY_PSOC4_4200L)
    #define GPRS_PWRKEY_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define GPRS_PWRKEY_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define GPRS_PWRKEY_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define GPRS_PWRKEY_DRIVE_MODE_SHIFT       (0x00u)
#define GPRS_PWRKEY_DRIVE_MODE_MASK        (0x07u << GPRS_PWRKEY_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins GPRS_PWRKEY_H */


/* [] END OF FILE */
