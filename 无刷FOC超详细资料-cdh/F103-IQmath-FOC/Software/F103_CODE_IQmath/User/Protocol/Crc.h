#ifndef Crc_h
#define Crc_h

#include "stdint.h"
unsigned short iot_calculate_crc16(unsigned char *data,unsigned int size,const unsigned short crc_poly,unsigned short init_value,unsigned char ref_flag);
//这里的多项式X^16+X^12+X^5+1怎么转成0x1021的呢，(1 << 16) | (1 << 12) | (1 << 5) | 1 = 0x1021
// 按CRC所用协议直接调用如下宏函数即可，这里只列出部分常用的
#define IOT_CRC16_CCITT_FALSE(data,size) iot_calculate_crc16(data,size,0x1021,0xFFFF,0)
#define IOT_CRC16_CCITT(data,size) iot_calculate_crc16(data,size,0x1021,0x0000,1)
#define IOT_CRC16_MODBUS(data,size) iot_calculate_crc16(data,size,0x8005,0xFFFF,1)
#define IOT_CRC16_XMODEM(data,size) iot_calculate_crc16(data,size,0x1021,0x0000,0)
#define IOT_CRC16_YMODEM(data,size) iot_calculate_crc16(data,size,0x1021,0x0000,0)

#endif /* Crc_h */
