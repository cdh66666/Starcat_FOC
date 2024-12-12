/***********************************************************************
 * @file Motor1SPI1.h
 * Description:
 *
 **********************************************************************/
#ifndef Motor1SPI1_h
#define Motor1SPI1_h

#include "stdint.h"


uint16_t Motor1SPI1WriteReadData(uint16_t dataW);
void Motor1SPI1WriteData(uint16_t data);
uint16_t Motor1SPI1ReadData(void);
void Motor1SPI1SetCS(uint8_t leave);


#endif // __Motor1SPI1_h__
