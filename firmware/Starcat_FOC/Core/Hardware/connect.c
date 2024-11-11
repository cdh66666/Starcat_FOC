#include "connect.h"

SerialDataBuffer serialBuffer;
float KP=0;
float KI=0;
float KD=0;
int DISPLAY_DATA=1; //显示数据标志位
uint8_t usart_rx_buf[1024];  // 假设缓冲区大小为 1024
float uq=0.0f;
float ud=0.0f;
float vel=0.0f;
__IO float pos=0.0f;

//通信接收初始化
void Connect_Init(void)
{
        //打开dma接收，ILDE中断
        //一定要记得在NVIC里开启相应串口中断
        HAL_UARTEx_ReceiveToIdle_DMA(&usart_connect,usart_rx_buf,1024);
        printf("connect succeed!\r\n");
}
// 函数用于打印无符号 16 位整数的二进制表示
void printBinary(uint16_t num) {
    for (int i = 15; i >= 0; i--) {
        // 通过位运算判断当前位是 0 还是 1
        uint16_t bit = (num >> i) & 1;
        printf("%d", bit);
    }
}
// 函数用于打印无符号 8 位整数的二进制表示
void printBinary_8(uint8_t num) {
    for (int i = 7; i >= 0; i--) {
        // 通过位运算判断当前位是 0 还是 1
    	uint8_t bit = (num >> i) & 1;
        printf("%d", bit);
    }
}

//重启DMA接收
void restartDMA(UART_HandleTypeDef *huart) {
    huart->hdmarx->Instance->NDTR = 1024;
    HAL_UARTEx_ReceiveToIdle_DMA(huart, (uint8_t *)usart_rx_buf, 1024);
}


//串口中断回调函数
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if(huart == &usart_connect)
	{
	    // 获取帧长并存入缓冲区长度变量
	    serialBuffer.data_len = 1024 - huart->hdmarx->Instance->NDTR;
	    // 将数据存入缓冲区
	    memcpy(serialBuffer.buffer, usart_rx_buf, serialBuffer.data_len);
	    serialBuffer.new_data_available = true;

	    // 不再重新开启 DMA 接收，因为数据已经存入缓冲区
	    // restartDMA(huart);

	}

}


int int1=0,int2=0;
float float1=0.0,float2=0.0;
//处理串口接收到的数据
void processSerialData(void)
{
    if (serialBuffer.new_data_available )
    {
        char tempBuffer[1024];
        memcpy(tempBuffer, serialBuffer.buffer, serialBuffer.data_len);


        /****************条件判断和处理**********************/
        if(sscanf(tempBuffer,"P:%f",&float1)>0)
                    {

                    	KP = float1;
    //                	printf("KP:%7.2f\n",KP);
                    }
                    else if(sscanf(tempBuffer,"I:%f",&float1)>0)
                    {
                    	KI= float1;

    //                	printf("KI:%7.2f\n",KI);
                    }
                    else if(sscanf(tempBuffer,"D:%f",&float1)>0)
                    {

                    	KD = float1;
    //                	printf("KD:%7.2f\n",KD);
                    }
                    else if(sscanf(tempBuffer,"uq:%f",&float1)>0)
                    {

                    	uq = float1;
    //                	printf("KP:%7.2f\n",KP);
                    }
                    else if(sscanf(tempBuffer,"ud:%f",&float1)>0)
                    {

                    	ud = float1;
    //                	printf("KP:%7.2f\n",KP);
                    }
                    else if(sscanf(tempBuffer,"vel:%f",&float1)>0)
                    {
                    	vel= float1;

    //                	printf("KI:%7.2f\n",KI);
                    }
                    else if(sscanf(tempBuffer,"pos:%f",&float1)>0)
                    {

                    	pos = float1;
    //                	printf("KD:%7.2f\n",KD);
                    }
                    else if(sscanf(tempBuffer,"RESET:%d",&int1)>0)
                    {
                    	if(int1==1)
                    	{
                            // 在适当的函数中调用
                            NVIC_SystemReset();

                    	}

    //                	printf("KD:%7.2f\n",KD);
                    }
                    else if(sscanf(tempBuffer,"CLEAR:%d",&int1)>0)
                    {
                    	if(int1==1)
                    	{
                    		  clear_Data(DRV8301_1.id);
                    		  clear_Data(DRV8301_2.id);
                    		  printf("clear_Data Succeed!\n");
                    	}
                    }
                    else if(sscanf(tempBuffer,"CHECK:%d",&int1)>0)
                    {
                    	if(int1==1)
                    	{
                     		//读寄存器参数
                     		 DRV8301_Read_Protect(&DRV8301_1);

                     		 DRV8301_Read_Protect(&DRV8301_2);

                    	}
                    }
                    else if(sscanf(tempBuffer,"DISPLAY_DATA:%d",&int1)>0)
                    {
                    	if(int1==1)
                    	{
                    		DISPLAY_DATA=1;
                    	}
                    	else
                    	{
                    		DISPLAY_DATA=0;
                    	}
                    }
                    else if(sscanf(tempBuffer,"clear_fault:%d",&int1)>0)
                    {
                    	if(int1==1)
                    	{
                      		 DRV8301_GATE_RESET(&DRV8301_1);
                      		 DRV8301_GATE_RESET(&DRV8301_2);

                      		//读寄存器参数
                      		 DRV8301_Read_Protect(&DRV8301_1);

                      		 DRV8301_Read_Protect(&DRV8301_2);
                    	}
                    }
                    else if(sscanf(tempBuffer,"EN:%d",&int1)>0)
                    {
                    	if(int1==1)
                    	{
                    		DRV8301_EN(&DRV8301_1,ON);
                    	}
                    	else
                    	{
                    		DRV8301_EN(&DRV8301_1,OFF);
                    	}
                    }

        /****************处理完之后重启DMA**********************/
        serialBuffer.new_data_available = false;
        restartDMA(&usart_connect);

    }
}







// voa快速发送数据函数
void vofa_JustFloat_output(float *data, uint8_t count)
{
    static uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};

    HAL_UART_Transmit(&usart_connect, (uint8_t*)data, sizeof(float) * count, 100); //发送数据
    // 发送帧尾
    HAL_UART_Transmit(&usart_connect, tail, 4, 100);
}





//串口输出printf重定向
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
        HAL_UART_Transmit(&usart_connect, (uint8_t*)&ch,1,HAL_MAX_DELAY);
    return ch;
}


//串口获取重定向
int fgetc(FILE *f)
//int fgetc(int ch, FILE *F)
{
        uint8_t ch;
        HAL_UART_Receive(&usart_connect ,(uint8_t *)&ch,1,HAL_MAX_DELAY );
        return ch;
}

