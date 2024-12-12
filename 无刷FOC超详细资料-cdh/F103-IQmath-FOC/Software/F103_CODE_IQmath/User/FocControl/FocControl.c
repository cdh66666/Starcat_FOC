#include "FocControl.h"
#include "FocMath.h"
#include "AngleEncoder.h"
#include "FocSVPWM.h"
#include "Motor1GPIO.h"
#include "SysManage.h"



#include "stdio.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"


//FOC模型创建 FOC_Mod
FOC_Struct FOC_Mod = 
{
     0,         //电机使能标志
    _IQ(11),    //极对数

    _IQ(0),     //目标q电流
    _IQ(0),     //目标i电流
    _IQ19(28.081055), //偏移角度（校准固化）
	  _IQ19(0),   //机械角度
    _IQ19(0),   //电角度
    _IQ19(0),   //电角度弧度
    _IQ(0),     //电角度正弦值（非弧度）
    _IQ(0),     //电角度余弦值（非弧度）
    
    3,          //电流采样个数
    _IQ(0),     //a 相实际电流
    _IQ(0),     //b 相实际电流
	  _IQ(0),     //c 相实际电流

    _IQ(0),     //clarke变换后 * 2/3后 I_Alpha
    _IQ(0),     //clarke变换后 * 2/3后 I_Beta

    _IQ(0),     //park反变换后 I_Alpha_Svpwm
    _IQ(0),     //park反变换后 I_Beta_Svpwm
    
    _IQ(0),     //park变换后 Iq
    _IQ(0),     //park变换后 Id
    
    _IQ19(0),   //tarSpeed
    _IQ19(0),   //preSpeed
    _IQ19(0),   //tarPosition
    _IQ19(0),   //preSpeed
    
    _IQ(0),     //id pid
    _IQ(0),     //iq pid
    _IQ19(0),   //speed pid
		_IQ19(0),   //position pid
};
/*************************************************************
** Function name:      FocPID_Init
** Descriptions:       FOCPID初始化
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void FocPID_Init(void)
{
    //iq PID参数初始化
    FOC_Mod.iqPID.kp       = _IQ(-0.08);  //比例
    FOC_Mod.iqPID.ki       = _IQ(-0.08);  //积分
    FOC_Mod.iqPID.kd       = _IQ(0);  //微分
    FOC_Mod.iqPID.pre      = _IQ(0);  //实际值
    FOC_Mod.iqPID.tar      = _IQ(0);  //目标值
    FOC_Mod.iqPID.bias     = _IQ(0);  //本次偏差
    FOC_Mod.iqPID.lastBias = _IQ(0);  //上次偏差
    FOC_Mod.iqPID.out      = _IQ(0);  //PID输出
    FOC_Mod.iqPID.outMax   = _IQ(7.0);  //PID输出限幅
    //id PID参数初始化
    FOC_Mod.idPID.kp       = _IQ(-0.04);  //比例
    FOC_Mod.idPID.ki       = _IQ(-0.08);  //积分
    FOC_Mod.idPID.kd       = _IQ(0);  //微分
    FOC_Mod.idPID.pre      = _IQ(0);  //实际值
    FOC_Mod.idPID.tar      = _IQ(0);  //目标值
    FOC_Mod.idPID.bias     = _IQ(0);  //本次偏差
    FOC_Mod.idPID.lastBias = _IQ(0);  //上次偏差
    FOC_Mod.idPID.out      = _IQ(0);  //PID输出
    FOC_Mod.idPID.outMax   = _IQ(7.0);  //PID输出限幅
    //Speed PID参数初始化
    FOC_Mod.sPID.kp        = _IQ19(-0.7);  //比例
    FOC_Mod.sPID.ki        = _IQ19(-0.014);  //积分
    FOC_Mod.sPID.kd        = _IQ19(0);  //微分
    FOC_Mod.sPID.pre       = _IQ19(0);  //实际值
    FOC_Mod.sPID.tar       = _IQ19(0);  //目标值
    FOC_Mod.sPID.bias      = _IQ19(0);  //本次偏差
    FOC_Mod.sPID.lastBias  = _IQ19(0);  //上次偏差
    FOC_Mod.sPID.err       = _IQ19(0);  //累计偏差
    FOC_Mod.sPID.out       = _IQ19(0);  //PID输出
    FOC_Mod.sPID.outMax    = _IQ19(2.6f);  //PID输出限幅
    //Position PID参数初始化
    FOC_Mod.pPID.kp        = _IQ19(0.2);  //比例
    FOC_Mod.pPID.ki        = _IQ19(0.0);  //积分
    FOC_Mod.pPID.kd        = _IQ19(2.2);  //微分
    FOC_Mod.pPID.pre       = _IQ19(0);  //实际值
    FOC_Mod.pPID.tar       = _IQ19(0);  //目标值
    FOC_Mod.pPID.bias      = _IQ19(0);  //本次偏差
    FOC_Mod.pPID.lastBias  = _IQ19(0);  //上次偏差
    FOC_Mod.pPID.err       = _IQ19(0);  //累计偏差
    FOC_Mod.pPID.out       = _IQ19(0);  //PID输出
    FOC_Mod.pPID.outMax    = _IQ19(4.0);  //PID输出限幅
}

/*************************************************************
** Function name:      FocControl_Init
** Descriptions:       FOC控制初始化
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void FocControl_Init(void)
{
    FocPID_Init();                        //FOC所有PID参数初始化
    FocMotor_Enable(0);                   //FOC使能初始化  失能
	  Motor1PosPIDSetTar(_IQ19(80));         //目标位置初始化0
	  Motor1SpeedPIDSetTar(_IQ19(0));       //目标速度初始化0
    Motor1SetTarIDIQ(_IQ(0.0),_IQ(0.0));  //目标电流初始化0
}

/*************************************************************
** Function name:      FocMotor_Enable
** Descriptions:       FOC电机控制使能
** Input parameters:   200us
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void FocMotor_Enable(uint8_t able)
{
	 Motor1SetEnable(able);   //电机驱动使能
	  FOC_Mod.isEnable = 1;    //FOC运算使能
}

/*************************************************************
** Function name:      GetMotor_mAngle
** Descriptions:       获取电机当前机械角度
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
_iq19 GetMotor_mAngle(void)
{
	return FOC_Mod.mAngle- _IQ19(180);
}


/*************************************************************
** Function name:       Motor1SetTarIDIQ
** Descriptions:        设置电机1目标电流
** Input parameters:    id：d轴电流单位A
**						iq: q轴电流单位A
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor1SetTarIDIQ(_iq id,_iq iq)
{
    FOC_Mod.tarid = id;
    FOC_Mod.tariq = iq;
}
/*************************************************************
** Function name:       Motor1PosPIDSetTar
** Descriptions:        设置电机1目标位置
** Input parameters:    tarPos：目标位置 -180 0 180
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor1PosPIDSetTar(_iq19 tarPos)
{
    FOC_Mod.tarPos = tarPos;
}
/*************************************************************
** Function name:       Motor1SpeedPIDSetTar
** Descriptions:        设置电机1目标转速
** Input parameters:    tarSpeed：目标转速
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor1SpeedPIDSetTar(_iq19 tarSpeed)
{
    FOC_Mod.tarSpeed = tarSpeed;
}
/*************************************************************
** Function name:      FocPosControl_Loop
** Descriptions:       FOC位置闭环控制循环
** Input parameters:   200us
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void FocPosControl_Loop(void)
{
    PositionPIDControl(&FOC_Mod);
}

/*************************************************************
** Function name:      FocSpeedControl_Loop
** Descriptions:       FOC速度闭环控制循环
** Input parameters:   200us
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void FocSpeedControl_Loop(void)
{
    SpeedPIDControl(&FOC_Mod);
}

/*************************************************************
** Function name:      FocControl_Loop
** Descriptions:       FOC控制循环
** Input parameters:   100us
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void FocControl_Loop(void)
{
    GetMotor1_ElectricalAngleMT6816(&FOC_Mod);              //Step-> 1  获取电机 当前所有电气角度信息
    CurrentReconstruction(&FOC_Mod);                        //Step-> 2  获取三相电流，重构电流
    clarke_calc(&FOC_Mod);                                  //Step-> 3  Clarke变换
    park_calc(&FOC_Mod);                                    //Step-> 4  Park变换
    CurrentPIControlID(&FOC_Mod);                           //Step-> 5  电流PI闭环输出
    CurrentPIControlIQ(&FOC_Mod);
/*
    FOC_Mod.idPID.out = _IQ(0.0); //开环测试,正常运行一定要注释掉
    FOC_Mod.iqPID.out = _IQ(7.0); //开环测试,正常运行一定要注释掉
*/
    ipark_calc(&FOC_Mod);                                   //Step-> 6  反park变换
    SvpwmCtrl(FOC_Mod.I_Alpha_Svpwm,FOC_Mod.I_Beta_Svpwm);  //Step-> 7  生成SVPWM
}
/*************************************************************
** Function name:      FocControl_Loop
** Descriptions:       FOC控制循环
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
float Tar_Iq,Tar_Id;
float Pre_Iq,Pre_Id;
void Motor1FOCConfig_Printf(void)
{
//    Tar_Iq = _IQtoF(FOC_Mod.iqPID.tar);
//    Tar_Id = _IQtoF(FOC_Mod.idPID.tar);
//    Pre_Iq = _IQtoF(FOC_Mod.iqPID.pre);
//    Pre_Id = _IQtoF(FOC_Mod.idPID.pre);
    
    
//   printf("1:%f\r\n",_IQ19toF(FOC_Mod.preSpeed)); 
    printf("2:%f\r\n",_IQ19toF(FOC_Mod.mAngle));   
    
    
//    printf("1:%f\r\n",_IQ19toF(FOC_Mod.sPID.tar));
//    printf("2:%f\r\n",_IQ19toF(FOC_Mod.sPID.pre));
//    printf("3:%f\r\n",_IQ19toF(FOC_Mod.sPID.bias));
//    printf("4:%f\r\n",_IQ19toF(FOC_Mod.sPID.lastBias));
//    printf("5:%f\r\n",_IQ19toF(FOC_Mod.sPID.out));

//    printf("3:%f\r\n",_IQtoF(FOC_Mod.ia));
//    printf("4:%f\r\n",_IQtoF(FOC_Mod.ib));
//    printf("5:%f\r\n",_IQtoF(FOC_Mod.I_Alpha));
//    printf("6:%f\r\n",_IQtoF(FOC_Mod.I_Beta));
//    
//    printf("5:%f\r\n",_IQtoF(SVPWM_Mod.u1));
//    printf("6:%f\r\n",_IQtoF(SVPWM_Mod.u2));
//    printf("7:%f\r\n",_IQtoF(SVPWM_Mod.u3));
//    
//    printf("8:%f\r\n",_IQ19toF(FOC_Mod.mAngle));
//		printf("9:%f\r\n",_IQ19toF(FOC_Mod.eAngle));
//    
//    printf("9:%d\r\n",SVPWM_Mod.sector);
//    printf("10:%f\r\n",_IQtoF(SVPWM_Mod.u1));
//    printf("11:%f\r\n",_IQtoF(SVPWM_Mod.u2));
//    printf("12:%f\r\n",_IQtoF(SVPWM_Mod.u3));

}




