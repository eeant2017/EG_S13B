/*******************************************************************************
* File Name: MG_STATE.h  
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

#if !defined(CY_PINS_MG_STATE_H) /* Pins MG_STATE_H */
#define CY_PINS_MG_STATE_H

#include "cytypes.h"
#include "cyfitter.h"
#include "MG_STATE_aliases.h"


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
} MG_STATE_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   MG_STATE_Read(void);
void    MG_STATE_Write(uint8 value);
uint8   MG_STATE_ReadDataReg(void);
#if defined(MG_STATE__PC) || (CY_PSOC4_4200L) 
    void    MG_STATE_SetDriveMode(uint8 mode);
#endif
void    MG_STATE_SetInterruptMode(uint16 position, uint16 mode);
uint8   MG_STATE_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void MG_STATE_Sleep(void); 
void MG_STATE_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(MG_STATE__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define MG_STATE_DRIVE_MODE_BITS        (3)
    #define MG_STATE_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - MG_STATE_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the MG_STATE_SetDriveMode() function.
         *  @{
         */
        #define MG_STATE_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define MG_STATE_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define MG_STATE_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define MG_STATE_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define MG_STATE_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define MG_STATE_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define MG_STATE_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define MG_STATE_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define MG_STATE_MASK               MG_STATE__MASK
#define MG_STATE_SHIFT              MG_STATE__SHIFT
#define MG_STATE_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MG_STATE_SetInterruptMode() function.
     *  @{
     */
        #define MG_STATE_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define MG_STATE_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define MG_STATE_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define MG_STATE_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(MG_STATE__SIO)
    #define MG_STATE_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(MG_STATE__PC) && (CY_PSOC4_4200L)
    #define MG_STATE_USBIO_ENABLE               ((uint32)0x80000000u)
    #define MG_STATE_USBIO_DISABLE              ((uint32)(~MG_STATE_USBIO_ENABLE))
    #define MG_STATE_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define MG_STATE_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define MG_STATE_USBIO_ENTER_SLEEP          ((uint32)((1u << MG_STATE_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << MG_STATE_USBIO_SUSPEND_DEL_SHIFT)))
    #define MG_STATE_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << MG_STATE_USBIO_SUSPEND_SHIFT)))
    #define MG_STATE_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << MG_STATE_USBIO_SUSPEND_DEL_SHIFT)))
    #define MG_STATE_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(MG_STATE__PC)
    /* Port Configuration */
    #define MG_STATE_PC                 (* (reg32 *) MG_STATE__PC)
#endif
/* Pin State */
#define MG_STATE_PS                     (* (reg32 *) MG_STATE__PS)
/* Data Register */
#define MG_STATE_DR                     (* (reg32 *) MG_STATE__DR)
/* Input Buffer Disable Override */
#define MG_STATE_INP_DIS                (* (reg32 *) MG_STATE__PC2)

/* Interrupt configuration Registers */
#define MG_STATE_INTCFG                 (* (reg32 *) MG_STATE__INTCFG)
#define MG_STATE_INTSTAT                (* (reg32 *) MG_STATE__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define MG_STATE_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(MG_STATE__SIO)
    #define MG_STATE_SIO_REG            (* (reg32 *) MG_STATE__SIO)
#endif /* (MG_STATE__SIO_CFG) */

/* USBIO registers */
#if !defined(MG_STATE__PC) && (CY_PSOC4_4200L)
    #define MG_STATE_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define MG_STATE_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define MG_STATE_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define MG_STATE_DRIVE_MODE_SHIFT       (0x00u)
#define MG_STATE_DRIVE_MODE_MASK        (0x07u << MG_STATE_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins MG_STATE_H */


/* [] END OF FILE */
