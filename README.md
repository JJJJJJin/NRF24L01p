# NRF24L01p

### Test Compile
For nrf main functionality test:
```sh
gcc -o nrf nrf24l01p.c nrf_debug.c -lwiringPi
```
For nrf prx functionality test:
```sh
gcc -DPRX=1 -fstack-protector-all -g -O0 -std=c99 -o prx nrf_prx.c nrf24l01p.c nrf_debug.c nrf_security.c nrf_test.c -lwiringPi
```
For nrd ptx functionality test:
```sh
gcc -fstack-protector-all -g -O0 -std=c99 -o ptx nrf_ptx.c nrf24l01p.c nrf_debug.c nrf_security.c -lwiringPi
```

