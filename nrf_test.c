/**
 * @file nrf_test.c
 * @brief Integration test for the nRF24L01+ driver.
 *
 * PRX test: listens for packets and reports receive rate and payload
 * correctness against a known reference payload.
 *
 * PTX test: not yet implemented.
 *
 * Build flags:
 *   PRX binary  -> -DPRX=1 -DNRF_TEST_PRX=1
 *   PTX binary  -> (no additional flags)
 *
 * See the Makefile for the recommended build commands.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "nrf_config.h"
#include "nrf_test.h"

#ifndef NRF_TEST_PRX
#define NRF_TEST_PRX 0
#endif

/* Total number of packets expected from the PTX transmit sequence */
#define NRF_TEST_TOTAL_PACKETS  0xFFFFu

/* Reference payload content used by the PTX (bytes 2 onward) */
static const uint8_t REFERENCE_PAYLOAD[NRF_PAYLOAD_WIDTH] =
    "aaaaaaaabbbbbbbbccccccccdddddddd";

/* --------------------------------------------------------------------------
 * PRX test state (updated from ISR context)
 * -------------------------------------------------------------------------- */
static volatile int g_packets_received  = 0;
static volatile int g_packets_valid     = 0;
static volatile int g_packets_corrupted = 0;

/* --------------------------------------------------------------------------
 * PRX ISR
 * -------------------------------------------------------------------------- */

static void test_prx_callback(void)
{
    uint8_t data[NRF_PAYLOAD_WIDTH + 1u];
    nrf24l01p_read_rx_payload(NRF24L01P_RX_PW_P0, data);

    /* data[0] = STATUS, data[1..2] = sequence number (big-endian uint16) */
    uint16_t seq = ((uint16_t)data[1] << 8) | (uint16_t)data[2];

    g_packets_received++;

    /* Validate payload content (skip STATUS byte and 2-byte sequence field) */
    uint8_t rx_payload[NRF_PAYLOAD_WIDTH - 2u];
    memcpy(rx_payload, data + 3, sizeof(rx_payload));

    uint8_t ref_payload[NRF_PAYLOAD_WIDTH - 2u];
    memcpy(ref_payload, REFERENCE_PAYLOAD + 2, sizeof(ref_payload));

    if (memcmp(ref_payload, rx_payload, sizeof(ref_payload)) != 0) {
        fprintf(stderr,
                "payload corruption detected: seq=%u expected=\"%.*s\" received=\"%.*s\"\n",
                (unsigned int)seq,
                (int)sizeof(ref_payload), ref_payload,
                (int)sizeof(rx_payload),  rx_payload);
        g_packets_corrupted++;
    } else {
        g_packets_valid++;
    }

    nrf24l01p_flush_rx();
    nrf24l01p_write_register(NRF24L01P_STATUS, NRF24L01P_STATUS_RX_DR);
}

static void test_prx_print_stats(void)
{
    int received  = g_packets_received;
    int valid     = g_packets_valid;
    int corrupted = g_packets_corrupted;

    printf("packets received  : %d / %u\n", received,  NRF_TEST_TOTAL_PACKETS);
    printf("packets valid     : %d\n", valid);
    printf("packets corrupted : %d\n", corrupted);
    if (received > 0) {
        printf("correctness rate  : %.6f\n", (double)valid     / received);
        printf("receive rate      : %.6f\n", (double)received  / NRF_TEST_TOTAL_PACKETS);
    }
    printf("---\n");
}

static void test_prx_listen(void)
{
    wiringPiISR(IRQ_PIN, INT_EDGE_FALLING, &test_prx_callback);
    digitalWrite(CE_PIN, HIGH);

    while (true) {
        sleep(5);
        test_prx_print_stats();
    }
}

/* --------------------------------------------------------------------------
 * Test entry points
 * -------------------------------------------------------------------------- */

static int test_prx(void)
{
    nrf_prx_setup();
    test_prx_listen();
    return 0;
}

static int test_ptx(void)
{
    /* PTX integration test is not yet implemented */
    fprintf(stderr, "nrf_test: PTX test is not implemented\n");
    return 1;
}

int main(void)
{
    if (NRF_TEST_PRX) {
        return test_prx();
    } else {
        return test_ptx();
    }
}
