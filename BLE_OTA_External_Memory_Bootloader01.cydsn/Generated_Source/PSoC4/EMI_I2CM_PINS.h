/***************************************************************************//**
* \file EMI_I2CM_PINS.h
* \version 3.20
*
* \brief
*  This file provides constants and parameter values for the pin components
*  buried into SCB Component.
*
* Note:
*
********************************************************************************
* \copyright
* Copyright 2013-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_SCB_PINS_EMI_I2CM_H)
#define CY_SCB_PINS_EMI_I2CM_H

#include "cydevice_trm.h"
#include "cyfitter.h"
#include "cytypes.h"


/***************************************
*   Conditional Compilation Parameters
****************************************/

/* Unconfigured pins */
#define EMI_I2CM_REMOVE_RX_WAKE_SDA_MOSI_PIN  (1u)
#define EMI_I2CM_REMOVE_RX_SDA_MOSI_PIN      (1u)
#define EMI_I2CM_REMOVE_TX_SCL_MISO_PIN      (1u)
#define EMI_I2CM_REMOVE_CTS_SCLK_PIN      (1u)
#define EMI_I2CM_REMOVE_RTS_SS0_PIN      (1u)
#define EMI_I2CM_REMOVE_SS1_PIN                 (1u)
#define EMI_I2CM_REMOVE_SS2_PIN                 (1u)
#define EMI_I2CM_REMOVE_SS3_PIN                 (1u)

/* Mode defined pins */
#define EMI_I2CM_REMOVE_I2C_PINS                (0u)
#define EMI_I2CM_REMOVE_SPI_MASTER_PINS         (1u)
#define EMI_I2CM_REMOVE_SPI_MASTER_SCLK_PIN     (1u)
#define EMI_I2CM_REMOVE_SPI_MASTER_MOSI_PIN     (1u)
#define EMI_I2CM_REMOVE_SPI_MASTER_MISO_PIN     (1u)
#define EMI_I2CM_REMOVE_SPI_MASTER_SS0_PIN      (1u)
#define EMI_I2CM_REMOVE_SPI_MASTER_SS1_PIN      (1u)
#define EMI_I2CM_REMOVE_SPI_MASTER_SS2_PIN      (1u)
#define EMI_I2CM_REMOVE_SPI_MASTER_SS3_PIN      (1u)
#define EMI_I2CM_REMOVE_SPI_SLAVE_PINS          (1u)
#define EMI_I2CM_REMOVE_SPI_SLAVE_MOSI_PIN      (1u)
#define EMI_I2CM_REMOVE_SPI_SLAVE_MISO_PIN      (1u)
#define EMI_I2CM_REMOVE_UART_TX_PIN             (1u)
#define EMI_I2CM_REMOVE_UART_RX_TX_PIN          (1u)
#define EMI_I2CM_REMOVE_UART_RX_PIN             (1u)
#define EMI_I2CM_REMOVE_UART_RX_WAKE_PIN        (1u)
#define EMI_I2CM_REMOVE_UART_RTS_PIN            (1u)
#define EMI_I2CM_REMOVE_UART_CTS_PIN            (1u)

/* Unconfigured pins */
#define EMI_I2CM_RX_WAKE_SDA_MOSI_PIN (0u == EMI_I2CM_REMOVE_RX_WAKE_SDA_MOSI_PIN)
#define EMI_I2CM_RX_SDA_MOSI_PIN     (0u == EMI_I2CM_REMOVE_RX_SDA_MOSI_PIN)
#define EMI_I2CM_TX_SCL_MISO_PIN     (0u == EMI_I2CM_REMOVE_TX_SCL_MISO_PIN)
#define EMI_I2CM_CTS_SCLK_PIN     (0u == EMI_I2CM_REMOVE_CTS_SCLK_PIN)
#define EMI_I2CM_RTS_SS0_PIN     (0u == EMI_I2CM_REMOVE_RTS_SS0_PIN)
#define EMI_I2CM_SS1_PIN                (0u == EMI_I2CM_REMOVE_SS1_PIN)
#define EMI_I2CM_SS2_PIN                (0u == EMI_I2CM_REMOVE_SS2_PIN)
#define EMI_I2CM_SS3_PIN                (0u == EMI_I2CM_REMOVE_SS3_PIN)

/* Mode defined pins */
#define EMI_I2CM_I2C_PINS               (0u == EMI_I2CM_REMOVE_I2C_PINS)
#define EMI_I2CM_SPI_MASTER_PINS        (0u == EMI_I2CM_REMOVE_SPI_MASTER_PINS)
#define EMI_I2CM_SPI_MASTER_SCLK_PIN    (0u == EMI_I2CM_REMOVE_SPI_MASTER_SCLK_PIN)
#define EMI_I2CM_SPI_MASTER_MOSI_PIN    (0u == EMI_I2CM_REMOVE_SPI_MASTER_MOSI_PIN)
#define EMI_I2CM_SPI_MASTER_MISO_PIN    (0u == EMI_I2CM_REMOVE_SPI_MASTER_MISO_PIN)
#define EMI_I2CM_SPI_MASTER_SS0_PIN     (0u == EMI_I2CM_REMOVE_SPI_MASTER_SS0_PIN)
#define EMI_I2CM_SPI_MASTER_SS1_PIN     (0u == EMI_I2CM_REMOVE_SPI_MASTER_SS1_PIN)
#define EMI_I2CM_SPI_MASTER_SS2_PIN     (0u == EMI_I2CM_REMOVE_SPI_MASTER_SS2_PIN)
#define EMI_I2CM_SPI_MASTER_SS3_PIN     (0u == EMI_I2CM_REMOVE_SPI_MASTER_SS3_PIN)
#define EMI_I2CM_SPI_SLAVE_PINS         (0u == EMI_I2CM_REMOVE_SPI_SLAVE_PINS)
#define EMI_I2CM_SPI_SLAVE_MOSI_PIN     (0u == EMI_I2CM_REMOVE_SPI_SLAVE_MOSI_PIN)
#define EMI_I2CM_SPI_SLAVE_MISO_PIN     (0u == EMI_I2CM_REMOVE_SPI_SLAVE_MISO_PIN)
#define EMI_I2CM_UART_TX_PIN            (0u == EMI_I2CM_REMOVE_UART_TX_PIN)
#define EMI_I2CM_UART_RX_TX_PIN         (0u == EMI_I2CM_REMOVE_UART_RX_TX_PIN)
#define EMI_I2CM_UART_RX_PIN            (0u == EMI_I2CM_REMOVE_UART_RX_PIN)
#define EMI_I2CM_UART_RX_WAKE_PIN       (0u == EMI_I2CM_REMOVE_UART_RX_WAKE_PIN)
#define EMI_I2CM_UART_RTS_PIN           (0u == EMI_I2CM_REMOVE_UART_RTS_PIN)
#define EMI_I2CM_UART_CTS_PIN           (0u == EMI_I2CM_REMOVE_UART_CTS_PIN)


/***************************************
*             Includes
****************************************/

#if (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN)
    #include "EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi.h"
#endif /* (EMI_I2CM_RX_SDA_MOSI) */

#if (EMI_I2CM_RX_SDA_MOSI_PIN)
    #include "EMI_I2CM_uart_rx_i2c_sda_spi_mosi.h"
#endif /* (EMI_I2CM_RX_SDA_MOSI) */

#if (EMI_I2CM_TX_SCL_MISO_PIN)
    #include "EMI_I2CM_uart_tx_i2c_scl_spi_miso.h"
#endif /* (EMI_I2CM_TX_SCL_MISO) */

#if (EMI_I2CM_CTS_SCLK_PIN)
    #include "EMI_I2CM_uart_cts_spi_sclk.h"
#endif /* (EMI_I2CM_CTS_SCLK) */

#if (EMI_I2CM_RTS_SS0_PIN)
    #include "EMI_I2CM_uart_rts_spi_ss0.h"
#endif /* (EMI_I2CM_RTS_SS0_PIN) */

#if (EMI_I2CM_SS1_PIN)
    #include "EMI_I2CM_spi_ss1.h"
#endif /* (EMI_I2CM_SS1_PIN) */

#if (EMI_I2CM_SS2_PIN)
    #include "EMI_I2CM_spi_ss2.h"
#endif /* (EMI_I2CM_SS2_PIN) */

#if (EMI_I2CM_SS3_PIN)
    #include "EMI_I2CM_spi_ss3.h"
#endif /* (EMI_I2CM_SS3_PIN) */

#if (EMI_I2CM_I2C_PINS)
    #include "EMI_I2CM_scl.h"
    #include "EMI_I2CM_sda.h"
#endif /* (EMI_I2CM_I2C_PINS) */

#if (EMI_I2CM_SPI_MASTER_PINS)
#if (EMI_I2CM_SPI_MASTER_SCLK_PIN)
    #include "EMI_I2CM_sclk_m.h"
#endif /* (EMI_I2CM_SPI_MASTER_SCLK_PIN) */

#if (EMI_I2CM_SPI_MASTER_MOSI_PIN)
    #include "EMI_I2CM_mosi_m.h"
#endif /* (EMI_I2CM_SPI_MASTER_MOSI_PIN) */

#if (EMI_I2CM_SPI_MASTER_MISO_PIN)
    #include "EMI_I2CM_miso_m.h"
#endif /*(EMI_I2CM_SPI_MASTER_MISO_PIN) */
#endif /* (EMI_I2CM_SPI_MASTER_PINS) */

#if (EMI_I2CM_SPI_SLAVE_PINS)
    #include "EMI_I2CM_sclk_s.h"
    #include "EMI_I2CM_ss_s.h"

#if (EMI_I2CM_SPI_SLAVE_MOSI_PIN)
    #include "EMI_I2CM_mosi_s.h"
#endif /* (EMI_I2CM_SPI_SLAVE_MOSI_PIN) */

#if (EMI_I2CM_SPI_SLAVE_MISO_PIN)
    #include "EMI_I2CM_miso_s.h"
#endif /*(EMI_I2CM_SPI_SLAVE_MISO_PIN) */
#endif /* (EMI_I2CM_SPI_SLAVE_PINS) */

#if (EMI_I2CM_SPI_MASTER_SS0_PIN)
    #include "EMI_I2CM_ss0_m.h"
#endif /* (EMI_I2CM_SPI_MASTER_SS0_PIN) */

#if (EMI_I2CM_SPI_MASTER_SS1_PIN)
    #include "EMI_I2CM_ss1_m.h"
#endif /* (EMI_I2CM_SPI_MASTER_SS1_PIN) */

#if (EMI_I2CM_SPI_MASTER_SS2_PIN)
    #include "EMI_I2CM_ss2_m.h"
#endif /* (EMI_I2CM_SPI_MASTER_SS2_PIN) */

#if (EMI_I2CM_SPI_MASTER_SS3_PIN)
    #include "EMI_I2CM_ss3_m.h"
#endif /* (EMI_I2CM_SPI_MASTER_SS3_PIN) */

#if (EMI_I2CM_UART_TX_PIN)
    #include "EMI_I2CM_tx.h"
#endif /* (EMI_I2CM_UART_TX_PIN) */

#if (EMI_I2CM_UART_RX_TX_PIN)
    #include "EMI_I2CM_rx_tx.h"
#endif /* (EMI_I2CM_UART_RX_TX_PIN) */

#if (EMI_I2CM_UART_RX_PIN)
    #include "EMI_I2CM_rx.h"
#endif /* (EMI_I2CM_UART_RX_PIN) */

#if (EMI_I2CM_UART_RX_WAKE_PIN)
    #include "EMI_I2CM_rx_wake.h"
#endif /* (EMI_I2CM_UART_RX_WAKE_PIN) */

#if (EMI_I2CM_UART_RTS_PIN)
    #include "EMI_I2CM_rts.h"
#endif /* (EMI_I2CM_UART_RTS_PIN) */

#if (EMI_I2CM_UART_CTS_PIN)
    #include "EMI_I2CM_cts.h"
#endif /* (EMI_I2CM_UART_CTS_PIN) */


/***************************************
*              Registers
***************************************/

#if (EMI_I2CM_RX_SDA_MOSI_PIN)
    #define EMI_I2CM_RX_SDA_MOSI_HSIOM_REG   (*(reg32 *) EMI_I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM)
    #define EMI_I2CM_RX_SDA_MOSI_HSIOM_PTR   ( (reg32 *) EMI_I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM)
    
    #define EMI_I2CM_RX_SDA_MOSI_HSIOM_MASK      (EMI_I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM_MASK)
    #define EMI_I2CM_RX_SDA_MOSI_HSIOM_POS       (EMI_I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM_SHIFT)
    #define EMI_I2CM_RX_SDA_MOSI_HSIOM_SEL_GPIO  (EMI_I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM_GPIO)
    #define EMI_I2CM_RX_SDA_MOSI_HSIOM_SEL_I2C   (EMI_I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM_I2C)
    #define EMI_I2CM_RX_SDA_MOSI_HSIOM_SEL_SPI   (EMI_I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM_SPI)
    #define EMI_I2CM_RX_SDA_MOSI_HSIOM_SEL_UART  (EMI_I2CM_uart_rx_i2c_sda_spi_mosi__0__HSIOM_UART)
    
#elif (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN)
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG   (*(reg32 *) EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM)
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_PTR   ( (reg32 *) EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM)
    
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_MASK      (EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_MASK)
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_POS       (EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_SHIFT)
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_SEL_GPIO  (EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_GPIO)
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_SEL_I2C   (EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_I2C)
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_SEL_SPI   (EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_SPI)
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_SEL_UART  (EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__HSIOM_UART)    
   
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_INTCFG_REG (*(reg32 *) EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__INTCFG)
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_INTCFG_PTR ( (reg32 *) EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__0__INTCFG)
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_INTCFG_TYPE_POS  (EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi__SHIFT)
    #define EMI_I2CM_RX_WAKE_SDA_MOSI_INTCFG_TYPE_MASK ((uint32) EMI_I2CM_INTCFG_TYPE_MASK << \
                                                                           EMI_I2CM_RX_WAKE_SDA_MOSI_INTCFG_TYPE_POS)
#else
    /* None of pins EMI_I2CM_RX_SDA_MOSI_PIN or EMI_I2CM_RX_WAKE_SDA_MOSI_PIN present.*/
#endif /* (EMI_I2CM_RX_SDA_MOSI_PIN) */

#if (EMI_I2CM_TX_SCL_MISO_PIN)
    #define EMI_I2CM_TX_SCL_MISO_HSIOM_REG   (*(reg32 *) EMI_I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM)
    #define EMI_I2CM_TX_SCL_MISO_HSIOM_PTR   ( (reg32 *) EMI_I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM)
    
    #define EMI_I2CM_TX_SCL_MISO_HSIOM_MASK      (EMI_I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM_MASK)
    #define EMI_I2CM_TX_SCL_MISO_HSIOM_POS       (EMI_I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM_SHIFT)
    #define EMI_I2CM_TX_SCL_MISO_HSIOM_SEL_GPIO  (EMI_I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM_GPIO)
    #define EMI_I2CM_TX_SCL_MISO_HSIOM_SEL_I2C   (EMI_I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM_I2C)
    #define EMI_I2CM_TX_SCL_MISO_HSIOM_SEL_SPI   (EMI_I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM_SPI)
    #define EMI_I2CM_TX_SCL_MISO_HSIOM_SEL_UART  (EMI_I2CM_uart_tx_i2c_scl_spi_miso__0__HSIOM_UART)
#endif /* (EMI_I2CM_TX_SCL_MISO_PIN) */

#if (EMI_I2CM_CTS_SCLK_PIN)
    #define EMI_I2CM_CTS_SCLK_HSIOM_REG   (*(reg32 *) EMI_I2CM_uart_cts_spi_sclk__0__HSIOM)
    #define EMI_I2CM_CTS_SCLK_HSIOM_PTR   ( (reg32 *) EMI_I2CM_uart_cts_spi_sclk__0__HSIOM)
    
    #define EMI_I2CM_CTS_SCLK_HSIOM_MASK      (EMI_I2CM_uart_cts_spi_sclk__0__HSIOM_MASK)
    #define EMI_I2CM_CTS_SCLK_HSIOM_POS       (EMI_I2CM_uart_cts_spi_sclk__0__HSIOM_SHIFT)
    #define EMI_I2CM_CTS_SCLK_HSIOM_SEL_GPIO  (EMI_I2CM_uart_cts_spi_sclk__0__HSIOM_GPIO)
    #define EMI_I2CM_CTS_SCLK_HSIOM_SEL_I2C   (EMI_I2CM_uart_cts_spi_sclk__0__HSIOM_I2C)
    #define EMI_I2CM_CTS_SCLK_HSIOM_SEL_SPI   (EMI_I2CM_uart_cts_spi_sclk__0__HSIOM_SPI)
    #define EMI_I2CM_CTS_SCLK_HSIOM_SEL_UART  (EMI_I2CM_uart_cts_spi_sclk__0__HSIOM_UART)
#endif /* (EMI_I2CM_CTS_SCLK_PIN) */

#if (EMI_I2CM_RTS_SS0_PIN)
    #define EMI_I2CM_RTS_SS0_HSIOM_REG   (*(reg32 *) EMI_I2CM_uart_rts_spi_ss0__0__HSIOM)
    #define EMI_I2CM_RTS_SS0_HSIOM_PTR   ( (reg32 *) EMI_I2CM_uart_rts_spi_ss0__0__HSIOM)
    
    #define EMI_I2CM_RTS_SS0_HSIOM_MASK      (EMI_I2CM_uart_rts_spi_ss0__0__HSIOM_MASK)
    #define EMI_I2CM_RTS_SS0_HSIOM_POS       (EMI_I2CM_uart_rts_spi_ss0__0__HSIOM_SHIFT)
    #define EMI_I2CM_RTS_SS0_HSIOM_SEL_GPIO  (EMI_I2CM_uart_rts_spi_ss0__0__HSIOM_GPIO)
    #define EMI_I2CM_RTS_SS0_HSIOM_SEL_I2C   (EMI_I2CM_uart_rts_spi_ss0__0__HSIOM_I2C)
    #define EMI_I2CM_RTS_SS0_HSIOM_SEL_SPI   (EMI_I2CM_uart_rts_spi_ss0__0__HSIOM_SPI)
#if !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1)
    #define EMI_I2CM_RTS_SS0_HSIOM_SEL_UART  (EMI_I2CM_uart_rts_spi_ss0__0__HSIOM_UART)
#endif /* !(EMI_I2CM_CY_SCBIP_V0 || EMI_I2CM_CY_SCBIP_V1) */
#endif /* (EMI_I2CM_RTS_SS0_PIN) */

#if (EMI_I2CM_SS1_PIN)
    #define EMI_I2CM_SS1_HSIOM_REG  (*(reg32 *) EMI_I2CM_spi_ss1__0__HSIOM)
    #define EMI_I2CM_SS1_HSIOM_PTR  ( (reg32 *) EMI_I2CM_spi_ss1__0__HSIOM)
    
    #define EMI_I2CM_SS1_HSIOM_MASK     (EMI_I2CM_spi_ss1__0__HSIOM_MASK)
    #define EMI_I2CM_SS1_HSIOM_POS      (EMI_I2CM_spi_ss1__0__HSIOM_SHIFT)
    #define EMI_I2CM_SS1_HSIOM_SEL_GPIO (EMI_I2CM_spi_ss1__0__HSIOM_GPIO)
    #define EMI_I2CM_SS1_HSIOM_SEL_I2C  (EMI_I2CM_spi_ss1__0__HSIOM_I2C)
    #define EMI_I2CM_SS1_HSIOM_SEL_SPI  (EMI_I2CM_spi_ss1__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SS1_PIN) */

#if (EMI_I2CM_SS2_PIN)
    #define EMI_I2CM_SS2_HSIOM_REG     (*(reg32 *) EMI_I2CM_spi_ss2__0__HSIOM)
    #define EMI_I2CM_SS2_HSIOM_PTR     ( (reg32 *) EMI_I2CM_spi_ss2__0__HSIOM)
    
    #define EMI_I2CM_SS2_HSIOM_MASK     (EMI_I2CM_spi_ss2__0__HSIOM_MASK)
    #define EMI_I2CM_SS2_HSIOM_POS      (EMI_I2CM_spi_ss2__0__HSIOM_SHIFT)
    #define EMI_I2CM_SS2_HSIOM_SEL_GPIO (EMI_I2CM_spi_ss2__0__HSIOM_GPIO)
    #define EMI_I2CM_SS2_HSIOM_SEL_I2C  (EMI_I2CM_spi_ss2__0__HSIOM_I2C)
    #define EMI_I2CM_SS2_HSIOM_SEL_SPI  (EMI_I2CM_spi_ss2__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SS2_PIN) */

#if (EMI_I2CM_SS3_PIN)
    #define EMI_I2CM_SS3_HSIOM_REG     (*(reg32 *) EMI_I2CM_spi_ss3__0__HSIOM)
    #define EMI_I2CM_SS3_HSIOM_PTR     ( (reg32 *) EMI_I2CM_spi_ss3__0__HSIOM)
    
    #define EMI_I2CM_SS3_HSIOM_MASK     (EMI_I2CM_spi_ss3__0__HSIOM_MASK)
    #define EMI_I2CM_SS3_HSIOM_POS      (EMI_I2CM_spi_ss3__0__HSIOM_SHIFT)
    #define EMI_I2CM_SS3_HSIOM_SEL_GPIO (EMI_I2CM_spi_ss3__0__HSIOM_GPIO)
    #define EMI_I2CM_SS3_HSIOM_SEL_I2C  (EMI_I2CM_spi_ss3__0__HSIOM_I2C)
    #define EMI_I2CM_SS3_HSIOM_SEL_SPI  (EMI_I2CM_spi_ss3__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SS3_PIN) */

#if (EMI_I2CM_I2C_PINS)
    #define EMI_I2CM_SCL_HSIOM_REG  (*(reg32 *) EMI_I2CM_scl__0__HSIOM)
    #define EMI_I2CM_SCL_HSIOM_PTR  ( (reg32 *) EMI_I2CM_scl__0__HSIOM)
    
    #define EMI_I2CM_SCL_HSIOM_MASK     (EMI_I2CM_scl__0__HSIOM_MASK)
    #define EMI_I2CM_SCL_HSIOM_POS      (EMI_I2CM_scl__0__HSIOM_SHIFT)
    #define EMI_I2CM_SCL_HSIOM_SEL_GPIO (EMI_I2CM_sda__0__HSIOM_GPIO)
    #define EMI_I2CM_SCL_HSIOM_SEL_I2C  (EMI_I2CM_sda__0__HSIOM_I2C)
    
    #define EMI_I2CM_SDA_HSIOM_REG  (*(reg32 *) EMI_I2CM_sda__0__HSIOM)
    #define EMI_I2CM_SDA_HSIOM_PTR  ( (reg32 *) EMI_I2CM_sda__0__HSIOM)
    
    #define EMI_I2CM_SDA_HSIOM_MASK     (EMI_I2CM_sda__0__HSIOM_MASK)
    #define EMI_I2CM_SDA_HSIOM_POS      (EMI_I2CM_sda__0__HSIOM_SHIFT)
    #define EMI_I2CM_SDA_HSIOM_SEL_GPIO (EMI_I2CM_sda__0__HSIOM_GPIO)
    #define EMI_I2CM_SDA_HSIOM_SEL_I2C  (EMI_I2CM_sda__0__HSIOM_I2C)
#endif /* (EMI_I2CM_I2C_PINS) */

#if (EMI_I2CM_SPI_SLAVE_PINS)
    #define EMI_I2CM_SCLK_S_HSIOM_REG   (*(reg32 *) EMI_I2CM_sclk_s__0__HSIOM)
    #define EMI_I2CM_SCLK_S_HSIOM_PTR   ( (reg32 *) EMI_I2CM_sclk_s__0__HSIOM)
    
    #define EMI_I2CM_SCLK_S_HSIOM_MASK      (EMI_I2CM_sclk_s__0__HSIOM_MASK)
    #define EMI_I2CM_SCLK_S_HSIOM_POS       (EMI_I2CM_sclk_s__0__HSIOM_SHIFT)
    #define EMI_I2CM_SCLK_S_HSIOM_SEL_GPIO  (EMI_I2CM_sclk_s__0__HSIOM_GPIO)
    #define EMI_I2CM_SCLK_S_HSIOM_SEL_SPI   (EMI_I2CM_sclk_s__0__HSIOM_SPI)
    
    #define EMI_I2CM_SS0_S_HSIOM_REG    (*(reg32 *) EMI_I2CM_ss0_s__0__HSIOM)
    #define EMI_I2CM_SS0_S_HSIOM_PTR    ( (reg32 *) EMI_I2CM_ss0_s__0__HSIOM)
    
    #define EMI_I2CM_SS0_S_HSIOM_MASK       (EMI_I2CM_ss0_s__0__HSIOM_MASK)
    #define EMI_I2CM_SS0_S_HSIOM_POS        (EMI_I2CM_ss0_s__0__HSIOM_SHIFT)
    #define EMI_I2CM_SS0_S_HSIOM_SEL_GPIO   (EMI_I2CM_ss0_s__0__HSIOM_GPIO)  
    #define EMI_I2CM_SS0_S_HSIOM_SEL_SPI    (EMI_I2CM_ss0_s__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SPI_SLAVE_PINS) */

#if (EMI_I2CM_SPI_SLAVE_MOSI_PIN)
    #define EMI_I2CM_MOSI_S_HSIOM_REG   (*(reg32 *) EMI_I2CM_mosi_s__0__HSIOM)
    #define EMI_I2CM_MOSI_S_HSIOM_PTR   ( (reg32 *) EMI_I2CM_mosi_s__0__HSIOM)
    
    #define EMI_I2CM_MOSI_S_HSIOM_MASK      (EMI_I2CM_mosi_s__0__HSIOM_MASK)
    #define EMI_I2CM_MOSI_S_HSIOM_POS       (EMI_I2CM_mosi_s__0__HSIOM_SHIFT)
    #define EMI_I2CM_MOSI_S_HSIOM_SEL_GPIO  (EMI_I2CM_mosi_s__0__HSIOM_GPIO)
    #define EMI_I2CM_MOSI_S_HSIOM_SEL_SPI   (EMI_I2CM_mosi_s__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SPI_SLAVE_MOSI_PIN) */

#if (EMI_I2CM_SPI_SLAVE_MISO_PIN)
    #define EMI_I2CM_MISO_S_HSIOM_REG   (*(reg32 *) EMI_I2CM_miso_s__0__HSIOM)
    #define EMI_I2CM_MISO_S_HSIOM_PTR   ( (reg32 *) EMI_I2CM_miso_s__0__HSIOM)
    
    #define EMI_I2CM_MISO_S_HSIOM_MASK      (EMI_I2CM_miso_s__0__HSIOM_MASK)
    #define EMI_I2CM_MISO_S_HSIOM_POS       (EMI_I2CM_miso_s__0__HSIOM_SHIFT)
    #define EMI_I2CM_MISO_S_HSIOM_SEL_GPIO  (EMI_I2CM_miso_s__0__HSIOM_GPIO)
    #define EMI_I2CM_MISO_S_HSIOM_SEL_SPI   (EMI_I2CM_miso_s__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SPI_SLAVE_MISO_PIN) */

#if (EMI_I2CM_SPI_MASTER_MISO_PIN)
    #define EMI_I2CM_MISO_M_HSIOM_REG   (*(reg32 *) EMI_I2CM_miso_m__0__HSIOM)
    #define EMI_I2CM_MISO_M_HSIOM_PTR   ( (reg32 *) EMI_I2CM_miso_m__0__HSIOM)
    
    #define EMI_I2CM_MISO_M_HSIOM_MASK      (EMI_I2CM_miso_m__0__HSIOM_MASK)
    #define EMI_I2CM_MISO_M_HSIOM_POS       (EMI_I2CM_miso_m__0__HSIOM_SHIFT)
    #define EMI_I2CM_MISO_M_HSIOM_SEL_GPIO  (EMI_I2CM_miso_m__0__HSIOM_GPIO)
    #define EMI_I2CM_MISO_M_HSIOM_SEL_SPI   (EMI_I2CM_miso_m__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SPI_MASTER_MISO_PIN) */

#if (EMI_I2CM_SPI_MASTER_MOSI_PIN)
    #define EMI_I2CM_MOSI_M_HSIOM_REG   (*(reg32 *) EMI_I2CM_mosi_m__0__HSIOM)
    #define EMI_I2CM_MOSI_M_HSIOM_PTR   ( (reg32 *) EMI_I2CM_mosi_m__0__HSIOM)
    
    #define EMI_I2CM_MOSI_M_HSIOM_MASK      (EMI_I2CM_mosi_m__0__HSIOM_MASK)
    #define EMI_I2CM_MOSI_M_HSIOM_POS       (EMI_I2CM_mosi_m__0__HSIOM_SHIFT)
    #define EMI_I2CM_MOSI_M_HSIOM_SEL_GPIO  (EMI_I2CM_mosi_m__0__HSIOM_GPIO)
    #define EMI_I2CM_MOSI_M_HSIOM_SEL_SPI   (EMI_I2CM_mosi_m__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SPI_MASTER_MOSI_PIN) */

#if (EMI_I2CM_SPI_MASTER_SCLK_PIN)
    #define EMI_I2CM_SCLK_M_HSIOM_REG   (*(reg32 *) EMI_I2CM_sclk_m__0__HSIOM)
    #define EMI_I2CM_SCLK_M_HSIOM_PTR   ( (reg32 *) EMI_I2CM_sclk_m__0__HSIOM)
    
    #define EMI_I2CM_SCLK_M_HSIOM_MASK      (EMI_I2CM_sclk_m__0__HSIOM_MASK)
    #define EMI_I2CM_SCLK_M_HSIOM_POS       (EMI_I2CM_sclk_m__0__HSIOM_SHIFT)
    #define EMI_I2CM_SCLK_M_HSIOM_SEL_GPIO  (EMI_I2CM_sclk_m__0__HSIOM_GPIO)
    #define EMI_I2CM_SCLK_M_HSIOM_SEL_SPI   (EMI_I2CM_sclk_m__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SPI_MASTER_SCLK_PIN) */

#if (EMI_I2CM_SPI_MASTER_SS0_PIN)
    #define EMI_I2CM_SS0_M_HSIOM_REG    (*(reg32 *) EMI_I2CM_ss0_m__0__HSIOM)
    #define EMI_I2CM_SS0_M_HSIOM_PTR    ( (reg32 *) EMI_I2CM_ss0_m__0__HSIOM)
    
    #define EMI_I2CM_SS0_M_HSIOM_MASK       (EMI_I2CM_ss0_m__0__HSIOM_MASK)
    #define EMI_I2CM_SS0_M_HSIOM_POS        (EMI_I2CM_ss0_m__0__HSIOM_SHIFT)
    #define EMI_I2CM_SS0_M_HSIOM_SEL_GPIO   (EMI_I2CM_ss0_m__0__HSIOM_GPIO)
    #define EMI_I2CM_SS0_M_HSIOM_SEL_SPI    (EMI_I2CM_ss0_m__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SPI_MASTER_SS0_PIN) */

#if (EMI_I2CM_SPI_MASTER_SS1_PIN)
    #define EMI_I2CM_SS1_M_HSIOM_REG    (*(reg32 *) EMI_I2CM_ss1_m__0__HSIOM)
    #define EMI_I2CM_SS1_M_HSIOM_PTR    ( (reg32 *) EMI_I2CM_ss1_m__0__HSIOM)
    
    #define EMI_I2CM_SS1_M_HSIOM_MASK       (EMI_I2CM_ss1_m__0__HSIOM_MASK)
    #define EMI_I2CM_SS1_M_HSIOM_POS        (EMI_I2CM_ss1_m__0__HSIOM_SHIFT)
    #define EMI_I2CM_SS1_M_HSIOM_SEL_GPIO   (EMI_I2CM_ss1_m__0__HSIOM_GPIO)
    #define EMI_I2CM_SS1_M_HSIOM_SEL_SPI    (EMI_I2CM_ss1_m__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SPI_MASTER_SS1_PIN) */

#if (EMI_I2CM_SPI_MASTER_SS2_PIN)
    #define EMI_I2CM_SS2_M_HSIOM_REG    (*(reg32 *) EMI_I2CM_ss2_m__0__HSIOM)
    #define EMI_I2CM_SS2_M_HSIOM_PTR    ( (reg32 *) EMI_I2CM_ss2_m__0__HSIOM)
    
    #define EMI_I2CM_SS2_M_HSIOM_MASK       (EMI_I2CM_ss2_m__0__HSIOM_MASK)
    #define EMI_I2CM_SS2_M_HSIOM_POS        (EMI_I2CM_ss2_m__0__HSIOM_SHIFT)
    #define EMI_I2CM_SS2_M_HSIOM_SEL_GPIO   (EMI_I2CM_ss2_m__0__HSIOM_GPIO)
    #define EMI_I2CM_SS2_M_HSIOM_SEL_SPI    (EMI_I2CM_ss2_m__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SPI_MASTER_SS2_PIN) */

#if (EMI_I2CM_SPI_MASTER_SS3_PIN)
    #define EMI_I2CM_SS3_M_HSIOM_REG    (*(reg32 *) EMI_I2CM_ss3_m__0__HSIOM)
    #define EMI_I2CM_SS3_M_HSIOM_PTR    ( (reg32 *) EMI_I2CM_ss3_m__0__HSIOM)
    
    #define EMI_I2CM_SS3_M_HSIOM_MASK      (EMI_I2CM_ss3_m__0__HSIOM_MASK)
    #define EMI_I2CM_SS3_M_HSIOM_POS       (EMI_I2CM_ss3_m__0__HSIOM_SHIFT)
    #define EMI_I2CM_SS3_M_HSIOM_SEL_GPIO  (EMI_I2CM_ss3_m__0__HSIOM_GPIO)
    #define EMI_I2CM_SS3_M_HSIOM_SEL_SPI   (EMI_I2CM_ss3_m__0__HSIOM_SPI)
#endif /* (EMI_I2CM_SPI_MASTER_SS3_PIN) */

#if (EMI_I2CM_UART_RX_PIN)
    #define EMI_I2CM_RX_HSIOM_REG   (*(reg32 *) EMI_I2CM_rx__0__HSIOM)
    #define EMI_I2CM_RX_HSIOM_PTR   ( (reg32 *) EMI_I2CM_rx__0__HSIOM)
    
    #define EMI_I2CM_RX_HSIOM_MASK      (EMI_I2CM_rx__0__HSIOM_MASK)
    #define EMI_I2CM_RX_HSIOM_POS       (EMI_I2CM_rx__0__HSIOM_SHIFT)
    #define EMI_I2CM_RX_HSIOM_SEL_GPIO  (EMI_I2CM_rx__0__HSIOM_GPIO)
    #define EMI_I2CM_RX_HSIOM_SEL_UART  (EMI_I2CM_rx__0__HSIOM_UART)
#endif /* (EMI_I2CM_UART_RX_PIN) */

#if (EMI_I2CM_UART_RX_WAKE_PIN)
    #define EMI_I2CM_RX_WAKE_HSIOM_REG   (*(reg32 *) EMI_I2CM_rx_wake__0__HSIOM)
    #define EMI_I2CM_RX_WAKE_HSIOM_PTR   ( (reg32 *) EMI_I2CM_rx_wake__0__HSIOM)
    
    #define EMI_I2CM_RX_WAKE_HSIOM_MASK      (EMI_I2CM_rx_wake__0__HSIOM_MASK)
    #define EMI_I2CM_RX_WAKE_HSIOM_POS       (EMI_I2CM_rx_wake__0__HSIOM_SHIFT)
    #define EMI_I2CM_RX_WAKE_HSIOM_SEL_GPIO  (EMI_I2CM_rx_wake__0__HSIOM_GPIO)
    #define EMI_I2CM_RX_WAKE_HSIOM_SEL_UART  (EMI_I2CM_rx_wake__0__HSIOM_UART)
#endif /* (EMI_I2CM_UART_WAKE_RX_PIN) */

#if (EMI_I2CM_UART_CTS_PIN)
    #define EMI_I2CM_CTS_HSIOM_REG   (*(reg32 *) EMI_I2CM_cts__0__HSIOM)
    #define EMI_I2CM_CTS_HSIOM_PTR   ( (reg32 *) EMI_I2CM_cts__0__HSIOM)
    
    #define EMI_I2CM_CTS_HSIOM_MASK      (EMI_I2CM_cts__0__HSIOM_MASK)
    #define EMI_I2CM_CTS_HSIOM_POS       (EMI_I2CM_cts__0__HSIOM_SHIFT)
    #define EMI_I2CM_CTS_HSIOM_SEL_GPIO  (EMI_I2CM_cts__0__HSIOM_GPIO)
    #define EMI_I2CM_CTS_HSIOM_SEL_UART  (EMI_I2CM_cts__0__HSIOM_UART)
#endif /* (EMI_I2CM_UART_CTS_PIN) */

#if (EMI_I2CM_UART_TX_PIN)
    #define EMI_I2CM_TX_HSIOM_REG   (*(reg32 *) EMI_I2CM_tx__0__HSIOM)
    #define EMI_I2CM_TX_HSIOM_PTR   ( (reg32 *) EMI_I2CM_tx__0__HSIOM)
    
    #define EMI_I2CM_TX_HSIOM_MASK      (EMI_I2CM_tx__0__HSIOM_MASK)
    #define EMI_I2CM_TX_HSIOM_POS       (EMI_I2CM_tx__0__HSIOM_SHIFT)
    #define EMI_I2CM_TX_HSIOM_SEL_GPIO  (EMI_I2CM_tx__0__HSIOM_GPIO)
    #define EMI_I2CM_TX_HSIOM_SEL_UART  (EMI_I2CM_tx__0__HSIOM_UART)
#endif /* (EMI_I2CM_UART_TX_PIN) */

#if (EMI_I2CM_UART_RX_TX_PIN)
    #define EMI_I2CM_RX_TX_HSIOM_REG   (*(reg32 *) EMI_I2CM_rx_tx__0__HSIOM)
    #define EMI_I2CM_RX_TX_HSIOM_PTR   ( (reg32 *) EMI_I2CM_rx_tx__0__HSIOM)
    
    #define EMI_I2CM_RX_TX_HSIOM_MASK      (EMI_I2CM_rx_tx__0__HSIOM_MASK)
    #define EMI_I2CM_RX_TX_HSIOM_POS       (EMI_I2CM_rx_tx__0__HSIOM_SHIFT)
    #define EMI_I2CM_RX_TX_HSIOM_SEL_GPIO  (EMI_I2CM_rx_tx__0__HSIOM_GPIO)
    #define EMI_I2CM_RX_TX_HSIOM_SEL_UART  (EMI_I2CM_rx_tx__0__HSIOM_UART)
#endif /* (EMI_I2CM_UART_RX_TX_PIN) */

#if (EMI_I2CM_UART_RTS_PIN)
    #define EMI_I2CM_RTS_HSIOM_REG      (*(reg32 *) EMI_I2CM_rts__0__HSIOM)
    #define EMI_I2CM_RTS_HSIOM_PTR      ( (reg32 *) EMI_I2CM_rts__0__HSIOM)
    
    #define EMI_I2CM_RTS_HSIOM_MASK     (EMI_I2CM_rts__0__HSIOM_MASK)
    #define EMI_I2CM_RTS_HSIOM_POS      (EMI_I2CM_rts__0__HSIOM_SHIFT)    
    #define EMI_I2CM_RTS_HSIOM_SEL_GPIO (EMI_I2CM_rts__0__HSIOM_GPIO)
    #define EMI_I2CM_RTS_HSIOM_SEL_UART (EMI_I2CM_rts__0__HSIOM_UART)    
#endif /* (EMI_I2CM_UART_RTS_PIN) */


/***************************************
*        Registers Constants
***************************************/

/* HSIOM switch values. */ 
#define EMI_I2CM_HSIOM_DEF_SEL      (0x00u)
#define EMI_I2CM_HSIOM_GPIO_SEL     (0x00u)
/* The HSIOM values provided below are valid only for EMI_I2CM_CY_SCBIP_V0 
* and EMI_I2CM_CY_SCBIP_V1. It is not recommended to use them for 
* EMI_I2CM_CY_SCBIP_V2. Use pin name specific HSIOM constants provided 
* above instead for any SCB IP block version.
*/
#define EMI_I2CM_HSIOM_UART_SEL     (0x09u)
#define EMI_I2CM_HSIOM_I2C_SEL      (0x0Eu)
#define EMI_I2CM_HSIOM_SPI_SEL      (0x0Fu)

/* Pins settings index. */
#define EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX   (0u)
#define EMI_I2CM_RX_SDA_MOSI_PIN_INDEX       (0u)
#define EMI_I2CM_TX_SCL_MISO_PIN_INDEX       (1u)
#define EMI_I2CM_CTS_SCLK_PIN_INDEX       (2u)
#define EMI_I2CM_RTS_SS0_PIN_INDEX       (3u)
#define EMI_I2CM_SS1_PIN_INDEX                  (4u)
#define EMI_I2CM_SS2_PIN_INDEX                  (5u)
#define EMI_I2CM_SS3_PIN_INDEX                  (6u)

/* Pins settings mask. */
#define EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_MASK ((uint32) 0x01u << EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX)
#define EMI_I2CM_RX_SDA_MOSI_PIN_MASK     ((uint32) 0x01u << EMI_I2CM_RX_SDA_MOSI_PIN_INDEX)
#define EMI_I2CM_TX_SCL_MISO_PIN_MASK     ((uint32) 0x01u << EMI_I2CM_TX_SCL_MISO_PIN_INDEX)
#define EMI_I2CM_CTS_SCLK_PIN_MASK     ((uint32) 0x01u << EMI_I2CM_CTS_SCLK_PIN_INDEX)
#define EMI_I2CM_RTS_SS0_PIN_MASK     ((uint32) 0x01u << EMI_I2CM_RTS_SS0_PIN_INDEX)
#define EMI_I2CM_SS1_PIN_MASK                ((uint32) 0x01u << EMI_I2CM_SS1_PIN_INDEX)
#define EMI_I2CM_SS2_PIN_MASK                ((uint32) 0x01u << EMI_I2CM_SS2_PIN_INDEX)
#define EMI_I2CM_SS3_PIN_MASK                ((uint32) 0x01u << EMI_I2CM_SS3_PIN_INDEX)

/* Pin interrupt constants. */
#define EMI_I2CM_INTCFG_TYPE_MASK           (0x03u)
#define EMI_I2CM_INTCFG_TYPE_FALLING_EDGE   (0x02u)

/* Pin Drive Mode constants. */
#define EMI_I2CM_PIN_DM_ALG_HIZ  (0u)
#define EMI_I2CM_PIN_DM_DIG_HIZ  (1u)
#define EMI_I2CM_PIN_DM_OD_LO    (4u)
#define EMI_I2CM_PIN_DM_STRONG   (6u)


/***************************************
*          Macro Definitions
***************************************/

/* Return drive mode of the pin */
#define EMI_I2CM_DM_MASK    (0x7u)
#define EMI_I2CM_DM_SIZE    (3u)
#define EMI_I2CM_GET_P4_PIN_DM(reg, pos) \
    ( ((reg) & (uint32) ((uint32) EMI_I2CM_DM_MASK << (EMI_I2CM_DM_SIZE * (pos)))) >> \
                                                              (EMI_I2CM_DM_SIZE * (pos)) )

#if (EMI_I2CM_TX_SCL_MISO_PIN)
    #define EMI_I2CM_CHECK_TX_SCL_MISO_PIN_USED \
                (EMI_I2CM_PIN_DM_ALG_HIZ != \
                    EMI_I2CM_GET_P4_PIN_DM(EMI_I2CM_uart_tx_i2c_scl_spi_miso_PC, \
                                                   EMI_I2CM_uart_tx_i2c_scl_spi_miso_SHIFT))
#endif /* (EMI_I2CM_TX_SCL_MISO_PIN) */

#if (EMI_I2CM_RTS_SS0_PIN)
    #define EMI_I2CM_CHECK_RTS_SS0_PIN_USED \
                (EMI_I2CM_PIN_DM_ALG_HIZ != \
                    EMI_I2CM_GET_P4_PIN_DM(EMI_I2CM_uart_rts_spi_ss0_PC, \
                                                   EMI_I2CM_uart_rts_spi_ss0_SHIFT))
#endif /* (EMI_I2CM_RTS_SS0_PIN) */

/* Set bits-mask in register */
#define EMI_I2CM_SET_REGISTER_BITS(reg, mask, pos, mode) \
                    do                                           \
                    {                                            \
                        (reg) = (((reg) & ((uint32) ~(uint32) (mask))) | ((uint32) ((uint32) (mode) << (pos)))); \
                    }while(0)

/* Set bit in the register */
#define EMI_I2CM_SET_REGISTER_BIT(reg, mask, val) \
                    ((val) ? ((reg) |= (mask)) : ((reg) &= ((uint32) ~((uint32) (mask)))))

#define EMI_I2CM_SET_HSIOM_SEL(reg, mask, pos, sel) EMI_I2CM_SET_REGISTER_BITS(reg, mask, pos, sel)
#define EMI_I2CM_SET_INCFG_TYPE(reg, mask, pos, intType) \
                                                        EMI_I2CM_SET_REGISTER_BITS(reg, mask, pos, intType)
#define EMI_I2CM_SET_INP_DIS(reg, mask, val) EMI_I2CM_SET_REGISTER_BIT(reg, mask, val)

/* EMI_I2CM_SET_I2C_SCL_DR(val) - Sets I2C SCL DR register.
*  EMI_I2CM_SET_I2C_SCL_HSIOM_SEL(sel) - Sets I2C SCL HSIOM settings.
*/
/* SCB I2C: scl signal */
#if (EMI_I2CM_CY_SCBIP_V0)
#if (EMI_I2CM_I2C_PINS)
    #define EMI_I2CM_SET_I2C_SCL_DR(val) EMI_I2CM_scl_Write(val)

    #define EMI_I2CM_SET_I2C_SCL_HSIOM_SEL(sel) \
                          EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_SCL_HSIOM_REG,  \
                                                         EMI_I2CM_SCL_HSIOM_MASK, \
                                                         EMI_I2CM_SCL_HSIOM_POS,  \
                                                         (sel))
    #define EMI_I2CM_WAIT_SCL_SET_HIGH  (0u == EMI_I2CM_scl_Read())

/* Unconfigured SCB: scl signal */
#elif (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN)
    #define EMI_I2CM_SET_I2C_SCL_DR(val) \
                            EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi_Write(val)

    #define EMI_I2CM_SET_I2C_SCL_HSIOM_SEL(sel) \
                    EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG,  \
                                                   EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_MASK, \
                                                   EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_POS,  \
                                                   (sel))

    #define EMI_I2CM_WAIT_SCL_SET_HIGH  (0u == EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi_Read())

#elif (EMI_I2CM_RX_SDA_MOSI_PIN)
    #define EMI_I2CM_SET_I2C_SCL_DR(val) \
                            EMI_I2CM_uart_rx_i2c_sda_spi_mosi_Write(val)


    #define EMI_I2CM_SET_I2C_SCL_HSIOM_SEL(sel) \
                            EMI_I2CM_SET_HSIOM_SEL(EMI_I2CM_RX_SDA_MOSI_HSIOM_REG,  \
                                                           EMI_I2CM_RX_SDA_MOSI_HSIOM_MASK, \
                                                           EMI_I2CM_RX_SDA_MOSI_HSIOM_POS,  \
                                                           (sel))

    #define EMI_I2CM_WAIT_SCL_SET_HIGH  (0u == EMI_I2CM_uart_rx_i2c_sda_spi_mosi_Read())

#else
    #define EMI_I2CM_SET_I2C_SCL_DR(val)        do{ /* Does nothing */ }while(0)
    #define EMI_I2CM_SET_I2C_SCL_HSIOM_SEL(sel) do{ /* Does nothing */ }while(0)

    #define EMI_I2CM_WAIT_SCL_SET_HIGH  (0u)
#endif /* (EMI_I2CM_I2C_PINS) */

/* SCB I2C: sda signal */
#if (EMI_I2CM_I2C_PINS)
    #define EMI_I2CM_WAIT_SDA_SET_HIGH  (0u == EMI_I2CM_sda_Read())
/* Unconfigured SCB: sda signal */
#elif (EMI_I2CM_TX_SCL_MISO_PIN)
    #define EMI_I2CM_WAIT_SDA_SET_HIGH  (0u == EMI_I2CM_uart_tx_i2c_scl_spi_miso_Read())
#else
    #define EMI_I2CM_WAIT_SDA_SET_HIGH  (0u)
#endif /* (EMI_I2CM_MOSI_SCL_RX_PIN) */
#endif /* (EMI_I2CM_CY_SCBIP_V0) */

/* Clear UART wakeup source */
#if (EMI_I2CM_RX_SDA_MOSI_PIN)
    #define EMI_I2CM_CLEAR_UART_RX_WAKE_INTR        do{ /* Does nothing */ }while(0)
    
#elif (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN)
    #define EMI_I2CM_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) EMI_I2CM_uart_rx_wake_i2c_sda_spi_mosi_ClearInterrupt(); \
            }while(0)

#elif(EMI_I2CM_UART_RX_WAKE_PIN)
    #define EMI_I2CM_CLEAR_UART_RX_WAKE_INTR \
            do{                                      \
                (void) EMI_I2CM_rx_wake_ClearInterrupt(); \
            }while(0)
#else
#endif /* (EMI_I2CM_RX_SDA_MOSI_PIN) */


/***************************************
* The following code is DEPRECATED and
* must not be used.
***************************************/

/* Unconfigured pins */
#define EMI_I2CM_REMOVE_MOSI_SCL_RX_WAKE_PIN    EMI_I2CM_REMOVE_RX_WAKE_SDA_MOSI_PIN
#define EMI_I2CM_REMOVE_MOSI_SCL_RX_PIN         EMI_I2CM_REMOVE_RX_SDA_MOSI_PIN
#define EMI_I2CM_REMOVE_MISO_SDA_TX_PIN         EMI_I2CM_REMOVE_TX_SCL_MISO_PIN
#ifndef EMI_I2CM_REMOVE_SCLK_PIN
#define EMI_I2CM_REMOVE_SCLK_PIN                EMI_I2CM_REMOVE_CTS_SCLK_PIN
#endif /* EMI_I2CM_REMOVE_SCLK_PIN */
#ifndef EMI_I2CM_REMOVE_SS0_PIN
#define EMI_I2CM_REMOVE_SS0_PIN                 EMI_I2CM_REMOVE_RTS_SS0_PIN
#endif /* EMI_I2CM_REMOVE_SS0_PIN */

/* Unconfigured pins */
#define EMI_I2CM_MOSI_SCL_RX_WAKE_PIN   EMI_I2CM_RX_WAKE_SDA_MOSI_PIN
#define EMI_I2CM_MOSI_SCL_RX_PIN        EMI_I2CM_RX_SDA_MOSI_PIN
#define EMI_I2CM_MISO_SDA_TX_PIN        EMI_I2CM_TX_SCL_MISO_PIN
#ifndef EMI_I2CM_SCLK_PIN
#define EMI_I2CM_SCLK_PIN               EMI_I2CM_CTS_SCLK_PIN
#endif /* EMI_I2CM_SCLK_PIN */
#ifndef EMI_I2CM_SS0_PIN
#define EMI_I2CM_SS0_PIN                EMI_I2CM_RTS_SS0_PIN
#endif /* EMI_I2CM_SS0_PIN */

#if (EMI_I2CM_MOSI_SCL_RX_WAKE_PIN)
    #define EMI_I2CM_MOSI_SCL_RX_WAKE_HSIOM_REG     EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define EMI_I2CM_MOSI_SCL_RX_WAKE_HSIOM_PTR     EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define EMI_I2CM_MOSI_SCL_RX_WAKE_HSIOM_MASK    EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define EMI_I2CM_MOSI_SCL_RX_WAKE_HSIOM_POS     EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG

    #define EMI_I2CM_MOSI_SCL_RX_WAKE_INTCFG_REG    EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define EMI_I2CM_MOSI_SCL_RX_WAKE_INTCFG_PTR    EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG

    #define EMI_I2CM_MOSI_SCL_RX_WAKE_INTCFG_TYPE_POS   EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
    #define EMI_I2CM_MOSI_SCL_RX_WAKE_INTCFG_TYPE_MASK  EMI_I2CM_RX_WAKE_SDA_MOSI_HSIOM_REG
#endif /* (EMI_I2CM_RX_WAKE_SDA_MOSI_PIN) */

#if (EMI_I2CM_MOSI_SCL_RX_PIN)
    #define EMI_I2CM_MOSI_SCL_RX_HSIOM_REG      EMI_I2CM_RX_SDA_MOSI_HSIOM_REG
    #define EMI_I2CM_MOSI_SCL_RX_HSIOM_PTR      EMI_I2CM_RX_SDA_MOSI_HSIOM_PTR
    #define EMI_I2CM_MOSI_SCL_RX_HSIOM_MASK     EMI_I2CM_RX_SDA_MOSI_HSIOM_MASK
    #define EMI_I2CM_MOSI_SCL_RX_HSIOM_POS      EMI_I2CM_RX_SDA_MOSI_HSIOM_POS
#endif /* (EMI_I2CM_MOSI_SCL_RX_PIN) */

#if (EMI_I2CM_MISO_SDA_TX_PIN)
    #define EMI_I2CM_MISO_SDA_TX_HSIOM_REG      EMI_I2CM_TX_SCL_MISO_HSIOM_REG
    #define EMI_I2CM_MISO_SDA_TX_HSIOM_PTR      EMI_I2CM_TX_SCL_MISO_HSIOM_REG
    #define EMI_I2CM_MISO_SDA_TX_HSIOM_MASK     EMI_I2CM_TX_SCL_MISO_HSIOM_REG
    #define EMI_I2CM_MISO_SDA_TX_HSIOM_POS      EMI_I2CM_TX_SCL_MISO_HSIOM_REG
#endif /* (EMI_I2CM_MISO_SDA_TX_PIN_PIN) */

#if (EMI_I2CM_SCLK_PIN)
    #ifndef EMI_I2CM_SCLK_HSIOM_REG
    #define EMI_I2CM_SCLK_HSIOM_REG     EMI_I2CM_CTS_SCLK_HSIOM_REG
    #define EMI_I2CM_SCLK_HSIOM_PTR     EMI_I2CM_CTS_SCLK_HSIOM_PTR
    #define EMI_I2CM_SCLK_HSIOM_MASK    EMI_I2CM_CTS_SCLK_HSIOM_MASK
    #define EMI_I2CM_SCLK_HSIOM_POS     EMI_I2CM_CTS_SCLK_HSIOM_POS
    #endif /* EMI_I2CM_SCLK_HSIOM_REG */
#endif /* (EMI_I2CM_SCLK_PIN) */

#if (EMI_I2CM_SS0_PIN)
    #ifndef EMI_I2CM_SS0_HSIOM_REG
    #define EMI_I2CM_SS0_HSIOM_REG      EMI_I2CM_RTS_SS0_HSIOM_REG
    #define EMI_I2CM_SS0_HSIOM_PTR      EMI_I2CM_RTS_SS0_HSIOM_PTR
    #define EMI_I2CM_SS0_HSIOM_MASK     EMI_I2CM_RTS_SS0_HSIOM_MASK
    #define EMI_I2CM_SS0_HSIOM_POS      EMI_I2CM_RTS_SS0_HSIOM_POS
    #endif /* EMI_I2CM_SS0_HSIOM_REG */
#endif /* (EMI_I2CM_SS0_PIN) */

#define EMI_I2CM_MOSI_SCL_RX_WAKE_PIN_INDEX EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_INDEX
#define EMI_I2CM_MOSI_SCL_RX_PIN_INDEX      EMI_I2CM_RX_SDA_MOSI_PIN_INDEX
#define EMI_I2CM_MISO_SDA_TX_PIN_INDEX      EMI_I2CM_TX_SCL_MISO_PIN_INDEX
#ifndef EMI_I2CM_SCLK_PIN_INDEX
#define EMI_I2CM_SCLK_PIN_INDEX             EMI_I2CM_CTS_SCLK_PIN_INDEX
#endif /* EMI_I2CM_SCLK_PIN_INDEX */
#ifndef EMI_I2CM_SS0_PIN_INDEX
#define EMI_I2CM_SS0_PIN_INDEX              EMI_I2CM_RTS_SS0_PIN_INDEX
#endif /* EMI_I2CM_SS0_PIN_INDEX */

#define EMI_I2CM_MOSI_SCL_RX_WAKE_PIN_MASK EMI_I2CM_RX_WAKE_SDA_MOSI_PIN_MASK
#define EMI_I2CM_MOSI_SCL_RX_PIN_MASK      EMI_I2CM_RX_SDA_MOSI_PIN_MASK
#define EMI_I2CM_MISO_SDA_TX_PIN_MASK      EMI_I2CM_TX_SCL_MISO_PIN_MASK
#ifndef EMI_I2CM_SCLK_PIN_MASK
#define EMI_I2CM_SCLK_PIN_MASK             EMI_I2CM_CTS_SCLK_PIN_MASK
#endif /* EMI_I2CM_SCLK_PIN_MASK */
#ifndef EMI_I2CM_SS0_PIN_MASK
#define EMI_I2CM_SS0_PIN_MASK              EMI_I2CM_RTS_SS0_PIN_MASK
#endif /* EMI_I2CM_SS0_PIN_MASK */

#endif /* (CY_SCB_PINS_EMI_I2CM_H) */


/* [] END OF FILE */
