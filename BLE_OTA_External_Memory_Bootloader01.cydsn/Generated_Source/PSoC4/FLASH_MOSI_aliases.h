/*******************************************************************************
* File Name: FLASH_MOSI.h  
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

#if !defined(CY_PINS_FLASH_MOSI_ALIASES_H) /* Pins FLASH_MOSI_ALIASES_H */
#define CY_PINS_FLASH_MOSI_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define FLASH_MOSI_0			(FLASH_MOSI__0__PC)
#define FLASH_MOSI_0_PS		(FLASH_MOSI__0__PS)
#define FLASH_MOSI_0_PC		(FLASH_MOSI__0__PC)
#define FLASH_MOSI_0_DR		(FLASH_MOSI__0__DR)
#define FLASH_MOSI_0_SHIFT	(FLASH_MOSI__0__SHIFT)
#define FLASH_MOSI_0_INTR	((uint16)((uint16)0x0003u << (FLASH_MOSI__0__SHIFT*2u)))

#define FLASH_MOSI_INTR_ALL	 ((uint16)(FLASH_MOSI_0_INTR))


#endif /* End Pins FLASH_MOSI_ALIASES_H */


/* [] END OF FILE */
