#include "main.h"
#include "Encoder.h"
#include "tim.h"
void Encoder_Init(void)
{
	  MX_TIM2_Init();
	  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
	  MX_TIM3_Init();
	  HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	  MX_TIM4_Init();
	  HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
	  MX_TIM5_Init();
	  HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
}

int16_t Encoder_GetA(void)
{
	int16_t TempA;
	TempA = (int16_t)__HAL_TIM_GET_COUNTER(&htim2);
    __HAL_TIM_SET_COUNTER(&htim2, 0);
	return TempA;
}

int16_t Encoder_GetB(void)
{
	int16_t TempB;
	TempB = (int16_t)__HAL_TIM_GET_COUNTER(&htim3);
    __HAL_TIM_SET_COUNTER(&htim3, 0);
	return TempB;
}

int16_t Encoder_GetC(void)
{
	int16_t TempC;
	TempC = (int16_t)__HAL_TIM_GET_COUNTER(&htim4);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
	return TempC;
}

int16_t Encoder_GetD(void)
{
	int16_t TempD;
	TempD = (int16_t)__HAL_TIM_GET_COUNTER(&htim5);
    __HAL_TIM_SET_COUNTER(&htim5, 0);
	return TempD;
}
