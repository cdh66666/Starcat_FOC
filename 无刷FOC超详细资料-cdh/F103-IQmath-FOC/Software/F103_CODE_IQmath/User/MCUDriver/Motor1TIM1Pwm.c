#include "Motor1TIM1Pwm.h"
#include "Motor1GPIO.h"
#include "tim.h"
#include "main.h"


/*************************************************************
** Function name:       Motor1TIM1_Init
** Descriptions:        电机1定时器初始化
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor1TIM1_Init(void)
{
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0);

    SetMotor1ChannelHighLeaveTime_us(_IQ(0.0),_IQ(0.0),_IQ(0.0));
    
    //HAL_TIM_Base_Start_IT(&htim1);  //系统管理中开始FOC中断
}

/*************************************************************
** Function name:       SetMotor1ChannelHighLeaveTime_us
** Descriptions:        设置电机1通道ABC高电平时间（中央对齐模式二）
** Input parameters:    time：高电平时间 单位us 范围 0-100
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void SetMotor1ChannelHighLeaveTime_us(_iq timeA,_iq timeB,_iq timeC)
{
    uint32_t ccr1,ccr2,ccr3;
    
    if (timeA < _IQ(0.0))    timeA = _IQ(0.0);
    if (timeA > _IQ(100.0))timeA = _IQ(100.0);
    ccr1 = (uint32_t)(18 * _IQtoF(timeA));
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, ccr1);
    
    if (timeB < _IQ(0.0))    timeB = _IQ(0.0);
    if (timeB > _IQ(100.0))timeB = _IQ(100.0);
    ccr2 = (uint32_t)(18 * _IQtoF(timeB));
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, ccr2);
    
    if (timeC < _IQ(0.0))    timeC = _IQ(0.0);
    if (timeC > _IQ(100.0))timeC = _IQ(100.0);
    ccr3 = (uint32_t)(18 * _IQtoF(timeC));
    __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, ccr3);
}
/*************************************************************
** Function name:       SetMotor1Channel_FreqDuty
** Descriptions:        设置电机通道1 频率周期 ,开机音乐使用
** Input parameters:    time：高电平时间 单位us 范围 0-100
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void SetMotor1Channel_FreqDuty(uint32_t Freq,float Duty)
{
	uint32_t usPrescaler ,usPeriod;
	
	if(Freq <= 1000)
	{
		usPrescaler = 999;
	  usPeriod    = (36000 / Freq)-1;
	}
	else
	{		
		usPrescaler = 0;
	  usPeriod    = (36000000 / Freq)-1;
	}

	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, (uint32_t)(usPeriod*Duty));
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, (uint32_t)(usPeriod*Duty));
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 0);
	
	if(Freq == 0)
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1, 0);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2, 0);
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3, 0);
	
	}
	
	TIM1->PSC = usPrescaler;  //预分频
	TIM1->ARR = usPeriod;     //重装值
	TIM1->EGR = TIM_EGR_UG;
}


