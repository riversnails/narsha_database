#include "stm32f10x.h"
//#include "stm32_eval.h"
#include <stdio.h>


int fputc(int ch, FILE *f)
{
	while(!(USART1->SR & (0x01 << 7)));
  USART1->DR = (unsigned char)ch;
  return ch;
}

void uart_init(USART_InitTypeDef* USART_InitStruct);
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;

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
	unsigned short in_data = 0;
	char toggle = 0;
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
	
	
//	RCC->APB2ENR |= (0x01 << 2); // GPIOA
//	GPIOA->CRH &= ~(0x0f << 1 * 4); // GPIOA.9
//	GPIOA->CRH |= (0x0b << 1 * 4); // GPIOA.9 : Alternate OUT push/pull
//	GPIOA->CRH &= ~(0x0f << 2 * 4); // GPIOA.10
//	GPIOA->CRH |= (0x04 << 2 * 4); // GPIOA.9 : Floating Input
//	
//	RCC->APB2ENR |= (0x01 << 14); // USART1
//	USART1->CR1 = (0x01 << 13) | (0x01 << 3) | (0x01 << 2); // UE, TE, RE
//	USART1->BRR = 0x271;
// USART1->DR = 0x31;
	
	
	//-----------------------------
	//SysTick
	if (SysTick_Config(720))
	{ 
		while (1);
	}
	//-----------------------------

	//-----------------------------
	//USART1
	USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	uart_init(&USART_InitStructure);
	//-----------------------------
	
//	//-----------------------------
//	// GPIOE
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
//	//-----------------------------

	//-----------------------------
	// GPIOC - rotary
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//-----------------------------
	
	printf("hello world \r\n\n\n\n\n");

	
	while(1)
	{
		in_data = GPIOC->IDR;
//		if( !(in_data & (0x01 << 6)) && (in_data & (0x01 << 7)) ) toggle++;
//		else if( !(in_data & (0x01 << 6)) && !(in_data & (0x01 << 7)) ) toggle++;
//		else if( (in_data & (0x01 << 6)) && !(in_data & (0x01 << 7)) ) toggle++;
//		else if( !(in_data & (0x01 << 6)) && !(in_data & (0x01 << 7)) ) toggle++;
//		
//		if(toggle == 4)
//		{
//			toggle = 0;
//			printf("rotate\r\n");
//		}
		
		
		if(in_data & (0x01 << 6) && toggle == 0) 
		{
			//toggle = 1;
			printf("high\r\n");
		}
		else if( !(in_data & (0x01 << 6) ) && toggle == 0)
		{			
			//toggle = 0;
			printf("low\r\n");
		}
		
		Delay_ms(10);
	}
	
	
	while(1)
	{ 
		GPIO_SetBits(GPIOE, GPIO_Pin_3); 
		Delay_ms(1000);
		GPIO_ResetBits(GPIOE, GPIO_Pin_3);
		Delay_ms(1000);
		printf("2sec\r\n");
	}
}

void uart_init(USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_Init(USART1, USART_InitStruct);
  USART_Cmd(USART1, ENABLE);
}