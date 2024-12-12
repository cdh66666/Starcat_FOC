#include "Motor2GPIO.h"
#include "main.h"


/*************************************************************
** Function name:       Motor2SetEnable
** Descriptions:        设置电机2使能标志
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor2SetEnable(uint8_t isEnable)
{
    HAL_GPIO_WritePin(M2_Enable_GPIO_Port,M2_Enable_Pin, (GPIO_PinState)isEnable);
}

