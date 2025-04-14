#ifndef NRF24L01P_H
#define NRF24L01P_H

#include "nrf_debug.h"

#ifndef PRX
#include "ptx_pinout.h"
#else
#include "prx_pinout.h"
#endif

/* Define Commands */
#define NRF24L01P_READ_REGISTER 0x00
#define NRF24L01P_WRITE_REGISTER 0x20
#define NRF24L01P_R_RX_PAYLOAD 0x61
#define NRF24L01P_W_TX_PAYLOAD 0xA0
#define NRF24L01P_FLUSH_TX 0xE1
#define NRF24L01P_FLUSH_RX 0xE2
#define NRF24L01P_REUSE_TX_PL 0xE3
#define NRF24L01P_NOP 0xFF

/* Define Register Map */
#define NRF24L01P_CONFIG 0x00
#define NRF24L01P_EN_AA 0x01
#define NRF24L01P_EN_RXADDR 0x02
#define NRF24L01P_SETUP_AW 0x03
#define NRF24L01P_SETUP_RETR 0x04
#define NRF24L01P_RF_CH 0x05
#define NRF24L01P_RF_SETUP 0x06
#define NRF24L01P_STATUS 0x07
#define NRF24L01P_OBSERVE_TX 0x08
#define NRF24L01P_RPD 0x09
#define NRF24L01P_RX_ADDR_P0 0x0A
#define NRF24L01P_RX_ADDR_P1 0x0B
#define NRF24L01P_RX_ADDR_P2 0x0C
#define NRF24L01P_RX_ADDR_P3 0x0D
#define NRF24L01P_RX_ADDR_P4 0x0E
#define NRF24L01P_RX_ADDR_P5 0x0F
#define NRF24L01P_TX_ADDR 0x10
#define NRF24L01P_RX_PW_P0 0x11
#define NRF24L01P_RX_PW_P1 0x12
#define NRF24L01P_RX_PW_P2 0x13
#define NRF24L01P_RX_PW_P3 0x14
#define NRF24L01P_RX_PW_P4 0x15
#define NRF24L01P_RX_PW_P5 0x16
#define NRF24L01P_FIFO_STATUS 0x17
#define NRF24L01P_DYNPD 0x1C
#define NRF24L01P_FEATURE 0x1D

/* Declare functions */
void nrf24l01p_gpio_init();

void nrf24l01p_spi_init(int, int);

void nrf24l01p_read_register(uint8_t, uint8_t*, int);

void nrf24l01p_write_register(uint8_t, uint8_t);

void nrf24l01p_read_pipeaddr(uint8_t, uint8_t*);

void nrf24l01p_write_pipeaddr_3_byte(uint8_t, uint8_t*);

void nrf24l01p_write_pipeaddr_4_byte(uint8_t, uint8_t*);

void nrf24l01p_write_pipeaddr_5_byte(uint8_t, uint8_t*);

void nrf24l01p_read_rx_payload(uint8_t, uint8_t*);

void nrf24l01p_write_tx_payload(uint8_t*, uint8_t);

void nrf24l01p_flush_rx();

void nrf24l01p_flush_tx();

#endif
