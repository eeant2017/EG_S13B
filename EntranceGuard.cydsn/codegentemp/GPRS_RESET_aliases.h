/*******************************************************************************
* File Name: GPRS_RESET.h  
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

#if !defined(CY_PINS_GPRS_RESET_ALIASES_H) /* Pins GPRS_RESET_ALIASES_H */
#define CY_PINS_GPRS_RESET_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define GPRS_RESET_0			(GPRS_RESET__0__PC)
#define GPRS_RESET_0_PS		(GPRS_RESET__0__PS)
#define GPRS_RESET_0_PC		(GPRS_RESET__0__PC)
#define GPRS_RESET_0_DR		(GPRS_RESET__0__DR)
#define GPRS_RESET_0_SHIFT	(GPRS_RESET__0__SHIFT)
#define GPRS_RESET_0_INTR	((uint16)((uint16)0x0003u << (GPRS_RESET__0__SHIFT*2u)))

#define GPRS_RESET_INTR_ALL	 ((uint16)(GPRS_RESET_0_INTR))


#endif /* End Pins GPRS_RESET_ALIASES_H */


/* [] END OF FILE */
