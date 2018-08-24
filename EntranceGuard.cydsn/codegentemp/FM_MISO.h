/*******************************************************************************
* File Name: FM_MISO.h  
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

#if !defined(CY_PINS_FM_MISO_H) /* Pins FM_MISO_H */
#define CY_PINS_FM_MISO_H

#include "cytypes.h"
#include "cyfitter.h"
#include "FM_MISO_aliases.h"


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
} FM_MISO_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   FM_MISO_Read(void);
void    FM_MISO_Write(uint8 value);
uint8   FM_MISO_ReadDataReg(void);
#if defined(FM_MISO__PC) || (CY_PSOC4_4200L) 
    void    FM_MISO_SetDriveMode(uint8 mode);
#endif
void    FM_MISO_SetInterruptMode(uint16 position, uint16 mode);
uint8   FM_MISO_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void FM_MISO_Sleep(void); 
void FM_MISO_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(FM_MISO__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define FM_MISO_DRIVE_MODE_BITS        (3)
    #define FM_MISO_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - FM_MISO_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the FM_MISO_SetDriveMode() function.
         *  @{
         */
        #define FM_MISO_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define FM_MISO_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define FM_MISO_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define FM_MISO_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define FM_MISO_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define FM_MISO_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define FM_MISO_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define FM_MISO_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define FM_MISO_MASK               FM_MISO__MASK
#define FM_MISO_SHIFT              FM_MISO__SHIFT
#define FM_MISO_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in FM_MISO_SetInterruptMode() function.
     *  @{
     */
        #define FM_MISO_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define FM_MISO_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define FM_MISO_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define FM_MISO_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(FM_MISO__SIO)
    #define FM_MISO_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(FM_MISO__PC) && (CY_PSOC4_4200L)
    #define FM_MISO_USBIO_ENABLE               ((uint32)0x80000000u)
    #define FM_MISO_USBIO_DISABLE              ((uint32)(~FM_MISO_USBIO_ENABLE))
    #define FM_MISO_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define FM_MISO_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define FM_MISO_USBIO_ENTER_SLEEP          ((uint32)((1u << FM_MISO_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << FM_MISO_USBIO_SUSPEND_DEL_SHIFT)))
    #define FM_MISO_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << FM_MISO_USBIO_SUSPEND_SHIFT)))
    #define FM_MISO_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << FM_MISO_USBIO_SUSPEND_DEL_SHIFT)))
    #define FM_MISO_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(FM_MISO__PC)
    /* Port Configuration */
    #define FM_MISO_PC                 (* (reg32 *) FM_MISO__PC)
#endif
/* Pin State */
#define FM_MISO_PS                     (* (reg32 *) FM_MISO__PS)
/* Data Register */
#define FM_MISO_DR                     (* (reg32 *) FM_MISO__DR)
/* Input Buffer Disable Override */
#define FM_MISO_INP_DIS                (* (reg32 *) FM_MISO__PC2)

/* Interrupt configuration Registers */
#define FM_MISO_INTCFG                 (* (reg32 *) FM_MISO__INTCFG)
#define FM_MISO_INTSTAT                (* (reg32 *) FM_MISO__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define FM_MISO_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(FM_MISO__SIO)
    #define FM_MISO_SIO_REG            (* (reg32 *) FM_MISO__SIO)
#endif /* (FM_MISO__SIO_CFG) */

/* USBIO registers */
#if !defined(FM_MISO__PC) && (CY_PSOC4_4200L)
    #define FM_MISO_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define FM_MISO_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define FM_MISO_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define FM_MISO_DRIVE_MODE_SHIFT       (0x00u)
#define FM_MISO_DRIVE_MODE_MASK        (0x07u << FM_MISO_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins FM_MISO_H */


/* [] END OF FILE */
