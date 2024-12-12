#include "Motor2EncoderNVIC.h"

#include "main.h"
#include "Motor2TIM4Encoder.h"




struct SMotor2EncoderCalibration_Struct
{
    /* data */
    int calibrationValue;  //校准值
    uint8_t calibrationFlag;     //第一次值
};

struct SMotor2EncoderCalibration_Struct gMotor2EncoderCal = {0};



/*************************************************************
** Function name:       Motor2EncoderZ_Init
** Descriptions:        电机1编码器中断中断初始化
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor2EncoderZ_Init(void)
{
    gMotor2EncoderCal.calibrationValue = 0;
    gMotor2EncoderCal.calibrationFlag = 0;
}
/*************************************************************
** Function name:       Motor2EncoderZ_DeInit
** Descriptions:        电机1编码器中断反初始化
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor2EncoderZ_DeInit(void)
{
    gMotor2EncoderCal.calibrationValue = 0;
    gMotor2EncoderCal.calibrationFlag = 0;
}

/*************************************************************
** Function name:       Motor2EncoderZEXITCallback
** Descriptions:        电机1编码器中断回调函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor2EncoderZEXITCallback(void)
{
	if (gMotor2EncoderCal.calibrationFlag == 0) {
		gMotor2EncoderCal.calibrationValue = Motor2TIM4EncoderGetCnt();
		gMotor2EncoderCal.calibrationFlag = 1;
	} else {
		Motor2TIM4EncoderSetCnt(gMotor2EncoderCal.calibrationValue);
	}
	//printf("1:%d\r\n",gMotor2EncoderCal.calibrationValue);

}


