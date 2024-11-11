/*
 * DRV8301.h
 *
 *  Created on: Aug 2, 2024
 *      Author: Administrator
 */

#ifndef HARDWARE_DRV8301_H_
#define HARDWARE_DRV8301_H_


#include "main.h"
#include "connect.h"


//如果定义了DEBUG_MODE，会打印调试信息
//#define DEBUG_MODE


// 读写操作宏定义
typedef enum {
    READ,
    WRITE
} OperationType;
// 开关操作宏定义
typedef enum {
    OFF,
	ON,
} OperationType2;
// pwm mode宏定义
typedef enum {
    PWM_3_MODE,
	PWM_6_MODE,
} Pwm_Mode_Type;
typedef enum {
	peak_current_1A7=0x0 ,
	peak_current_0A7=0x1,
	peak_current_0A25=0x2,

} Peak_Current_Type;
// 电测检测增益倍数
typedef enum {
	Gain_10=0x00,	//0x00
	Gain_20=0x01,	//0x01
	Gain_40=0x02,	//0x02
	Gain_80=0x03,	//0x03
} Gain_Type;
// 过流检测电压阈值
typedef enum {
	Vds_0_060=0x0,	//0x0
	Vds_0_109=0x5,	//0x5
	Vds_0_155=0x8,	//0x8
	Vds_0_403=0x10,	//0x10
} OC_Vds_Type;



#define DRV8301_FAULT   0x8000
//寄存器地址定义
#define Status_Register_1 	0x0000	//大部分设备错误状态
#define Status_Register_2 	0x0001	//GVDD_OV设备错误状态和设备id
#define Control_Register_1 	0x0002	//驱动电流设置，驱动复位设置，PWM模式设置，过流保护模式，过流电压设置
#define Control_Register_2 	0x0003	//过流过温模式设置，电流采样增益设置，校准设置

#define GATE_CURRENT_OFFSET 0		//在寄存器中偏移值
#define GATE_RESET_OFFSET   2
#define PWM_MODE_OFFSET  	3
#define OCP_MODE_OFFSET     4
#define OC_ADJ_SET_OFFSET   6

#define OCTW_MODE_OFFSET    0		//在寄存器中偏移值
#define GAIN_OFFSET         2
#define DC_CAL_CH1_OFFSET  	4
#define DC_CAL_CH2_OFFSET   5
#define OC_TOFF_OFFSET      6

//电流检测增益配置
/*
 * The output of the current shunt amplifier can be calculated as.
	V=REF/2 -G*(SNx- SPx)
	where
	VREF is the reference voltage (REF pin) --- 3.3V
	G is the gain of the amplifier(10,20,40 or 80 V/)
	SNx and SPx are the inputs of channel x.
 * */


//DRV8301参数定义
typedef struct
{
	GPIO_TypeDef 		*NSCS_PORT;		//CS引脚定义
	uint16_t       		 NSCS_PIN;
	GPIO_TypeDef 		*SCLK_PORT;		//SCLK引脚定义
	uint16_t       		 SCLK_PIN;
	GPIO_TypeDef 		*SDI_PORT;		//SDI引脚定义
	uint16_t       		 SDI_PIN;
	GPIO_TypeDef 		*SDO_PORT;		//SDO引脚定义
	uint16_t       		 SDO_PIN;
	GPIO_TypeDef 		*EN_GATE_PORT;		//EN_GATE引脚定义
	uint16_t       		 EN_GATE_PIN;
	GPIO_TypeDef 		*NFAULT_PORT;		//NFAULT引脚定义		需要上拉，下降沿中断
	uint16_t       		 NFAULT_PIN;

}DRV8301_GPIO_TypeDef;

//DRV8301保护参数
typedef struct
{
	OperationType2 FAULT;		//是否有错误状态
	OperationType2 GVDD_UV;		//栅极驱动欠压 <8V
	OperationType2 PVDD_UV;		//供电电源欠压 <6V
	OperationType2 OTSD;		//过温锁定驱动器标志位
	OperationType2 OTW;			//过温警告 >150°
	OperationType2 FETHA_OC;	//MOS过流 A_H
	OperationType2 FETLA_OC;	//MOS过流 A_L
	OperationType2 FETHB_OC;	//MOS过流 B_H
	OperationType2 FETLB_OC;	//MOS过流 B_L
	OperationType2 FETHC_OC;	//MOS过流 C_H
	OperationType2 FETLC_OC;	//MOS过流 C_L

	OperationType2 GVDD_OV;	    //栅极驱动过压 >16V

	uint16_t Device_ID;			//设备id

}DRV8301_Protect_TypeDef;


//DRV8301参数定义
typedef struct
{
	int id;
	OperationType2 FAULT_FLAG;	//是否出错标志位
	DRV8301_GPIO_TypeDef *GPIO_Parameter;
	DRV8301_Protect_TypeDef  *Protect_Parameter;
	OperationType2 EN_State;
	Gain_Type I_Gain_mode;
	float I_Gain;
	Pwm_Mode_Type PWM_Mode;
}DRV8301_TypeDef;


extern DRV8301_TypeDef DRV8301_1,DRV8301_2;

//双路芯片初始化
void DRV8301_Init(DRV8301_TypeDef *DRV8301);
// SPI 读写指定寄存器的值
uint16_t DRV8301_SPI_ReadWrite(DRV8301_TypeDef *DRV8301, uint8_t addr, uint16_t data, OperationType operation);
//进行电流检测通道校准
void DRV8301_DC_CAL(DRV8301_TypeDef *DRV8301,OperationType2 ON_OFF);
//电机使能/失能
void DRV8301_EN(DRV8301_TypeDef *DRV8301,OperationType2 ON_OFF);
//进行电流检测通道校准
void DRV8301_DC_CAL(DRV8301_TypeDef *DRV8301,OperationType2 ON_OFF);
//3PWM mode / 6PWM mode切换
void DRV8301_PWM_Mode(DRV8301_TypeDef *DRV8301,Pwm_Mode_Type mode);
//设置电流增益值
void DRV8301_Set_Gain(DRV8301_TypeDef *DRV8301,Gain_Type Gain);
//adc采样
void DRV8301_ADC_DOUBLE(void);
//设置NMOS过流保护值为0.109V，因为使用的NMOS导通电阻7mΩ,所以当NMOS中流过电流超过15.5A才会触发过流保护
void DRV8301_Set_OC(DRV8301_TypeDef *DRV8301,OC_Vds_Type Vds);
//设置栅极驱动电流，不能太大
void DRV8301_GATE_CURRENT(DRV8301_TypeDef *DRV8301,Peak_Current_Type current);
//读DRV8301里的保护参数,无异常返回1
int DRV8301_Read_Protect(DRV8301_TypeDef *DRV8301 );
//清除电机错误状态
void DRV8301_GATE_RESET(DRV8301_TypeDef *DRV8301 );
//检查是否出错
int DRV8301_Check_Fault(DRV8301_TypeDef *DRV8301);

#endif /* HARDWARE_DRV8301_H_ */
