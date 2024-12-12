#ifndef Protocol_h
#define Protocol_h

#include "stdint.h"


extern volatile uint32_t gProtocol_TimeCNT;

/*************************************************************
** Function name:       SysPower_TIMEBASE
** Descriptions:        时基,放在周期为1ms的函数里面执行
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
*************************************************************/
#define Protocol_TIMEBASE(ms)                \
        gProtocol_TimeCNT+= ms

/*************************************************************
** Function name:       SysPower_GETTIME
** Descriptions:        获取起始时间
** Input parameters:    None
** Output parameters:   None
** Returned value:      (uint32_t)起始时间
*************************************************************/
#define Protocol_GETTIME(void)                        \
    gProtocol_TimeCNT

/*************************************************************
** Function name:       SysPower_TIMEOUT
** Descriptions:        检查超时
** Input parameters:    timeOut：(uint32_t)超时时间
**                      startTime:(uint32_t)开始的时间
** Output parameters:   None
** Returned value:      false,未超时，true，超时
*************************************************************/
#define Protocol_TIMEOUT(timeOut,startTime)                \
    ((gProtocol_TimeCNT - startTime) >= timeOut ? 1 : 0)



void DataPacking(uint8_t *data,uint8_t length);
uint8_t DataFiltering(void);
void DataProcessing(void);
void ProtocolExcel(void);
#endif /* Protocol_h */
