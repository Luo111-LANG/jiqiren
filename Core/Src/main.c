/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Encoder.h"
#include "Pwm.h"
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
float TargetA, ActualA, OutA;
float TargetB, ActualB, OutB;
float TargetC, ActualC, OutC;
float TargetD, ActualD, OutD;
float Kp = 7.1, Ki = 0.1 , Kd = 0; 
float Error0A, Error1A, ErrorIntA;		
float Error0B, Error1B, ErrorIntB;
float Error0C, Error1C, ErrorIntC;
float Error0D, Error1D, ErrorIntD;
volatile float vx = 0, vy = 0, omega = 0; 
float Lx = 0.15, Ly = 0.15;  //Lx、Ly 是轮子中心到车体中心在 X、Y 方向的距离
char rx_data[43];
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_USART6_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  Encoder_Init();
  HAL_TIM_Base_Start_IT(&htim9);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ 
	if (htim->Instance == TIM9)  
  {
		static uint16_t CountA;
		static uint16_t CountB;
    static uint16_t CountC;
		static uint16_t CountD;
		CountA ++;
		CountB ++;
		CountC ++;
		CountD ++;
		if (CountA >= 20)
		{
			CountA = 0;
			ActualA = Encoder_GetA();
			Error1A = Error0A;
			Error0A = TargetA - ActualA;
			OutA = Kp*Error0A + ErrorIntA +Kd*(Error0A - Error1A);
			if(OutA > 100)
			{
				OutA = 100;							// 输出饱和，本周期不积分（抗积分饱和）
			}
			else if(OutA < -100)
			{
				OutA = -100;
			}
			else
			{
				ErrorIntA += Ki*Error0A;			// 只在未饱和时累加积分
			}
			if(ErrorIntA > 60) ErrorIntA = 60;		// 积分项限幅
			if(ErrorIntA < -60) ErrorIntA = -60;
			SetPWMA(OutA);
		}

		if (CountB >= 20)
		{
			CountB = 0;
			ActualB = Encoder_GetB();
			Error1B = Error0B;
			Error0B = TargetB - ActualB;
			OutB = Kp*Error0B + ErrorIntB +Kd*(Error0B - Error1B);
			if(OutB > 100)
			{
				OutB = 100;							// 输出饱和，本周期不积分（抗积分饱和）
			}
			else if(OutB < -100)
			{
				OutB = -100;
			}
			else
			{
				ErrorIntB += Ki*Error0B;			// 只在未饱和时累加积分
			}
			if(ErrorIntB > 60) ErrorIntB = 60;		// 积分项限幅
			if(ErrorIntB < -60) ErrorIntB = -60;
			SetPWMB(OutB);
		}

    if (CountC >= 20)
		{
			CountC = 0;
			ActualC = Encoder_GetC();
			Error1C = Error0C;
			Error0C = TargetC - ActualC;
			OutC = Kp*Error0C + ErrorIntC +Kd*(Error0C - Error1C);
			if(OutC > 100)
			{
				OutC = 100;							// 输出饱和，本周期不积分（抗积分饱和）
			}
			else if(OutC < -100)
			{
				OutC = -100;
			}
			else
			{
				ErrorIntC += Ki*Error0C;			// 只在未饱和时累加积分
			}
			if(ErrorIntC > 60) ErrorIntC = 60;		// 积分项限幅
			if(ErrorIntC < -60) ErrorIntC = -60;
			SetPWMC(OutC);
		}

		if (CountD >= 20)
		{
			CountD = 0;
			ActualD = Encoder_GetD();
			Error1D = Error0D;
			Error0D = TargetD - ActualD;
			OutD = Kp*Error0D + ErrorIntD +Kd*(Error0D - Error1D);
			if(OutD > 100)
			{
				OutD = 100;							// 输出饱和，本周期不积分（抗积分饱和）
			}
			else if(OutD < -100)
			{
				OutD = -100;
			}
			else
			{
				ErrorIntD += Ki*Error0D;			// 只在未饱和时累加积分
			}
			if(ErrorIntD > 60) ErrorIntD = 60;		// 积分项限幅
			if(ErrorIntD < -60) ErrorIntD = -60;
			SetPWMD(OutD);
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        HAL_UART_Receive_IT(&huart2, &rx_data, 43);
    }
}
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
