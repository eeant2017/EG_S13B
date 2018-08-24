/*******************************************************************************
* File Name: FLASH_NCS.h  
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

#if !defined(CY_PINS_FLASH_NCS_ALIASES_H) /* Pins FLASH_NCS_ALIASES_H */
#define CY_PINS_FLASH_NCS_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define FLASH_NCS_0			(FLASH_NCS__0__PC)
#define FLASH_NCS_0_PS		(FLASH_NCS__0__PS)
#define FLASH_NCS_0_PC		(FLASH_NCS__0__PC)
#define FLASH_NCS_0_DR		(FLASH_NCS__0__DR)
#define FLASH_NCS_0_SHIFT	(FLASH_NCS__0__SHIFT)
#define FLASH_NCS_0_INTR	((uint16)((uint16)0x0003u << (FLASH_NCS__0__SHIFT*2u)))

#define FLASH_NCS_INTR_ALL	 ((uint16)(FLASH_NCS_0_INTR))


#endif /* End Pins FLASH_NCS_ALIASES_H */


/* [] END OF FILE */
