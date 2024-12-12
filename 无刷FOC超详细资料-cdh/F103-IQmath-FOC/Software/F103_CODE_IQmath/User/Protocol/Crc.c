#include "Crc.h"


static void invert_uint8(unsigned char *dstBuf, unsigned char *srcBuf)
{
    unsigned char i,tmp = 0;
    for ( i = 0; i < 8; i++) {
        if (srcBuf[0] & (1 << i)){
            tmp |= 1 << (7-i);
        }
    }
    *dstBuf = tmp;
}
static void invert_uint16(unsigned short *dstBuf, unsigned short *srcBuf)
{
    unsigned short i,tmp = 0;
    for ( i = 0; i < 16; i++){
        if (srcBuf[0] & (1 << i)){
            tmp |= 1 << (15-i);
        }
    }
    *dstBuf = tmp;
}
/*****************************************************************
 * @Function: iot_calculate_crc16
 * @Description: 计算CRC16值的算法实现，适配所有CRC16的协议
 * @Param: ref_flag,数据位序翻转，true-低位在前，高位在后
 * @Return: void
 *****************************************************************/
unsigned short iot_calculate_crc16(unsigned char *data,unsigned int size,const unsigned short crc_poly,unsigned short init_value,unsigned char ref_flag)
{
    unsigned short crc_reg = init_value,tmp = 0;
    unsigned char j,byte = 0;

    while (size--){
        byte = *(data++);
        if (ref_flag)
            invert_uint8(&byte,&byte);
        crc_reg ^= byte << 8;
        for ( j = 0; j < 8; j++){
            tmp = crc_reg & 0x8000;
            crc_reg <<= 1;
            if (tmp)
                crc_reg ^= crc_poly;
        }
    }

    if (ref_flag)
        invert_uint16(&crc_reg,&crc_reg);

    // 这里XOROUT数值固定为0x0000
    return crc_reg;  // 等价于 return (crc_reg ^ 0X0000);
}
