/**
 * @file nrf_test.h
 * @brief Integration test interface for the nRF24L01+ driver.
 *
 * Include this header in nrf_test.c only.  The test binary is built with
 * -DPRX=1 -DNRF_TEST_PRX=1 for the PRX role (see Makefile).
 */

#ifndef NRF_TEST_H
#define NRF_TEST_H

#include "nrf24l01p.h"
#include "nrf_prx.h"
#include "nrf_ptx.h"
#include "nrf_debug.h"

#endif /* NRF_TEST_H */
