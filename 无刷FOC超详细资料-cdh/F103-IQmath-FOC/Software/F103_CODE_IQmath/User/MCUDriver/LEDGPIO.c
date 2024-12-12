#include "LEDGPIO.h"
#include "main.h"

/*************************************************************
** Function name:       LEDGPIOInit
** Descriptions:        初始化LED引脚
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void LEDGPIOInit(void)
{
    
}
/*************************************************************
** Function name:      SetLedLeave
** Descriptions:       设置LED引脚电平 0：点亮
** Input parameters:   leave：0:低电平 1:高电平
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void SetLedLeave(uint8_t leave)
{
	//HAL_GPIO_WritePin(SYS_LED_GPIO_Port, SYS_LED_Pin, leave);
}

