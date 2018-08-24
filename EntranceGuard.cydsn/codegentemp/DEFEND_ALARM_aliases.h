/*******************************************************************************
* File Name: DEFEND_ALARM.h  
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

#if !defined(CY_PINS_DEFEND_ALARM_ALIASES_H) /* Pins DEFEND_ALARM_ALIASES_H */
#define CY_PINS_DEFEND_ALARM_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define DEFEND_ALARM_0			(DEFEND_ALARM__0__PC)
#define DEFEND_ALARM_0_PS		(DEFEND_ALARM__0__PS)
#define DEFEND_ALARM_0_PC		(DEFEND_ALARM__0__PC)
#define DEFEND_ALARM_0_DR		(DEFEND_ALARM__0__DR)
#define DEFEND_ALARM_0_SHIFT	(DEFEND_ALARM__0__SHIFT)
#define DEFEND_ALARM_0_INTR	((uint16)((uint16)0x0003u << (DEFEND_ALARM__0__SHIFT*2u)))

#define DEFEND_ALARM_INTR_ALL	 ((uint16)(DEFEND_ALARM_0_INTR))


#endif /* End Pins DEFEND_ALARM_ALIASES_H */


/* [] END OF FILE */
