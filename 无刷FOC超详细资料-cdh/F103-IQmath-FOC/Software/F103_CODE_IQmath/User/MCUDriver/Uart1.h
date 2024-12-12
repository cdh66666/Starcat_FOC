
/***********************************************************************
 * @file Uart1.h
 * Description:
 *
 **********************************************************************
 * File name:      Uart1.h
 * Date:           2020-10-03
 * Version:        V1.0
 * Author          liuxiang
 * @history:
 * V1.0
***********************************************************************/
#ifndef __UART_h__
#define __UART_h__
#include "stdint.h"

#define Uart1_RINGBUFLENGTH 500


void Uart1_IT_Handle(void);
uint8_t Uart1_SendData(uint8_t *pData,uint16_t length);
uint16_t Uart1_ReadData(uint8_t *pData,uint16_t length);
uint16_t Uart1_GetDataNums(void);
uint8_t Uart1_LookData(uint8_t *pData,uint16_t length);
#endif // __UART_h__


