#include "Uart1.h"
#include "main.h"
#include "RingBuffer.h"




extern UART_HandleTypeDef huart1;

RingBuffer_EXPORT(gUart1_RX_RingBuffer,uint8_t,Uart1_RINGBUFLENGTH);
RingBuffer_EXPORT(gUart1_TX_RingBuffer,uint8_t,Uart1_RINGBUFLENGTH);


/*************************************************************
** Function name:       Uart1_IT_RxTxHandle
** Descriptions:        串口一中断服务函数
** Input parameters:    no
** Output parameters:   no
** min:                 no
** Returned             no
*************************************************************/
void Uart1_IT_Handle(void)
{
	uint8_t data = 0;
	if (__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE)) {
		if(RingBuffer_IsFull(&gUart1_RX_RingBuffer) != true){
            uint8_t data = huart1.Instance->DR;
            RingBuffer_En(&gUart1_RX_RingBuffer,&data);
        }
	}
	if (__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TXE)) {
		if(RingBuffer_IsEmpty(&gUart1_TX_RingBuffer) != true){
            RingBuffer_De(&gUart1_TX_RingBuffer,&data);
            huart1.Instance->DR = data;
        }else{
            __HAL_UART_DISABLE_IT(&huart1,UART_IT_TXE);
        }

	}
}
/*************************************************************
** Function name:       Uart1_SendData
** Descriptions:        串口一发送数据函数
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
uint8_t Uart1_SendData(uint8_t *pData,uint16_t length)
{
	uint16_t cir = 0;
	if (RingBuffer_GetUnuseMsgNum(&gUart1_TX_RingBuffer) < length) {
		return 1;
	}
	if (length == 0) {
		return 2;
	}
    for(cir = 0;cir < length;cir++){
        if (RingBuffer_IsFull(&gUart1_TX_RingBuffer) != true){
            RingBuffer_En(&gUart1_TX_RingBuffer,&pData[cir]);
        } else {
            break;
        }
    }
    if(length != 0){
        __HAL_UART_ENABLE_IT(&huart1,UART_IT_TXE);
    }
	return 0;
}
/*************************************************************
** Function name:       Uart1_ReadData
** Descriptions:        读取数据
** Input parameters:    pData：当前接收到的所有数据
**                      length:需要读取的数据长度
** Output parameters:   no
** min：                no
** Returned             返回读取的数据个数
*************************************************************/
uint16_t Uart1_ReadData(uint8_t *pData,uint16_t length){
    uint16_t cir = 0;
    for(cir = 0;cir < length;cir++){
        if(RingBuffer_IsEmpty(&gUart1_RX_RingBuffer) == true){
            return cir;
        }
        RingBuffer_De(&gUart1_RX_RingBuffer,pData++);
    }
    return cir;
}
/*************************************************************
** Function name:       Uart1_GetDataNums
** Descriptions:        获取串口一接收到的未处理的数据个数
** Input parameters:    no
** Output parameters:   no
** min：                no
** Returned             返回数据个数
*************************************************************/
uint16_t Uart1_GetDataNums(void)
{
    RingBuffer_GetMsgNum(&gUart1_RX_RingBuffer);
}
/*************************************************************
** Function name:       Uart1_LookData
** Descriptions:        看数据
** Input parameters:    pData：当前接收到的所有数据
**                      length:需要读取的数据长度
** Output parameters:   no
** min：                no
** Returned             返回读取状态 0：成功 1：失败（数据数量不够）
*************************************************************/
uint8_t Uart1_LookData(uint8_t *pData,uint16_t length)
{
    return RingBuffer_Look(&gUart1_RX_RingBuffer,pData,length);
}


