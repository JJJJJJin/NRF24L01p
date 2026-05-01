/**
 * @file nrf_config.h
 * @brief Shared link configuration for the nRF24L01+ driver.
 *
 * Both PTX and PRX roles must be compiled with identical values defined here.
 * Adjust these constants to match your hardware deployment before building.
 */

#ifndef NRF_CONFIG_H
#define NRF_CONFIG_H

/**
 * RF channel number.  Operating frequency = 2400 + NRF_RF_CHANNEL [MHz].
 * Valid range: 0-125 (2.400 GHz - 2.525 GHz).
 */
#define NRF_RF_CHANNEL     100u

/**
 * Static payload width in bytes.
 * Valid range: 1-32.  Must be identical on both PTX and PRX.
 */
#define NRF_PAYLOAD_WIDTH  32u

/**
 * 5-byte pipe 0 address.
 * Used as TX_ADDR on PTX and RX_ADDR_P0 on PRX.
 */
#define NRF_PIPE0_ADDR     { 0xE7u, 0xD3u, 0xF0u, 0x35u, 0x77u }

#endif /* NRF_CONFIG_H */
