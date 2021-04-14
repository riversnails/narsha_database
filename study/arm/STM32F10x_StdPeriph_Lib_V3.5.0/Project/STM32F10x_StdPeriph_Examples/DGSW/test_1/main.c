#include "stm32f10x.h"
//#include "stm32_eval.h"
#include <stdio.h>

GPIO_InitTypeDef GPIO_InitStructure;


volatile unsigned long sys_count = 0;
unsigned int TimingDelay = 0;


unsigned long micros_10us()
{
	return sys_count;
}


void Delay(unsigned int nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void Delay_ms(unsigned int nTime)
{ 
  Delay(nTime * 100);
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


int main(void)
{
//	SysTick->LOAD = 720;
//	SysTick->CTRL = 0x07;
//	
//	RCC->APB2ENR |= (0x01<<3);
//	GPIOB->CRL &= ~(0x0F<< 5 * 4);
//	GPIOB->CRL |= (0x03 << 5  * 4);
//	GPIOB->BSRR = 0x01 << 5;
//	
//	RCC->APB2ENR |= (0x01<<6);
//	GPIOE->CRL &= ~(0x0f << 3 * 4);
//	GPIOE->CRL |= (0x03 << 3 * 4);
	
	
	RCC->APB2ENR |= (0x01 << 2); // GPIOA
	GPIOA->CRH &= ~(0x0f << 1 * 4); // GPIOA.9
	GPIOA->CRH |= (0x0b << 1 * 4); // GPIOA.9 : Alternate OUT push/pull
	GPIOA->CRH &= ~(0x0f << 2 * 4); // GPIOA.10
	GPIOA->CRH |= (0x04 << 2 * 4); // GPIOA.9 : Floating Input
	
	RCC->APB2ENR |= (0x01 << 14); // USART1
	USART1->CR1 = (0x01 << 13) | (0x01 << 3) | (0x01 << 2); // UE, TE, RE
	USART1->BRR = 0x271;
	USART1->DR = 0x31;
	
	//-----------------------------
	//SysTick
	if (SysTick_Config(720))
	{ 
		while (1);
	}
	//-----------------------------


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	

	
	while(1)
	{ 
		GPIO_SetBits(GPIOE, GPIO_Pin_3); 
		Delay_ms(1000);
		GPIO_ResetBits(GPIOE, GPIO_Pin_3);
		Delay_ms(1000);
	}
}
