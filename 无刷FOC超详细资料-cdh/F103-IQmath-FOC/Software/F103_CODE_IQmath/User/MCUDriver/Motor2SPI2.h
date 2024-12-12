#ifndef Motor2SPI2_h
#define Motor2SPI2_h

#include "stdint.h"


void Motor2SPI2WriteData(uint16_t data);
uint16_t Motor2SPI2ReadData(void);
void Motor2SPI2SetCS(uint8_t leave);


#endif /* Motor2SPI2_h */
