/***********************************************************************
* @file Motor2ADC2
* Description:
*
**********************************************************************
* File name:      Motor2ADC2.h
* Date:           2021-05-16
* Version:        V1.0
* Author          liuxiang
* @history:
* V1.0 创建文件
***********************************************************************/
#ifndef __Motor2ADC2_H__
#define __Motor2ADC2_H__
#include "stdint.h"

void Motor2ADC2StartOnce(void);
void Motor2ADC2ValueStorage(void);
int32_t GetMotor2ADC2PhaseXValue(uint8_t Phase);

#endif //__Motor2ADC2_H__

