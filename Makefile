CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra -Wpedantic -fstack-protector-strong -g -O0
LDFLAGS := -lwiringPi

COMMON_SRCS := nrf24l01p.c nrf_debug.c nrf_security.c

.PHONY: all ptx prx clean

all: ptx prx

## Build the Primary Transmitter binary.
ptx: $(COMMON_SRCS) nrf_ptx.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

## Build the Primary Receiver binary (includes integration test).
prx: $(COMMON_SRCS) nrf_prx.c nrf_test.c
	$(CC) $(CFLAGS) -DPRX=1 -DNRF_TEST_PRX=1 -o $@ $^ $(LDFLAGS)

clean:
	rm -f ptx prx *.o
