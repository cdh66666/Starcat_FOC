#include "UartBus.h"
#include "string.h"
#include "AngleEncoder.h"

//串口结构体创建
UartBus_Struct UartBus = {0};
/*************************************************************
** Function name:      UartBus_Init
** Descriptions:       Uart初始化
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void UartBus_Init(void)
{
    UartBus.FrameStart1 = 0xFE; //帧头1
    UartBus.FrameStart2 = 0x55; //帧头2
    UartBus.FrameLength = 0x0A; //帧长度
    UartBus.FrameEnd    = 0xFD; //帧尾
    
    UartBus.FrameReceiveFalg  = 0x00;  //帧接收完成标志位
    UartBus.DMA_ReceiveLength = 0x00; //DMA接收数据长度
}
/*************************************************************
** Function name:      UartBus_Loop
** Descriptions:       Uart功能循环
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         none
** Created date:       none
*************************************************************/
void UartBus_Loop(void)
{
    static uint8_t Tx_Data[5];
    static int16_t PreAngle = 0;
    static uint8_t PreAngle_B = 0;
    static uint8_t PreAngle_S = 0;
    static uint8_t PreAngle_G = 0;
    
    
    if(UartBus.FrameReceiveFalg == 1)  //DMA接收一帧数据完成
    {
        UartBus.FrameReceiveFalg = 0;
        if((UartBus.UartRxDataBuf[0] == UartBus.FrameStart1)&&
            (UartBus.UartRxDataBuf[1] == UartBus.FrameStart2)&& 
             (UartBus.UartRxDataBuf[2] == UartBus.FrameLength)&&
              (UartBus.UartRxDataBuf[UartBus.FrameLength-1] == UartBus.FrameEnd))
        {
            memcpy(UartBus.UartRxDataAct,  UartBus.UartRxDataBuf,  UartBus.FrameLength);//结构体缓存转存至可用数组
            memset(UartBus.UartRxDataBuf,0,MaxDataBufSize);                             //结构体缓存清零
            HAL_UART_Transmit_DMA(&huart3,UartBus.UartRxDataAct,UartBus.DMA_ReceiveLength);
        }
        
    }
    
    //测试
    PreAngle = (int16_t)GetMotor1FOC_mAngle();

    PreAngle_B = PreAngle/100;
    PreAngle_S = PreAngle%100/10;
    PreAngle_G = PreAngle%10;
    
    Tx_Data[0] = 48 + PreAngle_B;
    Tx_Data[1] = 48 + PreAngle_S;
    Tx_Data[2] = 48 + PreAngle_G;
    Tx_Data[3] = 0x0d;
    Tx_Data[4] = 0x0a;
    
    
    HAL_UART_Transmit_DMA(&huart3,Tx_Data,5);
}



