#include "Motor2ADC2.h"
#include "main.h"


#include "stdio.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"


extern ADC_HandleTypeDef hadc2;



//一介低通滤波系数
#define FILTER_KP 0.9f


struct SMotor2ADC2_Struct{
	uint32_t adDmaValue[3];			//DMA原始数据
	int adValue[3];					//电流实际AD值
	int lastAdValue[3];				//上次电流实际AD值
};
struct SMotor2ADC2_Struct gMotor2ADC2 = {0};
/*************************************************************
** Function name:       Motor2ADC2StartOnce
** Descriptions:        Motor2ADC开始一次DMA转换
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor2ADC2StartOnce(void)
{
	HAL_ADC_Start_DMA(&hadc2,(uint32_t*)gMotor2ADC2.adDmaValue, 3);
}

/*************************************************************
** Function name:       Motor2ADCValueStorage
** Descriptions:        Motor2存储DMA获取到的数据
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor2ADC2ValueStorage(void)
{
	gMotor2ADC2.adValue[0] = gMotor2ADC2.adDmaValue[0] - 2048;
	gMotor2ADC2.adValue[1] = gMotor2ADC2.adDmaValue[1] - 2048;
	gMotor2ADC2.adValue[2] = gMotor2ADC2.adDmaValue[2] - 2048;
}
/*************************************************************
** Function name:       GetMotor2ADC2PhaseXValue
** Descriptions:        获取 Motor23相电流
** Input parameters:    Phase：1：A相 2：B相 3：C相
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
int32_t GetMotor2ADC2PhaseXValue(uint8_t Phase)
{
	if (Phase >= 3) {
		return 0;
	}
	int32_t value;
	value = (int32_t)(FILTER_KP * (float)gMotor2ADC2.adValue[Phase] + (1-FILTER_KP) * (float)gMotor2ADC2.lastAdValue[Phase]);
	gMotor2ADC2.lastAdValue[Phase] = value;
	return value;
}




