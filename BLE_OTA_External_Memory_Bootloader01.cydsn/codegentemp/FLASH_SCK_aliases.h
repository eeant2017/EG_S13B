/*******************************************************************************
* File Name: FLASH_SCK.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_FLASH_SCK_ALIASES_H) /* Pins FLASH_SCK_ALIASES_H */
#define CY_PINS_FLASH_SCK_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define FLASH_SCK_0			(FLASH_SCK__0__PC)
#define FLASH_SCK_0_PS		(FLASH_SCK__0__PS)
#define FLASH_SCK_0_PC		(FLASH_SCK__0__PC)
#define FLASH_SCK_0_DR		(FLASH_SCK__0__DR)
#define FLASH_SCK_0_SHIFT	(FLASH_SCK__0__SHIFT)
#define FLASH_SCK_0_INTR	((uint16)((uint16)0x0003u << (FLASH_SCK__0__SHIFT*2u)))

#define FLASH_SCK_INTR_ALL	 ((uint16)(FLASH_SCK_0_INTR))


#endif /* End Pins FLASH_SCK_ALIASES_H */


/* [] END OF FILE */
