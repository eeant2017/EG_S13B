/*******************************************************************************
* File Name: FLASH_NCS.h  
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

#if !defined(CY_PINS_FLASH_NCS_H) /* Pins FLASH_NCS_H */
#define CY_PINS_FLASH_NCS_H

#include "cytypes.h"
#include "cyfitter.h"
#include "FLASH_NCS_aliases.h"


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
} FLASH_NCS_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   FLASH_NCS_Read(void);
void    FLASH_NCS_Write(uint8 value);
uint8   FLASH_NCS_ReadDataReg(void);
#if defined(FLASH_NCS__PC) || (CY_PSOC4_4200L) 
    void    FLASH_NCS_SetDriveMode(uint8 mode);
#endif
void    FLASH_NCS_SetInterruptMode(uint16 position, uint16 mode);
uint8   FLASH_NCS_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void FLASH_NCS_Sleep(void); 
void FLASH_NCS_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(FLASH_NCS__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define FLASH_NCS_DRIVE_MODE_BITS        (3)
    #define FLASH_NCS_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - FLASH_NCS_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the FLASH_NCS_SetDriveMode() function.
         *  @{
         */
        #define FLASH_NCS_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define FLASH_NCS_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define FLASH_NCS_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define FLASH_NCS_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define FLASH_NCS_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define FLASH_NCS_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define FLASH_NCS_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define FLASH_NCS_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define FLASH_NCS_MASK               FLASH_NCS__MASK
#define FLASH_NCS_SHIFT              FLASH_NCS__SHIFT
#define FLASH_NCS_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in FLASH_NCS_SetInterruptMode() function.
     *  @{
     */
        #define FLASH_NCS_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define FLASH_NCS_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define FLASH_NCS_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define FLASH_NCS_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(FLASH_NCS__SIO)
    #define FLASH_NCS_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(FLASH_NCS__PC) && (CY_PSOC4_4200L)
    #define FLASH_NCS_USBIO_ENABLE               ((uint32)0x80000000u)
    #define FLASH_NCS_USBIO_DISABLE              ((uint32)(~FLASH_NCS_USBIO_ENABLE))
    #define FLASH_NCS_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define FLASH_NCS_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define FLASH_NCS_USBIO_ENTER_SLEEP          ((uint32)((1u << FLASH_NCS_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << FLASH_NCS_USBIO_SUSPEND_DEL_SHIFT)))
    #define FLASH_NCS_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << FLASH_NCS_USBIO_SUSPEND_SHIFT)))
    #define FLASH_NCS_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << FLASH_NCS_USBIO_SUSPEND_DEL_SHIFT)))
    #define FLASH_NCS_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(FLASH_NCS__PC)
    /* Port Configuration */
    #define FLASH_NCS_PC                 (* (reg32 *) FLASH_NCS__PC)
#endif
/* Pin State */
#define FLASH_NCS_PS                     (* (reg32 *) FLASH_NCS__PS)
/* Data Register */
#define FLASH_NCS_DR                     (* (reg32 *) FLASH_NCS__DR)
/* Input Buffer Disable Override */
#define FLASH_NCS_INP_DIS                (* (reg32 *) FLASH_NCS__PC2)

/* Interrupt configuration Registers */
#define FLASH_NCS_INTCFG                 (* (reg32 *) FLASH_NCS__INTCFG)
#define FLASH_NCS_INTSTAT                (* (reg32 *) FLASH_NCS__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define FLASH_NCS_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(FLASH_NCS__SIO)
    #define FLASH_NCS_SIO_REG            (* (reg32 *) FLASH_NCS__SIO)
#endif /* (FLASH_NCS__SIO_CFG) */

/* USBIO registers */
#if !defined(FLASH_NCS__PC) && (CY_PSOC4_4200L)
    #define FLASH_NCS_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define FLASH_NCS_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define FLASH_NCS_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define FLASH_NCS_DRIVE_MODE_SHIFT       (0x00u)
#define FLASH_NCS_DRIVE_MODE_MASK        (0x07u << FLASH_NCS_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins FLASH_NCS_H */


/* [] END OF FILE */
