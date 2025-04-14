#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "nrf_test.h"

#ifndef NRF_TEST_PRX
#define NRF_TEST_PRX 0
#endif

#define NRF_PRX_RX_PW 32
#define NRF_TEST_TOTAL_DATA_NUM 0xffff


void byte2bin(uint8_t num) {
    for (int i = sizeof(uint8_t) * 8 - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
}

int DR_num = 0; // data received number
int DR_valid_num = 0;
int DR_corrupted_num = 0;
int pre_seq = 0;
uint8_t payload[NRF_PRX_RX_PW] = "aaaaaaaabbbbbbbbccccccccdddddddd";

void test_prx_callback(){
  uint8_t data[NRF_PRX_RX_PW+1];
  nrf24l01p_read_rx_payload((NRF24L01P_RX_PW_P0), data);
  DR_num++;

  printf("Shoud have %d, Data Received: %d\n",0xffff, DR_num);

  uint16_t seq = 0;
  seq = ((uint16_t) data[1]) << 8 | (uint16_t) data[2];

  // perfrom data validation and loss rate calculation
  // seq validataion
  /*
  if(seq != pre_seq+1){
    printf("Sequence missing, should receive %d but receive %d\n", pre_seq+1, seq);
  }
  pre_seq = seq;
  */

  // payload validation
  uint8_t tmp1[NRF_PRX_RX_PW-1];
  strncpy(tmp1, payload+2, sizeof(tmp1)-1);
  tmp1[NRF_PRX_RX_PW-2] = '\0';
  uint8_t tmp2[NRF_PRX_RX_PW-1];
  strncpy(tmp2, data+3, sizeof(tmp1)-1);
  tmp2[NRF_PRX_RX_PW-2] = '\0';
  if(strcmp(tmp1, tmp2) != 0){
    printf("Payload corrupted detected\n");
    printf("Should have %s        \n", tmp1);
    printf("But had     %s        \n", tmp2);
    DR_corrupted_num++;
  }else{
    DR_valid_num++;
  }

  // receive rate
  printf(" ");
  //printf("\rTotal Data Num:        %d             \n", seq);
  printf("\rData Received:         %d             \n", DR_num);
  printf("\rValid Data:            %d             \n", DR_valid_num);
  //printf("\rLost Data:             %d             \n", seq - DR_num);
  printf("\rCorrupted Data:        %d             \n", DR_corrupted_num);
  printf("\r==========                            \n");
  // correctness of data payload OR 1 - corruption rate
  printf("\rCorrectness Rate:      %.8f           \n", (double)DR_valid_num/DR_num);
  // recieved / num_should_received OR 1 - loss rate
  printf("\rReceived Rate:         %.8f           \n", (double) DR_num/0xffff);
  printf("\033[6A");


  // printf("seq: %d: ", seq);
  // // ignore the first 2 bytes
  // for(int i=3; i<sizeof(data); i++){
  //   printf("%c", data[i]);
  // }
  // printf("\n");

  nrf24l01p_flush_rx();
  nrf24l01p_write_register(NRF24L01P_STATUS, 0b01000000);
}

void test_prx_listen(){
  wiringPiISR(IRQ_PIN, INT_EDGE_FALLING, &test_prx_callback);
  digitalWrite(CE_PIN, HIGH);
  
  while(TRUE){}
}

/* Testing code of PTX */
/* Test ptx by sending sequence number with data payload, which shares identical content with PRX for evaluating the data reeive/loss rate */
/* Not implemented yet*/
int test_ptx(){
  return 0;
}

/* Testing code of PRX */
/* Test prx by evaluating the received data payload and calculate the data receive/loss rate */
int test_prx(){
  nrf_prx_setup();
  test_prx_listen();

  return 0;
}



int main(){
  if(NRF_TEST_PRX){
    test_prx();
  }else{
    test_ptx();
  }
  
  
  return 0;
}