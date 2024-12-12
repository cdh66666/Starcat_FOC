#pragma once

// 电机物理参数：
#define POLE_PAIRS 7 // 极对数

// 电路参数：
#define R_SHUNT 0.01              // 低侧电流采样电阻，欧姆
#define OP_GAIN 50                // 低侧运放放大倍数
#define MAX_CURRENT 2             // 最大q轴电流，安培A
#define ADC_REFERENCE_VOLT 3.3    // 电流采样adc参考电压，伏
#define ADC_SCALE ((1 << 12) - 1) // 电流采样adc数字范围

// 处理器配置参数：
#define motor_pwm_freq (20000)                          // 驱动桥pwm频率，Hz
#define pwm_dead_time 500                               // 驱动桥pwm死区，ns ,must smaller than ((1<<8)-1)/(sclk_freq/(10^9))=1062
#define motor_speed_calc_freq 930                       // must bigger than 0.5,otherwise overflow the uint16_t，HZ
#define speed_calc_period (1.0 / motor_speed_calc_freq) // 速度计算周期，s

// 软件参数：
// 电机旋转累计角度，计数超过这个角度后，电机角度会被归零，
// 注意全周期由负半周期和正半周期组成
#define position_cycle 2 * 2 * 3.14159265358979
