
/***********************************************************************
* @file Motor2TIM4Encoder
* Description:
*
**********************************************************************
* File name:      Motor2TIM4Encoder.h
* Date:           2021-03-10
* Version:        V1.0
* Author          liuxiang
* @history:
* V1.0 创建文件
***********************************************************************/
#ifndef __Motor2TIM4Encoder_H__
#define __Motor2TIM4Encoder_H__
#include "stdint.h"

#include "stdio.h"
#include "stdint.h"





void Motor2TIM4EncoderInit(void);
void Motor2TIM4EncoderReset(void);
int32_t Motor2TIM4EncoderGetCnt(void);
void Motor2TIM4EncoderSetCnt(int32_t cnt);
float Motor2TIM4EncoderGetAngle(void);

#endif //__Motor2TIM4Encoder_H__





