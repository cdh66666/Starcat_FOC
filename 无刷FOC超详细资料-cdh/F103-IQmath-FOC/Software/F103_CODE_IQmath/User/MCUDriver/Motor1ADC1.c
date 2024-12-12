#include "Motor1ADC1.h"
#include "main.h"
#include "adc.h"

#include "stdio.h"
#include "SEGGER_RTT.h"
#include "SEGGER_RTT_Conf.h"


struct SMotor1ADC1_Struct
{
	uint32_t adDmaValue[4];			//DMA原始数据
	int32_t adValue[4];					//电流实际AD值
	_iq19 lastAdValue[4];	      //上次电流实际AD值
};
struct SMotor1ADC1_Struct gMotor1ADC1 = {0};

/*************************************************************
** Function name:       Motor1ADC1StartOnce
** Descriptions:        Motor1ADC开始一次DMA转换
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor1ADC1StartOnce(void)
{
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)gMotor1ADC1.adDmaValue, 3);
}

/*************************************************************
** Function name:       Motor1ADCValueStorage
** Descriptions:        Motor1存储DMA获取到的数据
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
//int32_t ADa,ADb,ADc; //Debug
void Motor1ADC1ValueStorage(void)
{
    gMotor1ADC1.adValue[0] =  gMotor1ADC1.adDmaValue[0];
	gMotor1ADC1.adValue[1] =  gMotor1ADC1.adDmaValue[1] - 1993;   //1993为电机不转时刻的ADC采样值
	gMotor1ADC1.adValue[2] =  gMotor1ADC1.adDmaValue[2] - 1983;
	gMotor1ADC1.adValue[3] = 0 - gMotor1ADC1.adValue[1] - gMotor1ADC1.adValue[2];
	
//	ADa = (int32_t)gMotor1ADC1.adValue[1]; //Debug
//	ADb = (int32_t)gMotor1ADC1.adValue[2]; //Debug
//	ADc = (int32_t)gMotor1ADC1.adValue[3]; //Debug
}
/*************************************************************
** Function name:       GetMotor1ADC1PhaseXValue
** Descriptions:        获取 Motor13相电流
** Input parameters:    Phase：1：A相 2：B相 3：C相
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
_iq19 GetMotor1ADC1PhaseXValue(uint8_t Phase)
{
	if (Phase >= 4) {
		return 0;
	}
    _iq19 iqvalue;
    
    iqvalue = _IQ19mpy(_IQ19((float)gMotor1ADC1.adValue[Phase]),FILTER_KP) + _IQ19mpy((float)gMotor1ADC1.lastAdValue[Phase],(_IQ19(1)-FILTER_KP));
    gMotor1ADC1.lastAdValue[Phase] = iqvalue;
    iqvalue = _IQ19(gMotor1ADC1.adValue[Phase]);
	return iqvalue;
}




