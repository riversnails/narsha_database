#include "stm32f10x.h"
//#include "stm32_eval.h"
#include <stdio.h>

enum rotary_state{
	ROTARY_STATE_1 = 1,
	ROTARY_STATE_2,
	ROTARY_STATE_3,
	ROTARY_STATE_4,
};

enum rotary_state_left{
	ROTARY_STATE_LEFT_1 = 1,
	ROTARY_STATE_LEFT_2,
	ROTARY_STATE_LEFT_3,
	ROTARY_STATE_LEFT_4,
};

enum rotary_state_right{
	ROTARY_STATE_RIGHT_1 = 1,
	ROTARY_STATE_RIGHT_2,
	ROTARY_STATE_RIGHT_3,
	ROTARY_STATE_RIGHT_4,
};

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
	char rotary_state = 0, rotary_state_left = 0, rotary_state_right = 0;
	char right_state_flag = 0, left_state_flag = 0;
	int output_a = 0, output_b = 0;
	int rotary_count = 0;
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
		output_a = (in_data & (0x01 << 6) );
		output_b = (in_data & (0x01 << 7) );
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
		
		
//		if(in_data & (0x01 << 6) ) 
//		{
//			//toggle = 1;
//			printf("pc6 = high  ");
//		}
//		else if( !(in_data & (0x01 << 6) ) )
//		{			
//			//toggle = 0;
//			printf("pc6 = low  ");
//		}
//		
//		if(in_data & (0x01 << 7) ) 
//		{
//			//toggle = 1;
//			printf("pc7 = high  ");
//		}
//		else if( !(in_data & (0x01 << 7) ) )
//		{			
//			//toggle = 0;
//			printf("pc7 = low  ");
//		}
		
		
		
//		if( output_a == 0 && output_b > 0 )
//		{
//			if(rotary_state == ROTARY_STATE_2)
//			{
//				if(right_state_flag == 0) 
//				{
//					right_state_flag = 1;
//					printf("right \r\n");
//				}
//			}
//			rotary_state = ROTARY_STATE_1;
//		}
//		else if( output_a == 0 && output_b == 0 )
//		{
//			rotary_state = ROTARY_STATE_2;
//		}
//		else if( output_a > 0 && output_b == 0 )
//		{
//			if(rotary_state == ROTARY_STATE_2)
//			{
//				if(left_state_flag == 0) 
//				{
//					left_state_flag = 1;
//					printf("left \r\n");
//				}
//			}
//			rotary_state = ROTARY_STATE_3;
//		}
//		else if( output_a > 0 && output_b > 0 )
//		{
//			rotary_state = ROTARY_STATE_4;
//			right_state_flag = 0;
//			left_state_flag = 0;
//		}
		
		
		if( output_a == 0 && output_b > 0 )
		{
			if(rotary_state_right == ROTARY_STATE_RIGHT_2)
			{
				if(right_state_flag == 0) 
				{
					right_state_flag = 1;
					printf("right : %5.d \r\n", ++rotary_count);
				}
			}
			rotary_state_left = ROTARY_STATE_LEFT_1;
			rotary_state_right = ROTARY_STATE_RIGHT_1;
		}
		else if( output_a == 0 && output_b == 0 )
		{
			rotary_state_left = ROTARY_STATE_LEFT_2;
			rotary_state_right = ROTARY_STATE_RIGHT_2;
		}
		else if( output_a > 0 && output_b == 0 )
		{
			if(rotary_state_left == ROTARY_STATE_2)
			{
				if(left_state_flag == 0) 
				{
					left_state_flag = 1;
					printf("left  : %5.d \r\n", --rotary_count);
				}
			}
			rotary_state_left = ROTARY_STATE_LEFT_3;
			rotary_state_right = ROTARY_STATE_RIGHT_3;
		}
		else if( output_a > 0 && output_b > 0 )
		{
			rotary_state_left = ROTARY_STATE_LEFT_4;
			rotary_state_right = ROTARY_STATE_RIGHT_4;
			
			right_state_flag = 0;
			left_state_flag = 0;
		}
		
		
		//printf("a = %d b = %d\r\n", output_a, output_b);
		//printf("state = %d\r\n", rotary_state);
		//printf("\r\n");
		Delay_ms(1);
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