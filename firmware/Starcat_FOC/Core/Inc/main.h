/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define NSCS_1_Pin GPIO_PIN_13
#define NSCS_1_GPIO_Port GPIOC
#define NSCS_2_Pin GPIO_PIN_14
#define NSCS_2_GPIO_Port GPIOC
#define MOSI_ENC_Z_2_Pin GPIO_PIN_15
#define MOSI_ENC_Z_2_GPIO_Port GPIOC
#define I_A_1_Pin GPIO_PIN_0
#define I_A_1_GPIO_Port GPIOC
#define I_B_1_Pin GPIO_PIN_1
#define I_B_1_GPIO_Port GPIOC
#define I_A_2_Pin GPIO_PIN_2
#define I_A_2_GPIO_Port GPIOC
#define I_B_2_Pin GPIO_PIN_3
#define I_B_2_GPIO_Port GPIOC
#define SERVO0_Pin GPIO_PIN_2
#define SERVO0_GPIO_Port GPIOA
#define SERVO1_Pin GPIO_PIN_3
#define SERVO1_GPIO_Port GPIOA
#define TEMP1_Pin GPIO_PIN_4
#define TEMP1_GPIO_Port GPIOA
#define SW2_Pin GPIO_PIN_5
#define SW2_GPIO_Port GPIOA
#define VBUS_Pin GPIO_PIN_6
#define VBUS_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_4
#define SW1_GPIO_Port GPIOC
#define TEMP2_Pin GPIO_PIN_5
#define TEMP2_GPIO_Port GPIOC
#define OLED_DC_Pin GPIO_PIN_2
#define OLED_DC_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_10
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_11
#define OLED_SDA_GPIO_Port GPIOB
#define EN_GATE_Pin GPIO_PIN_12
#define EN_GATE_GPIO_Port GPIOB
#define MOSI_ENC_Z_1_Pin GPIO_PIN_9
#define MOSI_ENC_Z_1_GPIO_Port GPIOC
#define OLED_CS_Pin GPIO_PIN_11
#define OLED_CS_GPIO_Port GPIOA
#define OLED_RES_Pin GPIO_PIN_12
#define OLED_RES_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOA
#define SPI_SCK_Pin GPIO_PIN_10
#define SPI_SCK_GPIO_Port GPIOC
#define SPI_MISO_Pin GPIO_PIN_11
#define SPI_MISO_GPIO_Port GPIOC
#define SPI_MOSI_Pin GPIO_PIN_12
#define SPI_MOSI_GPIO_Port GPIOC
#define NFAULT_1_Pin GPIO_PIN_2
#define NFAULT_1_GPIO_Port GPIOD
#define NFAULT_1_EXTI_IRQn EXTI2_IRQn
#define NFAULT_2_Pin GPIO_PIN_3
#define NFAULT_2_GPIO_Port GPIOB
#define NFAULT_2_EXTI_IRQn EXTI3_IRQn
#define CS_ENC_A_1_Pin GPIO_PIN_4
#define CS_ENC_A_1_GPIO_Port GPIOB
#define SCK_ENC_B_1_Pin GPIO_PIN_5
#define SCK_ENC_B_1_GPIO_Port GPIOB
#define CS_ENC_A_2_Pin GPIO_PIN_6
#define CS_ENC_A_2_GPIO_Port GPIOB
#define SCK_ENC_B_2_Pin GPIO_PIN_7
#define SCK_ENC_B_2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
