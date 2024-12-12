/***********************************************************************
* @file Motor2EncoderNVIC
* Description:
*
**********************************************************************
* File name:      Motor2EncoderNVIC.h
* Date:           2021-05-16
* Version:        V1.0
* Author          liuxiang
* @history:
* V1.0 创建文件
***********************************************************************/
#ifndef __Motor2EncoderNVIC_H__
#define __Motor2EncoderNVIC_H__
#include "stdint.h"

void Motor2EncoderZ_Init(void);
void Motor2EncoderZ_DeInit(void);
void Motor2EncoderZEXITCallback(void);


#endif //__Motor2EncoderNVIC_H__



