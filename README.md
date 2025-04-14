# NRF24L01p
This repository aims to develop the simple usage of NRF24L01+, the wireless communication module. The code implements the basic usage of NRF24L01+.

_Please noted that this repository does **NOT** include the ESB functionality._

## Introduction
This project is developed with Raspiberry Pi 4B, using [WiringPi](https://github.com/WiringPi/WiringPi) to perform SPI interaction.

### Code Introduction
The file strucure includes:
- [nrf24l01p.c](https://github.com/JJJJJJin/NRF24L01p/blob/dev/nrf24l01p.c) -> the basic functions to set up device
- [nrf_prx.c](https://github.com/JJJJJJin/NRF24L01p/blob/dev/nrf_prx.c) / [nrf_ptx.c](https://github.com/JJJJJJin/NRF24L01p/blob/dev/nrf_ptx.c) -> the setup of Primary Receiver device and Primary Transimitter device
- [nrf_debug.c](https://github.com/JJJJJJin/NRF24L01p/blob/dev/nrf_debug.c) -> the basic functions to print out values in all registers, aiming to capture device status during debugging
- [nrf_security.c](https://github.com/JJJJJJin/NRF24L01p/blob/dev/nrf_security.c) -> the basic functions of data security, currently no implementation


## Test and Product
The file [nrf_test](https://github.com/JJJJJJin/NRF24L01p/blob/dev/nrf_test.c) is used to set up all product test details. 

To set up as **PTX**, compiling as
```sh
gcc -fstack-protector-all -g -O0 -std=c99 -o ptx nrf_ptx.c nrf24l01p.c nrf_debug.c nrf_security.c -lwiringPi
```

To set up as **PRX**, compiling as 
```sh
gcc -DPRX=1 -fstack-protector-all -g -O0 -std=c99 -o prx nrf_prx.c nrf24l01p.c nrf_debug.c nrf_security.c nrf_test.c -lwiringPi
```

