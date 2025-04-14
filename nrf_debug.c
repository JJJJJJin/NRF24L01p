#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "nrf_debug.h"
#include "nrf24l01p.h"

/**
 * @brief Convert uint8_t Integer to Binary form
 *
 * @param str The string to store the binary data, the size \
 * of string must be 8 bit
 * @param data The data to be converted
 */
void nrf_debug_uint8_to_bin(char* str, uint8_t data){
    for(int i=8; i>=0; i--){
        str[7-i] = (data>>i) & 1 ? '1' : '0' ;
    }
}

/**
 * @brief Read register for debugging
 *
 * @param reg The register to be read
 * @return data read from the device
 */
uint8_t nrf_debug_read_register(uint8_t reg){
    uint8_t data[2];
    nrf24l01p_read_register(reg, (char*) data, sizeof(data));

    return data[1]; // ignore the STATUS register
}

/**
 * @brief Print register data in a uniformed format
 *
 * @param reg_name The register name
 * @param reg The register to be read
 */
void nrf_debug_print_reg(char* reg_name, uint8_t reg){
    uint8_t data = nrf_debug_read_register(reg);

    char bin_str[8];
    nrf_debug_uint8_to_bin(bin_str, data);
    
    printf("**REGISTER %s:\t\t%s\n", reg_name, bin_str);
}


void nrf_debug_print_pipe_addr(char* pipe_name, uint8_t reg){
  uint8_t d[2];
  nrf24l01p_read_register(NRF24L01P_SETUP_AW, (char*) d, sizeof(d));
  
  uint8_t length;
  if(d[1] == 0b00000001) length = 3;
  else if(d[1] == 0b00000010) length = 4;
  else if(d[1] == 0b00000011) length = 5;
  else {
    fprintf(stderr, "NRF24L01P error: invalid pipeline addr length: length=%d\n", length);
    exit(1);
  }
  
  uint8_t data[length+1];
  nrf24l01p_read_pipeaddr(reg, data);
  printf("**REGISTER %s:\t\t", pipe_name);
  for(int i=1; i< length+1; i++) printf("0x%02X ", data[i]);
  printf("\n");
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_CONFIG(){
    nrf_debug_print_reg("CONFIG", 0x00);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_EN_AA(){
    nrf_debug_print_reg("EN_AA", 0x01);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_EN_RXADDR(){
    nrf_debug_print_reg("EN_RXADDR", 0x02);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_SETUO_AW(){
    nrf_debug_print_reg("SETUO_AW", 0x03);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_SETUP_RETR(){
    nrf_debug_print_reg("SETUP_RETR", 0x04);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_RF_CH(){
    nrf_debug_print_reg("RF_CH", 0x05);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_RF_SETUP(){
    nrf_debug_print_reg("RF_SETUP", 0x06);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_STATUS(){
    nrf_debug_print_reg("STATUS", 0x07);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_OBSERVE_TX(){
    nrf_debug_print_reg("OBSERVE_TX", 0x08);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_RPD(){
    nrf_debug_print_reg("RPD", 0x09);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_RX_ADDR_PN(){
    nrf_debug_print_pipe_addr("RX_ADDR_P0", 0x0A);
    nrf_debug_print_pipe_addr("RX_ADDR_P1", 0x0B);
    nrf_debug_print_pipe_addr("RX_ADDR_P2", 0x0C);
    nrf_debug_print_pipe_addr("RX_ADDR_P3", 0x0D);
    nrf_debug_print_pipe_addr("RX_ADDR_P4", 0x0E);
    nrf_debug_print_pipe_addr("RX_ADDR_P5", 0x0F);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_TX_ADDR(){
    nrf_debug_print_pipe_addr("TX_ADDR", 0x10);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_RX_PW_PN(){
    nrf_debug_print_reg("RX_PW_P0", 0x11);
    nrf_debug_print_reg("RX_PW_P1", 0x12);
    nrf_debug_print_reg("RX_PW_P2", 0x12);
    nrf_debug_print_reg("RX_PW_P3", 0x14);
    nrf_debug_print_reg("RX_PW_P4", 0x15);
    nrf_debug_print_reg("RX_PW_P5", 0x16);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_FIFO_STATUS(){
    nrf_debug_print_reg("FIFO_STATUS", 0x17);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_DYNPD(){
    nrf_debug_print_reg("DYNPD", 0x1c);
}

/**
 * @brief Debug to print register
 */
void nrf_debug_print_FEATURE(){
    nrf_debug_print_reg("FEATURE", 0x1D);
}

/**
 * @brief Debug to print all registers
 */
void nrf_debug_print_all_reg(){
  printf("*** Print all register ***\n");
  nrf_debug_print_CONFIG();
  nrf_debug_print_EN_AA();
  nrf_debug_print_EN_RXADDR();
  nrf_debug_print_SETUO_AW();
  nrf_debug_print_SETUP_RETR();
  nrf_debug_print_RF_CH();
  nrf_debug_print_RF_SETUP();
  nrf_debug_print_STATUS();
  nrf_debug_print_OBSERVE_TX();
  nrf_debug_print_RPD();
  nrf_debug_print_RX_ADDR_PN();
  nrf_debug_print_TX_ADDR();
  nrf_debug_print_RX_PW_PN();
  nrf_debug_print_FIFO_STATUS();
  nrf_debug_print_DYNPD();
  nrf_debug_print_FEATURE();
  printf("***              ***\n");

}
