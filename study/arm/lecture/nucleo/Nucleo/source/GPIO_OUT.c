#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization

#include <stdio.h>
#include<stdarg.h>
#include <stdlib.h>
#include <string.h>

//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */

#define MENU_MAX 6

volatile unsigned int TimingDelay;	
volatile unsigned long sys_count;
unsigned long error_parity_garbage[128];
int i;
int j;

	
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

typedef struct CharLcd
{
	int length_menu;
	char **text_menu;
}CharLcd_menu;

//EXTI3---------------------------------------------------------------------------------------------------------------------------
char *main_menu_text[5] = {" 1. remocon     ", " 2. utrasonic   ", " 3. servo       ", " 4. buzzer      ", " 5. led         "};
char *rem_menu_text[2] = {" rem : ", " exit"};
char *ultra_menu_text[2] = {" ultra : ", " exit"};
char *servo_menu_text[5] = {" servo : ", " servo plus", " servo plus"," servo minus", " exit"};
char *buzzer_menu_text[4] = {" buzzer_hz : ", " buzzer_up", " buzzer_down", " exit"};
char *led_menu_text[3] = {" pwm_on", " pwm_off", " exit"};
	
int rem_count1 = 0;
unsigned long falling_edge_timing[40];
unsigned long diff_falling_edge_timing[33];
int falling_edge_index = 0;
int value_bit[32];
unsigned char value_hex = 0;
unsigned char value_num[10] = {0x16, 0x0C, 0x18, 0x5E, 0x08, 0x1C, 0x5A, 0x42, 0x52, 0x4A};
unsigned char remocon_num = 0;
char exti_callback_flag = 0;
int ultrasonic_toggle = -1;
volatile unsigned long ultrasonic_distance = 0;
volatile unsigned long ultrasonic_difftime = 0;
volatile unsigned long ultrasonic_starttime = 0;
volatile unsigned long ultrasonic_endtime = 0;
char exti_ultrasonic_flag = 0;
int servo_duty = 70;
CharLcd_menu *charLcd_arr[MENU_MAX];
int CharLcd_idx = 0;
int curr_pos = 0;
int curr_up_down = 0;
int up_flag = 0;
int down_flag = 0;
int right_flag = 0;
int left_flag = 0;
int enter_flag = 0;
int print_toggle = 0;
int cursor_toggle = 0;
char *array1;
char *array2;



void EXTI4_IRQHandler(void)
{
	//printf("hehe\r\n");
	if (EXTI->PR & (1<<4)) {                       // EXT3 interrupt pending?    
		
		if(ultrasonic_toggle == 0 )
		{
			ultrasonic_toggle = 1;
			//printf("y");
			ultrasonic_starttime = micros_10us();
			//printf("start:%ld \r\n", ultrasonic_starttime);
		}
		else if(ultrasonic_toggle == 1)
		{
			ultrasonic_toggle = 0;
			//printf("z");
			ultrasonic_endtime = micros_10us();
			//ultrasonic_difftime = );
			ultrasonic_distance = (ultrasonic_endtime - ultrasonic_starttime) * 0.17;
			//printf("end:%ld \r\n", ultrasonic_endtime);
			//printf("diff:%ld \r\n", ultrasonic_difftime);
			exti_ultrasonic_flag = 1;
		}
		else ultrasonic_toggle++;
		
		EXTI->PR = (1<<4);                          // clear pending interrupt
	}
}

void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR & (1<<10)) {                       // EXTI0 interrupt pending?
		//printf("%d \r\n", falling_edge_index);
		
		falling_edge_timing[falling_edge_index] = micros_10us() * 10;
		if(falling_edge_index > 0)
		{
			int diff_time = 0;
			diff_falling_edge_timing[falling_edge_index - 1] = 
			falling_edge_timing[falling_edge_index] - falling_edge_timing[falling_edge_index - 1];
			
			diff_time = diff_falling_edge_timing[falling_edge_index - 1];
			
			if(diff_time > 13000 && diff_time < 14000)
			{
				//printf("Lead Code\r\n");
				falling_edge_index = 1;
			}
			else if(diff_time > 11000 && diff_time < 12000)
			{
				printf("%d\r\n", remocon_num);
				//printf("R\r\n");
				falling_edge_index = -1;
			}
		}
		
		
		falling_edge_index++;
		if(falling_edge_index >= 34)
		{
			falling_edge_index = 0;
			exti_callback_flag = 1;
		}
		
		
		EXTI->PR = (1<<10);                          // clear pending interrupt
	}
}


void exit_callback_func()
{
	//printf("data_print\r\n");
			
			for(i = 0; i < 32; i++)
			{
				if(diff_falling_edge_timing[i + 1] > 1000 && diff_falling_edge_timing[i + 1] < 1500) value_bit[i] = 0;
				else if(diff_falling_edge_timing[i + 1] > 2000 && diff_falling_edge_timing[i + 1] < 2500) value_bit[i] = 1;
			}
			
			for(i = 0; i <32; i++)
			{
				//printf("%d \r\n", (int)diff_falling_edge_timing[i]);
				//printf("%d \r\n", (int)value_bit[i]);
			}
			
			for(i = 0; i < 8; i++)
			{
				value_hex >>= 1;
				
				if(value_bit[i+16] == 1) 
				{
					value_hex |= 0x80;
				}
			}
			
			//printf("%02x\r\n", value_hex);
			
			for(i = 0; i< 10; i++)
			{
				if(value_num[i] == value_hex) 
				{
					remocon_num = i;
				}
			}
			
			printf("rem:%d\r\n", remocon_num);
}

void TIM2_IRQHandler (void) {
	if ((TIM2->SR & 0x0001) != 0) { // check update interrupt source
		GPIOB->ODR |= (0x01 << 13);
		TIM2->SR &= ~(1<<0); // clear UIF flag
	}
	else if ((TIM2->SR & 0x0002) != 0) { // check capture compare interrupt source
		GPIOB->ODR &= ~(0x01 << 13);
		TIM2->CCR1 = servo_duty;
		TIM2->SR &= ~(1<<1); // clear CC1E flag
	}		
	else if ((TIM2->SR & 0x0004) != 0) { // check capture compare interrupt source

		TIM2->SR &= ~(1<<2); // clear CC1E flag
	}	
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

void set_cursor(int row, int col)
{
	if(row == 1)
	{
		set_instruction_4bit(0x80 + col);
	}
	else if(row == 2)
	{
		set_instruction_4bit(0xC0 + col);
	}
}

void charLCD_string(char *string)
{
	int i;
	for(i = 0; i < strlen(string); i++)
	{
		set_data_4bit(string[i]);
	}
}

void ultrasonic()
{
	GPIOB->ODR |= 0x01 << 5;
	Delay(1);
	GPIOB->ODR &= ~(0x01 << 5);
}

void create_menu(int length, char**menu_text) {
	CharLcd_menu *menu = (CharLcd_menu *)malloc(sizeof(CharLcd_menu));
	
	if(CharLcd_idx == MENU_MAX) return;
	
	menu->length_menu = length;
	menu->text_menu = (char **)malloc(sizeof(char*) * length);
	menu->text_menu = menu_text;
	//printf("%s \r\n", menu->text_menu[1]);
	charLcd_arr[CharLcd_idx++] = menu;
}

void clear_menu()
{
	set_cursor(1, 0);
	charLCD_string("                ");
	set_cursor(2, 0);
	charLCD_string("                ");
	set_cursor(1, 0);
}

void init_menu()
{
	create_menu(5,main_menu_text);
	create_menu(2,rem_menu_text);
	create_menu(2,ultra_menu_text);
	create_menu(5,servo_menu_text);
	create_menu(4,buzzer_menu_text);
	create_menu(3,led_menu_text);
	CharLcd_idx = 0;
	
	array1 = charLcd_arr[CharLcd_idx]->text_menu[0];
	array2  =charLcd_arr[CharLcd_idx]->text_menu[1];
	clear_menu();
	set_cursor(1, 0);
	charLCD_string(array1);
	set_cursor(2, 0);
	charLCD_string(array2);
	set_cursor(2,0);
	charLCD_string(" ");
	set_cursor(1,0);
	charLCD_string(">");
}

void clear_move_menu()
{
	curr_pos = 0;
	print_toggle = 1;
	cursor_toggle = 1;
	curr_up_down = 0;
}

void print_menu()
{
	if(curr_up_down == 0 && up_flag == 1)
	{
		up_flag = 0;
		if(curr_pos <= 0)
		{
		}
		else
		{
			curr_pos--;
			print_toggle = 1;
			cursor_toggle = 1;
		}
	}
	else if(up_flag == 1)
	{
		curr_pos--;
		up_flag = 0;
		curr_up_down = 0;
		cursor_toggle = 1;
	}
	if(curr_up_down == 1 && down_flag == 1)
	{
		down_flag = 0;
		if(curr_pos >= charLcd_arr[CharLcd_idx]->length_menu - 1)
		{
		}
		else
		{
			curr_pos++;
			print_toggle = 1;
			cursor_toggle = 1;
		}
	}
	else if(down_flag == 1)
	{
		curr_pos++;
		down_flag = 0;
		curr_up_down = 1;
		cursor_toggle = 1;
	}
	
	if(CharLcd_idx == 0)
	{
		if(enter_flag == 1)
		{
			enter_flag = 0;
			
			if(curr_pos == 0)
			{
				CharLcd_idx = 1;
				clear_move_menu();
			}
			else if(curr_pos == 1)
			{
				CharLcd_idx = 2;
				clear_move_menu();
			}
			else if(curr_pos == 2)
			{
				CharLcd_idx = 3;
				clear_move_menu();
			}
			else if(curr_pos == 3)
			{
				CharLcd_idx = 4;
				clear_move_menu();
			}
			else if(curr_pos == 4)
			{
				CharLcd_idx = 5;
				clear_move_menu();
			}
		}
	}
	else if(CharLcd_idx == 1)
	{
		
	}
	else if(CharLcd_idx == 2)
	{
		
	}
	else if(CharLcd_idx == 3)
	{
		
	}
	else if(CharLcd_idx == 4)
	{
		
	}
	else if(CharLcd_idx == 5)
	{
		
	}
	
	if(print_toggle)
	{
		array1 = charLcd_arr[CharLcd_idx]->text_menu[curr_pos];
		array2 = charLcd_arr[CharLcd_idx]->text_menu[curr_pos + 1];
		clear_menu();
		set_cursor(1, 0);
		charLCD_string(array1);
		set_cursor(2, 0);
		charLCD_string(array2);
	}
	if(cursor_toggle)
	{
		if(curr_up_down == 0)
		{
			set_cursor(2,0);
			charLCD_string(" ");
			set_cursor(1,0);
			charLCD_string(">");
		}
		else if(curr_up_down == 1)
		{
			set_cursor(1,0);
			charLCD_string(" ");
			set_cursor(2,0);
			charLCD_string(">");
		}
	}
	
}

void usart_pirnt_menu()
{
	for(i = 0; i < CharLcd_idx; i++)
	{
		for(j = 0; j < charLcd_arr[i]->length_menu; j++)
		{
			printf("%s ", charLcd_arr[i]->text_menu[j]);
		}
		printf("\r\n");
	}
}

/*--------------------------------------------------------------------------------------------------------------------*\
 | MIAN ENTRY                                               																																																													|
\*--------------------------------------------------------------------------------------------------------------------*/
int main (void) {
	unsigned long curr_millis = 0;
	unsigned long prev_millis = 0;
	
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
	
	// remocon
	RCC->APB2ENR |= (0x01 << 0);
	GPIOA->CRH &= ~(0x0F << (2 * 4)); // D2
	GPIOA->CRH |= (0x04 << (2 * 4)); // D2
	EXTI->IMR |= 0x01 << 10;
	EXTI->FTSR |= 0x01 << 10;
	AFIO->EXTICR[2] |= (0x00 <<  (2 * 4));
	
	// ultrasonic
	GPIOC->CRL &= ~(0x0F << (4 * 4)); // D3
	GPIOB->CRL &= ~(0x0F << (5 * 4)); // D4 == change pin
	GPIOC->CRL |= (0x04 << (4 * 4)); // D3
	GPIOB->CRL |= (0x03 << (5 * 4)); // D4
	EXTI->IMR |= 0x01 << 4;
	EXTI->FTSR |= 0x01 << 4;
	EXTI->RTSR |= 0x01 << 4;
	AFIO->EXTICR[1] |= (0x02 <<  (0 * 4));

	// servo
	RCC->APB1ENR |= (0x01 << 0);
	GPIOB->CRH &= ~(0x0F << (5 * 4)); // D5
	GPIOB->CRH |= (0x03 << (5 * 4)); // D5
	TIM2->PSC = 720 - 1;  // 10us
	TIM2->ARR = 2000 - 1;
	TIM2->CNT = 0x00;
	TIM2->DIER |= 0x03; // update/capture compare interrupt enable
	TIM2->CCMR1 = 0x68; // PWM mode
	TIM2->CCER = 0x01;
	TIM2->CCR1 = 70 - 1; // PWM count
	TIM2->CR1 |= 0x01;  // timer2 count enable

	printf("init charlcd-------------------------------------------- \r\n");
	
	charLCD_init();
	init_menu();
	usart_pirnt_menu();
	
	NVIC->ISER[0] |= (0x01 << 10); //EXTI 4
	NVIC->ISER[0] |= (0x01 << 28); // TIMER 2
	NVIC->ISER[1] |= (0x01 << 6); //USART 2
	NVIC->ISER[1] |= (0x01 << (40-32)); //EXTI 10-15
	Delay_ms(1000);
	
	printf("loop-------------------------------------------- \r\n");
	
	down_flag = 1;
	//up_flag = 1;
	print_menu();
	Delay_ms(1000);
	//down_flag = 1;
	//up_flag = 1;
	enter_flag = 1;
	print_menu();
	Delay_ms(1000);
		
	while(1);

	while(1)
	{
		//printf("%d\r\n", sys_count);
		curr_millis = micros_10us();
		
		
		if(curr_millis - prev_millis > 100000) // 1000ms
		{
			prev_millis = curr_millis;
			//printf("1sec \r\n");
			set_data_4bit(0x31);
			ultrasonic();
			servo_duty += 30;
			if(servo_duty >= 230)
			{
				servo_duty = 70;
			}
		}
		
		if(exti_callback_flag == 1)
		{
			exti_callback_flag = 0;
			exit_callback_func();
		}
		if(exti_ultrasonic_flag == 1)
		{
			exti_ultrasonic_flag = 0;
			//printf("distance:");
			printf("ultra:%ld \r\n", ultrasonic_distance);
		}
	}
	
//	while(1)
//	{
//		Delay(100000);
//		//set_instruction_4bit(0x01);
//		//set_instruction_4bit(0x80);
//		set_data_4bit(0x31);
//		
//		if(exti_callback_flag == 1)
//		{
//			exti_callback_flag = 0;
//			exit_callback_func();
//		}
//	}
	
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