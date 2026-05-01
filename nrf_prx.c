/**
 * @file nrf_prx.c
 * @brief Primary Receiver (PRX) role implementation.
 *
 * Implements ShockBurst-mode reception without Auto-Acknowledge or
 * Acknowledge Payload.  Both PTX and PRX must be configured with identical
 * RF channel, payload width, and pipe address values (see nrf_config.h).
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "nrf_config.h"
#include "nrf_prx.h"

/* --------------------------------------------------------------------------
 * Internal helpers
 * -------------------------------------------------------------------------- */

static void prx_data_handler(const uint8_t *data, int size)
{
    static int packet_count = 0;

    /* Bytes 1-2 carry the sequence number (big-endian uint16) */
    uint16_t seq = ((uint16_t)data[1] << 8) | (uint16_t)data[2];

    printf("packet %d  seq %u  payload: ", packet_count++, (unsigned int)seq);
    /* Payload starts at byte 3 (bytes 0-2 are status + sequence number) */
    for (int i = 3; i < size; i++) {
        printf("%c", data[i]);
    }
    printf("\n");
}

static void prx_irq_callback(void)
{
    uint8_t data[NRF_PAYLOAD_WIDTH + 1u];

    nrf24l01p_read_rx_payload(NRF24L01P_RX_PW_P0, data);
    prx_data_handler(data, (int)NRF_PAYLOAD_WIDTH);

    nrf24l01p_flush_rx();
    nrf24l01p_write_register(NRF24L01P_STATUS, NRF24L01P_STATUS_RX_DR);
}

/* --------------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------------- */

void nrf_prx_setup(void)
{
    nrf24l01p_spi_init(SPI_ID, SPEED);
    nrf24l01p_gpio_init();

    digitalWrite(CE_PIN, LOW);

    /*
     * CONFIG: PWR_UP=1, PRIM_RX=1, EN_CRC=1, CRCO=0 (1-byte CRC)
     * Interrupt mask bits left at 0 (all IRQ lines enabled on nIRQ pin).
     */
    nrf24l01p_write_register(NRF24L01P_CONFIG,
        NRF24L01P_CONFIG_EN_CRC | NRF24L01P_CONFIG_PWR_UP | NRF24L01P_CONFIG_PRIM_RX);

    /* Auto-Acknowledge disabled (ShockBurst only, no ESB) */
    nrf24l01p_write_register(NRF24L01P_EN_AA,      0x00u);

    /* Enable data pipes 0 and 1 */
    nrf24l01p_write_register(NRF24L01P_EN_RXADDR,  0x03u);

    /* Address width: 5 bytes */
    nrf24l01p_write_register(NRF24L01P_SETUP_AW,   NRF24L01P_AW_5_BYTES);

    /* Auto-retransmit disabled */
    nrf24l01p_write_register(NRF24L01P_SETUP_RETR, 0x00u);

    /* RF channel: F0 = 2400 + NRF_RF_CHANNEL [MHz] */
    nrf24l01p_write_register(NRF24L01P_RF_CH,      (uint8_t)NRF_RF_CHANNEL);

    /*
     * RF_SETUP: RF_DR_HIGH=1, RF_PWR=11 (0 dBm)
     * Results in 2 Mbps air data rate.
     */
    nrf24l01p_write_register(NRF24L01P_RF_SETUP,   0x0Eu);

    /* Set pipe 0 RX address to the shared pipe 0 address */
    const uint8_t pipe0_addr[5] = NRF_PIPE0_ADDR;
    nrf24l01p_write_pipe_addr(NRF24L01P_RX_ADDR_P0, pipe0_addr, 5u);

    /* Configure pipe 0 static payload width */
    nrf24l01p_write_register(NRF24L01P_RX_PW_P0, (uint8_t)NRF_PAYLOAD_WIDTH);

    /* Dynamic payload disabled */
    nrf24l01p_write_register(NRF24L01P_FEATURE, 0x00u);

    /* Flush RX FIFO and clear all interrupt flags */
    nrf24l01p_flush_rx();
    nrf24l01p_write_register(NRF24L01P_STATUS, NRF24L01P_STATUS_CLEAR_IRQ);
}

void nrf_prx_listen(void)
{
    wiringPiISR(IRQ_PIN, INT_EDGE_FALLING, &prx_irq_callback);
    digitalWrite(CE_PIN, HIGH);

    while (true) {
        /* Idle; packet processing occurs in prx_irq_callback() */
    }
}
