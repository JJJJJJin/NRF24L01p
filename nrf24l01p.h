/**
 * @file nrf24l01p.h
 * @brief nRF24L01+ hardware abstraction layer.
 *
 * Provides low-level SPI and GPIO primitives for driving the Nordic
 * nRF24L01+ 2.4 GHz transceiver on Raspberry Pi via WiringPi.
 *
 * Compile with -DPRX=1 to select the PRX pin assignment; omit the flag
 * (default) to use the PTX pin assignment.
 */

#ifndef NRF24L01P_H
#define NRF24L01P_H

#include <stdint.h>

#include "nrf_debug.h"

#ifndef PRX
#include "ptx_pinout.h"
#else
#include "prx_pinout.h"
#endif

/* --------------------------------------------------------------------------
 * SPI command bytes (nRF24L01+ product specification, table 19)
 * -------------------------------------------------------------------------- */
#define NRF24L01P_CMD_READ_REG      0x00u
#define NRF24L01P_CMD_WRITE_REG     0x20u
#define NRF24L01P_CMD_R_RX_PAYLOAD  0x61u
#define NRF24L01P_CMD_W_TX_PAYLOAD  0xA0u
#define NRF24L01P_CMD_FLUSH_TX      0xE1u
#define NRF24L01P_CMD_FLUSH_RX      0xE2u
#define NRF24L01P_CMD_REUSE_TX_PL   0xE3u
#define NRF24L01P_CMD_NOP           0xFFu

/* --------------------------------------------------------------------------
 * Register map (nRF24L01+ product specification, table 28)
 * -------------------------------------------------------------------------- */
#define NRF24L01P_CONFIG      0x00u
#define NRF24L01P_EN_AA       0x01u
#define NRF24L01P_EN_RXADDR   0x02u
#define NRF24L01P_SETUP_AW    0x03u
#define NRF24L01P_SETUP_RETR  0x04u
#define NRF24L01P_RF_CH       0x05u
#define NRF24L01P_RF_SETUP    0x06u
#define NRF24L01P_STATUS      0x07u
#define NRF24L01P_OBSERVE_TX  0x08u
#define NRF24L01P_RPD         0x09u
#define NRF24L01P_RX_ADDR_P0  0x0Au
#define NRF24L01P_RX_ADDR_P1  0x0Bu
#define NRF24L01P_RX_ADDR_P2  0x0Cu
#define NRF24L01P_RX_ADDR_P3  0x0Du
#define NRF24L01P_RX_ADDR_P4  0x0Eu
#define NRF24L01P_RX_ADDR_P5  0x0Fu
#define NRF24L01P_TX_ADDR     0x10u
#define NRF24L01P_RX_PW_P0   0x11u
#define NRF24L01P_RX_PW_P1   0x12u
#define NRF24L01P_RX_PW_P2   0x13u
#define NRF24L01P_RX_PW_P3   0x14u
#define NRF24L01P_RX_PW_P4   0x15u
#define NRF24L01P_RX_PW_P5   0x16u
#define NRF24L01P_FIFO_STATUS 0x17u
#define NRF24L01P_DYNPD       0x1Cu
#define NRF24L01P_FEATURE     0x1Du

/* --------------------------------------------------------------------------
 * CONFIG register bit masks
 * -------------------------------------------------------------------------- */
#define NRF24L01P_CONFIG_MASK_RX_DR  (1u << 6)
#define NRF24L01P_CONFIG_MASK_TX_DS  (1u << 5)
#define NRF24L01P_CONFIG_MASK_MAX_RT (1u << 4)
#define NRF24L01P_CONFIG_EN_CRC      (1u << 3)
#define NRF24L01P_CONFIG_CRCO        (1u << 2)
#define NRF24L01P_CONFIG_PWR_UP      (1u << 1)
#define NRF24L01P_CONFIG_PRIM_RX     (1u << 0)

/* --------------------------------------------------------------------------
 * STATUS register bit masks
 * -------------------------------------------------------------------------- */
#define NRF24L01P_STATUS_RX_DR   (1u << 6)
#define NRF24L01P_STATUS_TX_DS   (1u << 5)
#define NRF24L01P_STATUS_MAX_RT  (1u << 4)
#define NRF24L01P_STATUS_TX_FULL (1u << 0)

/* STATUS register: clear all interrupt flags */
#define NRF24L01P_STATUS_CLEAR_IRQ \
    (NRF24L01P_STATUS_RX_DR | NRF24L01P_STATUS_TX_DS | NRF24L01P_STATUS_MAX_RT)

/* --------------------------------------------------------------------------
 * SETUP_AW address width values
 * -------------------------------------------------------------------------- */
#define NRF24L01P_AW_3_BYTES  0x01u
#define NRF24L01P_AW_4_BYTES  0x02u
#define NRF24L01P_AW_5_BYTES  0x03u

/* --------------------------------------------------------------------------
 * Function declarations
 * -------------------------------------------------------------------------- */

/**
 * @brief Initialise GPIO pins (CE, CSN, IRQ).
 *
 * Calls wiringPiSetupGpio() and configures pin directions.
 * Terminates the process on failure.
 */
void nrf24l01p_gpio_init(void);

/**
 * @brief Initialise the SPI peripheral.
 *
 * @param spi   SPI channel (0 for /dev/spidev0.x, 1 for /dev/spidev1.x).
 * @param speed SPI clock frequency in Hz.
 *
 * Terminates the process on failure.
 */
void nrf24l01p_spi_init(int spi, int speed);

/**
 * @brief Read one or more bytes from a device register.
 *
 * @param reg  Register address.
 * @param data Output buffer.  data[0] receives the STATUS byte; data[1..size-1]
 *             receive the register contents.  Must be at least @p size bytes.
 * @param size Total transfer length in bytes (= desired register bytes + 1).
 */
void nrf24l01p_read_register(uint8_t reg, uint8_t *data, int size);

/**
 * @brief Write one byte to a device register.
 *
 * @param reg   Register address.
 * @param value Byte to write.
 */
void nrf24l01p_write_register(uint8_t reg, uint8_t value);

/**
 * @brief Read a multi-byte pipe address from the device.
 *
 * @param reg  Register address (e.g. NRF24L01P_RX_ADDR_P0).
 * @param addr Output buffer that receives @p len address bytes.
 * @param len  Address width in bytes (3, 4, or 5).  Must match SETUP_AW.
 */
void nrf24l01p_read_pipe_addr(uint8_t reg, uint8_t *addr, uint8_t len);

/**
 * @brief Write a multi-byte pipe address to the device.
 *
 * @param reg  Register address (e.g. NRF24L01P_TX_ADDR).
 * @param addr Address bytes to write.
 * @param len  Address width in bytes (3, 4, or 5).  Must match SETUP_AW.
 *
 * Terminates the process if @p len is not 3, 4, or 5.
 */
void nrf24l01p_write_pipe_addr(uint8_t reg, const uint8_t *addr, uint8_t len);

/**
 * @brief Read the received payload from the RX FIFO.
 *
 * @param pw_reg  The RX_PW_Px register address for the active pipe
 *                (e.g. NRF24L01P_RX_PW_P0).  Used to determine payload width.
 * @param buffer  Output buffer.  Must be at least (payload_width + 1) bytes.
 *                buffer[0] receives the STATUS byte; buffer[1..] the payload.
 */
void nrf24l01p_read_rx_payload(uint8_t pw_reg, uint8_t *buffer);

/**
 * @brief Write a payload into the TX FIFO.
 *
 * @param data Payload bytes.  Length must equal the PRX pipe's configured width.
 * @param size Number of bytes to write.
 */
void nrf24l01p_write_tx_payload(const uint8_t *data, uint8_t size);

/**
 * @brief Flush the RX FIFO.
 */
void nrf24l01p_flush_rx(void);

/**
 * @brief Flush the TX FIFO.
 */
void nrf24l01p_flush_tx(void);

#endif /* NRF24L01P_H */
