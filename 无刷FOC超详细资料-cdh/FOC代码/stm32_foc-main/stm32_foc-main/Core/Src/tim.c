/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    tim.c
 * @brief   This file provides code for the configuration
 *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;

/* TIM1 init function */
void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED3;
  htim1.Init.Period = pwm_counter_period - 1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_1;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);
}
/* TIM6 init function */
void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 72;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000000 / angle_calc_freq;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */
}
/* TIM7 init function */
void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 72;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1000000 / speed_calc_freq;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (tim_baseHandle->Instance == TIM1)
  {
    /* USER CODE BEGIN TIM1_MspInit 0 */

    /* USER CODE END TIM1_MspInit 0 */
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM1 GPIO Configuration
    PA6     ------> TIM1_BKIN
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_TIM1_PARTIAL();

    /* USER CODE BEGIN TIM1_MspInit 1 */

    /* USER CODE END TIM1_MspInit 1 */
  }
  else if (tim_baseHandle->Instance == TIM6)
  {
    /* USER CODE BEGIN TIM6_MspInit 0 */

    /* USER CODE END TIM6_MspInit 0 */
    /* TIM6 clock enable */
    __HAL_RCC_TIM6_CLK_ENABLE();

    /* TIM6 interrupt Init */
    HAL_NVIC_SetPriority(TIM6_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM6_IRQn);
    /* USER CODE BEGIN TIM6_MspInit 1 */

    /* USER CODE END TIM6_MspInit 1 */
  }
  else if (tim_baseHandle->Instance == TIM7)
  {
    /* USER CODE BEGIN TIM7_MspInit 0 */

    /* USER CODE END TIM7_MspInit 0 */
    /* TIM7 clock enable */
    __HAL_RCC_TIM7_CLK_ENABLE();

    /* TIM7 interrupt Init */
    HAL_NVIC_SetPriority(TIM7_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(TIM7_IRQn);
    /* USER CODE BEGIN TIM7_MspInit 1 */

    /* USER CODE END TIM7_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (timHandle->Instance == TIM1)
  {
    /* USER CODE BEGIN TIM1_MspPostInit 0 */

    /* USER CODE END TIM1_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    PA9     ------> TIM1_CH2
    PA10     ------> TIM1_CH3
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_TIM1_PARTIAL();

    /* USER CODE BEGIN TIM1_MspPostInit 1 */

    /* USER CODE END TIM1_MspPostInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle)
{

  if (tim_baseHandle->Instance == TIM1)
  {
    /* USER CODE BEGIN TIM1_MspDeInit 0 */

    /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /**TIM1 GPIO Configuration
    PA6     ------> TIM1_BKIN
    PA8     ------> TIM1_CH1
    PA9     ------> TIM1_CH2
    PA10     ------> TIM1_CH3
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);

    /* USER CODE BEGIN TIM1_MspDeInit 1 */

    /* USER CODE END TIM1_MspDeInit 1 */
  }
  else if (tim_baseHandle->Instance == TIM6)
  {
    /* USER CODE BEGIN TIM6_MspDeInit 0 */

    /* USER CODE END TIM6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM6_CLK_DISABLE();

    /* TIM6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM6_IRQn);
    /* USER CODE BEGIN TIM6_MspDeInit 1 */

    /* USER CODE END TIM6_MspDeInit 1 */
  }
  else if (tim_baseHandle->Instance == TIM7)
  {
    /* USER CODE BEGIN TIM7_MspDeInit 0 */

    /* USER CODE END TIM7_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM7_CLK_DISABLE();

    /* TIM7 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM7_IRQn);
    /* USER CODE BEGIN TIM7_MspDeInit 1 */

    /* USER CODE END TIM7_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#include "i2c.h"
#include "global_def.h"
#include "main.h"
#include "motor/motor_runtime_param.h"
#include "motor/foc.h"
#include "motor/conf.h"
#include "algorithm/filter.h"
#include <stdio.h>
#include "arm_math.h"
#include "adc.h"
uint8_t iic_buffer[2];
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6)
  {
    static float encoder_angle_last = 0;
    HAL_I2C_Mem_Read(&hi2c1, 0x36 << 1, 0x0c, I2C_MEMADD_SIZE_8BIT, iic_buffer, 2, HAL_MAX_DELAY);
    uint16_t as5600_angle = ((uint16_t)iic_buffer[0] << 8) | iic_buffer[1];
    float _encoder_angle = 2 * PI * as5600_angle / 4096.0;
    float diff_angle = regular_diff(_encoder_angle - encoder_angle_last, 2 * PI);
    encoder_angle_last = _encoder_angle;
    encoder_angle = _encoder_angle;
    motor_logic_angle = fmodf(motor_logic_angle + diff_angle, position_cycle);
    float u_a = ADC_REFERENCE_VOLT * (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1) / ADC_SCALE - ADC_REFERENCE_VOLT / 2;
    float u_b = ADC_REFERENCE_VOLT * (uint16_t)HAL_ADCEx_InjectedGetValue(&hadc2, ADC_INJECTED_RANK_1) / ADC_SCALE - ADC_REFERENCE_VOLT / 2;
    float motor_i_a = -u_a / R_SHUNT / OP_GAIN;
    float motor_i_b = -u_b / R_SHUNT / OP_GAIN;
    motor_i_u = motor_i_a;
    motor_i_v = -(motor_i_a + motor_i_b);

    float i_alpha = 0;
    float i_beta = 0;
    arm_clarke_f32(motor_i_u, motor_i_v, &i_alpha, &i_beta);
    float sin_value = arm_sin_f32(rotor_logic_angle);
    float cos_value = arm_cos_f32(rotor_logic_angle);
    float _motor_i_d = 0;
    float _motor_i_q = 0;
    arm_park_f32(i_alpha, i_beta, &_motor_i_d, &_motor_i_q, sin_value, cos_value);
    // float filter_alpha_i_d = 0.01 / (0.01 + 1.0 / angle_calc_freq);
    // float filter_alpha_i_q = filter_alpha_i_d;
    // low_pass_filter(_motor_i_d, &_motor_i_d, filter_alpha_i_d);
    // low_pass_filter(_motor_i_q, &_motor_i_q, filter_alpha_i_q);
    motor_i_d = _motor_i_d;
    motor_i_q = _motor_i_q;

    switch (motor_control_context.type)
    {
    case control_type_position:
      lib_position_control(motor_control_context.position);
      break;
    case control_type_speed:
      lib_speed_control(motor_control_context.speed);
      break;
    case control_type_torque:
      lib_torque_control(motor_control_context.torque_norm_d, motor_control_context.torque_norm_q);
      break;
    case control_type_speed_torque:
      lib_speed_torque_control(motor_control_context.speed, motor_control_context.max_torque_norm);
      break;
    case control_type_position_speed_torque:
      lib_position_speed_torque_control(motor_control_context.position, motor_control_context.max_speed, motor_control_context.max_torque_norm);
      break;
    default:
      break;
    }
    // HAL_I2C_Mem_Read_IT(&hi2c1, 0x36 << 1, 0x0c, I2C_MEMADD_SIZE_8BIT, iic_buffer, 2);
    // HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
  }
  else if (htim->Instance == TIM7)
  {
    static float encoder_angle_last = 0;
    float diff_angle = regular_diff(encoder_angle - encoder_angle_last, 2 * PI);
    encoder_angle_last = encoder_angle;
    float _motor_speed = diff_angle * motor_speed_calc_freq;
    float filter_alpha_speed = 0.01 / (0.01 + (1.0 / 100));
    low_pass_filter(_motor_speed, &motor_speed, filter_alpha_speed);
  }
}
/* USER CODE END 1 */
