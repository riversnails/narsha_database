#include "stm32f4xx.h"
#include "KEY&LED.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int fputc(int ch, FILE * f) {
   while (!(USART1->SR & (0x01 << 7))) {}
   USART1->DR = ch;
   return ch;
}

void TimingDelay_Decrement(void);


volatile unsigned long sys_count;
volatile unsigned int TimingDelay;

unsigned long micros_10us()
{
	return sys_count;
}

unsigned long millis() {
   return sys_count / 100;
}

void Delay(unsigned int nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay > 0);
}

 void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

int timer_toggle_flag = 0;
void TIM2_IRQHandler (void) 
{
	if ((TIM2->SR & 0x0001) != 0) { // check update interrupt source
		if(timer_toggle_flag == 0)
		{
			timer_toggle_flag = 1;
			GPIOE->ODR |= (0x01 << 9);
		}
		else 
		{
			timer_toggle_flag = 0;
			GPIOE->ODR &= ~(0x01 << 9);
		}
		 
		 TIM2->SR &= ~(1<<0); // clear UIF flag
	}
}

GPIO_InitTypeDef  GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

int main(void)
{   
   if (SysTick_Config(168000000/1000)  ) {
      while (1);
   }
   
   
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOE, &GPIO_InitStructure);
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 ;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
   GPIO_Init(GPIOF, &GPIO_InitStructure);
	 
	 GPIOF->ODR &= ~(0x01 << 1);
	 GPIOF->ODR &= ~(0x01 << 2);
	 
	 
	RCC->APB1ENR |= (0x01 << 0);
	TIM2->CR1 |= (0x01 << 0);
	TIM2->DIER |= (0x01 << 0);
	TIM2->PSC = 72-1;
	TIM2->ARR = 200-1;

	NVIC->ISER[0] |= (0x01 << 28); 
	 
   while(1) {
      Delay(1000);
      GPIOA->ODR |= (0x01 << 7);
      Delay(1000);
      GPIOA->ODR &= ~(0x01 << 7);
   }
   
   
   JOYState_LED_GPIO_Init();
   while (1)
   {
      Led_Toggle(Read_JOYState());
/*
      GPIO_SetBits(GPIOF , GPIO_Pin_9);
      Delay(0x1fffff);
      GPIO_ResetBits(GPIOF , GPIO_Pin_9);
      Delay(0x1fffff);
*/
   }
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/