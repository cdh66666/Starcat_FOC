#ifndef __OLED_H
#define __OLED_H
#include "main.h"
#include "stdio.h"
#include <stdarg.h>
#include "string.h"
#include "math.h"
#include <stdlib.h>

//此程序对于SSD1306和SH1107驱动的OLED通用


/*************************IO口操作宏定义********************************************/
#define		OLED_RES_SET()    	 HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_SET)	//SDA引脚高电平
#define		OLED_RES_CLR()  	 HAL_GPIO_WritePin(OLED_RES_GPIO_Port,OLED_RES_Pin,GPIO_PIN_RESET)	//SDA引脚低电平
#define	 	OLED_DC_Cmd()		 HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_RESET)	//DC引脚低电平表示命令
#define 	OLED_DC_Data()		 HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_SET)		//DC引脚高电平表示数据
#define	 	OLED_CS_CLR()		 HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_RESET)	//CS引脚低电平
#define 	OLED_CS_SET()		 HAL_GPIO_WritePin(OLED_CS_GPIO_Port,OLED_CS_Pin,GPIO_PIN_SET)		//CS引脚高电平
#define	 	OLED_SCL_CLR()		 HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,GPIO_PIN_RESET)	//SCL引脚低电平
#define 	OLED_SCL_SET()		 HAL_GPIO_WritePin(OLED_SCL_GPIO_Port,OLED_SCL_Pin,GPIO_PIN_SET)	//SCL引脚高电平
#define		OLED_SDA_SET()    	 HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_SET)	//SDA引脚高电平
#define		OLED_SDA_CLR()  	 HAL_GPIO_WritePin(OLED_SDA_GPIO_Port,OLED_SDA_Pin,GPIO_PIN_RESET)	//SDA引脚低电平
/*************************重要参数定义********************************************/
//定义使用软件SPI还是硬件SPI，都定义了则使用软件SPI
#define		Software_SPI
//#define 	Hardware_SPI
#define ABS(x) ((x) < 0 ? -(x) : (x))

//OLED界面重要参数定义
#define OLED_WIDTH 80 					//屏幕宽度
#define OLED_HIGH  128						//屏幕高度
#define OLED_WIDTH_HALF (OLED_WIDTH-1)/2	//屏幕宽度中心
#define OLED_HIGH_HALF  (OLED_HIGH-1)/2		//屏幕高度中心
#define OLED_ROW   OLED_HIGH/8				//屏幕页数


#define OLED_DRAW_FLAG  1 	// 定义绘制的标志位
#define OLED_CLEAR_FLAG 0 	// 定义消除的标志位


typedef enum {
    OLED_FONT_8x16 = 16,
    OLED_FONT_8x8 = 8,
    OLED_FONT_16x16 = 32,
} TextSize;

extern uint8_t OLED_GRAM[OLED_ROW][OLED_WIDTH];
//获取FPS
uint32_t OLED_GET_FPS(void);
/*************************OLED基本绘制函数********************************************/
// 绘制上下边框
void OLED_DrawUpperLowerBorder(int16_t centerX, int16_t centerY, uint16_t width, uint16_t height);
//显示中文,外接一个中文字库芯片
void OLED_Show_Chinese(int16_t x,int16_t y,int num);
//带淡入效果显示图片
void OLED_ShowImageWithFadeIn(int16_t centerX, int16_t centerY, uint8_t width, uint8_t height, const uint8_t *imageData, uint8_t fadeInSteps, uint16_t delay_ms);
// 显示传入的图像
void OLED_ShowImage(int16_t centerX, int16_t centerY,uint8_t width,uint8_t height,const uint8_t *imageData);
// printf重定义，指定行、列、字体大小
void OLED_Printf(int16_t x, int16_t y, uint8_t size, const char *pFormat, ...);
void OLED_Printf_Centered_And_Wrapped(int16_t y, uint8_t size, const char *pFormat, ...) ;
//自动换行打印
void OLED_Printf_Wrap(int16_t x, int16_t y, uint8_t size, const char *pFormat, ...);
//文本显示自动居中
void OLED_Printf_Centered(int16_t y, uint8_t size, const char *pFormat, ...);
// 在 OLED 屏幕上显示单个字符
void OLED_ShowChar(int16_t x, int16_t y, uint8_t size, uint8_t chr);
// 定义绘制圆角矩形的函数，使圆角矩形区域内的点取反
void OLED_Draw_Rounded_Rect(int16_t x_start, int16_t y_start, int16_t length, int16_t height);
//绘制圆角矩形
void OLED_Draw_Auto_Rounded_Rectangle(int16_t x_start, int16_t y_start, int16_t length, int16_t weighth, uint8_t draw_or_clear);
//画圆
void OLED_Draw_Clear_Circle(int16_t centerX, int16_t centerY, int16_t radius, uint8_t draw_or_clear);
// OLED绘制或清除矩形
void OLED_Draw_Clear_Rectangle(int16_t x_start, int16_t y_start, int16_t length, int16_t weighth, uint8_t draw_or_clear);
// OLED绘制或清除直线（使用Bresenham算法优化）
void OLED_Draw_Clear_Line(int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, uint8_t draw_or_clear);
// OLED绘制或清除点
void OLED_Draw_Clear_Point(int16_t x, int16_t y, uint8_t draw_or_clear);
// 获取指定坐标点的值，即获取是否需要清除
uint8_t OLED_Get_Clear_Point(int16_t x, int16_t y);
/*************************GRAM显存操作函数********************************************/
//更新显存到OLED,返回帧率
void OLED_Reflash(void);
//清零显存操作
void OLED_GRAM_CLR(void);
/*************************OLED基本驱动函数********************************************/
//初始化SSD1306,默认是水平寻址模式，页地址自动加一
void OLED_Init(void);
//开启OLED显示
void OLED_Display_On(void);
//关闭OLED显示
void OLED_Display_Off(void);
//参数x是列（0-79，也就是列地址最多为0x3F，SH1107最多可达0x7F），y是页（0-15，也就是页地址最多为0x0F）
void OLED_Set_Pos(uint8_t x, uint8_t y);
/*************************SPI基本驱动函数********************************************/
void OLED_WriteCmd(uint8_t Cmd);

void OLED_WriteData(uint8_t Cmd);
//软件spi写一个字节
void Software_SPI_Write(uint8_t Byte);
//硬件spi写一个字节
void Hardware_SPI_Write(uint8_t *Byte);

#endif




