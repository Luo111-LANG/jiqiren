#include "main.h"
#include "Pwm.h"
#include "tim.h"

void SetPWMA(int16_t PWMA)
{
   if(PWMA > 0)
   {
   HAL_GPIO_WritePin(GPIOA , GPIO_PIN_2,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOA , GPIO_PIN_3,GPIO_PIN_RESET);
   __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWMA*200 );
   }
   else
   {
      HAL_GPIO_WritePin(GPIOA , GPIO_PIN_2,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA , GPIO_PIN_3,GPIO_PIN_SET);
	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (-PWMA)*200 );
   }
}
void SetPWMB(int16_t PWMB)
{
   if(PWMB > 0)
   {
   HAL_GPIO_WritePin(GPIOA , GPIO_PIN_4,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOA , GPIO_PIN_5,GPIO_PIN_RESET);
   __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, PWMB*200 );
   }
   else
   {
      HAL_GPIO_WritePin(GPIOA , GPIO_PIN_4,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA , GPIO_PIN_5,GPIO_PIN_SET);
	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (-PWMB)*200 );
   }
}
void SetPWMC(int16_t PWMC)
{
   if(PWMC > 0)
   {
   HAL_GPIO_WritePin(GPIOA , GPIO_PIN_6,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOA , GPIO_PIN_7,GPIO_PIN_RESET);
   __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, PWMC*200 );
   }
   else
   {
      HAL_GPIO_WritePin(GPIOA , GPIO_PIN_6,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOA , GPIO_PIN_7,GPIO_PIN_SET);
	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (-PWMC)*200 );
   }
}
void SetPWMD(int16_t PWMD)
{
   if(PWMD > 0)
   {
   HAL_GPIO_WritePin(GPIOB , GPIO_PIN_0,GPIO_PIN_SET);
   HAL_GPIO_WritePin(GPIOB , GPIO_PIN_1,GPIO_PIN_RESET);
   __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, PWMD*200 );
   }
   else
   {
      HAL_GPIO_WritePin(GPIOB , GPIO_PIN_0,GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB , GPIO_PIN_1,GPIO_PIN_SET);
	  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, (-PWMD)*200 );
   }
}
