#include "MT6701.h"

#define ABS(x) ((x)>0?(x):-(x))
#define _2PI        6.28318530718f


MT6701_Encoder encoder1 = {
   .CS_GPIO_Port = CS_ENC_A_1_GPIO_Port,
   .CS_Pin = CS_ENC_A_1_Pin,
   .SCK_GPIO_Port = SCK_ENC_B_1_GPIO_Port,
   .SCK_Pin = SCK_ENC_B_1_Pin,
   .MOSI_GPIO_Port = MOSI_ENC_Z_1_GPIO_Port,
   .MOSI_Pin = MOSI_ENC_Z_1_Pin,
   .rotation_count = MT_CPR,
   .full_rotation_offset = 0,
   .rad = 0,
   .rad_prev = 0,
   .rad_all_prev = 0,
};

MT6701_Encoder encoder2 = {
    // 根据新的编码器引脚进行配置
   .CS_GPIO_Port = CS_ENC_A_2_GPIO_Port, // 假设新编码器 CS 引脚连接到 GPIOD
   .CS_Pin = CS_ENC_A_2_Pin,
   .SCK_GPIO_Port = SCK_ENC_B_2_GPIO_Port, // 假设新编码器 SCK 引脚连接到 GPIOD
   .SCK_Pin = SCK_ENC_B_2_Pin,
   .MOSI_GPIO_Port = MOSI_ENC_Z_2_GPIO_Port, // 假设新编码器 MOSI 引脚连接到 GPIOD
   .MOSI_Pin = MOSI_ENC_Z_2_Pin,
   .rotation_count = MT_CPR,
   .full_rotation_offset = 0,
   .rad = 0,
   .rad_prev = 0,
   .rad_all=0,
   .rad_all_prev = 0,
};

// 初始化磁编码器
void MT6701_Init(MT6701_Encoder* encoder) {
    encoder->rotation_count = MT_CPR;
    encoder->rad = encoder->rad_prev = MT6701_ReadDataSSI(encoder);
    encoder->full_rotation_offset = 0;
    encoder->rad_all = (encoder->full_rotation_offset + encoder->rad);
    encoder->rad_all_prev = encoder->rad_all;
}

// 获得从初始位置开始总共转过的角度(单位：rad),d_rad是转动弧度差，乘以频率就是电机转速 rad/s
float MT6701_GetAngle_ALL(MT6701_Encoder* encoder)
{
    encoder->rad = MT6701_ReadDataSSI(encoder);
    float d_rad = encoder->rad - encoder->rad_prev;
    if (ABS(d_rad) > (0.8 * _2PI)) {
        encoder->full_rotation_offset += (d_rad > 0? -_2PI : _2PI);
    }
    //重新赋值上一次弧度
    encoder->rad_prev = encoder->rad;
    encoder->rad_all = (encoder->full_rotation_offset + encoder->rad);
    encoder->d_rad = (encoder->rad_all - encoder->rad_all_prev);
    //重新赋值上一次总弧度
    encoder->rad_all_prev = encoder->rad_all;

    return encoder->rad_all;
}


//读取编码器数据，单位：rad，范围：0-2pi
float MT6701_ReadDataSSI(MT6701_Encoder* encoder) {

    uint16_t rad = 0;
    float frad = 0.0;
    int offset = 14;

    // 片选拉低选中芯片
    HAL_GPIO_WritePin(encoder->CS_GPIO_Port, encoder->CS_Pin, GPIO_PIN_RESET);

    // 跳过第一个时钟信号
    HAL_GPIO_WritePin(encoder->SCK_GPIO_Port, encoder->SCK_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(encoder->SCK_GPIO_Port, encoder->SCK_Pin, GPIO_PIN_SET);

    for (int i = 0; i < 14; i++) {
        HAL_GPIO_WritePin(encoder->SCK_GPIO_Port, encoder->SCK_Pin, GPIO_PIN_RESET);
        rad |= (HAL_GPIO_ReadPin(encoder->MOSI_GPIO_Port, encoder->MOSI_Pin) << offset);
        offset--;
        HAL_GPIO_WritePin(encoder->SCK_GPIO_Port, encoder->SCK_Pin, GPIO_PIN_SET);
    }

    // 片选拉高不选中
    HAL_GPIO_WritePin(encoder->CS_GPIO_Port, encoder->CS_Pin, GPIO_PIN_SET);

    frad = (float)(rad * _2PI / 2.0f) / 16384.0f;

    return frad;
}
