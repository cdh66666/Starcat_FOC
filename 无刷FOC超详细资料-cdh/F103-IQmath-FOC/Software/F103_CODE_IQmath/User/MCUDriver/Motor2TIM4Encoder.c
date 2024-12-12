#include "Motor2TIM4Encoder.h"
#include "SEGGER_RTT_Conf.h"
#include "SEGGER_RTT.h"
#include "main.h"

#define CNTZERO 10000
#define ENCODERPLUSE 4096


extern TIM_HandleTypeDef htim4;

struct SMotor2Tim4Enc_Struct {
    int32_t cnt;            //编码器脉冲数 0-ENCODERPLUSE
    float angle;			//编码器角度 0-360°
};
typedef struct SMotor2Tim4Enc_Struct Motor2Tim4Enc_Struct; /* TimEnc 类型 */
typedef Motor2Tim4Enc_Struct *PMotor2Tim4Enc_Struct;      /* PTimEnc 指针类型 */

Motor2Tim4Enc_Struct gMotor2Timer4Encoder = {
    .cnt = 0,
    .angle = 0,
};

/*************************************************************
** Function name:       Motor2TIM4EncoderInit
** Descriptions:        初始化定时器4
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
*************************************************************/
void Motor2TIM4EncoderInit(void)
{
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1|TIM_CHANNEL_2);
    __HAL_TIM_SET_COUNTER(&htim4,CNTZERO);
    gMotor2Timer4Encoder.angle = 0.0;
    gMotor2Timer4Encoder.cnt = 0;
}
/*************************************************************
** Function name:       Motor2TIM4EncoderReset
** Descriptions:        复位定时器4
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
*************************************************************/
void Motor2TIM4EncoderReset(void)
{
    __HAL_TIM_SET_COUNTER(&htim4,CNTZERO);
    gMotor2Timer4Encoder.angle = 0.0;
    gMotor2Timer4Encoder.cnt = 0;
}
/*************************************************************
** Function name:       Motor2TIM4EncoderGetCnt
** Descriptions:        获取定时器4编码器模式CNT的值
** Input parameters:    None
** Output parameters:   None
** Returned value:      Timer4->CNT的值
*************************************************************/
int32_t Motor2TIM4EncoderGetCnt(void)
{
    gMotor2Timer4Encoder.cnt += __HAL_TIM_GET_COUNTER(&htim4) - CNTZERO;
	__HAL_TIM_SET_COUNTER(&htim4,CNTZERO);
	if (gMotor2Timer4Encoder.cnt < 0) {
		gMotor2Timer4Encoder.cnt += ENCODERPLUSE;
	}
    gMotor2Timer4Encoder.cnt = gMotor2Timer4Encoder.cnt % ENCODERPLUSE;
    return gMotor2Timer4Encoder.cnt;
}
/*************************************************************
** Function name:       Motor2TIM4EncoderSetCnt
** Descriptions:        设置Timer4Cnt的值
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor2TIM4EncoderSetCnt(int32_t cnt)
{
	gMotor2Timer4Encoder.cnt = cnt;
}
/*************************************************************
** Function name:       Motor2TIM4EncoderGetAngle
** Descriptions:        获取编码器角度
** Input parameters:    None
** Output parameters:   None
** Returned value:      编码器角度
*************************************************************/
float Motor2TIM4EncoderGetAngle(void)
{
    gMotor2Timer4Encoder.angle = (float)Motor2TIM4EncoderGetCnt() / (float)ENCODERPLUSE * 360.0f;
    if (gMotor2Timer4Encoder.angle < 0) {
        gMotor2Timer4Encoder.angle = gMotor2Timer4Encoder.angle + 360.0f;
    }
    return gMotor2Timer4Encoder.angle;
}
