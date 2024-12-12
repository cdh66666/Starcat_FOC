#include "Motor2TIM3Pwm.h"
#include "main.h"

extern TIM_HandleTypeDef htim3;


/*************************************************************
** Function name:       Motor2TIM3_Init
** Descriptions:        电机2定时器初始化
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor2TIM3_Init(void)
{
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3,0);

    SetMotor2ChannelAHighLeaveTime_us(0);
    SetMotor2ChannelBHighLeaveTime_us(0);
    SetMotor2ChannelCHighLeaveTime_us(0);
}



/*************************************************************
** Function name:       SetMotor2ChannelAHighLeaveTime_us
** Descriptions:        设置电机2通道A高电平时间（中央对齐模式二）
** Input parameters:    time：高电平时间 单位us 范围 0-100
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void SetMotor2ChannelAHighLeaveTime_us(float time)
{
    uint32_t ccr;
    if (time < 0) {
        time = 0;
    }
    if (time > 100) {
        time = 100;
    }
    ccr = 42 * time;
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1, ccr);
}
/*************************************************************
** Function name:       SetMotor2ChannelBHighLeaveTime_us
** Descriptions:        设置电机2通道B高电平时间（中央对齐模式二）
** Input parameters:    time：高电平时间 单位us 范围 0-100
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void SetMotor2ChannelBHighLeaveTime_us(float time)
{
    uint32_t ccr;
    if (time < 0) {
        time = 0;
    }
    if (time > 100) {
        time = 100;
    }
    ccr = 42 * time;
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2, ccr);
}
/*************************************************************
** Function name:       SetMotor2ChannelCHighLeaveTime_us
** Descriptions:        设置电机2通道C高电平时间（中央对齐模式二）
** Input parameters:    time：高电平时间 单位us 范围 0-100
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void SetMotor2ChannelCHighLeaveTime_us(float time)
{
    uint32_t ccr;
    if (time < 0) {
        time = 0;
    }
    if (time > 100) {
        time = 100;
    }
    ccr = 42 * time;
    __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_3, ccr);
}




