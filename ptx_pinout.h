#ifndef PTX_PINOUT_H
#define PTX_PINOUT_H

#ifdef SPI_ID
#undef SPI_ID
#endif
/* Set up SPI channel */
#define SPI_ID 1 // use spi1 for ptx

#ifdef SPEED
#undef SPEED
#endif
/* Set up SPI speed */
#define SPEED 1000000

#ifdef IRQ_PIN
#undef IRQ_PIN
#endif
/* Set up IRQ_PIN */
#define IRQ_PIN  6

#ifdef CE_PIN
#undef CE_PIN
#endif
/* Set up CE_PIN */
#define CE_PIN 26

#ifdef CSN_PIN
#undef CSN_PIN
#endif
/* Set up CSN_PIN */
#define CSN_PIN 13

#endif
