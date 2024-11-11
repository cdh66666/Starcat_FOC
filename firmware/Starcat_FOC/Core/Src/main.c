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
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MT6701.h"
#include "NTC.h"
#include "oled.h"
#include "DRV8301.h"
#include "connect.h"
#include "FOC.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

int vel_flag=0;
int pos_flag=0;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	//定时�???1000hz 速度�???
	if (htim->Instance == TIM4)
	{
		static int i=0;
		i++;
		vel_flag=1;
		if(i>=10)
		{
			i=0;
			pos_flag=1;
		}
	}
}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern int fps,FPS;
extern float TIME;
extern float err_speed;
extern int DISPLAY_DATA;
extern float  I_A,I_B,ud,uq,v,pos,vel;
extern float Uq,Ud;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_UART4_Init();
  MX_ADC1_Init();
  MX_TIM8_Init();
  MX_TIM1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_TIM2_Init();
  MX_CAN1_Init();
  MX_TIM5_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
//  //用于生成NTC温度对应�????????????????
//  generateLookupTable();
//  HAL_Delay(10000);


  OLED_Init();
  Connect_Init();
  //初始化DRV8301芯片
  DRV8301_Init(&DRV8301_1);
  DRV8301_Init(&DRV8301_2);
  MT6701_Init(&encoder1);
  MT6701_Init(&encoder2);


  FOC_Regular_ADC();

//  FOC_Init(&motor1);
  FOC_Init(&motor2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_TIM_Base_Start_IT(&htim4);
  //�?????????启motor1的foc算法
//  FOC_Start(&motor1);
  FOC_Start(&motor2);


  OLED_Printf(0, 0, 8, "CDH");
  OLED_Reflash();

  float DATA[20];
//  float KI_prev=0;

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  //电池电压，双路温度固定频率采�???????????-100hz
	  FOC_Regular_ADC();  //函数耗时
	  //处理串口数据
	  processSerialData();//函数耗时
//	  	  motor2.Iq_pid->kp = KP;
//	  	  motor2.Iq_pid->ki = KI;
//	  	  motor2.Iq_pid->kd = KD;
//	  	  motor2.Id_pid->kp = KP;
//	  	  motor2.Id_pid->ki = KI;
//	  	  motor2.Id_pid->kd = KD;
//	  motor2.P_pid->kp = KP;
//	  motor2.P_pid->ki = KI;
//	  motor2.P_pid->kd = KD;
	  motor2.V_pid->kp = KP;
	  motor2.V_pid->ki = KI;
	  motor2.V_pid->kd = KD;
//	  motor1.Id_pid->kp = KP;
//	  motor1.Id_pid->ki = KI;
//	  DATA[0] = motor1.speed;
//	  DATA[1] = err_speed;

//	  DATA[0] = motor2.I_q;
//	  DATA[1] = motor2.I_d;
//	  DATA[2] = uq;
//	  DATA[3] = ud;
	  DATA[0] = motor2.I_q;
	  DATA[1] = motor2.speed;
	  DATA[2] = motor2.encoder->rad_all;
	  DATA[3] = pos;
	  DATA[4] = vel;
	  DATA[5] = timer_foc.time;
	  DATA[6] = FPS;
	  fps++;
	  if(DISPLAY_DATA==1)
	  {
		  vofa_JustFloat_output(DATA,7); //send a float data need 65us，波特率�??????????2000000
	  }
//	  //位置环控制代�???
//	  if(pos_flag==1)
//	  {
//		  pos_flag=0;
//		//位置闭环
////		motor1.except_vel =FOC_CLOSED_Pos(&motor1,pos);
////		motor2.except_vel =FOC_CLOSED_Pos(&motor2,pos);
//			if(pos>motor2.encoder->rad_all)
//			{
//				motor2.except_iq+=KP;
//			}
//			else if(vel<motor2.encoder->rad_all)
//			{
//				motor2.except_iq-=KP;
//			}
//			motor2.except_vel = LIMIT(motor2.except_vel,-20.0f,20.0f);
//
//	  }
//	  //速度环控制代�???
//	  if(vel_flag==1)
//	  {
//		  	vel_flag=0;
//			//获取电机电角�???
////			FOC_Get_electricalAngle_speed(&motor1);
////			//获取电机电角�???
//			FOC_Get_electricalAngle_speed(&motor2);
//			//速度闭环
////			motor1.except_iq = FOC_CLOSED_Vel(&motor1,motor1.except_vel);
////			motor2.except_iq = FOC_CLOSED_Vel(&motor2,motor2.except_vel);
////			motor1.except_iq = FOC_CLOSED_Vel(&motor1,vel);
////			if(motor2.except_vel>motor2.speed)
////			{
////				motor2.except_iq+=0.01;
////			}
////			else if(motor2.except_vel<motor2.speed)
////			{
////				motor2.except_iq-=0.01;
////			}
////			motor2.except_iq = FOC_CLOSED_Vel(&motor2,vel);
////			motor1.except_id = 0;
//			motor2.except_id = 0;
//			//这个航模电机只能承受这么大uq，ud
//			motor2.except_iq = LIMIT(motor2.except_iq,-5.0f,5.0f);
//
//
//
//
//
//
//	  }

//	  motor1.Id_pid->kp = KP;
//	  motor1.Id_pid->ki = KI;
//	  motor1.Id_pid->kd = KD;
//	  motor2.Id_pid->kp = KP;
//	  motor2.Id_pid->ki = KI;
//	  motor2.Id_pid->kd = KD;

//	  DATA[1] = motor1.hadc->Instance->JDR2;
//	  DATA[0] = TTA;
//	  DATA[1] = TTB;
//	  DATA[2] = TTC;
//	  DATA[5] = QQ;
//	  DATA[6] = DD;
//	  DATA[7] = 4096.0f-motor1.hadc->Instance->JDR1-motor1.hadc->Instance->JDR2;
//	  DATA[6] = motor1.electricalAngle;

//	  DATA[0] = FOC_Get_VBUS();	//误差10mv以内，更新频�?????????2hz
//	  DATA[0] = motor1.I_q;
//	  DATA[1] = AAA;
//	  DATA[2] = motor1.electricalAngle;
//	  DATA[3] = FPS;
//	  DATA[2] = FOC_Get_TEMP1();
//	  DATA[3] = FOC_Get_TEMP2();
//	  DATA[0] = motor1.encoder->rad_all;		//M0 ALL
//	  DATA[1] = motor2.encoder->rad_all;;		//M1 ALL
//	  DATA[2] = FOC_Get_VBUS();
//	  DATA[3] = FOC_Get_TEMP1();
//	  DATA[4] = FOC_Get_TEMP2();
//	  DATA[5] = motor1.I_A;  				//	M0-A
//	  DATA[6] = motor1.I_B;					//	M0-B
//	  DATA[7] = motor1.I_C;	//	M0-C
//	  DATA[8] = motor2.I_A;					//  M1-A
//	  DATA[9] = motor2.I_B ;					//  M1-B
//	  DATA[10] =motor2.I_C;	//  M1-C
//	  DATA[11] = motor1.DRV8301->EN_State;			//使能状�??1
//	  DATA[12] = motor2.DRV8301->EN_State;			//使能状�??2
//	  DATA[13] = motor1.I_d;
//	  DATA[14] = motor1.I_q;
//	  DATA[15] = motor2.I_d;
//	  DATA[16] = motor2.I_q;
//	  DATA[17] = TIME;
//	  DATA[18] = FPS;
//
//	  printf("encoder1 VBUS motor1_a_b_c_EN_d_q TIME FPS:%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7d\n",
//			  motor1.encoder->rad_all,FOC_Get_VBUS(),motor1.I_A,motor1.I_B,motor1.I_C,
//			  motor1.DRV8301->EN_State,motor1.I_d,motor1.I_q,TIME,FPS);
//	  //过热保护
//	  if((FOC_Get_TEMP1()>60.0f))
//	  {
//		  printf("The overheating protection has been activated and the %2d motors have been shut down!",1);
//		  HAL_Delay(1000);
//	  }
//	  else if (FOC_Get_TEMP2()>60.0f)
//	  {
//		  printf("The overheating protection has been activated and the %2d motors have been shut down!",2);
//		  HAL_Delay(1000);
//	  }
//	  else
//	  {


//	  }
//	  printf("I_A:%7d\n",I_A);

//	  fps++;

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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

#ifdef  USE_FULL_ASSERT
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
