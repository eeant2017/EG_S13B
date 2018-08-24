/*******************************************************************************
* File Name: EMI_I2CM_SCBCLK.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_EMI_I2CM_SCBCLK_H)
#define CY_CLOCK_EMI_I2CM_SCBCLK_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void EMI_I2CM_SCBCLK_StartEx(uint32 alignClkDiv);
#define EMI_I2CM_SCBCLK_Start() \
    EMI_I2CM_SCBCLK_StartEx(EMI_I2CM_SCBCLK__PA_DIV_ID)

#else

void EMI_I2CM_SCBCLK_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void EMI_I2CM_SCBCLK_Stop(void);

void EMI_I2CM_SCBCLK_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 EMI_I2CM_SCBCLK_GetDividerRegister(void);
uint8  EMI_I2CM_SCBCLK_GetFractionalDividerRegister(void);

#define EMI_I2CM_SCBCLK_Enable()                         EMI_I2CM_SCBCLK_Start()
#define EMI_I2CM_SCBCLK_Disable()                        EMI_I2CM_SCBCLK_Stop()
#define EMI_I2CM_SCBCLK_SetDividerRegister(clkDivider, reset)  \
    EMI_I2CM_SCBCLK_SetFractionalDividerRegister((clkDivider), 0u)
#define EMI_I2CM_SCBCLK_SetDivider(clkDivider)           EMI_I2CM_SCBCLK_SetDividerRegister((clkDivider), 1u)
#define EMI_I2CM_SCBCLK_SetDividerValue(clkDivider)      EMI_I2CM_SCBCLK_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define EMI_I2CM_SCBCLK_DIV_ID     EMI_I2CM_SCBCLK__DIV_ID

#define EMI_I2CM_SCBCLK_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define EMI_I2CM_SCBCLK_CTRL_REG   (*(reg32 *)EMI_I2CM_SCBCLK__CTRL_REGISTER)
#define EMI_I2CM_SCBCLK_DIV_REG    (*(reg32 *)EMI_I2CM_SCBCLK__DIV_REGISTER)

#define EMI_I2CM_SCBCLK_CMD_DIV_SHIFT          (0u)
#define EMI_I2CM_SCBCLK_CMD_PA_DIV_SHIFT       (8u)
#define EMI_I2CM_SCBCLK_CMD_DISABLE_SHIFT      (30u)
#define EMI_I2CM_SCBCLK_CMD_ENABLE_SHIFT       (31u)

#define EMI_I2CM_SCBCLK_CMD_DISABLE_MASK       ((uint32)((uint32)1u << EMI_I2CM_SCBCLK_CMD_DISABLE_SHIFT))
#define EMI_I2CM_SCBCLK_CMD_ENABLE_MASK        ((uint32)((uint32)1u << EMI_I2CM_SCBCLK_CMD_ENABLE_SHIFT))

#define EMI_I2CM_SCBCLK_DIV_FRAC_MASK  (0x000000F8u)
#define EMI_I2CM_SCBCLK_DIV_FRAC_SHIFT (3u)
#define EMI_I2CM_SCBCLK_DIV_INT_MASK   (0xFFFFFF00u)
#define EMI_I2CM_SCBCLK_DIV_INT_SHIFT  (8u)

#else 

#define EMI_I2CM_SCBCLK_DIV_REG        (*(reg32 *)EMI_I2CM_SCBCLK__REGISTER)
#define EMI_I2CM_SCBCLK_ENABLE_REG     EMI_I2CM_SCBCLK_DIV_REG
#define EMI_I2CM_SCBCLK_DIV_FRAC_MASK  EMI_I2CM_SCBCLK__FRAC_MASK
#define EMI_I2CM_SCBCLK_DIV_FRAC_SHIFT (16u)
#define EMI_I2CM_SCBCLK_DIV_INT_MASK   EMI_I2CM_SCBCLK__DIVIDER_MASK
#define EMI_I2CM_SCBCLK_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_EMI_I2CM_SCBCLK_H) */

/* [] END OF FILE */
