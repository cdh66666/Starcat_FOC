#include "TouchDriver.h"


KeyInit Key = {0};

/*************************************************************
** Function name:       Touch_GetKey
** Descriptions:        获取按键状态
** Input parameters:    None
** Output parameters:   None
** Returned value:      按键状态
** Remarks:             None
*************************************************************/
uint8_t Touch_GetKey(void)
{
	uint8_t touchKeyAction = NONE_KEY;
	touchKeyAction = Key.Action;
	Key.Action = NONE_KEY;
	return touchKeyAction;
}
/*************************************************************
** Function name:       TouchDriver_Loop
** Descriptions:        按键底层刷新函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void TouchDriver_Loop(void)
{
	//电源按键
    if(POWER_KEY_PinRead == 0)
	{
		if(Key.CntTimer[POWER_KEY] < 300)Key.CntTimer[POWER_KEY] ++;
		else if(Key.CntTimer[POWER_KEY] == 300)
		{
			Key.CntTimer[POWER_KEY] ++;
			Key.Action = (POWER_KEY << 4) | LONG_TOUCH;
		}
	}
	else
	{
		if(Key.CntTimer[POWER_KEY] > 3)Key.Action = (POWER_KEY << 4) | SHORT_TOUCH;
		Key.CntTimer[POWER_KEY] = 0;
	}
	//设置按键
    if(SET_KEY_PinRead == 0)
	{
		if(Key.CntTimer[SET_KEY] < 300)Key.CntTimer[SET_KEY] ++;
		else if(Key.CntTimer[SET_KEY] == 300)
		{
			Key.CntTimer[SET_KEY] ++;
			Key.Action = (SET_KEY << 4) | LONG_TOUCH;
		}
	}
	else
	{
		if(Key.CntTimer[SET_KEY] > 3)Key.Action = (SET_KEY << 4) | SHORT_TOUCH;
		Key.CntTimer[SET_KEY] = 0;
	}
}


