/***********************************************************************
 * @file LedDriver.h
 * Description:
 *
 **********************************************************************/
#ifndef __LedDriver_h__
#define __LedDriver_h__

#include "stdint.h"
#include "main.h"

#define LED_R_OFF  HAL_GPIO_WritePin(SYS_RLED_GPIO_Port,SYS_RLED_Pin,GPIO_PIN_SET)
#define LED_R_ON   HAL_GPIO_WritePin(SYS_RLED_GPIO_Port,SYS_RLED_Pin,GPIO_PIN_RESET)

#define LED_B_OFF  HAL_GPIO_WritePin(SYS_BLED_GPIO_Port,SYS_BLED_Pin,GPIO_PIN_SET)
#define LED_B_ON   HAL_GPIO_WritePin(SYS_BLED_GPIO_Port,SYS_BLED_Pin,GPIO_PIN_RESET)



void LedDriver_Loop(void);

#endif // __LedDriver_h__

