/*******************************************************************************
* File Name: GPRS_STATE.h  
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

#if !defined(CY_PINS_GPRS_STATE_ALIASES_H) /* Pins GPRS_STATE_ALIASES_H */
#define CY_PINS_GPRS_STATE_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define GPRS_STATE_0			(GPRS_STATE__0__PC)
#define GPRS_STATE_0_PS		(GPRS_STATE__0__PS)
#define GPRS_STATE_0_PC		(GPRS_STATE__0__PC)
#define GPRS_STATE_0_DR		(GPRS_STATE__0__DR)
#define GPRS_STATE_0_SHIFT	(GPRS_STATE__0__SHIFT)
#define GPRS_STATE_0_INTR	((uint16)((uint16)0x0003u << (GPRS_STATE__0__SHIFT*2u)))

#define GPRS_STATE_INTR_ALL	 ((uint16)(GPRS_STATE_0_INTR))


#endif /* End Pins GPRS_STATE_ALIASES_H */


/* [] END OF FILE */
