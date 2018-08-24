/*******************************************************************************
* File Name: FM_NCS.h  
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

#if !defined(CY_PINS_FM_NCS_H) /* Pins FM_NCS_H */
#define CY_PINS_FM_NCS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "FM_NCS_aliases.h"


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
} FM_NCS_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   FM_NCS_Read(void);
void    FM_NCS_Write(uint8 value);
uint8   FM_NCS_ReadDataReg(void);
#if defined(FM_NCS__PC) || (CY_PSOC4_4200L) 
    void    FM_NCS_SetDriveMode(uint8 mode);
#endif
void    FM_NCS_SetInterruptMode(uint16 position, uint16 mode);
uint8   FM_NCS_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void FM_NCS_Sleep(void); 
void FM_NCS_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(FM_NCS__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define FM_NCS_DRIVE_MODE_BITS        (3)
    #define FM_NCS_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - FM_NCS_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the FM_NCS_SetDriveMode() function.
         *  @{
         */
        #define FM_NCS_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define FM_NCS_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define FM_NCS_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define FM_NCS_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define FM_NCS_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define FM_NCS_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define FM_NCS_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define FM_NCS_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define FM_NCS_MASK               FM_NCS__MASK
#define FM_NCS_SHIFT              FM_NCS__SHIFT
#define FM_NCS_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in FM_NCS_SetInterruptMode() function.
     *  @{
     */
        #define FM_NCS_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define FM_NCS_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define FM_NCS_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define FM_NCS_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(FM_NCS__SIO)
    #define FM_NCS_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(FM_NCS__PC) && (CY_PSOC4_4200L)
    #define FM_NCS_USBIO_ENABLE               ((uint32)0x80000000u)
    #define FM_NCS_USBIO_DISABLE              ((uint32)(~FM_NCS_USBIO_ENABLE))
    #define FM_NCS_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define FM_NCS_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define FM_NCS_USBIO_ENTER_SLEEP          ((uint32)((1u << FM_NCS_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << FM_NCS_USBIO_SUSPEND_DEL_SHIFT)))
    #define FM_NCS_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << FM_NCS_USBIO_SUSPEND_SHIFT)))
    #define FM_NCS_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << FM_NCS_USBIO_SUSPEND_DEL_SHIFT)))
    #define FM_NCS_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(FM_NCS__PC)
    /* Port Configuration */
    #define FM_NCS_PC                 (* (reg32 *) FM_NCS__PC)
#endif
/* Pin State */
#define FM_NCS_PS                     (* (reg32 *) FM_NCS__PS)
/* Data Register */
#define FM_NCS_DR                     (* (reg32 *) FM_NCS__DR)
/* Input Buffer Disable Override */
#define FM_NCS_INP_DIS                (* (reg32 *) FM_NCS__PC2)

/* Interrupt configuration Registers */
#define FM_NCS_INTCFG                 (* (reg32 *) FM_NCS__INTCFG)
#define FM_NCS_INTSTAT                (* (reg32 *) FM_NCS__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define FM_NCS_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(FM_NCS__SIO)
    #define FM_NCS_SIO_REG            (* (reg32 *) FM_NCS__SIO)
#endif /* (FM_NCS__SIO_CFG) */

/* USBIO registers */
#if !defined(FM_NCS__PC) && (CY_PSOC4_4200L)
    #define FM_NCS_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define FM_NCS_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define FM_NCS_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define FM_NCS_DRIVE_MODE_SHIFT       (0x00u)
#define FM_NCS_DRIVE_MODE_MASK        (0x07u << FM_NCS_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins FM_NCS_H */


/* [] END OF FILE */
