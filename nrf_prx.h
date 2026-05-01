/**
 * @file nrf_prx.h
 * @brief Public interface for the Primary Receiver (PRX) role.
 */

#ifndef NRF_PRX_H
#define NRF_PRX_H

#include <stdint.h>

#include "nrf24l01p.h"
#include "nrf_security.h"
#include "nrf_debug.h"

/**
 * @brief Configure the nRF24L01+ as a PRX.
 *
 * Initialises SPI, GPIO, and all required registers.  Must be called once
 * before any call to nrf_prx_listen().
 */
void nrf_prx_setup(void);

/**
 * @brief Enter continuous receive mode.
 *
 * Registers an ISR on the IRQ pin and asserts CE.  Blocks indefinitely;
 * incoming packets are processed inside the ISR.
 */
void nrf_prx_listen(void);

#endif /* NRF_PRX_H */
