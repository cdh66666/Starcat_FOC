#include "Motor1GPIO.h"
#include "main.h"

/*************************************************************
** Function name:       Motor1SetEnable
** Descriptions:        设置电机1使能标志
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor1SetEnable(uint8_t isEnable)
{
    HAL_GPIO_WritePin(DRV_EN_GPIO_Port,DRV_EN_Pin, (GPIO_PinState)isEnable);  //电机1驱动器使能，高电平使能
    HAL_GPIO_WritePin(nSLEEP_GPIO_Port,nSLEEP_Pin, (GPIO_PinState)isEnable);  //电机1驱动器休眠，高电平保持唤醒
}
