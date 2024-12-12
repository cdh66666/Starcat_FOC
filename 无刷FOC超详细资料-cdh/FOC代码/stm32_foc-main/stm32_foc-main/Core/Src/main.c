/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor/foc.h"
#include "arm_math.h"
#include "global_def.h"
#include "motor/motor_runtime_param.h"
#include <stdio.h>
#include "motor/conf.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void set_pwm_duty(float d_u, float d_v, float d_w)
{
  __disable_irq();
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0.995 * d_u * (pwm_counter_period - 1));
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0.995 * d_v * (pwm_counter_period - 1));
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 0.995 * d_w * (pwm_counter_period - 1));
  __enable_irq();
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */
  setvbuf(stdout, NULL, _IONBF, 0);
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_ADC2_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  set_motor_pid(
      // 3.25, 0, 13,
      1, 0, 8,
      0.011, 0.0001, 0,
      1, 0.1, 0,
      1, 0.1, 0);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADCEx_Calibration_Start(&hadc2);
  HAL_ADCEx_InjectedStart_IT(&hadc1);
  HAL_ADCEx_InjectedStart(&hadc2);
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_TIM_Base_Start_IT(&htim7);
  HAL_Delay(50);
  set_pwm_duty(0.5, 0, 0);
  HAL_Delay(400);
  rotor_zero_angle = motor_logic_angle;
  set_pwm_duty(0, 0, 0);
  HAL_Delay(200);

  motor_control_context.max_speed = deg2rad(90);
  motor_control_context.max_torque_norm = 0.3;
  motor_control_context.position = deg2rad(90);
  motor_control_context.speed = 5 * 2 * PI;
  motor_control_context.torque_norm_d = 0;
  motor_control_context.torque_norm_q = 0.1;
  // motor_control_context.type = control_type_position;
  // motor_control_context.type = control_type_speed;
  // motor_control_context.type = control_type_torque;
  // motor_control_context.type = control_type_speed_torque;
  // motor_control_context.type = control_type_position_speed_torque;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  extern void foc(float u_d, float u_q, float rad);
  extern float diff;
  extern uint8_t iic_buffer[2];
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // HAL_Delay(500);
    // motor_control_context.position = init_angle + deg2rad(90);
    // HAL_Delay(500);
    // motor_control_context.position = init_angle;
    // float u_u = ADC_REFERENCE_VOLT * (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1) / ADC_SCALE - ADC_REFERENCE_VOLT / 2;
    // float u_v = ADC_REFERENCE_VOLT * (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_1) / ADC_SCALE - ADC_REFERENCE_VOLT / 2;

    // printf("%f,%f\n", motor_i_d / MAX_CURRENT, motor_i_q / MAX_CURRENT);
    // printf("%f,%f,%f\n", diff, motor_i_d / MAX_CURRENT, motor_i_q / MAX_CURRENT);
    // printf("%f,%f\n", u_d, u_q);
    // motor_control_context.position = 0;
    // printf("%f,%f,%f\n", motor_i_u, motor_i_v, (-motor_i_u - motor_i_v));
    // for (float i = 0; i < 360; i += 10)
    // {
    //   foc(0, 0.5, rad2deg(i));
    //   HAL_Delay(10);
    //   printf("%f,%f\n", motor_i_u, motor_i_v);
    // }
    // lib_speed_control(deg2rad(360 * 1));
    // lib_position_control(deg2rad(80));

    // printf("%f\n", diff);
    // printf("%f\n", motor_logic_angle);
    printf("%f\n", motor_speed);
    // printf("%f\n", motor_i_q / MAX_CURRENT);
    // printf("%f,%f\n", motor_i_q / MAX_CURRENT, motor_speed);
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
