#include "Motor2SPI2.h"
#include "main.h"

extern SPI_HandleTypeDef hspi2;





/*************************************************************
** Function name:       Motor2SPI2WriteData
** Descriptions:        SPI1写数据
** Input parameters:    data：要发送的数据
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor2SPI2WriteData(uint16_t data)
{
    HAL_SPI_Transmit(&hspi2,(uint8_t *)&data,1,1000);
}
/*************************************************************
** Function name:       Motor2SPI2ReadData
** Descriptions:        SPI1读数据
** Input parameters:    None
** Output parameters:   None
** Returned value:      读取到的数据
** Remarks:             None
*************************************************************/
uint16_t Motor2SPI2ReadData(void)
{
    uint16_t data;
    HAL_SPI_Receive(&hspi2,(uint8_t *)&data,1,1000);
    return data;
}
/*************************************************************
** Function name:       Motor2SPI2SetCS
** Descriptions:        设置SPI1 片选1引脚电平
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void Motor2SPI2SetCS(uint8_t leave)
{
    HAL_GPIO_WritePin(M2_SPI_CS_GPIO_Port,M2_SPI_CS_Pin, (GPIO_PinState)leave);
}

