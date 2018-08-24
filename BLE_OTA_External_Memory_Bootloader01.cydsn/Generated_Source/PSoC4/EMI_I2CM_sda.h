/*******************************************************************************
* File Name: EMI_I2CM_sda.h  
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

#if !defined(CY_PINS_EMI_I2CM_sda_H) /* Pins EMI_I2CM_sda_H */
#define CY_PINS_EMI_I2CM_sda_H

#include "cytypes.h"
#include "cyfitter.h"
#include "EMI_I2CM_sda_aliases.h"


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
} EMI_I2CM_sda_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   EMI_I2CM_sda_Read(void);
void    EMI_I2CM_sda_Write(uint8 value);
uint8   EMI_I2CM_sda_ReadDataReg(void);
#if defined(EMI_I2CM_sda__PC) || (CY_PSOC4_4200L) 
    void    EMI_I2CM_sda_SetDriveMode(uint8 mode);
#endif
void    EMI_I2CM_sda_SetInterruptMode(uint16 position, uint16 mode);
uint8   EMI_I2CM_sda_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void EMI_I2CM_sda_Sleep(void); 
void EMI_I2CM_sda_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(EMI_I2CM_sda__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define EMI_I2CM_sda_DRIVE_MODE_BITS        (3)
    #define EMI_I2CM_sda_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - EMI_I2CM_sda_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the EMI_I2CM_sda_SetDriveMode() function.
         *  @{
         */
        #define EMI_I2CM_sda_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define EMI_I2CM_sda_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define EMI_I2CM_sda_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define EMI_I2CM_sda_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define EMI_I2CM_sda_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define EMI_I2CM_sda_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define EMI_I2CM_sda_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define EMI_I2CM_sda_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define EMI_I2CM_sda_MASK               EMI_I2CM_sda__MASK
#define EMI_I2CM_sda_SHIFT              EMI_I2CM_sda__SHIFT
#define EMI_I2CM_sda_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in EMI_I2CM_sda_SetInterruptMode() function.
     *  @{
     */
        #define EMI_I2CM_sda_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define EMI_I2CM_sda_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define EMI_I2CM_sda_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define EMI_I2CM_sda_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(EMI_I2CM_sda__SIO)
    #define EMI_I2CM_sda_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(EMI_I2CM_sda__PC) && (CY_PSOC4_4200L)
    #define EMI_I2CM_sda_USBIO_ENABLE               ((uint32)0x80000000u)
    #define EMI_I2CM_sda_USBIO_DISABLE              ((uint32)(~EMI_I2CM_sda_USBIO_ENABLE))
    #define EMI_I2CM_sda_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define EMI_I2CM_sda_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define EMI_I2CM_sda_USBIO_ENTER_SLEEP          ((uint32)((1u << EMI_I2CM_sda_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << EMI_I2CM_sda_USBIO_SUSPEND_DEL_SHIFT)))
    #define EMI_I2CM_sda_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << EMI_I2CM_sda_USBIO_SUSPEND_SHIFT)))
    #define EMI_I2CM_sda_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << EMI_I2CM_sda_USBIO_SUSPEND_DEL_SHIFT)))
    #define EMI_I2CM_sda_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(EMI_I2CM_sda__PC)
    /* Port Configuration */
    #define EMI_I2CM_sda_PC                 (* (reg32 *) EMI_I2CM_sda__PC)
#endif
/* Pin State */
#define EMI_I2CM_sda_PS                     (* (reg32 *) EMI_I2CM_sda__PS)
/* Data Register */
#define EMI_I2CM_sda_DR                     (* (reg32 *) EMI_I2CM_sda__DR)
/* Input Buffer Disable Override */
#define EMI_I2CM_sda_INP_DIS                (* (reg32 *) EMI_I2CM_sda__PC2)

/* Interrupt configuration Registers */
#define EMI_I2CM_sda_INTCFG                 (* (reg32 *) EMI_I2CM_sda__INTCFG)
#define EMI_I2CM_sda_INTSTAT                (* (reg32 *) EMI_I2CM_sda__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define EMI_I2CM_sda_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(EMI_I2CM_sda__SIO)
    #define EMI_I2CM_sda_SIO_REG            (* (reg32 *) EMI_I2CM_sda__SIO)
#endif /* (EMI_I2CM_sda__SIO_CFG) */

/* USBIO registers */
#if !defined(EMI_I2CM_sda__PC) && (CY_PSOC4_4200L)
    #define EMI_I2CM_sda_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define EMI_I2CM_sda_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define EMI_I2CM_sda_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define EMI_I2CM_sda_DRIVE_MODE_SHIFT       (0x00u)
#define EMI_I2CM_sda_DRIVE_MODE_MASK        (0x07u << EMI_I2CM_sda_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins EMI_I2CM_sda_H */


/* [] END OF FILE */
