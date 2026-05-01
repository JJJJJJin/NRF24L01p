/**
 * @file nrf_ptx.c
 * @brief Primary Transmitter (PTX) role implementation.
 *
 * Implements ShockBurst-mode transmission without Auto-Acknowledge or
 * Acknowledge Payload.  Both PTX and PRX must be configured with identical
 * RF channel, payload width, and pipe address values (see nrf_config.h).
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "nrf_config.h"
#include "nrf_ptx.h"

void nrf_ptx_setup(void)
{
    nrf24l01p_spi_init(SPI_ID, SPEED);
    nrf24l01p_gpio_init();

    digitalWrite(CE_PIN, LOW);

    /*
     * CONFIG: PWR_UP=1, PRIM_RX=0, EN_CRC=1, CRCO=0 (1-byte CRC)
     * Interrupt mask bits left at 0 (all IRQ lines enabled on nIRQ pin).
     */
    nrf24l01p_write_register(NRF24L01P_CONFIG,
        NRF24L01P_CONFIG_EN_CRC | NRF24L01P_CONFIG_PWR_UP);

    /* Auto-Acknowledge disabled (ShockBurst only, no ESB) */
    nrf24l01p_write_register(NRF24L01P_EN_AA,      0x00u);

    /* Enable data pipe 0 for RX_ADDR_P0 (required for auto-retransmit if used) */
    nrf24l01p_write_register(NRF24L01P_EN_RXADDR,  0x01u);

    /* Address width: 5 bytes */
    nrf24l01p_write_register(NRF24L01P_SETUP_AW,   NRF24L01P_AW_5_BYTES);

    /* Auto-retransmit disabled */
    nrf24l01p_write_register(NRF24L01P_SETUP_RETR, 0x00u);

    /* RF channel: F0 = 2400 + NRF_RF_CHANNEL [MHz] */
    nrf24l01p_write_register(NRF24L01P_RF_CH,      (uint8_t)NRF_RF_CHANNEL);

    /*
     * RF_SETUP: RF_DR_LOW=1, RF_PWR=11 (0 dBm), PLL_LOCK=0
     * Results in 250 kbps air data rate.
     */
    nrf24l01p_write_register(NRF24L01P_RF_SETUP,   0x26u);

    /* Set TX address and pipe 0 RX address to the shared pipe 0 address */
    const uint8_t pipe0_addr[5] = NRF_PIPE0_ADDR;
    nrf24l01p_write_pipe_addr(NRF24L01P_TX_ADDR,    pipe0_addr, 5u);
    nrf24l01p_write_pipe_addr(NRF24L01P_RX_ADDR_P0, pipe0_addr, 5u);

    /* Dynamic payload disabled */
    nrf24l01p_write_register(NRF24L01P_FEATURE, 0x00u);

    /* Flush TX FIFO and clear all interrupt flags */
    nrf24l01p_flush_tx();
    nrf24l01p_write_register(NRF24L01P_STATUS, NRF24L01P_STATUS_CLEAR_IRQ);
}

void nrf_ptx_send(const uint8_t *data, uint8_t size)
{
    /* Clear any pending MAX_RT condition before loading a new payload */
    if (!digitalRead(IRQ_PIN)) {
        nrf24l01p_flush_tx();
        nrf24l01p_write_register(NRF24L01P_STATUS, NRF24L01P_STATUS_MAX_RT);
    }

    nrf24l01p_write_tx_payload(data, size);

    /* Pulse CE for at least 10 us to initiate transmission */
    digitalWrite(CE_PIN, HIGH);
    usleep(10u);
    digitalWrite(CE_PIN, LOW);
}
