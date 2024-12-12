/***********************************************************************
* @file Motor1ADC1.h
* Description:
*
***********************************************************************/
#ifndef __Motor1ADC1_H__
#define __Motor1ADC1_H__

#include "stdint.h"
#include "FocMath.h"

#define FILTER_KP _IQ19(0.9f) //一介低通滤波系数


void Motor1ADC1StartOnce(void);
void Motor1ADC1ValueStorage(void);
_iq19 GetMotor1ADC1PhaseXValue(uint8_t Phase);

#endif //__Motor1ADC1_H__

