#pragma once
#include "conf.h"
#define rotor_phy_angle (motor_logic_angle - rotor_zero_angle)
#define rotor_logic_angle rotor_phy_angle *POLE_PAIRS
extern float motor_i_u;
extern float motor_i_v;
extern float motor_i_d;
extern float motor_i_q;
extern float motor_speed;
extern float motor_logic_angle;
extern float encoder_angle;
extern float rotor_zero_angle; // d轴为零度时的编码器角度
