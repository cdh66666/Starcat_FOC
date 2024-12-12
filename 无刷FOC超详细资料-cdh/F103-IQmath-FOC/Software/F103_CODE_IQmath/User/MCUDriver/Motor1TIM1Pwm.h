/***********************************************************************
 * @file Motor1TIM1Pwm.h
 * Description:
 *
***********************************************************************/
#ifndef __Motor1TIM1Pwm_h__
#define __Motor1TIM1Pwm_h__

#include "stdint.h"
#include "IQmathLib.h"

void Motor1TIM1_Init(void);
void SetMotor1ChannelAHighLeaveTime_us(_iq time);
void SetMotor1ChannelBHighLeaveTime_us(_iq time);
void SetMotor1ChannelCHighLeaveTime_us(_iq time);
void SetMotor1ChannelHighLeaveTime_us(_iq timeA,_iq timeB,_iq timeC);


void SetMotor1Channel_FreqDuty(uint32_t Freq,float Duty);
	

#endif // __Motor1TIM1Pwm_h__
