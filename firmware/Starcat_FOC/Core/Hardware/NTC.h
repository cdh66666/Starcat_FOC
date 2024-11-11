/*
 * NTC.h
 *
 *  Created on: Jul 29, 2024
 *      Author: Administrator
 */

#ifndef HARDWARE_NTC_H_
#define HARDWARE_NTC_H_

#include <math.h>
#include "main.h"
//获取温度值,传入NTC热敏电阻阻值
float Get_Temp(float Rt);
// 生成查找表
void generateLookupTable(void);
// 二分查找获取温度
float getTemperatureFromTable(float Rt);



#endif /* HARDWARE_NTC_H_ */
