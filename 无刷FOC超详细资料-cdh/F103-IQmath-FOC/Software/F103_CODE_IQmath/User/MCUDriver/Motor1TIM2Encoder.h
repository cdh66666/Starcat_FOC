
/***********************************************************************
* @file Motor1TIM2Encoder
* Description:
*
**********************************************************************
* File name:      Motor1TIM2Encoder.h
* Date:           2021-03-10
* Version:        V1.0
* Author          liuxiang
* @history:
* V1.0 创建文件
***********************************************************************/
#ifndef __Motor1TIM2Encoder_H__
#define __Motor1TIM2Encoder_H__
#include "stdint.h"

#include "stdio.h"
#include "stdint.h"





void Motor1TIM2EncoderInit(void);
void Motor1TIM2EncoderReset(void);
int32_t Motor1TIM2EncoderGetCnt(void);
void Motor1TIM2EncoderSetCnt(int32_t cnt);
float Motor1TIM2EncoderGetAngle(void);

#endif //__Motor1TIM2Encoder_H__





