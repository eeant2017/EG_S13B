/***************************************************************************//**
* \file UART_433_PINS.h
* \version 4.0
*
* \brief
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_UART_433_H)
#define CY_SCB_PINS_UART_433_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define UART_433_REMOVE_RX_WAKE_SDA_MOSI_PIN  (1u)
#define UART_433_REMOVE_RX_SDA_MOSI_PIN      (1u)
#define UART_433_REMOVE_TX_SCL_MISO_PIN      (1u)
#define UART_433_REMOVE_CTS_SCLK_PIN      (1u)
#define UART_433_REMOVE_RTS_SS0_PIN      (1u)
#define UART_433_REMOVE_SS1_PIN                 (1u)
#define UART_433_REMOVE_SS2_PIN                 (1u)
#define UART_433_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define UART_433_REMOVE_I2C_PINS                (1u)
#define UART_433_REMOVE_SPI_MASTER_PINS         (1u)
#define UART_433_REMOVE_SPI_MASTER_SCLK_PIN     (1u)
#define UART_433_REMOVE_SPI_MASTER_MOSI_PIN     (1u)
#define UART_433_REMOVE_SPI_MASTER_MISO_PIN     (1u)
#define UART_433_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define UART_433_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define UART_433_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define UART_433_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define UART_433_REMOVE_SPI_SLAVE_PINS          (1u)
#define UART_433_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define UART_433_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define UART_433_REMOVE_UART_TX_PIN             (0u)
#define UART_433_REMOVE_UART_RX_TX_PIN          (1u)
#define UART_433_REMOVE_UART_RX_PIN             (0u)
#define UART_433_REMOVE_UART_RX_WAKE_PIN        (1u)
#define UART_433_REMOVE_UART_RTS_PIN            (1u)
#define UART_433_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define UART_433_RX_WAKE_SDA_MOSI_PIN (0u == UART_433_REMOVE_RX_WAKE_SDA_MOSI_PIN)
#define UART_433_RX_SDA_MOSI_PIN     (0u == UART_433_REMOVE_RX_SDA_MOSI_PIN)
#define UART_433_TX_SCL_MISO_PIN     (0u == UART_433_REMOVE_TX_SCL_MISO_PIN)
#define UART_433_CTS_SCLK_PIN     (0u == UART_433_REMOVE_CTS_SCLK_PIN)
#define UART_433_RTS_SS0_PIN     (0u == UART_433_REMOVE_RTS_SS0_PIN)
#define UART_433_SS1_PIN                (0u == UART_433_REMOVE_SS1_PIN)
#define UART_433_SS2_PIN                (0u == UART_433_REMOVE_SS2_PIN)
#define UART_433_SS3_PIN                (0u == UART_433_REMOVE_SS3_PIN)

/* Mode defined pins */
#define UART_433_I2C_PINS               (0u == UART_433_REMOVE_I2C_PINS)
#define UART_433_SPI_MASTER_PINS        (0u == UART_433_REMOVE_SPI_MASTER_PINS)
#define UART_433_SPI_MASTER_SCLK_PIN    (0u == UART_433_REMOVE_SPI_MASTER_SCLK_PIN)
#define UART_433_SPI_MASTER_MOSI_PIN    (0u == UART_433_REMOVE_SPI_MASTER_MOSI_PIN)
#define UART_433_SPI_MASTER_MISO_PIN    (0u == UART_433_REMOVE_SPI_MASTER_MISO_PIN)
#define UART_433_SPI_MASTER_SS0_PIN     (0u == UART_433_REMOVE_SPI_MASTER_SS0_PIN)
#define UART_433_SPI_MASTER_SS1_PIN     (0u == UART_433_REMOVE_SPI_MASTER_SS1_PIN)
#define UART_433_SPI_MASTER_SS2_PIN     (0u == UART_433_REMOVE_SPI_MASTER_SS2_PIN)
#define UART_433_SPI_MASTER_SS3_PIN     (0u == UART_433_REMOVE_SPI_MASTER_SS3_PIN)
#define UART_433_SPI_SLAVE_PINS         (0u == UART_433_REMOVE_SPI_SLAVE_PINS)
#define UART_433_SPI_SLAVE_MOSI_PIN     (0u == UART_433_REMOVE_SPI_SLAVE_MOSI_PIN)
#define UART_433_SPI_SLAVE_MISO_PIN     (0u == UART_433_REMOVE_SPI_SLAVE_MISO_PIN)
#define UART_433_UART_TX_PIN            (0u == UART_433_REMOVE_UART_TX_PIN)
#define UART_433_UART_RX_TX_PIN         (0u == UART_433_REMOVE_UART_RX_TX_PIN)
#define UART_433_UART_RX_PIN            (0u == UART_433_REMOVE_UART_RX_PIN)
#define UART_433_UART_RX_WAKE_PIN       (0u == UART_433_REMOVE_UART_RX_WAKE_PIN)
#define UART_433_UART_RTS_PIN           (0u == UART_433_REMOVE_UART_RTS_PIN)
#define UART_433_UART_CTS_PIN           (0u == UART_433_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (UART_433_RX_WAKE_SDA_MOSI_PIN)
    #include "UART_433_uart_rx_wake_i2c_sda_spi_mosi.h"
#endif /* (UART_433_RX_SDA_MOSI) */

#if (UART_433_RX_SDA_MOSI_PIN)
    #include "UART_433_uart_rx_i2c_sda_spi_mosi.h"
#endif /* (UART_433_RX_SDA_MOSI) */

#if (UART_433_TX_SCL_MISO_PIN)
    #include "UART_433_uart_tx_i2c_scl_spi_miso.h"
#endif /* (UART_433_TX_SCL_MISO) */

#if (UART_433_CTS_SCLK_PIN)
    #include "UART_433_uart_cts_spi_sclk.h"
#endif /* (UART_433_CTS_SCLK) */

#if (UART_433_RTS_SS0_PIN)
    #include "UART_433_uart_rts_spi_ss0.h"
#endif /* (UART_433_RTS_SS0_PIN) */

#if (UART_433_SS1_PIN)
    #include "UART_433_spi_ss1.h"
#endif /* (UART_433_SS1_PIN) */

#if (UART_433_SS2_PIN)
    #include "UART_433_spi_ss2.h"
#endif /* (UART_433_SS2_PIN) */

#if (UART_433_SS3_PIN)
    #include "UART_433_spi_ss3.h"
#endif /* (UART_433_SS3_PIN) */

#if (UART_433_I2C_PINS)
    #include "UART_433_scl.h"
    #include "UART_433_sda.h"
#endif /* (UART_433_I2C_PINS) */

#if (UART_433_SPI_MASTER_PINS)
#if (UART_433_SPI_MASTER_SCLK_PIN)
    #include "UART_433_sclk_m.h"
#endif /* (UART_433_SPI_MASTER_SCLK_PIN) */

#if (UART_433_SPI_MASTER_MOSI_PIN)
    #include "UART_433_mosi_m.h"
#endif /* (UART_433_SPI_MASTER_MOSI_PIN) */

#if (UART_433_SPI_MASTER_MISO_PIN)
    #include "UART_433_miso_m.h"
#endif /*(UART_433_SPI_MASTER_MISO_PIN) */
#endif /* (UART_433_SPI_MASTER_PINS) */

#if (UART_433_SPI_SLAVE_PINS)
    #include "UART_433_sclk_s.h"
    #include "UART_433_ss_s.h"

#if (UART_433_SPI_SLAVE_MOSI_PIN)
    #include "UART_433_mosi_s.h"
#endif /* (UART_433_SPI_SLAVE_MOSI_PIN) */

#if (UART_433_SPI_SLAVE_MISO_PIN)
    #include "UART_433_miso_s.h"
#endif /*(UART_433_SPI_SLAVE_MISO_PIN) */
#endif /* (UART_433_SPI_SLAVE_PINS) */

#if (UART_433_SPI_MASTER_SS0_PIN)
    #include "UART_433_ss0_m.h"
#endif /* (UART_433_SPI_MASTER_SS0_PIN) */

#if (UART_433_SPI_MASTER_SS1_PIN)
    #include "UART_433_ss1_m.h"
#endif /* (UART_433_SPI_MASTER_SS1_PIN) */

#if (UART_433_SPI_MASTER_SS2_PIN)
    #include "UART_433_ss2_m.h"
#endif /* (UART_433_SPI_MASTER_SS2_PIN) */

#if (UART_433_SPI_MASTER_SS3_PIN)
    #include "UART_433_ss3_m.h"
#endif /* (UART_433_SPI_MASTER_SS3_PIN) */

#if (UART_433_UART_TX_PIN)
    #include "UART_433_tx.h"
#endif /* (UART_433_UART_TX_PIN) */

#if (UART_433_UART_RX_TX_PIN)
    #include "UART_433_rx_tx.h"
#endif /* (UART_433_UART_RX_TX_PIN) */

#if (UART_433_UART_RX_PIN)
    #include "UART_433_rx.h"
#endif /* (UART_433_UART_RX_PIN) */

#if (UART_433_UART_RX_WAKE_PIN)
    #include "UART_433_rx_wake.h"
#endif /* (UART_433_UART_RX_WAKE_PIN) */

#if (UART_433_UART_RTS_PIN)
    #include "UART_433_rts.h"
#endif /* (UART_433_UART_RTS_PIN) */

#if (UART_433_UART_CTS_PIN)
    #include "UART_433_cts.h"
#endif /* (UART_433_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (UART_433_RX_SDA_MOSI_PIN)
    #define UART_433_RX_SDA_MOSI_HSIOM_REG   (*(reg32 *) UART_433_uart_rx_i2c_sda_spi_mosi__0__HSIOM)
    #define UART_433_RX_SDA_MOSI_HSIOM_PTR   ( (reg32 *) UART_433_uart_rx_i2c_sda_spi_mosi__0__HSIOM)
    
    #define UART_433_RX_SDA_MOSI_HSIOM_MASK      (UART_433_uart_rx_i2c_sda_spi_mosi__0__HSIOM_MASK)
    #define UART_433_RX_SDA_MOSI_HSIOM_POS       (UART_433_uart_rx_i2c_sda_spi_mosi__0__HSIOM_SHIFT)
    #define UART_433_RX_SDA_MOSI_HSIOM_SEL_GPIO  (UART_433_uart_rx_i2c_sda_spi_mosi__0__HSIOM_GPIO)
    #define UART_433_RX_SDA_MOSI_HSIOM_SEL_I2C   (UART_433_uart_rx_i2c_sda_spi_mosi__0__HSIOM_I2C)
    #define UART_433_RX_SDA_MOSI_HSIOM_SEL_SPI   (UART_433_uart_rx_i2c_sda_spi_mosi__0__HSIOM_SPI)
    #define UART_433_RX_SDA_MOSI_HSIOM_SEL_UART  (UART_433_uart_rx_i2c_sda_spi_mosi__0__HSIOM_UART)
    
#elif (UART_433_RX_WAKE_SDA_MOSI_PIN)
    #define UART_433_RX_WAKE_SDA_MOSI_HSIOM_REG   (*(reg32 *) UART_433_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM)
    #define UART_433_RX_WAKE_SDA_MOSI_HSIOM_PTR   ( (reg32 *) UART_433_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM)
    
    #define UART_433_RX_WAKE_SDA_MOSI_HSIOM_MASK      (UART_433_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_MASK)
    #define UART_433_RX_WAKE_SDA_MOSI_HSIOM_POS       (UART_433_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_SHIFT)
    #define UART_433_RX_WAKE_SDA_MOSI_HSIOM_SEL_GPIO  (UART_433_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_GPIO)
    #define UART_433_RX_WAKE_SDA_MOSI_HSIOM_SEL_I2C   (UART_433_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_I2C)
    #define UART_433_RX_WAKE_SDA_MOSI_HSIOM_SEL_SPI   (UART_433_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_SPI)
    #define UART_433_RX_WAKE_SDA_MOSI_HSIOM_SEL_UART  (UART_433_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_UART)    
   
    #define UART_433_RX_WAKE_SDA_MOSI_INTCFG_REG (*(reg32 *) UART_433_uart_rx_wake_i2c_sda_spi_mosi__0__INTCFG)
    #define UART_433_RX_WAKE_SDA_MOSI_INTCFG_PTR ( (reg32 *) UART_433_uart_rx_wake_i2c_sda_spi_mosi__0__INTCFG)
    #define UART_433_RX_WAKE_SDA_MOSI_INTCFG_TYPE_POS  (UART_433_uart_rx_wake_i2c_sda_spi_mosi__SHIFT)
    #define UART_433_RX_WAKE_SDA_MOSI_INTCFG_TYPE_MASK ((uint32) UART_433_INTCFG_TYPE_MASK << \
                                                                           UART_433_RX_WAKE_SDA_MOSI_INTCFG_TYPE_POS)
#else
    /* None of pins UART_433_RX_SDA_MOSI_PIN or UART_433_RX_WAKE_SDA_MOSI_PIN present.*/
#endif /* (UART_433_RX_SDA_MOSI_PIN) */

#if (UART_433_TX_SCL_MISO_PIN)
    #define UART_433_TX_SCL_MISO_HSIOM_REG   (*(reg32 *) UART_433_uart_tx_i2c_scl_spi_miso__0__HSIOM)
    #define UART_433_TX_SCL_MISO_HSIOM_PTR   ( (reg32 *) UART_433_uart_tx_i2c_scl_spi_miso__0__HSIOM)
    
    #define UART_433_TX_SCL_MISO_HSIOM_MASK      (UART_433_uart_tx_i2c_scl_spi_miso__0__HSIOM_MASK)
    #define UART_433_TX_SCL_MISO_HSIOM_POS       (UART_433_uart_tx_i2c_scl_spi_miso__0__HSIOM_SHIFT)
    #define UART_433_TX_SCL_MISO_HSIOM_SEL_GPIO  (UART_433_uart_tx_i2c_scl_spi_miso__0__HSIOM_GPIO)
    #define UART_433_TX_SCL_MISO_HSIOM_SEL_I2C   (UART_433_uart_tx_i2c_scl_spi_miso__0__HSIOM_I2C)
    #define UART_433_TX_SCL_MISO_HSIOM_SEL_SPI   (UART_433_uart_tx_i2c_scl_spi_miso__0__HSIOM_SPI)
    #define UART_433_TX_SCL_MISO_HSIOM_SEL_UART  (UART_433_uart_tx_i2c_scl_spi_miso__0__HSIOM_UART)
#endif /* (UART_433_TX_SCL_MISO_PIN) */

#if (UART_433_CTS_SCLK_PIN)
    #define UART_433_CTS_SCLK_HSIOM_REG   (*(reg32 *) UART_433_uart_cts_spi_sclk__0__HSIOM)
    #define UART_433_CTS_SCLK_HSIOM_PTR   ( (reg32 *) UART_433_uart_cts_spi_sclk__0__HSIOM)
    
    #define UART_433_CTS_SCLK_HSIOM_MASK      (UART_433_uart_cts_spi_sclk__0__HSIOM_MASK)
    #define UART_433_CTS_SCLK_HSIOM_POS       (UART_433_uart_cts_spi_sclk__0__HSIOM_SHIFT)
    #define UART_433_CTS_SCLK_HSIOM_SEL_GPIO  (UART_433_uart_cts_spi_sclk__0__HSIOM_GPIO)
    #define UART_433_CTS_SCLK_HSIOM_SEL_I2C   (UART_433_uart_cts_spi_sclk__0__HSIOM_I2C)
    #define UART_433_CTS_SCLK_HSIOM_SEL_SPI   (UART_433_uart_cts_spi_sclk__0__HSIOM_SPI)
    #define UART_433_CTS_SCLK_HSIOM_SEL_UART  (UART_433_uart_cts_spi_sclk__0__HSIOM_UART)
#endif /* (UART_433_CTS_SCLK_PIN) */

#if (UART_433_RTS_SS0_PIN)
    #define UART_433_RTS_SS0_HSIOM_REG   (*(reg32 *) UART_433_uart_rts_spi_ss0__0__HSIOM)
    #define UART_433_RTS_SS0_HSIOM_PTR   ( (reg32 *) UART_433_uart_rts_spi_ss0__0__HSIOM)
    
    #define UART_433_RTS_SS0_HSIOM_MASK      (UART_433_uart_rts_spi_ss0__0__HSIOM_MASK)
    #define UART_433_RTS_SS0_HSIOM_POS       (UART_433_uart_rts_spi_ss0__0__HSIOM_SHIFT)
    #define UART_433_RTS_SS0_HSIOM_SEL_GPIO  (UART_433_uart_rts_spi_ss0__0__HSIOM_GPIO)
    #define UART_433_RTS_SS0_HSIOM_SEL_I2C   (UART_433_uart_rts_spi_ss0__0__HSIOM_I2C)
    #define UART_433_RTS_SS0_HSIOM_SEL_SPI   (UART_433_uart_rts_spi_ss0__0__HSIOM_SPI)
#if !(UART_433_CY_SCBIP_V0 || UART_433_CY_SCBIP_V1)
    #define UART_433_RTS_SS0_HSIOM_SEL_UART  (UART_433_uart_rts_spi_ss0__0__HSIOM_UART)
#endif /* !(UART_433_CY_SCBIP_V0 || UART_433_CY_SCBIP_V1) */
#endif /* (UART_433_RTS_SS0_PIN) */

#if (UART_433_SS1_PIN)
    #define UART_433_SS1_HSIOM_REG  (*(reg32 *) UART_433_spi_ss1__0__HSIOM)
    #define UART_433_SS1_HSIOM_PTR  ( (reg32 *) UART_433_spi_ss1__0__HSIOM)
    
    #define UART_433_SS1_HSIOM_MASK     (UART_433_spi_ss1__0__HSIOM_MASK)
    #define UART_433_SS1_HSIOM_POS      (UART_433_spi_ss1__0__HSIOM_SHIFT)
    #define UART_433_SS1_HSIOM_SEL_GPIO (UART_433_spi_ss1__0__HSIOM_GPIO)
    #define UART_433_SS1_HSIOM_SEL_I2C  (UART_433_spi_ss1__0__HSIOM_I2C)
    #define UART_433_SS1_HSIOM_SEL_SPI  (UART_433_spi_ss1__0__HSIOM_SPI)
#endif /* (UART_433_SS1_PIN) */

#if (UART_433_SS2_PIN)
    #define UART_433_SS2_HSIOM_REG     (*(reg32 *) UART_433_spi_ss2__0__HSIOM)
    #define UART_433_SS2_HSIOM_PTR     ( (reg32 *) UART_433_spi_ss2__0__HSIOM)
    
    #define UART_433_SS2_HSIOM_MASK     (UART_433_spi_ss2__0__HSIOM_MASK)
    #define UART_433_SS2_HSIOM_POS      (UART_433_spi_ss2__0__HSIOM_SHIFT)
    #define UART_433_SS2_HSIOM_SEL_GPIO (UART_433_spi_ss2__0__HSIOM_GPIO)
    #define UART_433_SS2_HSIOM_SEL_I2C  (UART_433_spi_ss2__0__HSIOM_I2C)
    #define UART_433_SS2_HSIOM_SEL_SPI  (UART_433_spi_ss2__0__HSIOM_SPI)
#endif /* (UART_433_SS2_PIN) */

#if (UART_433_SS3_PIN)
    #define UART_433_SS3_HSIOM_REG     (*(reg32 *) UART_433_spi_ss3__0__HSIOM)
    #define UART_433_SS3_HSIOM_PTR     ( (reg32 *) UART_433_spi_ss3__0__HSIOM)
    
    #define UART_433_SS3_HSIOM_MASK     (UART_433_spi_ss3__0__HSIOM_MASK)
    #define UART_433_SS3_HSIOM_POS      (UART_433_spi_ss3__0__HSIOM_SHIFT)
    #define UART_433_SS3_HSIOM_SEL_GPIO (UART_433_spi_ss3__0__HSIOM_GPIO)
    #define UART_433_SS3_HSIOM_SEL_I2C  (UART_433_spi_ss3__0__HSIOM_I2C)
    #define UART_433_SS3_HSIOM_SEL_SPI  (UART_433_spi_ss3__0__HSIOM_SPI)
#endif /* (UART_433_SS3_PIN) */

#if (UART_433_I2C_PINS)
    #define UART_433_SCL_HSIOM_REG  (*(reg32 *) UART_433_scl__0__HSIOM)
    #define UART_433_SCL_HSIOM_PTR  ( (reg32 *) UART_433_scl__0__HSIOM)
    
    #define UART_433_SCL_HSIOM_MASK     (UART_433_scl__0__HSIOM_MASK)
    #define UART_433_SCL_HSIOM_POS      (UART_433_scl__0__HSIOM_SHIFT)
    #define UART_433_SCL_HSIOM_SEL_GPIO (UART_433_sda__0__HSIOM_GPIO)
    #define UART_433_SCL_HSIOM_SEL_I2C  (UART_433_sda__0__HSIOM_I2C)
    
    #define UART_433_SDA_HSIOM_REG  (*(reg32 *) UART_433_sda__0__HSIOM)
    #define UART_433_SDA_HSIOM_PTR  ( (reg32 *) UART_433_sda__0__HSIOM)
    
    #define UART_433_SDA_HSIOM_MASK     (UART_433_sda__0__HSIOM_MASK)
    #define UART_433_SDA_HSIOM_POS      (UART_433_sda__0__HSIOM_SHIFT)
    #define UART_433_SDA_HSIOM_SEL_GPIO (UART_433_sda__0__HSIOM_GPIO)
    #define UART_433_SDA_HSIOM_SEL_I2C  (UART_433_sda__0__HSIOM_I2C)
#endif /* (UART_433_I2C_PINS) */

#if (UART_433_SPI_SLAVE_PINS)
    #define UART_433_SCLK_S_HSIOM_REG   (*(reg32 *) UART_433_sclk_s__0__HSIOM)
    #define UART_433_SCLK_S_HSIOM_PTR   ( (reg32 *) UART_433_sclk_s__0__HSIOM)
    
    #define UART_433_SCLK_S_HSIOM_MASK      (UART_433_sclk_s__0__HSIOM_MASK)
    #define UART_433_SCLK_S_HSIOM_POS       (UART_433_sclk_s__0__HSIOM_SHIFT)
    #define UART_433_SCLK_S_HSIOM_SEL_GPIO  (UART_433_sclk_s__0__HSIOM_GPIO)
    #define UART_433_SCLK_S_HSIOM_SEL_SPI   (UART_433_sclk_s__0__HSIOM_SPI)
    
    #define UART_433_SS0_S_HSIOM_REG    (*(reg32 *) UART_433_ss0_s__0__HSIOM)
    #define UART_433_SS0_S_HSIOM_PTR    ( (reg32 *) UART_433_ss0_s__0__HSIOM)
    
    #define UART_433_SS0_S_HSIOM_MASK       (UART_433_ss0_s__0__HSIOM_MASK)
    #define UART_433_SS0_S_HSIOM_POS        (UART_433_ss0_s__0__HSIOM_SHIFT)
    #define UART_433_SS0_S_HSIOM_SEL_GPIO   (UART_433_ss0_s__0__HSIOM_GPIO)  
    #define UART_433_SS0_S_HSIOM_SEL_SPI    (UART_433_ss0_s__0__HSIOM_SPI)
#endif /* (UART_433_SPI_SLAVE_PINS) */

#if (UART_433_SPI_SLAVE_MOSI_PIN)
    #define UART_433_MOSI_S_HSIOM_REG   (*(reg32 *) UART_433_mosi_s__0__HSIOM)
    #define UART_433_MOSI_S_HSIOM_PTR   ( (reg32 *) UART_433_mosi_s__0__HSIOM)
    
    #define UART_433_MOSI_S_HSIOM_MASK      (UART_433_mosi_s__0__HSIOM_MASK)
    #define UART_433_MOSI_S_HSIOM_POS       (UART_433_mosi_s__0__HSIOM_SHIFT)
    #define UART_433_MOSI_S_HSIOM_SEL_GPIO  (UART_433_mosi_s__0__HSIOM_GPIO)
    #define UART_433_MOSI_S_HSIOM_SEL_SPI   (UART_433_mosi_s__0__HSIOM_SPI)
#endif /* (UART_433_SPI_SLAVE_MOSI_PIN) */

#if (UART_433_SPI_SLAVE_MISO_PIN)
    #define UART_433_MISO_S_HSIOM_REG   (*(reg32 *) UART_433_miso_s__0__HSIOM)
    #define UART_433_MISO_S_HSIOM_PTR   ( (reg32 *) UART_433_miso_s__0__HSIOM)
    
    #define UART_433_MISO_S_HSIOM_MASK      (UART_433_miso_s__0__HSIOM_MASK)
    #define UART_433_MISO_S_HSIOM_POS       (UART_433_miso_s__0__HSIOM_SHIFT)
    #define UART_433_MISO_S_HSIOM_SEL_GPIO  (UART_433_miso_s__0__HSIOM_GPIO)
    #define UART_433_MISO_S_HSIOM_SEL_SPI   (UART_433_miso_s__0__HSIOM_SPI)
#endif /* (UART_433_SPI_SLAVE_MISO_PIN) */

#if (UART_433_SPI_MASTER_MISO_PIN)
    #define UART_433_MISO_M_HSIOM_REG   (*(reg32 *) UART_433_miso_m__0__HSIOM)
    #define UART_433_MISO_M_HSIOM_PTR   ( (reg32 *) UART_433_miso_m__0__HSIOM)
    
    #define UART_433_MISO_M_HSIOM_MASK      (UART_433_miso_m__0__HSIOM_MASK)
    #define UART_433_MISO_M_HSIOM_POS       (UART_433_miso_m__0__HSIOM_SHIFT)
    #define UART_433_MISO_M_HSIOM_SEL_GPIO  (UART_433_miso_m__0__HSIOM_GPIO)
    #define UART_433_MISO_M_HSIOM_SEL_SPI   (UART_433_miso_m__0__HSIOM_SPI)
#endif /* (UART_433_SPI_MASTER_MISO_PIN) */

#if (UART_433_SPI_MASTER_MOSI_PIN)
    #define UART_433_MOSI_M_HSIOM_REG   (*(reg32 *) UART_433_mosi_m__0__HSIOM)
    #define UART_433_MOSI_M_HSIOM_PTR   ( (reg32 *) UART_433_mosi_m__0__HSIOM)
    
    #define UART_433_MOSI_M_HSIOM_MASK      (UART_433_mosi_m__0__HSIOM_MASK)
    #define UART_433_MOSI_M_HSIOM_POS       (UART_433_mosi_m__0__HSIOM_SHIFT)
    #define UART_433_MOSI_M_HSIOM_SEL_GPIO  (UART_433_mosi_m__0__HSIOM_GPIO)
    #define UART_433_MOSI_M_HSIOM_SEL_SPI   (UART_433_mosi_m__0__HSIOM_SPI)
#endif /* (UART_433_SPI_MASTER_MOSI_PIN) */

#if (UART_433_SPI_MASTER_SCLK_PIN)
    #define UART_433_SCLK_M_HSIOM_REG   (*(reg32 *) UART_433_sclk_m__0__HSIOM)
    #define UART_433_SCLK_M_HSIOM_PTR   ( (reg32 *) UART_433_sclk_m__0__HSIOM)
    
    #define UART_433_SCLK_M_HSIOM_MASK      (UART_433_sclk_m__0__HSIOM_MASK)
    #define UART_433_SCLK_M_HSIOM_POS       (UART_433_sclk_m__0__HSIOM_SHIFT)
    #define UART_433_SCLK_M_HSIOM_SEL_GPIO  (UART_433_sclk_m__0__HSIOM_GPIO)
    #define UART_433_SCLK_M_HSIOM_SEL_SPI   (UART_433_sclk_m__0__HSIOM_SPI)
#endif /* (UART_433_SPI_MASTER_SCLK_PIN) */

#if (UART_433_SPI_MASTER_SS0_PIN)
    #define UART_433_SS0_M_HSIOM_REG    (*(reg32 *) UART_433_ss0_m__0__HSIOM)
    #define UART_433_SS0_M_HSIOM_PTR    ( (reg32 *) UART_433_ss0_m__0__HSIOM)
    
    #define UART_433_SS0_M_HSIOM_MASK       (UART_433_ss0_m__0__HSIOM_MASK)
    #define UART_433_SS0_M_HSIOM_POS        (UART_433_ss0_m__0__HSIOM_SHIFT)
    #define UART_433_SS0_M_HSIOM_SEL_GPIO   (UART_433_ss0_m__0__HSIOM_GPIO)
    #define UART_433_SS0_M_HSIOM_SEL_SPI    (UART_433_ss0_m__0__HSIOM_SPI)
#endif /* (UART_433_SPI_MASTER_SS0_PIN) */

#if (UART_433_SPI_MASTER_SS1_PIN)
    #define UART_433_SS1_M_HSIOM_REG    (*(reg32 *) UART_433_ss1_m__0__HSIOM)
    #define UART_433_SS1_M_HSIOM_PTR    ( (reg32 *) UART_433_ss1_m__0__HSIOM)
    
    #define UART_433_SS1_M_HSIOM_MASK       (UART_433_ss1_m__0__HSIOM_MASK)
    #define UART_433_SS1_M_HSIOM_POS        (UART_433_ss1_m__0__HSIOM_SHIFT)
    #define UART_433_SS1_M_HSIOM_SEL_GPIO   (UART_433_ss1_m__0__HSIOM_GPIO)
    #define UART_433_SS1_M_HSIOM_SEL_SPI    (UART_433_ss1_m__0__HSIOM_SPI)
#endif /* (UART_433_SPI_MASTER_SS1_PIN) */

#if (UART_433_SPI_MASTER_SS2_PIN)
    #define UART_433_SS2_M_HSIOM_REG    (*(reg32 *) UART_433_ss2_m__0__HSIOM)
    #define UART_433_SS2_M_HSIOM_PTR    ( (reg32 *) UART_433_ss2_m__0__HSIOM)
    
    #define UART_433_SS2_M_HSIOM_MASK       (UART_433_ss2_m__0__HSIOM_MASK)
    #define UART_433_SS2_M_HSIOM_POS        (UART_433_ss2_m__0__HSIOM_SHIFT)
    #define UART_433_SS2_M_HSIOM_SEL_GPIO   (UART_433_ss2_m__0__HSIOM_GPIO)
    #define UART_433_SS2_M_HSIOM_SEL_SPI    (UART_433_ss2_m__0__HSIOM_SPI)
#endif /* (UART_433_SPI_MASTER_SS2_PIN) */

#if (UART_433_SPI_MASTER_SS3_PIN)
    #define UART_433_SS3_M_HSIOM_REG    (*(reg32 *) UART_433_ss3_m__0__HSIOM)
    #define UART_433_SS3_M_HSIOM_PTR    ( (reg32 *) UART_433_ss3_m__0__HSIOM)
    
    #define UART_433_SS3_M_HSIOM_MASK      (UART_433_ss3_m__0__HSIOM_MASK)
    #define UART_433_SS3_M_HSIOM_POS       (UART_433_ss3_m__0__HSIOM_SHIFT)
    #define UART_433_SS3_M_HSIOM_SEL_GPIO  (UART_433_ss3_m__0__HSIOM_GPIO)
    #define UART_433_SS3_M_HSIOM_SEL_SPI   (UART_433_ss3_m__0__HSIOM_SPI)
#endif /* (UART_433_SPI_MASTER_SS3_PIN) */

#if (UART_433_UART_RX_PIN)
    #define UART_433_RX_HSIOM_REG   (*(reg32 *) UART_433_rx__0__HSIOM)
    #define UART_433_RX_HSIOM_PTR   ( (reg32 *) UART_433_rx__0__HSIOM)
    
    #define UART_433_RX_HSIOM_MASK      (UART_433_rx__0__HSIOM_MASK)
    #define UART_433_RX_HSIOM_POS       (UART_433_rx__0__HSIOM_SHIFT)
    #define UART_433_RX_HSIOM_SEL_GPIO  (UART_433_rx__0__HSIOM_GPIO)
    #define UART_433_RX_HSIOM_SEL_UART  (UART_433_rx__0__HSIOM_UART)
#endif /* (UART_433_UART_RX_PIN) */

#if (UART_433_UART_RX_WAKE_PIN)
    #define UART_433_RX_WAKE_HSIOM_REG   (*(reg32 *) UART_433_rx_wake__0__HSIOM)
    #define UART_433_RX_WAKE_HSIOM_PTR   ( (reg32 *) UART_433_rx_wake__0__HSIOM)
    
    #define UART_433_RX_WAKE_HSIOM_MASK      (UART_433_rx_wake__0__HSIOM_MASK)
    #define UART_433_RX_WAKE_HSIOM_POS       (UART_433_rx_wake__0__HSIOM_SHIFT)
    #define UART_433_RX_WAKE_HSIOM_SEL_GPIO  (UART_433_rx_wake__0__HSIOM_GPIO)
    #define UART_433_RX_WAKE_HSIOM_SEL_UART  (UART_433_rx_wake__0__HSIOM_UART)
#endif /* (UART_433_UART_WAKE_RX_PIN) */

#if (UART_433_UART_CTS_PIN)
    #define UART_433_CTS_HSIOM_REG   (*(reg32 *) UART_433_cts__0__HSIOM)
    #define UART_433_CTS_HSIOM_PTR   ( (reg32 *) UART_433_cts__0__HSIOM)
    
    #define UART_433_CTS_HSIOM_MASK      (UART_433_cts__0__HSIOM_MASK)
    #define UART_433_CTS_HSIOM_POS       (UART_433_cts__0__HSIOM_SHIFT)
    #define UART_433_CTS_HSIOM_SEL_GPIO  (UART_433_cts__0__HSIOM_GPIO)
    #define UART_433_CTS_HSIOM_SEL_UART  (UART_433_cts__0__HSIOM_UART)
#endif /* (UART_433_UART_CTS_PIN) */

#if (UART_433_UART_TX_PIN)
    #define UART_433_TX_HSIOM_REG   (*(reg32 *) UART_433_tx__0__HSIOM)
    #define UART_433_TX_HSIOM_PTR   ( (reg32 *) UART_433_tx__0__HSIOM)
    
    #define UART_433_TX_HSIOM_MASK      (UART_433_tx__0__HSIOM_MASK)
    #define UART_433_TX_HSIOM_POS       (UART_433_tx__0__HSIOM_SHIFT)
    #define UART_433_TX_HSIOM_SEL_GPIO  (UART_433_tx__0__HSIOM_GPIO)
    #define UART_433_TX_HSIOM_SEL_UART  (UART_433_tx__0__HSIOM_UART)
#endif /* (UART_433_UART_TX_PIN) */

#if (UART_433_UART_RX_TX_PIN)
    #define UART_433_RX_TX_HSIOM_REG   (*(reg32 *) UART_433_rx_tx__0__HSIOM)
    #define UART_433_RX_TX_HSIOM_PTR   ( (reg32 *) UART_433_rx_tx__0__HSIOM)
    
    #define UART_433_RX_TX_HSIOM_MASK      (UART_433_rx_tx__0__HSIOM_MASK)
    #define UART_433_RX_TX_HSIOM_POS       (UART_433_rx_tx__0__HSIOM_SHIFT)
    #define UART_433_RX_TX_HSIOM_SEL_GPIO  (UART_433_rx_tx__0__HSIOM_GPIO)
    #define UART_433_RX_TX_HSIOM_SEL_UART  (UART_433_rx_tx__0__HSIOM_UART)
#endif /* (UART_433_UART_RX_TX_PIN) */

#if (UART_433_UART_RTS_PIN)
    #define UART_433_RTS_HSIOM_REG      (*(reg32 *) UART_433_rts__0__HSIOM)
    #define UART_433_RTS_HSIOM_PTR      ( (reg32 *) UART_433_rts__0__HSIOM)
    
    #define UART_433_RTS_HSIOM_MASK     (UART_433_rts__0__HSIOM_MASK)
    #define UART_433_RTS_HSIOM_POS      (UART_433_rts__0__HSIOM_SHIFT)    
    #define UART_433_RTS_HSIOM_SEL_GPIO (UART_433_rts__0__HSIOM_GPIO)
    #define UART_433_RTS_HSIOM_SEL_UART (UART_433_rts__0__HSIOM_UART)    
#endif /* (UART_433_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* HSIOM switch values. */ 
#define UART_433_HSIOM_DEF_SEL      (0x00u)
#define UART_433_HSIOM_GPIO_SEL     (0x00u)
/* The HSIOM values provided below are valid only for UART_433_CY_SCBIP_V0 
* and UART_433_CY_SCBIP_V1. It is not recommended to use them for 
* UART_433_CY_SCBIP_V2. Use pin name specific HSIOM constants provided 
* above instead for any SCB IP block version.
*/
#define UART_433_HSIOM_UART_SEL     (0x09u)
#define UART_433_HSIOM_I2C_SEL      (0x0Eu)
#define UART_433_HSIOM_SPI_SEL      (0x0Fu)

/* Pins settings index. */
#define UART_433_RX_WAKE_SDA_MOSI_PIN_INDEX   (0u)
#define UART_433_RX_SDA_MOSI_PIN_INDEX       (0u)
#define UART_433_TX_SCL_MISO_PIN_INDEX       (1u)
#define UART_433_CTS_SCLK_PIN_INDEX       (2u)
#define UART_433_RTS_SS0_PIN_INDEX       (3u)
#define UART_433_SS1_PIN_INDEX                  (4u)
#define UART_433_SS2_PIN_INDEX                  (5u)
#define UART_433_SS3_PIN_INDEX                  (6u)

/* Pins settings mask. */
#define UART_433_RX_WAKE_SDA_MOSI_PIN_MASK ((uint32) 0x01u << UART_433_RX_WAKE_SDA_MOSI_PIN_INDEX)
#define UART_433_RX_SDA_MOSI_PIN_MASK     ((uint32) 0x01u << UART_433_RX_SDA_MOSI_PIN_INDEX)
#define UART_433_TX_SCL_MISO_PIN_MASK     ((uint32) 0x01u << UART_433_TX_SCL_MISO_PIN_INDEX)
#define UART_433_CTS_SCLK_PIN_MASK     ((uint32) 0x01u << UART_433_CTS_SCLK_PIN_INDEX)
#define UART_433_RTS_SS0_PIN_MASK     ((uint32) 0x01u << UART_433_RTS_SS0_PIN_INDEX)
#define UART_433_SS1_PIN_MASK                ((uint32) 0x01u << UART_433_SS1_PIN_INDEX)
#define UART_433_SS2_PIN_MASK                ((uint32) 0x01u << UART_433_SS2_PIN_INDEX)
#define UART_433_SS3_PIN_MASK                ((uint32) 0x01u << UART_433_SS3_PIN_INDEX)

/* Pin interrupt constants. */
#define UART_433_INTCFG_TYPE_MASK           (0x03u)
#define UART_433_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants. */
#define UART_433_PIN_DM_ALG_HIZ  (0u)
#define UART_433_PIN_DM_DIG_HIZ  (1u)
#define UART_433_PIN_DM_OD_LO    (4u)
#define UART_433_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define UART_433_DM_MASK    (0x7u)
#define UART_433_DM_SIZE    (3u)
#define UART_433_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) UART_433_DM_MASK << (UART_433_DM_SIZE * (pos)))) >> \
                                                              (UART_433_DM_SIZE * (pos)) )

#if (UART_433_TX_SCL_MISO_PIN)
    #define UART_433_CHECK_TX_SCL_MISO_PIN_USED \
                (UART_433_PIN_DM_ALG_HIZ != \
                    UART_433_GET_P4_PIN_DM(UART_433_uart_tx_i2c_scl_spi_miso_PC, \
                                                   UART_433_uart_tx_i2c_scl_spi_miso_SHIFT))
#endif /* (UART_433_TX_SCL_MISO_PIN) */

#if (UART_433_RTS_SS0_PIN)
    #define UART_433_CHECK_RTS_SS0_PIN_USED \
                (UART_433_PIN_DM_ALG_HIZ != \
                    UART_433_GET_P4_PIN_DM(UART_433_uart_rts_spi_ss0_PC, \
                                                   UART_433_uart_rts_spi_ss0_SHIFT))
#endif /* (UART_433_RTS_SS0_PIN) */

/* Set bits-mask in register */
#define UART_433_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define UART_433_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define UART_433_SET_HSIOM_SEL(reg, mask, pos, sel) UART_433_SET_REGISTER_BITS(reg, mask, pos, sel)
#define UART_433_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        UART_433_SET_REGISTER_BITS(reg, mask, pos, intType)
#define UART_433_SET_INP_DIS(reg, mask, val) UART_433_SET_REGISTER_BIT(reg, mask, val)

/* UART_433_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  UART_433_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (UART_433_CY_SCBIP_V0)
#if (UART_433_I2C_PINS)
    #define UART_433_SET_I2C_SCL_DR(val) UART_433_scl_Write(val)

    #define UART_433_SET_I2C_SCL_HSIOM_SEL(sel) \
                          UART_433_SET_HSIOM_SEL(UART_433_SCL_HSIOM_REG,  \
                                                         UART_433_SCL_HSIOM_MASK, \
                                                         UART_433_SCL_HSIOM_POS,  \
                                                         (sel))
    #define UART_433_WAIT_SCL_SET_HIGH  (0u == UART_433_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (UART_433_RX_WAKE_SDA_MOSI_PIN)
    #define UART_433_SET_I2C_SCL_DR(val) \
                            UART_433_uart_rx_wake_i2c_sda_spi_mosi_Write(val)

    #define UART_433_SET_I2C_SCL_HSIOM_SEL(sel) \
                    UART_433_SET_HSIOM_SEL(UART_433_RX_WAKE_SDA_MOSI_HSIOM_REG,  \
                                                   UART_433_RX_WAKE_SDA_MOSI_HSIOM_MASK, \
                                                   UART_433_RX_WAKE_SDA_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define UART_433_WAIT_SCL_SET_HIGH  (0u == UART_433_uart_rx_wake_i2c_sda_spi_mosi_Read())

#elif (UART_433_RX_SDA_MOSI_PIN)
    #define UART_433_SET_I2C_SCL_DR(val) \
                            UART_433_uart_rx_i2c_sda_spi_mosi_Write(val)


    #define UART_433_SET_I2C_SCL_HSIOM_SEL(sel) \
                            UART_433_SET_HSIOM_SEL(UART_433_RX_SDA_MOSI_HSIOM_REG,  \
                                                           UART_433_RX_SDA_MOSI_HSIOM_MASK, \
                                                           UART_433_RX_SDA_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define UART_433_WAIT_SCL_SET_HIGH  (0u == UART_433_uart_rx_i2c_sda_spi_mosi_Read())

#else
    #define UART_433_SET_I2C_SCL_DR(val)        do{ /* Does nothing */ }while(0)
    #define UART_433_SET_I2C_SCL_HSIOM_SEL(sel) do{ /* Does nothing */ }while(0)

    #define UART_433_WAIT_SCL_SET_HIGH  (0u)
#endif /* (UART_433_I2C_PINS) */

/* SCB I2C: sda signal */
#if (UART_433_I2C_PINS)
    #define UART_433_WAIT_SDA_SET_HIGH  (0u == UART_433_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (UART_433_TX_SCL_MISO_PIN)
    #define UART_433_WAIT_SDA_SET_HIGH  (0u == UART_433_uart_tx_i2c_scl_spi_miso_Read())
#else
    #define UART_433_WAIT_SDA_SET_HIGH  (0u)
#endif /* (UART_433_MOSI_SCL_RX_PIN) */
#endif /* (UART_433_CY_SCBIP_V0) */

/* Clear UART wakeup source */
#if (UART_433_RX_SDA_MOSI_PIN)
    #define UART_433_CLEAR_UART_RX_WAKE_INTR        do{ /* Does nothing */ }while(0)
    
#elif (UART_433_RX_WAKE_SDA_MOSI_PIN)
    #define UART_433_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) UART_433_uart_rx_wake_i2c_sda_spi_mosi_ClearInterrupt(); \
            }while(0)

#elif(UART_433_UART_RX_WAKE_PIN)
    #define UART_433_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) UART_433_rx_wake_ClearInterrupt(); \
            }while(0)
#else
#endif /* (UART_433_RX_SDA_MOSI_PIN) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define UART_433_REMOVE_MOSI_SCL_RX_WAKE_PIN    UART_433_REMOVE_RX_WAKE_SDA_MOSI_PIN
#define UART_433_REMOVE_MOSI_SCL_RX_PIN         UART_433_REMOVE_RX_SDA_MOSI_PIN
#define UART_433_REMOVE_MISO_SDA_TX_PIN         UART_433_REMOVE_TX_SCL_MISO_PIN
#ifndef UART_433_REMOVE_SCLK_PIN
#define UART_433_REMOVE_SCLK_PIN                UART_433_REMOVE_CTS_SCLK_PIN
#endif /* UART_433_REMOVE_SCLK_PIN */
#ifndef UART_433_REMOVE_SS0_PIN
#define UART_433_REMOVE_SS0_PIN                 UART_433_REMOVE_RTS_SS0_PIN
#endif /* UART_433_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define UART_433_MOSI_SCL_RX_WAKE_PIN   UART_433_RX_WAKE_SDA_MOSI_PIN
#define UART_433_MOSI_SCL_RX_PIN        UART_433_RX_SDA_MOSI_PIN
#define UART_433_MISO_SDA_TX_PIN        UART_433_TX_SCL_MISO_PIN
#ifndef UART_433_SCLK_PIN
#define UART_433_SCLK_PIN               UART_433_CTS_SCLK_PIN
#endif /* UART_433_SCLK_PIN */
#ifndef UART_433_SS0_PIN
#define UART_433_SS0_PIN                UART_433_RTS_SS0_PIN
#endif /* UART_433_SS0_PIN */

#if (UART_433_MOSI_SCL_RX_WAKE_PIN)
    #define UART_433_MOSI_SCL_RX_WAKE_HSIOM_REG     UART_433_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define UART_433_MOSI_SCL_RX_WAKE_HSIOM_PTR     UART_433_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define UART_433_MOSI_SCL_RX_WAKE_HSIOM_MASK    UART_433_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define UART_433_MOSI_SCL_RX_WAKE_HSIOM_POS     UART_433_RX_WAKE_SDA_MOSI_HSIOM_REG

    #define UART_433_MOSI_SCL_RX_WAKE_INTCFG_REG    UART_433_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define UART_433_MOSI_SCL_RX_WAKE_INTCFG_PTR    UART_433_RX_WAKE_SDA_MOSI_HSIOM_REG

    #define UART_433_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   UART_433_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define UART_433_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  UART_433_RX_WAKE_SDA_MOSI_HSIOM_REG
#endif /* (UART_433_RX_WAKE_SDA_MOSI_PIN) */

#if (UART_433_MOSI_SCL_RX_PIN)
    #define UART_433_MOSI_SCL_RX_HSIOM_REG      UART_433_RX_SDA_MOSI_HSIOM_REG
    #define UART_433_MOSI_SCL_RX_HSIOM_PTR      UART_433_RX_SDA_MOSI_HSIOM_PTR
    #define UART_433_MOSI_SCL_RX_HSIOM_MASK     UART_433_RX_SDA_MOSI_HSIOM_MASK
    #define UART_433_MOSI_SCL_RX_HSIOM_POS      UART_433_RX_SDA_MOSI_HSIOM_POS
#endif /* (UART_433_MOSI_SCL_RX_PIN) */

#if (UART_433_MISO_SDA_TX_PIN)
    #define UART_433_MISO_SDA_TX_HSIOM_REG      UART_433_TX_SCL_MISO_HSIOM_REG
    #define UART_433_MISO_SDA_TX_HSIOM_PTR      UART_433_TX_SCL_MISO_HSIOM_REG
    #define UART_433_MISO_SDA_TX_HSIOM_MASK     UART_433_TX_SCL_MISO_HSIOM_REG
    #define UART_433_MISO_SDA_TX_HSIOM_POS      UART_433_TX_SCL_MISO_HSIOM_REG
#endif /* (UART_433_MISO_SDA_TX_PIN_PIN) */

#if (UART_433_SCLK_PIN)
    #ifndef UART_433_SCLK_HSIOM_REG
    #define UART_433_SCLK_HSIOM_REG     UART_433_CTS_SCLK_HSIOM_REG
    #define UART_433_SCLK_HSIOM_PTR     UART_433_CTS_SCLK_HSIOM_PTR
    #define UART_433_SCLK_HSIOM_MASK    UART_433_CTS_SCLK_HSIOM_MASK
    #define UART_433_SCLK_HSIOM_POS     UART_433_CTS_SCLK_HSIOM_POS
    #endif /* UART_433_SCLK_HSIOM_REG */
#endif /* (UART_433_SCLK_PIN) */

#if (UART_433_SS0_PIN)
    #ifndef UART_433_SS0_HSIOM_REG
    #define UART_433_SS0_HSIOM_REG      UART_433_RTS_SS0_HSIOM_REG
    #define UART_433_SS0_HSIOM_PTR      UART_433_RTS_SS0_HSIOM_PTR
    #define UART_433_SS0_HSIOM_MASK     UART_433_RTS_SS0_HSIOM_MASK
    #define UART_433_SS0_HSIOM_POS      UART_433_RTS_SS0_HSIOM_POS
    #endif /* UART_433_SS0_HSIOM_REG */
#endif /* (UART_433_SS0_PIN) */

#define UART_433_MOSI_SCL_RX_WAKE_PIN_INDEX UART_433_RX_WAKE_SDA_MOSI_PIN_INDEX
#define UART_433_MOSI_SCL_RX_PIN_INDEX      UART_433_RX_SDA_MOSI_PIN_INDEX
#define UART_433_MISO_SDA_TX_PIN_INDEX      UART_433_TX_SCL_MISO_PIN_INDEX
#ifndef UART_433_SCLK_PIN_INDEX
#define UART_433_SCLK_PIN_INDEX             UART_433_CTS_SCLK_PIN_INDEX
#endif /* UART_433_SCLK_PIN_INDEX */
#ifndef UART_433_SS0_PIN_INDEX
#define UART_433_SS0_PIN_INDEX              UART_433_RTS_SS0_PIN_INDEX
#endif /* UART_433_SS0_PIN_INDEX */

#define UART_433_MOSI_SCL_RX_WAKE_PIN_MASK UART_433_RX_WAKE_SDA_MOSI_PIN_MASK
#define UART_433_MOSI_SCL_RX_PIN_MASK      UART_433_RX_SDA_MOSI_PIN_MASK
#define UART_433_MISO_SDA_TX_PIN_MASK      UART_433_TX_SCL_MISO_PIN_MASK
#ifndef UART_433_SCLK_PIN_MASK
#define UART_433_SCLK_PIN_MASK             UART_433_CTS_SCLK_PIN_MASK
#endif /* UART_433_SCLK_PIN_MASK */
#ifndef UART_433_SS0_PIN_MASK
#define UART_433_SS0_PIN_MASK              UART_433_RTS_SS0_PIN_MASK
#endif /* UART_433_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_UART_433_H) */


/* [] END OF FILE */
