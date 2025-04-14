#ifndef PRX_PINOUT_H
#define PRX_PINOUT_H

#ifdef SPI_ID
#undef SPI_ID
#endif
/* Set up SPI channel */
#define SPI_ID 0


#ifdef SPEED
#undef SPEED
#endif
/* Set up SPI speed */
#define SPEED 1000000

#ifdef IRQ_PIN
#undef IRQ_PIN
#endif
/* Set up IRQ_PIN */
#define IRQ_PIN  4
#ifdef CE_PIN
#undef CE_PIN
#endif
/* Set up CE_PIN */
#define CE_PIN 22

#ifdef CSN_PIN
#undef CSN_PIN
#endif
/* Set up CSN_PIN */
#define CSN_PIN 27

#endif
