/*******************************************************************************
* File Name: KEY_OPEN.h  
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

#if !defined(CY_PINS_KEY_OPEN_ALIASES_H) /* Pins KEY_OPEN_ALIASES_H */
#define CY_PINS_KEY_OPEN_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define KEY_OPEN_0			(KEY_OPEN__0__PC)
#define KEY_OPEN_0_PS		(KEY_OPEN__0__PS)
#define KEY_OPEN_0_PC		(KEY_OPEN__0__PC)
#define KEY_OPEN_0_DR		(KEY_OPEN__0__DR)
#define KEY_OPEN_0_SHIFT	(KEY_OPEN__0__SHIFT)
#define KEY_OPEN_0_INTR	((uint16)((uint16)0x0003u << (KEY_OPEN__0__SHIFT*2u)))

#define KEY_OPEN_INTR_ALL	 ((uint16)(KEY_OPEN_0_INTR))


#endif /* End Pins KEY_OPEN_ALIASES_H */


/* [] END OF FILE */
