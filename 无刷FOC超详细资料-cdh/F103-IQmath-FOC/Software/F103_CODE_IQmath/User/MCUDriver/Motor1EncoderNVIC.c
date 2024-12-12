#include "Motor1EncoderNVIC.h"

#include "main.h"
#include "Motor1TIM2Encoder.h"




struct SMotor1EncoderCalibration_Struct
{
    /* data */
    int calibrationValue;  //校准值
    uint8_t calibrationFlag;     //第一次值
};

struct SMotor1EncoderCalibration_Struct gMotor1EncoderCal = {0};



/*************************************************************
** Function name:       Motor1EncoderZ_Init
** Descriptions:        电机1编码器中断中断初始化
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor1EncoderZ_Init(void)
{
    gMotor1EncoderCal.calibrationValue = 0;
    gMotor1EncoderCal.calibrationFlag = 0;
}
/*************************************************************
** Function name:       Motor1EncoderZ_DeInit
** Descriptions:        电机1编码器中断反初始化
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor1EncoderZ_DeInit(void)
{
    gMotor1EncoderCal.calibrationValue = 0;
    gMotor1EncoderCal.calibrationFlag = 0;
}

/*************************************************************
** Function name:       Motor1EncoderZEXITCallback
** Descriptions:        电机1编码器中断回调函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor1EncoderZEXITCallback(void)
{
	if (gMotor1EncoderCal.calibrationFlag == 0) {
		gMotor1EncoderCal.calibrationValue = Motor1TIM2EncoderGetCnt();
		gMotor1EncoderCal.calibrationFlag = 1;
	} else {
		Motor1TIM2EncoderSetCnt(gMotor1EncoderCal.calibrationValue);
	}
	//printf("1:%d\r\n",gMotor1EncoderCal.calibrationValue);

}


