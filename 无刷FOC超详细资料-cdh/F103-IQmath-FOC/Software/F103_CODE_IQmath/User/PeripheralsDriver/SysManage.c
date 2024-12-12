#include "SysManage.h"
#include "tim.h"


SystemState Sys = {0};


/*************************************************************
** Function name:       Get_SysState
** Descriptions:        获取当前系统状态
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint8_t Get_SysState(void)
{
  return Sys.StateAction;
}

/*************************************************************
** Function name:       Set_SysState
** Descriptions:        设置当前系统状态
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Set_SysState(SystemAction SysAction)
{
    Sys.StateAction = SysAction;
}

/*************************************************************
** Function name:       SysManage_Loop
** Descriptions:        设置当前系统状态
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void SysManage_Loop(void)
{
	static uint16_t Ticktime = 0;
	
	switch(Sys.StateAction)
	{
		case Startup_Check:
			if(Ticktime < 200)Ticktime ++;
			else
			{
				HAL_TIM_Base_Start_IT(&htim1);   //开启foc中断
				Sys.StateAction = NormalRun_Action;
			} 
			break;
		case Debug_Action:
			break;
		default:break;
	}
}








