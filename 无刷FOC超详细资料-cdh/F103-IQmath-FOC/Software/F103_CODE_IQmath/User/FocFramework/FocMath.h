/***********************************************************************
 * @file FocMath.h
 * Description:
 *
 **********************************************************************/
#ifndef __FocMath_h__
#define __FocMath_h__

#include "stdint.h"
#include "IQmathLib.h"
#include "FocControl.h"


#define FOC_SSQRT_3 _IQ(0.577350269)      //三分之根号三
#define FOC_SQRT_3 _IQ(1.732050808)       //根号三
#define SVPWM_SQRT3_2 _IQ(0.866025404)    //二分之根号三
#define AD_TO_CURRENT _IQ19(0.001611328)  //电流采样比例系数

void ipark_calc(FOC_Struct *v);
void park_calc(FOC_Struct *v);
void clarke_calc(FOC_Struct *v);
void CurrentReconstruction(FOC_Struct *pFOC);
void CurrentPIControlID(FOC_Struct *pFOC);
void CurrentPIControlIQ(FOC_Struct *pFOC);
void SpeedPIDControl(FOC_Struct *pFOC);
void PositionPIDControl(FOC_Struct *pFOC);

#endif // __FocMath_h__
