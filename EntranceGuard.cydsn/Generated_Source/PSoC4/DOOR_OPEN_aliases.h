/*******************************************************************************
* File Name: DOOR_OPEN.h  
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

#if !defined(CY_PINS_DOOR_OPEN_ALIASES_H) /* Pins DOOR_OPEN_ALIASES_H */
#define CY_PINS_DOOR_OPEN_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DOOR_OPEN_0			(DOOR_OPEN__0__PC)
#define DOOR_OPEN_0_PS		(DOOR_OPEN__0__PS)
#define DOOR_OPEN_0_PC		(DOOR_OPEN__0__PC)
#define DOOR_OPEN_0_DR		(DOOR_OPEN__0__DR)
#define DOOR_OPEN_0_SHIFT	(DOOR_OPEN__0__SHIFT)
#define DOOR_OPEN_0_INTR	((uint16)((uint16)0x0003u << (DOOR_OPEN__0__SHIFT*2u)))

#define DOOR_OPEN_INTR_ALL	 ((uint16)(DOOR_OPEN_0_INTR))


#endif /* End Pins DOOR_OPEN_ALIASES_H */


/* [] END OF FILE */
