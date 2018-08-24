/*******************************************************************************
* File Name: GPRS_PWRKEY.h  
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

#if !defined(CY_PINS_GPRS_PWRKEY_ALIASES_H) /* Pins GPRS_PWRKEY_ALIASES_H */
#define CY_PINS_GPRS_PWRKEY_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define GPRS_PWRKEY_0			(GPRS_PWRKEY__0__PC)
#define GPRS_PWRKEY_0_PS		(GPRS_PWRKEY__0__PS)
#define GPRS_PWRKEY_0_PC		(GPRS_PWRKEY__0__PC)
#define GPRS_PWRKEY_0_DR		(GPRS_PWRKEY__0__DR)
#define GPRS_PWRKEY_0_SHIFT	(GPRS_PWRKEY__0__SHIFT)
#define GPRS_PWRKEY_0_INTR	((uint16)((uint16)0x0003u << (GPRS_PWRKEY__0__SHIFT*2u)))

#define GPRS_PWRKEY_INTR_ALL	 ((uint16)(GPRS_PWRKEY_0_INTR))


#endif /* End Pins GPRS_PWRKEY_ALIASES_H */


/* [] END OF FILE */
