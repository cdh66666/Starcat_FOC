#include "PerDriverMain.h"
#include "main.h"
#include "Timer.h"       //时间调度
#include "TouchDriver.h" //按键
#include "LedDriver.h"   //LED
#include "SysManage.h"   //系统状态管理
#include "IQmathLib.h"   //数学库

#include "SDS.h"         //串口曲线打印

#include "FocControl.h"  //FOC核


/*************************************************************
** Function name:      Data_Output
** Descriptions:       串口曲线打印
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void Data_Output(void)
{
	  float Send_Out[4] = {0};  //串口曲线打印Buf
	
    Send_Out[0]= _IQ19toF(FOC_Mod.sPID.tar)*100;
    Send_Out[1]= _IQ19toF(FOC_Mod.sPID.pre)*100;
    
    
//    Send_Out[0]= _IQtoF(FOC_Mod.tariq)*1000;
//    Send_Out[1]= _IQtoF(FOC_Mod.Iq)*1000;
//    Send_Out[2]= _IQ19toF(GetMotor1ADC1PhaseXValue(3));
    
    
//    Send_Out[0]= _IQtoF(FOC_Mod.ia)*100;
//    Send_Out[1]= _IQtoF(FOC_Mod.ib)*100;
//    Send_Out[2]= _IQtoF(FOC_Mod.ic)*100;
    
//    Send_Out[0]= _IQ19toF(GetMotor1ADC1PhaseXValue(1));
//    Send_Out[1]= _IQ19toF(GetMotor1ADC1PhaseXValue(2));
//    Send_Out[2]= _IQ19toF(GetMotor1ADC1PhaseXValue(3));
//    Send_Out[3]= _IQtoF(Motor1GetAngle());

    SDS_OutPut_Data(Send_Out);
}
/*************************************************************
** Function name:      Key_Function
** Descriptions:       按键功能触发
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void Key_Function(void)
{
    uint8_t KeyAction;
    KeyAction = Touch_GetKey(); //获取当前按键状态
    
    if(KeyAction == ((POWER_KEY << 4) | SHORT_TOUCH))   //POWER_KEY键 短按
    {
        FocMotor_Enable(1);
    }
    else if(KeyAction == ((SET_KEY << 4) | SHORT_TOUCH))//SET_KEY键 短按
    {
        FocMotor_Enable(0);
    }
}
/*************************************************************
** Function name:      PerDriverMain_Init
** Descriptions:       外设初始化
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void PerDriverMain_Init(void)
{
    FocControl_Init();
}

/*************************************************************
** Function name:      PerDriverMain_Loop
** Descriptions:       外设循环
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
uint8_t currentflag;
uint8_t speedflag;
uint8_t positionflag;
int16_t angle = 0;
_iq19 Preangle = _IQ19(0);

void PerDriverMain_Loop(void)
{
	//打印调试信息
	//RUN_BY_LIMIT_BLOCK(100,Motor1FOCConfig_Printf())
	//RUN_BY_LIMIT_BLOCK(5,Data_Output())

	//电流环调试
//	RUN_BY_LIMIT_BLOCK(2000,
//		if (currentflag == 0) {
//			Motor1SetTarIDIQ(_IQ(0.0),_IQ(0.28));
//			currentflag = 1;
//		} else {
//			Motor1SetTarIDIQ(_IQ(0.0),_IQ(0.15));
//			currentflag = 0;
//		}
//	)
	//速度环调试
	 RUN_BY_LIMIT_BLOCK(2000,
	 	if (currentflag == 0) {
	 		Motor1SpeedPIDSetTar(_IQ19(0.3));  //0.4-67/min
			
	 		currentflag = 1;
	 	} else {
	 		Motor1SpeedPIDSetTar(_IQ19(-0.4));
	 		currentflag = 0;
	 	}
	 )
	//位置环调试
//	RUN_BY_LIMIT_BLOCK(2000,
//		if (positionflag == 0) {
//			Motor1PosPIDSetTar(_IQ19(-20));
//			positionflag = 1;
//		} else {
//			Motor1PosPIDSetTar(_IQ19(90));
//			positionflag = 0;
//		}
//	)
	//位置环应用1
//	RUN_BY_LIMIT_BLOCK(1000,                    
//		angle += 20;
//		if(angle >= 360) {
//			angle = 0;
//		}
//		Motor1PosPIDSetTar(_IQ19(180) - _IQ19(angle));
//	)	
	//位置环应用2
    		
//	  RUN_BY_LIMIT_BLOCK(10,
//    Preangle = GetMotor_mAngle();
//		if((Preangle>_IQ19(40))&&(Preangle<_IQ19(80)))Motor1PosPIDSetTar(_IQ19(60));
//		else if((Preangle>_IQ19(100))&&(Preangle<_IQ19(140)))Motor1PosPIDSetTar(_IQ19(120));
//		else if((Preangle>_IQ19(160))&&(Preangle<_IQ19(-200)))Motor1PosPIDSetTar(_IQ19(180));
//		else if((Preangle>_IQ19(-140))&&(Preangle<_IQ19(-100)))Motor1PosPIDSetTar(_IQ19(-120));
//		else if((Preangle>_IQ19(-80))&&(Preangle<_IQ19(-40)))Motor1PosPIDSetTar(_IQ19(-60));
//		else if((Preangle>_IQ19(-20))&&(Preangle<_IQ19(20)))Motor1PosPIDSetTar(_IQ19(0));
//	)	

    RUN_BY_LIMIT_BLOCK(10,SysManage_Loop());    //系统管理
    RUN_BY_LIMIT_BLOCK(1,LedDriver_Loop())      //LED底层刷新
    RUN_BY_LIMIT_BLOCK(10,Key_Function());      //按键功能触发
    RUN_BY_LIMIT_BLOCK(10,TouchDriver_Loop());  //按键底层刷新10ms
}

