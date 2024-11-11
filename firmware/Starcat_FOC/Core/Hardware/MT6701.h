#ifndef HARDWARE_MT6701_H_
#define HARDWARE_MT6701_H_

#include "main.h"

// 定义磁编码器结构体
typedef struct {
    GPIO_TypeDef* CS_GPIO_Port;
    uint16_t CS_Pin;
    GPIO_TypeDef* SCK_GPIO_Port;
    uint16_t SCK_Pin;
    GPIO_TypeDef* MOSI_GPIO_Port;
    uint16_t MOSI_Pin;
    float rotation_count;		//转一圈计数值
    float full_rotation_offset;	//总便宜圈数
    float rad;			//当前弧度
    float rad_prev;		//上次弧度
    float rad_all_prev;	//上次转过总弧度
    float rad_all;		//转过总弧度
    float d_rad;		//d_rad是两次转动弧度差，乘以频率就是电机转速 rad/s
} MT6701_Encoder;

// 定义两个编码器实例
extern MT6701_Encoder encoder1;
extern MT6701_Encoder encoder2;

#define MT_CPR      16384.0f   // 磁编码器一圈计数值 14 位磁编码器 - 16384


// 初始化磁编码器
void MT6701_Init(MT6701_Encoder* encoder);
// 获得从初始位置开始总共转过的角度(单位：rad)
float MT6701_GetAngle_ALL(MT6701_Encoder* encoder);
// 返回十四位编码器数据
float MT6701_ReadDataSSI(MT6701_Encoder* encoder);

#endif /* HARDWARE_MT6701_H_ */
