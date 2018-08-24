/*******************************************************************************
* File Name: DEFEND_ALARM.h  
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

#if !defined(CY_PINS_DEFEND_ALARM_H) /* Pins DEFEND_ALARM_H */
#define CY_PINS_DEFEND_ALARM_H

#include "cytypes.h"
#include "cyfitter.h"
#include "DEFEND_ALARM_aliases.h"


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
} DEFEND_ALARM_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   DEFEND_ALARM_Read(void);
void    DEFEND_ALARM_Write(uint8 value);
uint8   DEFEND_ALARM_ReadDataReg(void);
#if defined(DEFEND_ALARM__PC) || (CY_PSOC4_4200L) 
    void    DEFEND_ALARM_SetDriveMode(uint8 mode);
#endif
void    DEFEND_ALARM_SetInterruptMode(uint16 position, uint16 mode);
uint8   DEFEND_ALARM_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void DEFEND_ALARM_Sleep(void); 
void DEFEND_ALARM_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(DEFEND_ALARM__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define DEFEND_ALARM_DRIVE_MODE_BITS        (3)
    #define DEFEND_ALARM_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - DEFEND_ALARM_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the DEFEND_ALARM_SetDriveMode() function.
         *  @{
         */
        #define DEFEND_ALARM_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define DEFEND_ALARM_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define DEFEND_ALARM_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define DEFEND_ALARM_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define DEFEND_ALARM_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define DEFEND_ALARM_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define DEFEND_ALARM_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define DEFEND_ALARM_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define DEFEND_ALARM_MASK               DEFEND_ALARM__MASK
#define DEFEND_ALARM_SHIFT              DEFEND_ALARM__SHIFT
#define DEFEND_ALARM_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in DEFEND_ALARM_SetInterruptMode() function.
     *  @{
     */
        #define DEFEND_ALARM_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define DEFEND_ALARM_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define DEFEND_ALARM_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define DEFEND_ALARM_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(DEFEND_ALARM__SIO)
    #define DEFEND_ALARM_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(DEFEND_ALARM__PC) && (CY_PSOC4_4200L)
    #define DEFEND_ALARM_USBIO_ENABLE               ((uint32)0x80000000u)
    #define DEFEND_ALARM_USBIO_DISABLE              ((uint32)(~DEFEND_ALARM_USBIO_ENABLE))
    #define DEFEND_ALARM_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define DEFEND_ALARM_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define DEFEND_ALARM_USBIO_ENTER_SLEEP          ((uint32)((1u << DEFEND_ALARM_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << DEFEND_ALARM_USBIO_SUSPEND_DEL_SHIFT)))
    #define DEFEND_ALARM_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << DEFEND_ALARM_USBIO_SUSPEND_SHIFT)))
    #define DEFEND_ALARM_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << DEFEND_ALARM_USBIO_SUSPEND_DEL_SHIFT)))
    #define DEFEND_ALARM_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(DEFEND_ALARM__PC)
    /* Port Configuration */
    #define DEFEND_ALARM_PC                 (* (reg32 *) DEFEND_ALARM__PC)
#endif
/* Pin State */
#define DEFEND_ALARM_PS                     (* (reg32 *) DEFEND_ALARM__PS)
/* Data Register */
#define DEFEND_ALARM_DR                     (* (reg32 *) DEFEND_ALARM__DR)
/* Input Buffer Disable Override */
#define DEFEND_ALARM_INP_DIS                (* (reg32 *) DEFEND_ALARM__PC2)

/* Interrupt configuration Registers */
#define DEFEND_ALARM_INTCFG                 (* (reg32 *) DEFEND_ALARM__INTCFG)
#define DEFEND_ALARM_INTSTAT                (* (reg32 *) DEFEND_ALARM__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define DEFEND_ALARM_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(DEFEND_ALARM__SIO)
    #define DEFEND_ALARM_SIO_REG            (* (reg32 *) DEFEND_ALARM__SIO)
#endif /* (DEFEND_ALARM__SIO_CFG) */

/* USBIO registers */
#if !defined(DEFEND_ALARM__PC) && (CY_PSOC4_4200L)
    #define DEFEND_ALARM_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define DEFEND_ALARM_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define DEFEND_ALARM_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define DEFEND_ALARM_DRIVE_MODE_SHIFT       (0x00u)
#define DEFEND_ALARM_DRIVE_MODE_MASK        (0x07u << DEFEND_ALARM_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins DEFEND_ALARM_H */


/* [] END OF FILE */
