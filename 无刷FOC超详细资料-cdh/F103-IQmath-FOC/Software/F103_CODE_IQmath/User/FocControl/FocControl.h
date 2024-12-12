/***********************************************************************
 * @file FocControl.h
 * Description:
 *
 **********************************************************************/
#ifndef __FocControl_h__
#define __FocControl_h__

#include "stdint.h"
#include "IQmathLib.h"

// 速度位置环 PID结构体
struct PID_Struct
{
    /* data */
    _iq19 kp;
    _iq19 ki;
    _iq19 kd;

    _iq19 pre;
    _iq19 tar;
    _iq19 bias;
    _iq19 lastBias;
    _iq19 err;
    _iq19 out;
    _iq19 outMax;
};
// 电流环 PI结构体
struct PI_Struct
{
    /* data */
    _iq kp;
    _iq ki;
    _iq kd;

    _iq pre;
    _iq tar;
    _iq bias;
    _iq lastBias;
    _iq out;
    _iq outMax;
};

// FOC结构体创建
typedef struct
{
    uint8_t isEnable; //电机使能标志
    _iq polePairs;    //极对数

    _iq tariq;        //目标q电流
    _iq tarid;        //目标i电流
    _iq19 mAngle_Offect;//偏移角度
	_iq19 mAngle;       //机械角度
    _iq19 eAngle;       //电角度
    _iq eRadian;      //电角度弧度
    _iq eAngleSine;   //电角度正弦值（非弧度）
    _iq eAngleCosine; //电角度余弦值（非弧度）

    uint8_t iNum;     //a 电流采样个数
    _iq ia;           //a 相实际电流
    _iq ib;           //b 相实际电流
	_iq ic;           //c 相实际电流

    _iq I_Alpha;      //clarke变换后 * 2/3后 I_Alpha
    _iq I_Beta;       //clarke变换后 * 2/3后 I_Beta

    _iq I_Alpha_Svpwm;//park反变换后 I_Alpha_Svpwm
    _iq I_Beta_Svpwm; //park反变换后 I_Beta_Svpwm
    
    _iq Iq;           //park变换后 Iq
    _iq Id;           //park变换后 Id
    
    _iq19 tarSpeed;     //目标转速 机械角度
    _iq19 preSpeed;     //当前转速 机械角度
		_iq19 tarPos;       //目标位置 机械角度
		_iq19 prePos;       //当前位置 机械角度
    struct PI_Struct idPID;    //id pid
    struct PI_Struct iqPID;    //iq pid
    struct PID_Struct sPID;    //speed pid
		struct PID_Struct pPID;    //position pid
 
}FOC_Struct;

extern FOC_Struct FOC_Mod;


void FocMotor_Enable(uint8_t able);
void FocControl_Init(void);
void FocControl_Loop(void);
void Motor1FOCConfig_Printf(void);
void Motor1SetTarIDIQ(_iq id,_iq iq);
void Motor1PosPIDSetTar(_iq19 tarPos);
void Motor1SpeedPIDSetTar(_iq19 tarSpeed);
void FocSpeedControl_Loop(void);
void FocPosControl_Loop(void);
_iq19 GetMotor_mAngle(void);

#endif // __FocControl_h__



