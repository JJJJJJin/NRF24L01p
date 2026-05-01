# NRF24L01+

A C driver library for the Nordic nRF24L01+ 2.4 GHz wireless transceiver, targeting **Raspberry Pi 4B** with [WiringPi](https://github.com/WiringPi/WiringPi) as the SPI and GPIO backend.

The library implements **ShockBurst** mode only.  Enhanced ShockBurst (Auto-Acknowledge, Acknowledge Payload, etc.) is not implemented.

---

## Repository structure

| File | Description |
|------|-------------|
| `nrf_config.h` | Shared link configuration (RF channel, payload width, pipe address) |
| `nrf24l01p.h/.c` | Hardware abstraction layer: SPI/GPIO init, register I/O, FIFO access |
| `nrf_ptx.h/.c` | Primary Transmitter (PTX) role setup and transmit API |
| `nrf_prx.h/.c` | Primary Receiver (PRX) role setup and ISR-driven receive API |
| `nrf_debug.h/.c` | Register dump utilities for bring-up and debugging |
| `nrf_security.h/.c` | Payload encryption/decryption stubs (unimplemented) |
| `nrf_test.h/.c` | On-device integration test (PRX receive rate and correctness) |
| `ptx_pinout.h` | GPIO and SPI pin assignments for the PTX board |
| `prx_pinout.h` | GPIO and SPI pin assignments for the PRX board |

---

## Configuration

Before building, open `nrf_config.h` and verify the following constants match on **both** nodes:

```c
#define NRF_RF_CHANNEL    100u   /* RF channel: F0 = 2400 + this value [MHz] */
#define NRF_PAYLOAD_WIDTH  32u   /* Static payload width in bytes (1-32)      */
#define NRF_PIPE0_ADDR    { 0xE7u, 0xD3u, 0xF0u, 0x35u, 0x77u }
```

Then open `ptx_pinout.h` and `prx_pinout.h` and set the BCM GPIO pin numbers to match your wiring.

---

## Build

```sh
make ptx    # builds the PTX binary
make prx    # builds the PRX binary (includes integration test)
make clean
```

Both binaries require WiringPi to be installed.  See [INSTALL.md](INSTALL.md) for setup instructions.

---

## Usage

Run with root privileges (required for GPIO and SPI access):

```sh
sudo ./ptx
sudo ./prx
```

To dump all registers at startup for verification, call `nrf_debug_print_all_reg()` after `nrf_ptx_setup()` or `nrf_prx_setup()`.

---

## Integration test (PRX)

The PRX binary runs a packet receive test that reports:

- Total packets received vs. expected
- Payload correctness rate
- Overall receive rate

Statistics are printed to stdout every 5 seconds.  The PTX test is not yet implemented.
