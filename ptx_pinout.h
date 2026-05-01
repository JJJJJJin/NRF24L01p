/**
 * @file ptx_pinout.h
 * @brief GPIO and SPI pin assignments for the PTX (Primary Transmitter) role.
 *
 * All pin numbers use the BCM (Broadcom) numbering scheme as required by
 * wiringPiSetupGpio().  Adjust to match your wiring before building.
 */

#ifndef PTX_PINOUT_H
#define PTX_PINOUT_H

/* SPI peripheral index: 0 = /dev/spidev0.x, 1 = /dev/spidev1.x */
#define SPI_ID   0

/* SPI clock frequency in Hz */
#define SPEED    1000000

/* BCM pin connected to the module IRQ line (active-low, input) */
#define IRQ_PIN  4

/* BCM pin connected to the module CE (Chip Enable) line (output) */
#define CE_PIN   22

/* BCM pin connected to the module CSN (Chip Select, active-low) (output) */
#define CSN_PIN  27

#endif /* PTX_PINOUT_H */
