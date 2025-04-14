/**
 * This library is developed especially for MUR 2024 project.
 * This library uses the ShockBurst functionality of the \
 * device NRF24L01+ for achieving the wireless communication.
 *
 * This library develops the basic functionality for the Primary \
 * Receiver device (PRX), without using the functionalities including \
 * Auto-Acknowledge, Acknowledge Payload and so on. More \
 * functionality can be added in as the requirements increase.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "nrf_prx.h"

#define NRF_PRX_PIPELINE_0 {0xE7, 0xD3, 0xF0, 0x35, 0x77}
#define NRF_PRX_RX_PW 32 // set up the length of pipeline to be 32byte

void nrf_prx_setup(){
  nrf24l01p_spi_init(SPI_ID, SPEED);
  nrf24l01p_gpio_init();
  
  digitalWrite(CE_PIN, LOW);
  
  // Set register CONFIG with PWR_UP=1, PRIM_RX=1
  nrf24l01p_write_register(NRF24L01P_CONFIG, 0b00001011);
  nrf24l01p_write_register(NRF24L01P_EN_AA, 0b00000000);
  nrf24l01p_write_register(NRF24L01P_EN_RXADDR, 0b00000011);
  nrf24l01p_write_register(NRF24L01P_SETUP_AW, 0b00000011);
  nrf24l01p_write_register(NRF24L01P_SETUP_RETR, 0b00000011);
  // F0 = 2400 + RF_CH[MHz], 2.4GHz <= F0 <= 2.525GHz
  nrf24l01p_write_register(NRF24L01P_RF_CH, 120);
  nrf24l01p_write_register(NRF24L01P_RF_SETUP, 0b00001110);
  
  // set up 5-byte pipeline addr
  uint8_t addr[5] = NRF_PRX_PIPELINE_0;
  nrf24l01p_write_pipeaddr_5_byte(NRF24L01P_RX_ADDR_P0, addr);
  nrf24l01p_write_register(NRF24L01P_RX_PW_P0, NRF_PRX_RX_PW);
  nrf24l01p_write_register(NRF24L01P_FEATURE, 0b00000000);
  
  // flush RX_FIFO, reset STATUS
  nrf24l01p_flush_rx();
  nrf24l01p_write_register(NRF24L01P_STATUS, 0b01110000);
}

int count = 0;

void nrf_prx_data_handler(uint8_t* data, int size){

  uint8_t info[size];

  // retrieve sequence number
  uint16_t seq = ((uint16_t) data[1]) << 8 | (uint16_t) data[2];
  
  printf("***count: %d\tSeq %d\tMessage:", count++, seq);
  // ignore the first 2 bytes
  for(int i=3; i<size; i++){
    printf("%c", data[i]);
  }
  printf("\n");

}

void callback(){
  // uint8_t data[NRF_PRX_RX_PW+1];
  uint8_t data[100];
  //read payload from pipeline 0
  nrf24l01p_read_rx_payload((NRF24L01P_RX_PW_P0), data);
  
  // nrf_prx_data_handler(data, sizeof(data));
  nrf_prx_data_handler(data, NRF_PRX_RX_PW);

  // clear IRQ and flush RX FIFO
  nrf24l01p_flush_rx();
  nrf24l01p_write_register(NRF24L01P_STATUS, 0b01000000);
}

void nrf_prx_listen(){
  wiringPiISR(IRQ_PIN, INT_EDGE_FALLING, &callback);
  digitalWrite(CE_PIN, HIGH);
  
  while(TRUE){}
}

/*
int main(){
  nrf_prx_setup();
  // nrf_debug_print_all_reg();
  nrf_prx_listen();
  return 0;
}
*/