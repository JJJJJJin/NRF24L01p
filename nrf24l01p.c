
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "nrf24l01p.h"

/**
 * Function: Initialise GPIO setting, especially for CSN, CE and IRQd[1]
 */
void nrf24l01p_gpio_init() {
    if(wiringPiSetupGpio() == -1){
      fprintf(stderr, "GPIO init failed\n");
      exit(1);
    }
    
    pinMode(CE_PIN, OUTPUT);
    pinMode(CSN_PIN, OUTPUT);
    pinMode(IRQ_PIN, INPUT);

    digitalWrite(CE_PIN, LOW);
    digitalWrite(CSN_PIN, HIGH);  // CSN_PIN is low-active
}

/**
 * @brief Inistialise the SPI communication
 *
 * @param SPI SPI channel, 0 for spi0, 1 for spi 1
 */
void nrf24l01p_spi_init(int spi, int speed){
  if(wiringPiSPISetup(spi, speed) == -1){
    fprintf(stderr, "SPI init failed\n");
    exit(1);
  }
}

/**
 * @brief Read data from device through SPI
 *
 * @param reg The register address to be read
 * @param data Read the data from the device and store into this \
 * parameter, note that the first 8-bit data is always from the \
 * NRF24L01P_STATUS register. The length of data must be more than \
 * 2
 * @param size The size in byte of data wish to be read from device. \
 * Note that the size must be 1 more than the actual size wished to \
 * be read. For example, if reading only 8 bits (1 byte) from \
 * register 0x00, the size here should 2 instead.
 */
void nrf24l01p_read_register(uint8_t reg, uint8_t* data, int size){
  data[0] = NRF24L01P_READ_REGISTER | reg;
  
  digitalWrite(CSN_PIN, LOW);
  wiringPiSPIDataRW(SPI_ID, (char*) data, size);
  digitalWrite(CSN_PIN, HIGH);
}

/**
 * @brief Write data to the device
 *
 * @param reg The register to write data in.
 * @param value The value to be written into the register
 */
void nrf24l01p_write_register(uint8_t reg, uint8_t value){
  uint8_t data[2];
  data[0] = NRF24L01P_WRITE_REGISTER | reg;
  data[1] = value;
  
  digitalWrite(CSN_PIN, LOW);
  wiringPiSPIDataRW(SPI_ID, (char*) data, sizeof(data));
  digitalWrite(CSN_PIN, HIGH);
}

/**
 * @brief Read pipeline address from device
 *
 * @param reg The register to be written
 * @param addr The address value, address must be either 4, 5 or 6 \ 
 * bytes long (always 1 more byte)
 */
void nrf24l01p_read_pipeaddr(uint8_t reg, uint8_t* addr){
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
  
  addr[0] = NRF24L01P_READ_REGISTER | reg;
  digitalWrite(CSN_PIN, LOW);
  wiringPiSPIDataRW(SPI_ID, (char*) addr, length+1);
  digitalWrite(CSN_PIN, HIGH);
}

/**
 * @brief Write pipeline address (3-byte long) into device
 *
 * @param reg The register to be written
 * @param addr The address value, the value must be 3 bytes long
 */
void nrf24l01p_write_pipeaddr_3_byte(uint8_t reg, uint8_t* addr){
  uint8_t d[2];
  nrf24l01p_read_register(NRF24L01P_SETUP_AW, (char*) d, sizeof(d));
  
  uint8_t length;
  if(d[1] == 0b00000001) length = 3;
  else if(d[1] == 0b00000010) length = 4;
  else if(d[1] == 0b00000011) length = 5;
  else {
    fprintf(stderr, "NRF24L01P error: invalid pipeline addr length: length=%d\n", d[1]);
    exit(1);
  }
  
  uint8_t data[length+1];
  for(int i=0; i<length; i++){
    if(i==0) data[i] = NRF24L01P_WRITE_REGISTER | reg;
    else data[i] = addr[i-1];
  }
  digitalWrite(CSN_PIN, LOW);
  wiringPiSPIDataRW(SPI_ID, (char*) data, sizeof(data));
  digitalWrite(CSN_PIN, HIGH);
}

/**
 * @brief Write pipeline address (4-byte long) into device
 *
 * @param reg The register to be written
 * @param addr The address value, the value must be 4 bytes long
 */
void nrf24l01p_write_pipeaddr_4_byte(uint8_t reg, uint8_t* addr){
  uint8_t d[2];
  nrf24l01p_read_register(NRF24L01P_SETUP_AW, (char*) d, sizeof(d));
  
  uint8_t length;
  if(d[1] == 0b00000001) length = 3;
  else if(d[1] == 0b00000010) length = 4;
  else if(d[1] == 0b00000011) length = 5;
  else {
    fprintf(stderr, "NRF24L01P error: invalid pipeline addr length: length=%d\n", d[1]);
    exit(1);
  }
  
  uint8_t data[length+1];
  for(int i=0; i<length; i++){
    if(i==0) data[i] = NRF24L01P_WRITE_REGISTER | reg;
    else data[i] = addr[i-1];
  }
  digitalWrite(CSN_PIN, LOW);
  wiringPiSPIDataRW(SPI_ID, (char*) data, sizeof(data));
  digitalWrite(CSN_PIN, HIGH);
}

/**
 * @brief Write pipeline address (5-byte long) into device
 *
 * @param reg The register to be written
 * @param addr The address value, the value must be 5 bytes long
 */
void nrf24l01p_write_pipeaddr_5_byte(uint8_t reg, uint8_t* addr){
  uint8_t d[2];
  nrf24l01p_read_register(NRF24L01P_SETUP_AW, (char*) d, sizeof(d));
  
  uint8_t length;
  if(d[1] == 0b00000001) length = 3;
  else if(d[1] == 0b00000010) length = 4;
  else if(d[1] == 0b00000011) length = 5;
  else {
    fprintf(stderr, "NRF24L01P error: invalid pipeline addr length: length=%d\n", d[1]);
    exit(1);
  }
  
  uint8_t data[length+1];
  for(int i=0; i<length+1; i++){
    if(i==0) {
      data[i] = NRF24L01P_WRITE_REGISTER | reg;
    }else {
      data[i] = addr[i-1];
    }
  }

  digitalWrite(CSN_PIN, LOW);
  wiringPiSPIDataRW(SPI_ID, (char*) data, sizeof(data));
  digitalWrite(CSN_PIN, HIGH);
}

/**
 * @brief Read RX FIFO from payload
 *
 * @param pipeline_number The RX FIFO pipeline addr to be read
 * @param buffer The buffer to store message. The length of \
 * buffer must be 1 more byte to NRF24L01P_RX_PW_PN (defined payload \ 
 * width of the pipeline number).
 */
void nrf24l01p_read_rx_payload(uint8_t pipeline_number, uint8_t* buffer){
  uint8_t length[2];
  nrf24l01p_read_register(pipeline_number, length, sizeof(length));
  
  buffer[0] = NRF24L01P_R_RX_PAYLOAD;
  // read payload
  digitalWrite(CSN_PIN, LOW);
  wiringPiSPIDataRW(SPI_ID, (char*) buffer, length[1]);
  digitalWrite(CSN_PIN, HIGH);
}

/**
 * @brief Write data into TX FIFO
 *
 * @param data Data to be written, the length must be equal to the \
 * PRX's pipeline width
 * @param size Size of data to be written
 */
void nrf24l01p_write_tx_payload(uint8_t* data, uint8_t size){
  uint8_t buffer[size+1];
  buffer[0] = (uint8_t) NRF24L01P_W_TX_PAYLOAD;
  for(int i=1; i<size+1; i++){
    buffer[i] = data[i-1];
  }
  
  digitalWrite(CSN_PIN, LOW);
  wiringPiSPIDataRW(SPI_ID, (char*) buffer, size+1);
  digitalWrite(CSN_PIN, HIGH);
}

/**
 * @brief Flush RX FIFO
 */
void nrf24l01p_flush_rx(){
  uint8_t data[2];
  data[0] = NRF24L01P_FLUSH_RX;
  
  digitalWrite(CSN_PIN, LOW);
  wiringPiSPIDataRW(SPI_ID, (char*) data, 1);
  digitalWrite(CSN_PIN, HIGH);
}

/**
 * @brief Flush TX FIFO
 */
void nrf24l01p_flush_tx(){
  uint8_t data[2];
  data[0] = NRF24L01P_FLUSH_TX;
  digitalWrite(CSN_PIN, LOW);
  wiringPiSPIDataRW(SPI_ID, (char*) data, 1);
  digitalWrite(CSN_PIN, HIGH);
}

