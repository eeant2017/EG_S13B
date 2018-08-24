/*******************************************************************************
* File Name: FLASH_MOSI.h  
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

#if !defined(CY_PINS_FLASH_MOSI_H) /* Pins FLASH_MOSI_H */
#define CY_PINS_FLASH_MOSI_H

#include "cytypes.h"
#include "cyfitter.h"
#include "FLASH_MOSI_aliases.h"


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
} FLASH_MOSI_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   FLASH_MOSI_Read(void);
void    FLASH_MOSI_Write(uint8 value);
uint8   FLASH_MOSI_ReadDataReg(void);
#if defined(FLASH_MOSI__PC) || (CY_PSOC4_4200L) 
    void    FLASH_MOSI_SetDriveMode(uint8 mode);
#endif
void    FLASH_MOSI_SetInterruptMode(uint16 position, uint16 mode);
uint8   FLASH_MOSI_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void FLASH_MOSI_Sleep(void); 
void FLASH_MOSI_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(FLASH_MOSI__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define FLASH_MOSI_DRIVE_MODE_BITS        (3)
    #define FLASH_MOSI_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - FLASH_MOSI_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the FLASH_MOSI_SetDriveMode() function.
         *  @{
         */
        #define FLASH_MOSI_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define FLASH_MOSI_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define FLASH_MOSI_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define FLASH_MOSI_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define FLASH_MOSI_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define FLASH_MOSI_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define FLASH_MOSI_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define FLASH_MOSI_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define FLASH_MOSI_MASK               FLASH_MOSI__MASK
#define FLASH_MOSI_SHIFT              FLASH_MOSI__SHIFT
#define FLASH_MOSI_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in FLASH_MOSI_SetInterruptMode() function.
     *  @{
     */
        #define FLASH_MOSI_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define FLASH_MOSI_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define FLASH_MOSI_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define FLASH_MOSI_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(FLASH_MOSI__SIO)
    #define FLASH_MOSI_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(FLASH_MOSI__PC) && (CY_PSOC4_4200L)
    #define FLASH_MOSI_USBIO_ENABLE               ((uint32)0x80000000u)
    #define FLASH_MOSI_USBIO_DISABLE              ((uint32)(~FLASH_MOSI_USBIO_ENABLE))
    #define FLASH_MOSI_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define FLASH_MOSI_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define FLASH_MOSI_USBIO_ENTER_SLEEP          ((uint32)((1u << FLASH_MOSI_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << FLASH_MOSI_USBIO_SUSPEND_DEL_SHIFT)))
    #define FLASH_MOSI_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << FLASH_MOSI_USBIO_SUSPEND_SHIFT)))
    #define FLASH_MOSI_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << FLASH_MOSI_USBIO_SUSPEND_DEL_SHIFT)))
    #define FLASH_MOSI_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(FLASH_MOSI__PC)
    /* Port Configuration */
    #define FLASH_MOSI_PC                 (* (reg32 *) FLASH_MOSI__PC)
#endif
/* Pin State */
#define FLASH_MOSI_PS                     (* (reg32 *) FLASH_MOSI__PS)
/* Data Register */
#define FLASH_MOSI_DR                     (* (reg32 *) FLASH_MOSI__DR)
/* Input Buffer Disable Override */
#define FLASH_MOSI_INP_DIS                (* (reg32 *) FLASH_MOSI__PC2)

/* Interrupt configuration Registers */
#define FLASH_MOSI_INTCFG                 (* (reg32 *) FLASH_MOSI__INTCFG)
#define FLASH_MOSI_INTSTAT                (* (reg32 *) FLASH_MOSI__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define FLASH_MOSI_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(FLASH_MOSI__SIO)
    #define FLASH_MOSI_SIO_REG            (* (reg32 *) FLASH_MOSI__SIO)
#endif /* (FLASH_MOSI__SIO_CFG) */

/* USBIO registers */
#if !defined(FLASH_MOSI__PC) && (CY_PSOC4_4200L)
    #define FLASH_MOSI_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define FLASH_MOSI_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define FLASH_MOSI_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define FLASH_MOSI_DRIVE_MODE_SHIFT       (0x00u)
#define FLASH_MOSI_DRIVE_MODE_MASK        (0x07u << FLASH_MOSI_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins FLASH_MOSI_H */


/* [] END OF FILE */
