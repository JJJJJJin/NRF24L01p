/**
 * This library is developed especially for MUR 2024 project.
 * This library uses the ShockBurst functionality of the \
 * device NRF24L01+ for achieving the wireless communication.
 *
 * This library develops the basic functionality for the Primary \
 * Transmitter device (PTX), without using the functionalities \ 
 * including Auto-Acknowledge, Acknowledge Payload and so on. More \
 * functionality can be added in as the requirements increase.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "nrf_ptx.h"

// Must be identical
#define NRF_PTX_PIPELINE_TX {0xE7, 0xD3, 0xF0, 0x35, 0x77}
#define NRF_PTX_PIPELINE_0 {0xE7, 0xD3, 0xF0, 0x35, 0x77}

// Must be equal to PRX's PW
#define NRF_PTX_TX_PW 32

void nrf_ptx_setup(){
  nrf24l01p_spi_init(SPI_ID, SPEED);
  nrf24l01p_gpio_init();
  
  digitalWrite(CE_PIN, LOW);
  
  // Set register CONFIG with PWR_UP=1, PRIM_RX=0
  nrf24l01p_write_register(NRF24L01P_CONFIG, 0b00000010);
  nrf24l01p_write_register(NRF24L01P_EN_AA, 0b00000000);
  nrf24l01p_write_register(NRF24L01P_EN_RXADDR, 0b00000001);
  nrf24l01p_write_register(NRF24L01P_SETUP_AW, 0b00000011);
  nrf24l01p_write_register(NRF24L01P_SETUP_RETR, 0b00000000);
  // F0 = 2400 + RF_CH[MHz], 2.4GHz <= F0 <= 2.525GHz
  nrf24l01p_write_register(NRF24L01P_RF_CH, 100); // 
  nrf24l01p_write_register(NRF24L01P_RF_SETUP, 0b00100110);
  
  // set up 5-byte pipeline addr
  uint8_t addr_tx[5] = NRF_PTX_PIPELINE_TX;
  uint8_t addr_p0[5] = NRF_PTX_PIPELINE_0;
  nrf24l01p_write_pipeaddr_5_byte(NRF24L01P_TX_ADDR, addr_tx);
  nrf24l01p_write_pipeaddr_5_byte(NRF24L01P_RX_ADDR_P0, addr_p0);
  nrf24l01p_write_register(NRF24L01P_FEATURE, 0b00000000);
  
  // flush RX_FIFO, reset STATUS
  nrf24l01p_flush_tx();
  nrf24l01p_write_register(NRF24L01P_STATUS, 0b01110000);
}


void nrf_ptx_send(uint8_t* data, int size){
  char msg[NRF_PTX_TX_PW];
  
  if(!digitalRead(IRQ_PIN)){
    nrf24l01p_flush_tx();
    nrf24l01p_write_register(NRF24L01P_STATUS, 0b00100000);
  }

  printf("message: %s\n", (char*) data);
  nrf24l01p_write_tx_payload(data, size);
  
  // send data
  digitalWrite(CE_PIN, HIGH);
  sleep(0.00001); // 10us
  digitalWrite(CE_PIN, LOW);

}
  
/*
int main(){
  nrf_ptx_setup();
  
  uint8_t data[32] = "aaaaaaaabbbbbbbbccccccccdddddddd";  // payload
  uint16_t seq = 0;
  while(seq < 0x00ff){
      // add seq to data, taking data[0] and data[1]
      uint16_t seq_tmp = seq;
      data[0] = (uint8_t) (seq_tmp >> 8);
      seq_tmp = seq;
      data[1] = (uint8_t) seq_tmp;

      printf("data0: %u", data[0]);
      printf("\tdata1: %u\n", data[1]);

      nrf_ptx_send(data, sizeof(data));
      sleep(0.01);
      seq ++;
  }

  
  // for testing purpose
  // nrf_debug_print_all_reg();
  
  // // DONT execute sending loop, only for testing config
  // while(1){
  //   nrf_ptx_send(data, sizeof(data));
  //   sleep(0.01);
  // }

  return 0;
}
*/
