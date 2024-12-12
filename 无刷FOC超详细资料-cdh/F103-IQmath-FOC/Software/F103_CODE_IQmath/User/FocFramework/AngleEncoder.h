/***********************************************************************
 * @file AngleEncoder.h
 * Description:
 *
 **********************************************************************/
#ifndef __AngleEncoder_h__
#define __AngleEncoder_h__

#include "stdint.h"
#include "FocControl.h"
#include "IQmathLib.h"

//TLE5012B 寄存器地址
#define READ_STATUS            0x8001
#define READ_ANGLE_VALUE       0x8021
#define READ_SPEED_VALUE       0x8031


//MT6816 寄存器地址
#define ANGLE_VALUE1           (uint16_t)0x8300
#define ANGLE_VALUE2           (uint16_t)0x8400

//角度值转化弧度制系数
#define FOC_EANGLE_TO_ERADIN _IQ19(0.017453293)


void GetMotor1_ElectricalAngleMT6816(FOC_Struct *pFOC);
void GetMotor1_ElectricalAngleTle5012(FOC_Struct *pFOC);
float GetMotor1_mAngle(void);
void GetMotor1_preSpeed(FOC_Struct *pFOC);
_iq19 GetMotor1_prePosition(FOC_Struct *pFOC);

float GetMotor1FOC_mAngle(void);
#endif // __AngleEncoder_h__

