#include "AngleCalibrationFunc.h"
#include "main.h"         //系统
#include "Motor1GPIO.h"   //电机1使能引脚
#include "AngleEncoder.h" //电机1磁编码器
#include "Motor1TIM1PWM.h"//Motor1PWM生成定时器

#include "FocControl.h"   //FOC核
#include "IQmathLib.h"    //数学库


#include "stdio.h"        //RTT打印
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"
/*************************************************************
** Function name:       Motor1AngleCalibration
** Descriptions:        电机一角度校准
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor1AngleCalibration(void)
{
	float angle;
	_iq time0 = _IQ(0);
	_iq timeA = _IQ(70);
	_iq timeB = _IQ(20);
	_iq timeC = _IQ(20);
	//1.使能电机
	Motor1SetEnable(1);
	//2.电机旋转至a轴
    SetMotor1ChannelHighLeaveTime_us(timeA,timeB,timeC);
	HAL_Delay(500);
	//3.读取角度
	for (uint8_t i = 0; i < 10; i++) {
		angle = GetMotor1_mAngle();
		HAL_Delay(100);
	}
	printf("1:float%f\r\n",angle);
//  HAL_Delay(1000);
//	HAL_Delay(1000);
//	HAL_Delay(1000);
	//4.电机失能
  SetMotor1ChannelHighLeaveTime_us(time0,time0,time0);
	Motor1SetEnable(0);
    while(1){
		HAL_Delay(10);
		Motor1SetEnable(0);
	}
}

/*************************************************************
** Function name:      AngleCalibrationFunc_Init
** Descriptions:       编码器角度校准
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void AngleCalibrationFunc_Init(void)
{
    Motor1AngleCalibration();
}




