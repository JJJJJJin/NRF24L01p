/**
 * @file nrf_ptx.h
 * @brief Public interface for the Primary Transmitter (PTX) role.
 */

#ifndef NRF_PTX_H
#define NRF_PTX_H

#include <stdint.h>

#include "nrf24l01p.h"
#include "nrf_security.h"
#include "nrf_debug.h"

/**
 * @brief Configure the nRF24L01+ as a PTX.
 *
 * Initialises SPI, GPIO, and all required registers.  Must be called once
 * before any call to nrf_ptx_send().
 */
void nrf_ptx_setup(void);

/**
 * @brief Transmit a payload.
 *
 * Writes @p data into the TX FIFO and pulses CE to trigger transmission.
 * If a previous MAX_RT condition is detected the TX FIFO is flushed and the
 * flag is cleared before loading the new payload.
 *
 * @param data Payload buffer.  Length must equal NRF_PAYLOAD_WIDTH.
 * @param size Number of bytes to transmit.
 */
void nrf_ptx_send(const uint8_t *data, uint8_t size);

#endif /* NRF_PTX_H */
