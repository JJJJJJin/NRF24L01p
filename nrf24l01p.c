/**
 * @file nrf24l01p.c
 * @brief nRF24L01+ hardware abstraction layer implementation.
 *
 * Drives the Nordic nRF24L01+ via WiringPi SPI and GPIO on Raspberry Pi.
 * ShockBurst mode only; Enhanced ShockBurst (ESB) is not implemented.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "nrf24l01p.h"

void nrf24l01p_gpio_init(void)
{
    if (wiringPiSetupGpio() == -1) {
        fprintf(stderr, "nrf24l01p: GPIO initialisation failed\n");
        exit(1);
    }

    pinMode(CE_PIN,  OUTPUT);
    pinMode(CSN_PIN, OUTPUT);
    pinMode(IRQ_PIN, INPUT);

    digitalWrite(CE_PIN,  LOW);
    digitalWrite(CSN_PIN, HIGH);  /* CSN is active-low; idle high */
}

void nrf24l01p_spi_init(int spi, int speed)
{
    if (wiringPiSPISetup(spi, speed) == -1) {
        fprintf(stderr, "nrf24l01p: SPI initialisation failed\n");
        exit(1);
    }
}

void nrf24l01p_read_register(uint8_t reg, uint8_t *data, int size)
{
    data[0] = NRF24L01P_CMD_READ_REG | reg;

    digitalWrite(CSN_PIN, LOW);
    wiringPiSPIDataRW(SPI_ID, (unsigned char *)data, size);
    digitalWrite(CSN_PIN, HIGH);
}

void nrf24l01p_write_register(uint8_t reg, uint8_t value)
{
    uint8_t buf[2];
    buf[0] = NRF24L01P_CMD_WRITE_REG | reg;
    buf[1] = value;

    digitalWrite(CSN_PIN, LOW);
    wiringPiSPIDataRW(SPI_ID, (unsigned char *)buf, (int)sizeof(buf));
    digitalWrite(CSN_PIN, HIGH);
}

void nrf24l01p_read_pipe_addr(uint8_t reg, uint8_t *addr, uint8_t len)
{
    /* +1 for the command/status byte prepended to every SPI transfer */
    uint8_t buf[len + 1u];
    buf[0] = NRF24L01P_CMD_READ_REG | reg;

    digitalWrite(CSN_PIN, LOW);
    wiringPiSPIDataRW(SPI_ID, (unsigned char *)buf, (int)(len + 1u));
    digitalWrite(CSN_PIN, HIGH);

    /* Strip the STATUS byte at index 0; copy address bytes to caller buffer */
    for (uint8_t i = 0u; i < len; i++) {
        addr[i] = buf[i + 1u];
    }
}

void nrf24l01p_write_pipe_addr(uint8_t reg, const uint8_t *addr, uint8_t len)
{
    if (len < 3u || len > 5u) {
        fprintf(stderr, "nrf24l01p: invalid pipe address length: %u (must be 3, 4, or 5)\n",
                (unsigned int)len);
        exit(1);
    }

    /* buf[0] = command byte; buf[1..len] = address bytes */
    uint8_t buf[len + 1u];
    buf[0] = NRF24L01P_CMD_WRITE_REG | reg;
    for (uint8_t i = 0u; i < len; i++) {
        buf[i + 1u] = addr[i];
    }

    digitalWrite(CSN_PIN, LOW);
    wiringPiSPIDataRW(SPI_ID, (unsigned char *)buf, (int)(len + 1u));
    digitalWrite(CSN_PIN, HIGH);
}

void nrf24l01p_read_rx_payload(uint8_t pw_reg, uint8_t *buffer)
{
    uint8_t pw_buf[2];
    nrf24l01p_read_register(pw_reg, pw_buf, (int)sizeof(pw_buf));
    uint8_t payload_width = pw_buf[1];

    /* buffer[0] = command (overwritten with STATUS); buffer[1..] = payload */
    buffer[0] = NRF24L01P_CMD_R_RX_PAYLOAD;

    digitalWrite(CSN_PIN, LOW);
    wiringPiSPIDataRW(SPI_ID, (unsigned char *)buffer, (int)(payload_width + 1u));
    digitalWrite(CSN_PIN, HIGH);
}

void nrf24l01p_write_tx_payload(const uint8_t *data, uint8_t size)
{
    uint8_t buf[size + 1u];
    buf[0] = NRF24L01P_CMD_W_TX_PAYLOAD;
    for (uint8_t i = 0u; i < size; i++) {
        buf[i + 1u] = data[i];
    }

    digitalWrite(CSN_PIN, LOW);
    wiringPiSPIDataRW(SPI_ID, (unsigned char *)buf, (int)(size + 1u));
    digitalWrite(CSN_PIN, HIGH);
}

void nrf24l01p_flush_rx(void)
{
    uint8_t cmd = NRF24L01P_CMD_FLUSH_RX;

    digitalWrite(CSN_PIN, LOW);
    wiringPiSPIDataRW(SPI_ID, (unsigned char *)&cmd, 1);
    digitalWrite(CSN_PIN, HIGH);
}

void nrf24l01p_flush_tx(void)
{
    uint8_t cmd = NRF24L01P_CMD_FLUSH_TX;

    digitalWrite(CSN_PIN, LOW);
    wiringPiSPIDataRW(SPI_ID, (unsigned char *)&cmd, 1);
    digitalWrite(CSN_PIN, HIGH);
}
