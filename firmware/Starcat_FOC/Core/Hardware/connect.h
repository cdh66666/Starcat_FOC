 #ifndef HARDWARE_CONNECT_H_
#define HARDWARE_CONNECT_H_

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "usart.h"

#include "DRV8301.h"
#include "flash.h"

#define usart_connect huart4



typedef enum
{
	false=0,
	true=1,
}bool;


//缓冲数组
typedef struct {
    uint8_t buffer[1024];
    int data_len;
    bool new_data_available;
} SerialDataBuffer;



extern float KP;
extern float KI;
extern float KD;

//串口重定向,输出直接用printf
int fgetc(FILE *f);
//串口接收中断函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
//通信接收初始化
void Connect_Init(void);
// voa快速发送数据函数
void vofa_JustFloat_output(float *data, uint8_t count);

// 函数用于打印无符号 16 位整数的二进制表示
void printBinary(uint16_t num);

// 函数用于打印无符号 8 位整数的二进制表示
void printBinary_8(uint8_t num);
//重启DMA接收
void restartDMA(UART_HandleTypeDef *huart);
//处理串口接收到的数据
void processSerialData(void);






#endif /* HARDWARE_CONNECT_H_ */
