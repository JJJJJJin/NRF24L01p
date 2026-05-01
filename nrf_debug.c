/**
 * @file nrf_debug.c
 * @brief Register dump utilities for the nRF24L01+ driver.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "nrf_debug.h"
#include "nrf24l01p.h"

/* --------------------------------------------------------------------------
 * Internal helpers
 * -------------------------------------------------------------------------- */

/**
 * Convert a uint8_t to its 8-character binary string representation.
 *
 * @param buf  Output buffer of at least 9 bytes (8 digits + null terminator).
 * @param val  Value to convert.
 */
static void uint8_to_bin_str(char buf[9], uint8_t val)
{
    for (int i = 7; i >= 0; i--) {
        buf[7 - i] = ((val >> i) & 1u) ? '1' : '0';
    }
    buf[8] = '\0';
}

/**
 * Read a single-byte register and return its value.
 * data[0] (STATUS) is discarded.
 */
static uint8_t debug_read_register(uint8_t reg)
{
    uint8_t buf[2];
    nrf24l01p_read_register(reg, buf, (int)sizeof(buf));
    return buf[1];
}

/**
 * Derive the configured address width by reading SETUP_AW.
 * Terminates the process if SETUP_AW contains an invalid value.
 */
static uint8_t get_address_width(void)
{
    uint8_t buf[2];
    nrf24l01p_read_register(NRF24L01P_SETUP_AW, buf, (int)sizeof(buf));

    switch (buf[1] & 0x03u) {
        case NRF24L01P_AW_3_BYTES: return 3u;
        case NRF24L01P_AW_4_BYTES: return 4u;
        case NRF24L01P_AW_5_BYTES: return 5u;
        default:
            fprintf(stderr, "nrf24l01p: invalid SETUP_AW value: 0x%02X\n", buf[1]);
            exit(1);
    }
}

/**
 * Print a single-byte register in binary and hex format.
 *
 * @param reg_name Human-readable register name.
 * @param reg      Register address.
 */
static void print_reg(const char *reg_name, uint8_t reg)
{
    uint8_t val = debug_read_register(reg);
    char bin[9];
    uint8_to_bin_str(bin, val);
    printf("  %-20s  0b%s  (0x%02X)\n", reg_name, bin, val);
}

/**
 * Print a multi-byte pipe address register in hex format.
 *
 * @param pipe_name Human-readable pipe name.
 * @param reg       Register address.
 */
static void print_pipe_addr(const char *pipe_name, uint8_t reg)
{
    uint8_t len = get_address_width();
    uint8_t addr[5];   /* maximum address width */
    nrf24l01p_read_pipe_addr(reg, addr, len);

    printf("  %-20s  ", pipe_name);
    for (uint8_t i = 0u; i < len; i++) {
        printf("0x%02X ", addr[i]);
    }
    printf("\n");
}

/* --------------------------------------------------------------------------
 * Public API
 * -------------------------------------------------------------------------- */

void nrf_debug_print_CONFIG(void)    { print_reg("CONFIG",      NRF24L01P_CONFIG);      }
void nrf_debug_print_EN_AA(void)     { print_reg("EN_AA",       NRF24L01P_EN_AA);       }
void nrf_debug_print_EN_RXADDR(void) { print_reg("EN_RXADDR",   NRF24L01P_EN_RXADDR);   }
void nrf_debug_print_SETUP_AW(void)  { print_reg("SETUP_AW",    NRF24L01P_SETUP_AW);    }
void nrf_debug_print_SETUP_RETR(void){ print_reg("SETUP_RETR",  NRF24L01P_SETUP_RETR);  }
void nrf_debug_print_RF_CH(void)     { print_reg("RF_CH",       NRF24L01P_RF_CH);       }
void nrf_debug_print_RF_SETUP(void)  { print_reg("RF_SETUP",    NRF24L01P_RF_SETUP);    }
void nrf_debug_print_STATUS(void)    { print_reg("STATUS",      NRF24L01P_STATUS);      }
void nrf_debug_print_OBSERVE_TX(void){ print_reg("OBSERVE_TX",  NRF24L01P_OBSERVE_TX);  }
void nrf_debug_print_RPD(void)       { print_reg("RPD",         NRF24L01P_RPD);         }
void nrf_debug_print_FIFO_STATUS(void){ print_reg("FIFO_STATUS", NRF24L01P_FIFO_STATUS); }
void nrf_debug_print_DYNPD(void)     { print_reg("DYNPD",       NRF24L01P_DYNPD);       }
void nrf_debug_print_FEATURE(void)   { print_reg("FEATURE",     NRF24L01P_FEATURE);     }

void nrf_debug_print_RX_ADDR_PN(void)
{
    print_pipe_addr("RX_ADDR_P0", NRF24L01P_RX_ADDR_P0);
    print_pipe_addr("RX_ADDR_P1", NRF24L01P_RX_ADDR_P1);
    print_pipe_addr("RX_ADDR_P2", NRF24L01P_RX_ADDR_P2);
    print_pipe_addr("RX_ADDR_P3", NRF24L01P_RX_ADDR_P3);
    print_pipe_addr("RX_ADDR_P4", NRF24L01P_RX_ADDR_P4);
    print_pipe_addr("RX_ADDR_P5", NRF24L01P_RX_ADDR_P5);
}

void nrf_debug_print_TX_ADDR(void)
{
    print_pipe_addr("TX_ADDR", NRF24L01P_TX_ADDR);
}

void nrf_debug_print_RX_PW_PN(void)
{
    print_reg("RX_PW_P0", NRF24L01P_RX_PW_P0);
    print_reg("RX_PW_P1", NRF24L01P_RX_PW_P1);
    print_reg("RX_PW_P2", NRF24L01P_RX_PW_P2);
    print_reg("RX_PW_P3", NRF24L01P_RX_PW_P3);
    print_reg("RX_PW_P4", NRF24L01P_RX_PW_P4);
    print_reg("RX_PW_P5", NRF24L01P_RX_PW_P5);
}

void nrf_debug_print_all_reg(void)
{
    printf("--- NRF24L01+ Register Dump ---\n");
    nrf_debug_print_CONFIG();
    nrf_debug_print_EN_AA();
    nrf_debug_print_EN_RXADDR();
    nrf_debug_print_SETUP_AW();
    nrf_debug_print_SETUP_RETR();
    nrf_debug_print_RF_CH();
    nrf_debug_print_RF_SETUP();
    nrf_debug_print_STATUS();
    nrf_debug_print_OBSERVE_TX();
    nrf_debug_print_RPD();
    nrf_debug_print_RX_ADDR_PN();
    nrf_debug_print_TX_ADDR();
    nrf_debug_print_RX_PW_PN();
    nrf_debug_print_FIFO_STATUS();
    nrf_debug_print_DYNPD();
    nrf_debug_print_FEATURE();
    printf("--- End of Register Dump ---\n");
}
