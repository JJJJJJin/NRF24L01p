/**
 * @file nrf_debug.h
 * @brief Register dump utilities for the nRF24L01+ driver.
 *
 * All functions read live values from the device over SPI.  They are intended
 * for bring-up and debugging only and should not be called in time-critical
 * paths.
 */

#ifndef NRF_DEBUG_H
#define NRF_DEBUG_H

/* Full register dump */
void nrf_debug_print_all_reg(void);

/* Individual register print functions */
void nrf_debug_print_CONFIG(void);
void nrf_debug_print_EN_AA(void);
void nrf_debug_print_EN_RXADDR(void);
void nrf_debug_print_SETUP_AW(void);
void nrf_debug_print_SETUP_RETR(void);
void nrf_debug_print_RF_CH(void);
void nrf_debug_print_RF_SETUP(void);
void nrf_debug_print_STATUS(void);
void nrf_debug_print_OBSERVE_TX(void);
void nrf_debug_print_RPD(void);
void nrf_debug_print_RX_ADDR_PN(void);
void nrf_debug_print_TX_ADDR(void);
void nrf_debug_print_RX_PW_PN(void);
void nrf_debug_print_FIFO_STATUS(void);
void nrf_debug_print_DYNPD(void);
void nrf_debug_print_FEATURE(void);

#endif /* NRF_DEBUG_H */
