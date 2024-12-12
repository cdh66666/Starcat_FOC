/***********************************************************************
* @file Motor1EncoderNVIC
* Description:
*
**********************************************************************
* File name:      Motor1EncoderNVIC.h
* Date:           2021-05-16
* Version:        V1.0
* Author          liuxiang
* @history:
* V1.0 创建文件
***********************************************************************/
#ifndef __Motor1EncoderNVIC_H__
#define __Motor1EncoderNVIC_H__
#include "stdint.h"

void Motor1EncoderZ_Init(void);
void Motor1EncoderZ_DeInit(void);
void Motor1EncoderZEXITCallback(void);


#endif //__Motor1EncoderNVIC_H__



