#ifndef HARDWARE_FOC_H_
#define HARDWARE_FOC_H_

#include "main.h"

#include "DRV8301.h"
#include "MT6701.h"
#include "tim.h"
#include "math.h"
#include "arm_math.h"
#include "NTC.h"
#include "adc.h"
#include "flash.h"	//用于存储电机参数

#include "connect.h"

/*
 * 克拉克变换公式:  Iα=ia; Iβ=(2*ib+ia)/genhao3;
 * 克拉克逆变换公式: ia=Iα;ib=(sqrt_3*Iβ-Iα)/2.0;ic=(-Iα-sqrt_3*Iβ)/2.0;
 * 帕克逆变换公式:   Uα=Ud*cosθ-Uq*sinθ;Uβ=Uq*cosθ+Ud*sinθ;
 * 帕克变换公式:    Ud=Uα*cosθ+sinθ*Uβ;Uq=-sinθ*Uα+cosθ*Uβ;
*/

/*无刷电机型号:4010,24N22P(磁铁22片),内阻12.5欧姆,工作电压11.1-14.8V,KV值76KV*/
/*无刷电机型号:LD2808-700kV,12N14P(磁铁22片),内阻0.5欧姆,工作电压3S-4S */

/************************************************************************/
// 定义限幅宏
#define LIMIT(value, minVal, maxVal) (((value) > (maxVal))? (maxVal) : (((value) < (minVal))? (minVal) : (value)))

#define Sample_R    0.01f 	//采样电阻阻值
#define _1_sqrt3    0.5773502691896f
#define sqrt3 		1.732050807f
#define sqrt3_2		0.866025403f
#define PI_3 		1.04719755f 	//三分之pi
#define PI_2 		1.570796326f 	//二分之pi
#define _3_PI 		9.42477796f 	//3pi
#define _2_PI  		6.28318530718f //2pi



typedef struct {
    uint8_t flag;
    float time;
    TIM_HandleTypeDef *htim_us;
} TimerStruct;
extern TimerStruct timer_foc;
/************************以下是无刷电机重要参数***************************************/
//Rs,Ls,Ld,Lq,转子磁链，反电动势系数，转动惯量，摩擦系数
typedef struct
{
	float zero_electric_angle;	//零电角度
	float DIR;			//旋转方向
	float Pole_pair;	//磁极对数

    float Rs; // 定子电阻
    float Ls; // 定子电感
    float Ld; // 直轴电感
    float Lq; // 交轴电感
    float Gr; // 转子磁链
    float Ke; // 反电动势系数
    float J;  // 转动惯量
    float u;  // 摩擦系数
}Motor_Parameters_Typedef;

typedef struct {
    float kp;
    float ki;
    float kd;
    float integral;
    float prevError;
    float integralLimitMin;
    float integralLimitMax;
} PIDControllerFloat;

typedef struct
{

	uint16_t PWM_Period;	//ARR最大值
	TIM_HandleTypeDef *htim;				// 控制三相逆变的定时器
	DRV8301_TypeDef *DRV8301;				// 驱动芯片结构体
	Motor_Parameters_Typedef *Parameters;	// 电机参数结构体
	MT6701_Encoder *encoder;				// 编码器结构体
	ADC_HandleTypeDef *hadc;					// adc采样结构体

	float interruptFrequency;				//定时器pwm频率
	int run_flag;							//foc运行标志位

	float I_A;	 	// A 相电流
	float I_B; 		// B 相电流
	float I_C;		// A 相电流
	float I_A_Offset;// A 相电流偏移量
	float I_B_Offset;// B 相电流偏移量
//	float I_C_Offset;// B 相电流偏移量
	float I_d;	//电机保持力矩电流矢量
	float I_q;	//电机旋转力矩电流矢量
	float Torque;//电机实时力矩

	float except_vel;	//期望速度
	float except_iq;	//期望电流iq
	float except_id;	//期望电流id
	int count;			//计数值
	float electricalAngle;//电机电角度

	float speed;		  //电机转速 r/s
	float prev_speed;
	PIDControllerFloat *Iq_pid;//电流环pid参数
	PIDControllerFloat *Id_pid;//电流环pid参数
	PIDControllerFloat *V_pid;//速度环pid参数
	PIDControllerFloat *P_pid;//位置环pid参数


//	float I_Coeff;//adc电流转换系数：1/4095.0f*3.3f/ Sample_R / motor1.DRV8301->I_Gain
}Motor_Run_Typedef;



extern Motor_Run_Typedef motor1;
extern Motor_Run_Typedef motor2;



//只调用一次
void FOC_Adc_Init(void);
//FOC运行初始化
void FOC_Init(Motor_Run_Typedef *motor);
//电流采样初始化
void FOC_Current_Init(Motor_Run_Typedef *motor);
//常规采样初始化
void FOC_Regular_ADC_INIT(void);
//简单运行foc算法
void FOC_Run(Motor_Run_Typedef *motor);
// FOC-adc采样
void FOC_Regular_ADC(void);
//设置三相占空比
void FOC_SVPWM(Motor_Run_Typedef *motor,float Uq, float Ud, float theta);
//pid运算函数
float pidUpdateFloat(PIDControllerFloat* pid, float expectedValue, float actualValue);
//闭环力距控制函数
void FOC_CLOSED_Iq(Motor_Run_Typedef *motor,float Iq,float Id);
//闭环速度控制函数 r/s ,返回iq
float FOC_CLOSED_Vel(Motor_Run_Typedef *motor,float speed);
//闭环位置控制函数 单位：弧度
float FOC_CLOSED_Pos(Motor_Run_Typedef *motor,float Pos);
//使能电机驱动芯片,输出UVW三相电压
void FOC_EN(Motor_Run_Typedef *motor,OperationType2 ON_OFF);
//开启foc
void FOC_Start(Motor_Run_Typedef *motor);
//关闭foc
void FOC_Stop(Motor_Run_Typedef *motor);
//获取电源电压
float FOC_Get_VBUS(void);
//获取温度1
float FOC_Get_TEMP1(void);
//获取温度2
float FOC_Get_TEMP2(void);

//电机参数识别
int Motor_Parameter_Identification(Motor_Run_Typedef *motor,float* data);
// 归一化角度到 [0,2PI]
float _normalizeAngle(float angle);
//获取电角度
float _electricalAngle(Motor_Run_Typedef *motor);
// 获取定时器的自动重装载值（ARR）
uint16_t getTimerARR(TIM_HandleTypeDef *htim);
//计算进中断的频率
void calculateInterruptFrequency(Motor_Run_Typedef *motor);
//获取三相电流值
void FOC_Get_Ia_Ib_Ic(Motor_Run_Typedef *motor);
//a,b,c三相电流转换成Id,Iq
void FOC_clarke_park_Transform(Motor_Run_Typedef *motor);
//获取电机电角度
void FOC_Get_electricalAngle_speed(Motor_Run_Typedef *motor);


#endif /* HARDWARE_FOC_H_ */
