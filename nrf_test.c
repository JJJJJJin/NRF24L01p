#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


void decomponent(uint8_t* data, uint64_t seq, uint8_t* p){
  uint8_t d[8];
  seq = 0;
  for(int i=0; i<16; i++){
    if(i<8){
      d[i] = data[i];
      seq |= ((uint64_t)data[i]) << (7-i);
    }else{
      p[i-8] = data[i];
    }
  }
  
}
