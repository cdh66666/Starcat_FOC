#ifndef Motor2TIM3Pwm_h
#define Motor2TIM3Pwm_h

#include "stdint.h"


void Motor2TIM3_Init(void);
void SetMotor2ChannelAHighLeaveTime_us(float time);
void SetMotor2ChannelBHighLeaveTime_us(float time);
void SetMotor2ChannelCHighLeaveTime_us(float time);
#endif /* Motor2TIM3Pwm_h */
