#include "oled.h"
#include "oledfont.h"

uint8_t OLED_GRAM[OLED_ROW][OLED_WIDTH];

uint32_t OLED_FPS = 0;




//获取FPS
uint32_t OLED_GET_FPS(void)
{
	return OLED_FPS;
}
/*************************OLED基本显示函数********************************************/

//绘制选定框
void OLED_DrawUpperLowerBorder(int16_t centerX, int16_t centerY, uint16_t width, uint16_t height) {
    // 计算图像左上角坐标
    int16_t x = centerX - width / 2;
    int16_t y = centerY - height / 2;

    // 绘制上边框
    for (uint16_t i = x + 2; i < x + width - 2; ++i) {
        OLED_Draw_Clear_Point(i, y, OLED_DRAW_FLAG); // 绘制点
    }

    // 绘制下边框
    for (uint16_t i = x + 2; i < x + width - 2; ++i) {
        OLED_Draw_Clear_Point(i, y + height - 1, OLED_DRAW_FLAG); // 绘制点
    }

    // 绘制左上角弧度
    OLED_Draw_Clear_Point(x + 1, y + 1, OLED_DRAW_FLAG); // 绘制点
    OLED_Draw_Clear_Point(x, y + 2, OLED_DRAW_FLAG); // 绘制点

    // 绘制右上角弧度
    OLED_Draw_Clear_Point(x + width - 2, y + 1, OLED_DRAW_FLAG); // 绘制点
    OLED_Draw_Clear_Point(x + width - 1, y + 2, OLED_DRAW_FLAG); // 绘制点

    // 绘制左下角弧度
    OLED_Draw_Clear_Point(x + 1, y + height - 2, OLED_DRAW_FLAG); // 绘制点
    OLED_Draw_Clear_Point(x, y + height - 3, OLED_DRAW_FLAG); // 绘制点

    // 绘制右下角弧度
    OLED_Draw_Clear_Point(x + width - 2, y + height - 2, OLED_DRAW_FLAG); // 绘制点
    OLED_Draw_Clear_Point(x + width - 1, y + height - 3, OLED_DRAW_FLAG); // 绘制点
}

// 显示传入的图像
void OLED_ShowImage(int16_t centerX, int16_t centerY,uint8_t width,uint8_t height,const uint8_t *imageData) {
    // 计算图像左上角坐标
    int16_t x = centerX - width / 2;
    int16_t y = centerY - height / 2;

	// 绘制图像
	uint16_t dot_count_max = width * height;
	uint16_t Byte_Num = dot_count_max / 8;
	if (dot_count_max % 8) Byte_Num++;

	uint8_t data;
	int16_t px, py;
	for (uint16_t byteIndex = 0; byteIndex < Byte_Num; byteIndex++) {
		data = *imageData++;
		px = x + byteIndex % width;
		for (uint8_t bitIndex = 0; bitIndex < 8; bitIndex++) {
			py = y + byteIndex / width * 8 + bitIndex;
			if ((px < OLED_WIDTH) && (px >=0) &&  (py < OLED_HIGH) && (py>=0)) {
				if (data & (0x01 << bitIndex)) {
					OLED_GRAM[py / 8][px] |= (0x01 << (py % 8));
				} else {
					OLED_GRAM[py / 8][px] &= ~(0x01 << (py % 8));
				}
			}
		}
	}

}

//带淡入效果显示图片
void OLED_ShowImageWithFadeIn(int16_t centerX, int16_t centerY, uint8_t width, uint8_t height, const uint8_t *imageData, uint8_t fadeInSteps, uint16_t delay_ms) {
    // 计算图像左上角坐标
    int16_t x = centerX - width / 2;
    int16_t y = centerY - height / 2;

    // 绘制图像
    uint16_t dot_count_max = width * height;
    uint16_t Byte_Num = dot_count_max / 8;
    if (dot_count_max % 8) Byte_Num++;

    const uint8_t *imageData1 = imageData;
    uint8_t data;
    int16_t px, py;
    for (uint8_t step = 0; step < fadeInSteps; step++)
    {
    OLED_GRAM_CLR();
    imageData1 = imageData;
    for (uint16_t byteIndex = 0; byteIndex < Byte_Num; byteIndex++) {
        data = *imageData1++;
        px = x + byteIndex % width;
        for (uint8_t bitIndex = 0; bitIndex < 8; bitIndex++) {
            py = y + byteIndex / width * 8 + bitIndex;
            if ((px < OLED_WIDTH) && (px >=0) &&  (py < OLED_HIGH) && (py>=0)) {
                        if (data & (0x01 << bitIndex)) {
                        	if (rand() % fadeInSteps <= step) {
                            OLED_Draw_Clear_Point(px, py, 1);  // 点亮
                        }
                    }
                }
            }
        }
    OLED_Reflash();  // 更新 OLED 显示
    HAL_Delay(delay_ms);   // 延迟
    }


}


// OLED 显示 printf 函数
void OLED_Printf(int16_t x, int16_t y, uint8_t size, const char *pFormat,...) {
    char pStr[100] = {'\0'};
    va_list ap;

    va_start(ap, pFormat);
    // 使用 vsnprintf 替代 vsprintf
    vsnprintf(pStr, sizeof(pStr), pFormat, ap);
    va_end(ap);

    uint8_t i = 0;
    while (pStr[i]!= '\0' && x < OLED_WIDTH && y < OLED_HIGH) {
        OLED_ShowChar(x, y, size, pStr[i]);
        x += 8;
        i++;
    }
}

//文本显示自动居中
void OLED_Printf_Centered(int16_t y, uint8_t size, const char *pFormat, ...) {
    char pStr[100] = {'\0'};
    va_list ap;

    va_start(ap, pFormat);
    vsprintf((char *)pStr, pFormat, ap);
    va_end(ap);

    // 计算文本总宽度
    uint8_t textWidth = 0;
    uint8_t i = 0;
    while (pStr[i] != '\0') {
        textWidth += 8; // 字符宽度假设为8像素
        i++;
    }

    // 计算文本起始位置使其居中
    int16_t x = (OLED_WIDTH - textWidth) / 2;


    // 显示文本
    i = 0;
    while (pStr[i] != '\0' && x < OLED_WIDTH && y < OLED_HIGH) {
        OLED_ShowChar(x, y, size, pStr[i]);
        x += 8;
        i++;
    }
}

//自动换行打印
void OLED_Printf_Wrap(int16_t x, int16_t y, uint8_t size, const char *pFormat, ...) {
    char pStr[100] = {'\0'};
    va_list ap;

    va_start(ap, pFormat);
    vsprintf((char *)pStr, pFormat, ap);
    va_end(ap);

    uint8_t i = 0;
    int16_t current_x = x;
    int16_t current_y = y;

    while (pStr[i] != '\0') {
        if (current_x + 8 >= OLED_WIDTH) {
            // 换行到下一行
            current_x = x;
            if(size == OLED_FONT_8x16)
            {
            	current_y += 16;
            }
            else
            {
            	current_y += 8;
			}
        }
        // 检查是否超出屏幕范围
        if ((current_y >= OLED_HIGH) || (current_y < 0))  {
            // 如果超出屏幕范围则停止打印
            break;
        }
        OLED_ShowChar(current_x, current_y, size, pStr[i]);
        current_x += 8;
        i++;
        if(i>=100)break;;
    }
}

//显示中文
void OLED_Show_Chinese(int16_t x,int16_t y,int num)
{
	uint8_t col;
	//分成上下两排来绘制
	for (uint8_t drow = 0; drow <2; drow++)
	{
    	//分成左右两列来绘制
    	for (uint8_t dcol = 0; dcol <2; dcol++)
    	{
			for (uint8_t dx = 0; dx < 8; dx++)
				{
					for (uint8_t dy = 0; dy < 8; dy++)
					{
						//判断字符点没有超出范围
						if (((x+dx+8*dcol) < OLED_WIDTH) && ((y+dy+8*drow)<OLED_HIGH))
						{
							if(drow==1)
							{
								col = Char_16_16[num][dx+16+8*dcol];
							}
							else
							{
								col = Char_16_16[num][dx+8*dcol];
							}
							if((col & (0x01 << dy))>0)
							{
								OLED_GRAM[(y+dy+8*drow)/8][x+dx+8*dcol] |= (0x01 << ((y+dy+8*drow)%8)) ;
							}
							else
							{
								// 设置指定点的值为 0，使用位运算设置对应位
								OLED_GRAM[(y+dy+8*drow)/8][x+dx+8*dcol] &= ~(0x01 << ((y+dy+8*drow)%8));
							}
						}
					}
				}
    	}
	}
}



// 显示指定字符,阴码+列行式+逆向+C51格式
void OLED_ShowChar(int16_t x, int16_t y, TextSize size, uint8_t chr)
{
	//如果超出范围则退出
	if((chr>= ' ') && (chr <= '~'))
	{
	    chr -= ' '; // 将字符索引转换为内部数组索引


        	//定义一个变量
        	uint8_t col;
    	    // 使用 8x16 字体
    	    if (size == OLED_FONT_8x16)
    	    {
    	    	//分成上下两排来绘制
    	    	for (uint8_t drow = 0; drow <2; drow++)
    	    	{
    				for (uint8_t dx = 0; dx < 8; dx++)
    					{
    						for (uint8_t dy = 0; dy < 8; dy++)
    						{
    							//判断字符点没有超出范围
    							 if (((x+dx) < OLED_WIDTH) && ((x+dx) >=0) &&  ((y+dy+8*drow) < OLED_HIGH) && ((y+dy+8*drow)>=0))
    							{
    								col = Char_16[chr][dx+8*drow];
    								if((col & (0x01 << dy))>0)
    								{
    									OLED_GRAM[(y+dy+8*drow)/8][x+dx] |= (0x01 << ((y+dy+8*drow)%8)) ;
    								}
    								else
    								{
    						            // 设置指定点的值为 0，使用位运算设置对应位
    									OLED_GRAM[(y+dy+8*drow)/8][x+dx] &= ~(0x01 << ((y+dy+8*drow)%8));
    								}
    							}
    						}
    					}
    	    	}

    	    }
    	    else if(size == OLED_FONT_8x8) // 默认使用 8x8 字体
    	    {
				for (uint8_t dx = 0; dx < 8; dx++)
				{
					for (uint8_t dy = 0; dy < 8; dy++)
					{

						//判断字符点没有超出范围
						if (((x+dx) < OLED_WIDTH) && ((x+dx) >=0) &&  ((y+dy) < OLED_HIGH) && ((y+dy)>=0))
						{
							col = Char_8[chr][dx];
							if((col & (0x01 << dy))>0)
							{
								OLED_GRAM[(y+dy)/8][x+dx] |= (0x01 << ((y+dy)%8)) ;
							}
							else
							{
					            // 设置指定点的值为 0，使用位运算设置对应位
								OLED_GRAM[(y+dy)/8][x+dx] &= ~(0x01 << ((y+dy)%8));
							}
						}
					}
				}
			}
    	    else if(size == OLED_FONT_16x16) // 默认使用 8x8 字体
    	    {
    	    	//待添加
    	    }



    }


}


// 绘制圆角矩形，并清除矩形内的点
void OLED_Draw_Auto_Rounded_Rectangle(int16_t x_start, int16_t y_start, int16_t length, int16_t weighth, uint8_t draw_or_clear) {
    // 如果长度和宽度为负数，则改变起点坐标和长度或宽度
    if (length < 0) {
        x_start += length + 1;
        length = -length;
    }
    if (weighth < 0) {
        y_start += weighth + 1;
        weighth = -weighth;
    }
	length-=1;
	weighth-=1;


    // 检查长度和宽度是否合法
    if (length < 4 || weighth < 4) {
        return; // 尺寸过小，直接返回
    }

    // 自动选择圆角半径
    uint16_t min_dimension = (length < weighth) ? length : weighth;
    uint16_t radius = min_dimension / 5; // 比例选择

    // 绘制矩形的四条边
    OLED_Draw_Clear_Line(x_start + radius, y_start, x_start + length - radius, y_start,draw_or_clear);// 上边
    OLED_Draw_Clear_Line(x_start, y_start + radius, x_start, y_start + weighth - radius,draw_or_clear);// 左边
    OLED_Draw_Clear_Line(x_start + length, y_start + radius, x_start + length, y_start + weighth - radius,draw_or_clear);// 右边
    OLED_Draw_Clear_Line(x_start + radius, y_start + weighth, x_start + length - radius, y_start + weighth,draw_or_clear);// 下边

    // 绘制圆角
    int16_t x = radius;
    int16_t y = 0;
    int16_t radiusError = 1 - x;

    while (x >= y) {
        // 绘制四个角的圆弧
    	OLED_Draw_Clear_Point(x_start + radius - y, y_start + radius - x,draw_or_clear);// 左上
    	OLED_Draw_Clear_Point(x_start + length - radius + y, y_start + radius - x,draw_or_clear);// 右上
    	OLED_Draw_Clear_Point(x_start + radius - y, y_start + weighth - radius + x,draw_or_clear);// 左下
    	OLED_Draw_Clear_Point(x_start + length - radius + y, y_start + weighth - radius + x,draw_or_clear);// 右下
    	OLED_Draw_Clear_Point(x_start + radius - x, y_start + radius - y,draw_or_clear);// 左上
    	OLED_Draw_Clear_Point(x_start + length - radius + x, y_start + radius - y,draw_or_clear);// 右上
    	OLED_Draw_Clear_Point(x_start + radius - x, y_start + weighth - radius + y,draw_or_clear);// 左下
    	OLED_Draw_Clear_Point(x_start + length - radius + x, y_start + weighth - radius + y,draw_or_clear);// 右下

        y++;
        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError += 2 * (y - x) + 1;
        }
    }
}
// 定义绘制圆角矩形的函数，使圆角矩形区域内的点取反
void OLED_Draw_Rounded_Rect(int16_t x_start, int16_t y_start, int16_t length, int16_t height) {
    // 如果长度和宽度为负数，则改变起点坐标和长度或宽度
    if (length < 0) {
        x_start += length + 1;
        length = -length;
    }
    if (height < 0) {
        y_start += height + 1;
        height = -height;
    }
    length-=1;
    height-=1;

    // 检查长度和宽度是否合法
    if (length < 4 || height < 4) {
        return; // 尺寸过小，直接返回
    }

    // 自动选择圆角半径
    uint16_t min_dimension = (length < height) ? length : height;
    uint16_t radius = min_dimension / 5; // 比例选择

    uint16_t radius_2 =  radius*radius;
    // 绘制整体矩形
    for (int y = y_start; y < y_start + height; y++) {
        for (int x = x_start; x < x_start + length; x++) {
        	if(x >= (x_start + radius) && x <= (x_start + length - radius) && y >= (y_start ) && y <= (y_start + height))
        	{
        	    OLED_Draw_Clear_Point(x, y, !OLED_Get_Clear_Point(x, y));
        	}
        	else if(x >= (x_start ) && x <= (x_start + length ) && y >= (y_start+radius ) && y <= (y_start + height-radius))
        	{
        	    OLED_Draw_Clear_Point(x, y, !OLED_Get_Clear_Point(x, y));
        	}
        	else if((x - (x_start + radius)) * (x - (x_start + radius)) + (y - (y_start + radius)) * (y - (y_start + radius)) < radius_2 )
        	{
        	    OLED_Draw_Clear_Point(x, y, !OLED_Get_Clear_Point(x, y));
        	}
        	else if((x - (x_start + length - radius)) * (x - (x_start + length - radius)) + (y - (y_start + radius)) * (y - (y_start + radius)) < radius_2)
        	{
        	    OLED_Draw_Clear_Point(x, y, !OLED_Get_Clear_Point(x, y));
        	}
        	else if((x - (x_start + radius)) * (x - (x_start + radius)) + (y - (y_start + height - radius)) * (y - (y_start + height - radius)) < radius_2)
        	{
        	    OLED_Draw_Clear_Point(x, y, !OLED_Get_Clear_Point(x, y));
        	}
        	else if((x - (x_start + length - radius)) * (x - (x_start + length - radius)) + (y - (y_start + height - radius)) * (y - (y_start + height - radius)) < radius_2)
        	{
        	    OLED_Draw_Clear_Point(x, y, !OLED_Get_Clear_Point(x, y));
        	}

        }
    }
}
// OLED绘制或清除矩形
void OLED_Draw_Clear_Rectangle(int16_t x_start, int16_t y_start, int16_t length, int16_t weighth, uint8_t draw_or_clear)
{
    // 如果长度和高度为负数，则改变起点坐标和长度或高度
    if (length < 0)
    {
        x_start += length + 1;
        length = -length;
    }
    if (weighth < 0)
    {
        y_start += weighth + 1;
        weighth = -weighth;
    }

    length = length - 1;
    weighth = weighth - 1;

    OLED_Draw_Clear_Line(x_start, y_start, x_start + length, y_start, draw_or_clear);
    OLED_Draw_Clear_Line(x_start, y_start, x_start, y_start + weighth, draw_or_clear);
    OLED_Draw_Clear_Line(x_start + length, y_start, x_start + length, y_start + weighth, draw_or_clear);
    OLED_Draw_Clear_Line(x_start, y_start + weighth, x_start + length, y_start + weighth, draw_or_clear);
}

//画圆
void OLED_Draw_Clear_Circle(int16_t centerX, int16_t centerY, int16_t radius, uint8_t draw_or_clear) {
    int16_t x = radius;
    int16_t y = 0;
    int16_t err = 0;

    while (x >= y) {
        OLED_Draw_Clear_Point(centerX + x, centerY + y, draw_or_clear);
        OLED_Draw_Clear_Point(centerX - x, centerY + y, draw_or_clear);
        OLED_Draw_Clear_Point(centerX + x, centerY - y, draw_or_clear);
        OLED_Draw_Clear_Point(centerX - x, centerY - y, draw_or_clear);
        OLED_Draw_Clear_Point(centerX + y, centerY + x, draw_or_clear);
        OLED_Draw_Clear_Point(centerX - y, centerY + x, draw_or_clear);
        OLED_Draw_Clear_Point(centerX + y, centerY - x, draw_or_clear);
        OLED_Draw_Clear_Point(centerX - y, centerY - x, draw_or_clear);

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }

        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

// OLED绘制或清除直线
void OLED_Draw_Clear_Line(int16_t x_start, int16_t y_start, int16_t x_end, int16_t y_end, uint8_t draw_or_clear)
{
    // 如果 x_start 大于 x_end，交换它们的值
    if (x_start > x_end)
    {
        int16_t temp = x_start;
        x_start = x_end;
        x_end = temp;

        temp = y_start;
        y_start = y_end;
        y_end = temp;
    }

    // 判断是否为垂直线
    if (x_start == x_end)
    {
        // 画垂直线
        int16_t y = y_start;
        int16_t step = (y_end > y_start) ? 1 : -1;
        while (y != y_end)
        {
            OLED_Draw_Clear_Point(x_start, y, draw_or_clear);
            y += step;
        }
        OLED_Draw_Clear_Point(x_end, y_end, draw_or_clear);
    }
    else
    {
        // 计算斜率
        float k = (float)(y_end - y_start) / (float)(x_end - x_start);

        // 根据斜率的正负确定递增还是递减的方向
        int16_t step = (y_end > y_start) ? 1 : -1;

        int16_t last_y = y_start;
        for (int16_t x = x_start; x <= x_end; x++)
        {
            int16_t y = (int16_t)(k * (x - x_start) + y_start);
            if (y != last_y)
            {
                int16_t yy = last_y + step;
                while (yy != y)
                {
                    OLED_Draw_Clear_Point(x, yy, draw_or_clear);
                    yy += step;
                }
            }
            OLED_Draw_Clear_Point(x, y, draw_or_clear);
            last_y = y;
        }
    }
}
// OLED绘制或清除点
void OLED_Draw_Clear_Point(int16_t x, int16_t y, uint8_t draw_or_clear)
{
	int16_t row = y / 8;     // 行号
	int16_t y_bias = y % 8;  // 在 row 行中的偏移量

    // 限制行数和列数，防止数组越界
    if (row >= 0 && row < OLED_ROW && x >= 0 && x < OLED_WIDTH) {
        if (draw_or_clear) {
            // 设置指定点的值为 1，使用位运算设置对应位
            OLED_GRAM[row][x] |= (0x01 << y_bias);
        } else {
            // 设置指定点的值为 0，使用位运算设置对应位
            OLED_GRAM[row][x] &= ~(0x01 << y_bias);
        }
    }
}
// 获取指定坐标点的值，即获取是否需要清除
uint8_t OLED_Get_Clear_Point(int16_t x, int16_t y) {
    int16_t row = y / 8;     // 行号
    int16_t y_bias = y % 8;  // 在 row 行中的偏移量

    // 限制行数和列数，防止数组越界
    if (row >= 0 && row < OLED_ROW && x >= 0 && x < OLED_WIDTH) {
        // 获取指定点的值，即判断对应位是否为 1
        return (OLED_GRAM[row][x] >> y_bias) & 0x01;
    } else {
        return 0; // 如果坐标超出范围，返回 0
    }
}


/*************************GRAM显存操作函数********************************************/
//更新显存到OLED
void OLED_Reflash(void)
{
	static uint8_t init_flag=0;
    static uint32_t lastTick = 0;
    static uint32_t frameCount = 0;

    if(init_flag==0)
    {
    	//初始化lastTick
    	lastTick = HAL_GetTick();
    	init_flag=1;
    }



    /*****************核心显示**********************/
//	for(uint8_t i=0;i<OLED_ROW;i++)
//	{
//		for(uint8_t n=0;n<OLED_WIDTH;n++)
//		{
//			OLED_WriteData(OLED_GRAM[i][n]);
//		}
//	}

	for(uint8_t i=0;i<OLED_ROW;i++)
	{
		//设置页地址
		OLED_Set_Pos(0,i);
		for(uint8_t n=0;n<OLED_WIDTH;n++)
		{
			OLED_WriteData(OLED_GRAM[i][n]);
		}
	}


	/*****************核心显示**********************/




	// 每秒更新一次帧率
	uint32_t currentTick = HAL_GetTick();
    if (currentTick >= lastTick)
    {
        if((currentTick - lastTick)>=1000)
        {
        	OLED_FPS = frameCount;
            frameCount = 0;
            lastTick = HAL_GetTick();
        }
    }
    else
    {
        if((0xffffffff - lastTick + currentTick + 1)>=1000)
          {
          	OLED_FPS = frameCount;
              frameCount = 0;
              lastTick = HAL_GetTick();
          }
    }


    frameCount++;


}
//清零显存操作
void OLED_GRAM_CLR(void)
{
	for(uint8_t i=0;i<OLED_ROW;i++)
	{
		for(uint8_t n=0;n<OLED_WIDTH;n++)
		{
			OLED_GRAM[i][n] = 0;
		}
	}
}
/*************************OLED基本显示函数********************************************/
//参数x是列（0-79，也就是列地址最多为0x3F，SH1107最多可达0x7F），y是页（0-15，也就是页地址最多为0x0F）
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WriteCmd(0xb0+y);					 // 设置页地址
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);   	 // 设置列高地址
	OLED_WriteCmd(x&0x0f);         			 // 设置列低地址
}

//开启OLED显示
void OLED_Display_On(void)
{
	OLED_WriteCmd(0X8D);  //SET DCDC命令
	OLED_WriteCmd(0X14);  //DCDC ON
	OLED_WriteCmd(0XAF);  //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void)
{
	OLED_WriteCmd(0X8D);  //SET DCDC命令
	OLED_WriteCmd(0X10);  //DCDC OFF
	OLED_WriteCmd(0XAE);  //DISPLAY OFF
}

//初始化SSD1306,默认是水平寻址模式，页地址自动加一
void OLED_Init(void)
{
	OLED_RES_CLR();
	//延时100ms等待OLED电源稳定
	HAL_Delay(100);
	OLED_RES_SET();

	OLED_WriteCmd(0xAE);//--display off
	OLED_WriteCmd(0x00);//--set lower column address
	OLED_WriteCmd(0x10);//--set high column address
	OLED_WriteCmd(0xB0);//--set page address
	OLED_WriteCmd(0x81);//---contract control
	OLED_WriteCmd(0xBF);//--128
	OLED_WriteCmd(0xA1);//--set segment remap
	OLED_WriteCmd(0xA6);//--normal / reverse
	OLED_WriteCmd(0xA8);//--set multiplex ratio 驱动列数，屏幕有多宽就是多少
	OLED_WriteCmd(OLED_WIDTH-1);//--1/80 duty
	OLED_WriteCmd(0xad);//--set charge pump enable
	OLED_WriteCmd(0x8a);//--0x8a 外供 VCC
	OLED_WriteCmd(0x32);//--0X30---0X33 set VPP 8V
	OLED_WriteCmd(0xC8);//--Com scan direction
	OLED_WriteCmd(0xD3);//--set display offset
	OLED_WriteCmd(0x18);//--SH1107 OLED 128x80 显示屏需要偏移24个像素点
	OLED_WriteCmd(0xD5);//--set osc division
	OLED_WriteCmd(0x80);//--128
	OLED_WriteCmd(0xD9);//--set pre-charge period
	OLED_WriteCmd(0x22);//--0x22
	OLED_WriteCmd(0xDA);//--set COM pins
	OLED_WriteCmd(0x12);//--
	OLED_WriteCmd(0xDC);//--Set Display Start Line
	OLED_WriteCmd(0x00);//--
	OLED_WriteCmd(0xdb);//--set vcomh
	OLED_WriteCmd(0x40);//--
	OLED_WriteCmd(0x8D);//--set DC-DC enable
	OLED_WriteCmd(0x14);
	//先清屏
	OLED_GRAM_CLR();
	OLED_Reflash();
	//屏幕开显示
	OLED_WriteCmd(0xAF);//--display ON
//
//	// 由于场景需要旋转90°,设置为垂直寻址模式
//	OLED_WriteCmd(0x21);//-Set Vertical Addressing Mode


//	//设置为水平寻址模式，页地址可自动加一
//	OLED_WriteCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
//	OLED_WriteCmd(0x00);//
//
//	OLED_WriteCmd(0x21);//设置列地址
//	OLED_WriteCmd(0x00);
//	OLED_WriteCmd(0x7f);
//
//
//	OLED_WriteCmd(0x22);//设置页地址
//	OLED_WriteCmd(0x00);
//	OLED_WriteCmd(0x07);

//	OLED_GRAM_CLR();
//	OLED_Reflash();
}

/*************************OLED基本驱动函数********************************************/
//写命令
void OLED_WriteCmd(uint8_t Cmd)
{
	OLED_CS_CLR();
	OLED_DC_Cmd();//DC引脚低电平表示命令
	#ifdef Software_SPI
		//使用软件SPI
		Software_SPI_Write(Cmd);
	#else
		//使用硬件SPI
		Hardware_SPI_Write(&Cmd);
	#endif
	OLED_CS_SET();
}
//写数据
void OLED_WriteData(uint8_t Cmd)
{
	OLED_CS_CLR();
	OLED_DC_Data();//DC引脚高电平表示数据
	#ifdef Software_SPI
		//使用软件SPI
		Software_SPI_Write(Cmd);
	#else
		//使用硬件SPI
		Hardware_SPI_Write(&Cmd);
#endif
	OLED_CS_SET();
}
//软件spi写一个字节
void Software_SPI_Write(uint8_t Byte)
{
	for(uint8_t i=0;i<8;i++)
	{
		OLED_SCL_CLR();
		if(Byte & 0x80)	OLED_SDA_SET();   //高位先行
		else 			OLED_SDA_CLR();
		Byte <<= 1;
		OLED_SCL_SET();
	}
}
//硬件spi写
void Hardware_SPI_Write(uint8_t *Byte)
{
//	HAL_SPI_Transmit(&mpu_spi, Byte, 1, 1000);
}


