#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization

#include <stdio.h>
#include<stdarg.h>



//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */

volatile unsigned int TimingDelay;	
volatile unsigned long sys_count;
unsigned long error_parity_garbage[128];
	
	
int fputc(int ch, FILE *f)
{
	while(!(USART2->SR & (0x01 << 7)));
  USART2->DR = (unsigned char)ch;
	
  return ch;
}

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

void SysTick_Handler(void)
{
	TimingDelay_Decrement();
	sys_count++;
}


void rs_high()
{	
	//printf("rs high \r\n");
	GPIOA->ODR |= (0x01 << 8); // D7
}

void rs_low() 
{
	//printf("rs low \r\n");
	GPIOA->ODR &= ~(0x01 << 8); // D7
}

void enable_high_low()
{
	//printf("en high low \r\n");
	GPIOA->ODR |= (0x01 << 9); // D8
	GPIOA->ODR &= ~(0x01 << 9); // D8
}

void set_value_4bit(unsigned char value)
{
	// data pin 4bit mod need
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;
	
	if(value & 0x01 << 0) a = 1;
	if(value & 0x01 << 1) b = 1;
	if(value & 0x01 << 2) c = 1;
	if(value & 0x01 << 3) d = 1;
	
	GPIOC->ODR &= ~(0x01 << 7);
	GPIOB->ODR &= ~(0x01 << 6);
	GPIOA->ODR &= ~(0x01 << 7);
	GPIOA->ODR &= ~(0x01 << 6);
	
	GPIOC->ODR |= (a << 7);
	GPIOB->ODR |= (b << 6);
	GPIOA->ODR |= (c << 7);
	GPIOA->ODR |= (d << 6);
	
//	printf("%d ", (value & 0x01 << 0));
//	printf("%d ", (value & 0x01 << 1));
//	printf("%d ", (value & 0x01 << 2));
//	printf("%d ", (value & 0x01 << 3));

//	printf("%x \r\n", value);


//	printf("%d ", (int)(GPIOC->ODR & (0x01 << 7)) );
//	printf("%d ", (int)(GPIOB->ODR & (0x01 << 6)) );
//	printf("%d ", (int)(GPIOA->ODR & (0x01 << 7)) );
//	printf("%d ", (int)(GPIOA->ODR & (0x01 << 6)) );

//	printf("%x \r\n", value);
}

void set_instruction_4bit(unsigned char inst)
{
	rs_low();
	set_value_4bit((inst >> 4) & 0x0F);
	enable_high_low();
	rs_high();
	Delay(5); // 50us
	rs_low();
	set_value_4bit(inst & 0x0F);
	enable_high_low();
	rs_high();
	Delay(5); // 50us
}

void set_data_4bit(unsigned char data)
{
	rs_high();
	set_value_4bit((data >> 4) & 0x0F);
	enable_high_low();
	Delay(5); // 50us
	set_value_4bit(data & 0x0F);
	enable_high_low();
	Delay(5); // 50us
}

void charLCD_init()
{
	rs_high();
  Delay(100);
	
	set_instruction_4bit(0x28); // 4 bit mode
	set_instruction_4bit(0x06); // entry mode set
	set_instruction_4bit(0x0C); // display on
	set_instruction_4bit(0x80); // display off
	set_instruction_4bit(0x01); // display clear
	Delay(200); // 2ms
}

void charLCD_string(char *string)
{
	int i;
	for(i = 0; i < strlen(string); i++)
	{
		set_data_4bit(string[i]);
	}
}



/*--------------------------------------------------------------------------------------------------------------------*\
 | MIAN ENTRY                                               																																																													|
\*--------------------------------------------------------------------------------------------------------------------*/
int main (void) {
	stm32_Init();
	
	SysTick->LOAD = 720;
	SysTick->CTRL = 0x07;
	
	//usart
	RCC->APB1ENR |= (0x01 << 17);
	RCC->APB2ENR |= (0x01 << 2);
	GPIOA->CRL &= ~(0x0F << 2 * 4); // GPIO:2
	GPIOA->CRL &= ~(0x0F << 3 * 4); // GPIO:3
	GPIOA->CRL |= (0x0B << 2 * 4); // GPIO:2
	GPIOA->CRL |= (0x04 << 3 * 4); // GPIO:3
	USART2->CR1 = (0x01 << 2) | (0x01 << 3 )| (0x01 << 13); // RE, TE, UE
	USART2->CR2 = 0x00;
	USART2->CR3 = 0x00;
	USART2->BRR = 0xEA6; // BRR:9600
	
	// char lcd
	RCC->APB2ENR |= (0x01 << 3) | (0x01 << 4) | (0x01 << 5) | (0x01 << 6);
	GPIOA->CRH &= ~(0x0F << (0 * 4)); // D7
	GPIOA->CRH &= ~(0x0F << (1 * 4)); // D8
	GPIOC->CRL &= ~(0x0F << (7 * 4)); // D9
	GPIOB->CRL &= ~(0x0F << (6 * 4)); // D10
	GPIOA->CRL &= ~(0x0F << (7 * 4)); // D11
	GPIOA->CRL &= ~(0x0F << (6 * 4)); // D12
	
	GPIOA->CRH |= (0x03 << (0 * 4)); // D7
	GPIOA->CRH |= (0x03 << (1 * 4)); // D8
	GPIOC->CRL |= (0x03 << (7 * 4)); // D9
	GPIOB->CRL |= (0x03 << (6 * 4)); // D10
	GPIOA->CRL |= (0x03 << (7 * 4)); // D11
	GPIOA->CRL |= (0x03 << (6 * 4)); // D12
	
	
//	GPIOA->ODR |= (1 << 8);	//((value & 0x01 << 1) << 7);
//	GPIOA->ODR |= (1 << 9);
//	GPIOC->ODR |= (1 << 7);	//((value & 0x01 << 3) << 7);
//	GPIOB->ODR |= (1 << 6);	//((value & 0x01 << 2) << 6);
//	GPIOA->ODR |= (1 << 7);	//((value & 0x01 << 1) << 7);
//	GPIOA->ODR |= (1 << 6);
//	
//	while(1);
	
	printf("init char-------------------------------------------- \r\n");
	charLCD_init();
	NVIC->ISER[1] = (0x01 << 6);
	
	printf("hello world\r\n");
	//charLCD_string("hello");
	//while(1);
	
	printf("loop-------------------------------------------- \r\n");
	while(1)
	{
		Delay(100000);
		//set_instruction_4bit(0x01);
		set_data_4bit(0x31);
	}
	
}

// D2 == a10
// D3 == b3
// D4 == b5
// D5 == b4
// D6 == b10
// D7 == a8
// D8 == a9
// D9 == c7
// D10 == b6
// D11 == a7
// D12 == a6