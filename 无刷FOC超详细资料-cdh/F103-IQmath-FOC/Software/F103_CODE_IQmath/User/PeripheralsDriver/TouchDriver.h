/***********************************************************************
 * @file TouchDriver.h
 * Description:
 *
 **********************************************************************/
#ifndef __TouchDriver_h__
#define __TouchDriver_h__

#include "stdint.h"
#include "main.h"

#define KEY_NUMBER_ALL	2	//按键数量

#define POWER_KEY_PinRead  HAL_GPIO_ReadPin(KEY_1_GPIO_Port,KEY_1_Pin) //电源按键
#define SET_KEY_PinRead	   HAL_GPIO_ReadPin(KEY_2_GPIO_Port,KEY_2_Pin) //设置按键

//按键结构体声明
typedef enum
{
    NONE_KEY 	= 0x00,	//无按键
    POWER_KEY = 0x01, //电源按键
    SET_KEY  	= 0x02, //设置按键
}KeyName;

typedef enum
{
	SHORT_TOUCH = 0x00,	//短按
	LONG_TOUCH  = 0x01,	//长按
}KeyActionName;

typedef struct
{
	unsigned int  CntTimer[KEY_NUMBER_ALL + 1];	//按键触发时间缓存
	unsigned char Action;						//按键动作
}KeyInit;


uint8_t Touch_GetKey(void);
void TouchDriver_Loop(void);


#endif // __TouchDriver_h__
