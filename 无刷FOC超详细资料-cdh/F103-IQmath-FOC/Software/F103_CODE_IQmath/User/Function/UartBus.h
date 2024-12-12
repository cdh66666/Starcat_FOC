/***********************************************************************
 * @file UartBus.h
 * Description:
***********************************************************************/
#ifndef __UartBus_h__
#define __UartBus_h__

#include "stdint.h"
#include "usart.h"

// 串口结构体创建
typedef struct
{
    uint8_t  FrameStart1; //帧头1
    uint8_t  FrameStart2; //帧头2
    uint16_t FrameLength; //帧长度
    uint8_t  FrameEnd;    //帧尾
    
    uint8_t  FrameReceiveFalg;  //帧接收完成标志位
    uint16_t DMA_ReceiveLength; //DMA接收数据长度
    
    uint8_t UartRxDataBuf[MaxDataBufSize]; //串口帧数据缓存
    uint8_t UartRxDataAct[MaxDataBufSize]; //串口可用数据
 
}UartBus_Struct;

extern UartBus_Struct UartBus;

void UartBus_Init(void);
void UartBus_Loop(void);
    


#endif // __UartBus_h__
