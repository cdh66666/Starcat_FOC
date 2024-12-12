#include "FunctionMain.h"
#include "main.h"                  //系统
#include "AngleCalibrationFunc.h"  //角度校准
#include "Timer.h"                 //时间调度
#include "FocControl.h"            //FOC核
#include "UartBus.h"               //串口总线功能
#include "CanBus.h"                //CAN 总线功能
#include "SysManage.h"             //系统管理
#include "StartupMusic.h"          //开机音乐

/*************************************************************
** Function name:      FunctionMain_Init
** Descriptions:       功能初始化
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void FunctionMain_Init(void)
{
	//AngleCalibrationFunc_Init();  //零偏角度校准
	CanBus_Init();
	UartBus_Init();
}

/*************************************************************
** Function name:      FunctionMain_Loop
** Descriptions:       功能循环
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/

void FunctionMain_Loop(void)
{
	if(Get_SysState() == Startup_Check)  //开机自检阶段
	{
		RUN_BY_LIMIT_BLOCK(1,StartupMusic_Loop());
	}
	else
	{
		CanBus_Loop();      //CanBus_Loop
        
        RUN_BY_LIMIT_BLOCK(500,UartBus_Loop()); //UartBus_Loop

	}
}



