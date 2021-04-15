#include "stm32f10x.h"
//#include "stm32_eval.h"
#include <stdio.h>
#include<stdarg.h>


int rotary_state = 0, rotary_state_left = 0, rotary_state_right = 0;

int right_state_flag = 0, left_state_flag = 0;
int prev_rotary_state = 0; 

int prev_output_a = 1;

int rotary_button_value = 0;
int rotary_button_flag = 0;
	
int rotary_count = 0;


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

enum {
	SELECT_INDEX_LED_PWM = 0,
	SELECT_INDEX_SERVO_MOTOR,
	SELECT_INDEX_STEP_MOTOR,
	SELECT_INDEX_LED_ON_OFF,
};

enum {
	MENU_STEP_MAIN = 0,
	MENU_STEP_PWM_LED,
	MENU_STEP_SERVO_MOTOR,	
	MENU_STEP_STEP_MOTOR,	
	MENU_STEP_LED_ON_OFF	
};


void char_lcd_string(char *string);


char *char_lcd_menu[4] = {" 1. LED PWM     ",
						 " 2. SERVO Motor ",
						 " 3. Step Motor  ",
						 " 4. LED On/Off  " };

char *char_lcd_submenu_step_motor = {" motor pos :    "};//,
										 // ""	};

int right_state_changed = 0;
int left_state_changed = 0;
int menu_idx = 0;
int cursor_pos = 0;
int select_index = 0;
int button_pressed = 0;
int menu_step = MENU_STEP_MAIN;
int count = 0;
int step_flag = 0;


void printf_lcd(char *format, ...)
{
	char buf[512];
	va_list arglist;

	va_start(arglist, format);
	vsprintf(buf, format, arglist);
	va_end(arglist);
	char_lcd_string(buf);
}


int fputc(int ch, FILE *f)
{
	while(!(USART1->SR & (0x01 << 7)));
  USART1->DR = (unsigned char)ch;
  return ch;
}


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


void systick_func()
{
	//-----------------------------
	//SysTick
	if (SysTick_Config(720))
	{ 
		while (1);
	}
	//-----------------------------
}

void gpio_set()
{
	GPIO_InitTypeDef GPIO_InitStructure;
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
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//-----------------------------

	//-----------------------------
	// char LCD
	// RS, RW, E
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	// DATA 8bit, Backlight
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	//-----------------------------

}

void uart_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	//usart
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}

void rotary_switch_func()
{
	unsigned short in_data = 0;
	int output_a = 0, output_b = 0;
	
	in_data = GPIOC->IDR;
	output_a = (in_data & (0x01 << 6) );
	output_b = (in_data & (0x01 << 7) );
	rotary_button_value = (in_data & (0x01 << 15) );
	
	
//---------------------------------------------------
	if( output_a == 0 && output_b > 0 ) rotary_state = ROTARY_STATE_1;
	else if( output_a == 0 && output_b == 0 ) rotary_state = ROTARY_STATE_2;
	else if( output_a > 0 && output_b == 0 ) rotary_state = ROTARY_STATE_3;
	else if( output_a > 0 && output_b > 0 ) rotary_state = ROTARY_STATE_4;
	
	if( rotary_state == ROTARY_STATE_3 && prev_rotary_state == ROTARY_STATE_2 ) 
	{
		if(left_state_flag == 0) 
		{
			left_state_flag = 1;
			left_state_changed = 1;
			//printf("left \r\n");
		}
	}
	if( rotary_state == ROTARY_STATE_1 && prev_rotary_state == ROTARY_STATE_2 )
	{
		if(right_state_flag == 0) 
		{
			right_state_flag = 1;
			right_state_changed = 1;
			//printf("right \r\n");
		}
	}
	else if(rotary_state == ROTARY_STATE_4 )
	{
			right_state_flag = 0;
			left_state_flag = 0;
	}
	
	//printf("%d %d", right_state_flag, left_state_flag);
	//printf("%d\r\n", rotary_state);
	
	prev_rotary_state = rotary_state;
//---------------------------------------------------
	
	if(rotary_button_value == 0)
	{
		if( rotary_button_flag == 0 )
		{
			rotary_button_flag = 1;
			//printf("rotary button down \r\n");
			printf("select_index = %d\r\n", select_index);

			button_pressed = 1;
		}
	}
	else
	{
		rotary_button_flag = 0;
	}
}


	
void rs_high_char_lcd()
{

	GPIOD->ODR |= (0x01 << 11);
}

void rs_low_char_lcd()
{

	GPIOD->ODR&= ~(0x01 << 11);
}

void rw_high_char_lcd()
{

	GPIOD->ODR |= (0x01 << 5);
}

void rw_low_char_lcd()
{

	GPIOD->ODR&= ~(0x01 << 5);
}

void enable_high_char_lcd()
{

	GPIOD->ODR |= (0x01 << 4);
}

void enable_low_char_lcd()
{

	GPIOD->ODR&= ~(0x01 << 4);
}

void backlight_On_char_lcd()
{

	GPIOE->ODR |= (0x01 << 5);
}

void backlight_Off_char_lcd()
{

	GPIOE->ODR&= ~(0x01 << 5);
}

void data_set_char_lcd(unsigned char data)
{
	GPIOE->ODR &= ~(0xff00);
	GPIOE->ODR |= (data << 8);
}

void char_lcd_inst(unsigned char inst)
{
	rs_low_char_lcd();
	data_set_char_lcd(inst);
	enable_high_char_lcd();
	enable_low_char_lcd();
	rs_high_char_lcd();


	Delay(5); // 50us
}

void char_lcd_data(unsigned char data)
{
	data_set_char_lcd(data);
	enable_high_char_lcd();
	enable_low_char_lcd();
	
	Delay(5); // 50us
}

void char_lcd_string(char *string)
{

	int i;
	for(i=0;i<strlen(string);i++)
	{
		char_lcd_data(*(string+i));
	}
}

void char_lcd_init()
{
	backlight_On_char_lcd();
	
	rw_low_char_lcd();
	rs_high_char_lcd();

	char_lcd_inst(0x30);
	char_lcd_inst(0x30);
	char_lcd_inst(0x30);
	char_lcd_inst(0x38);
	char_lcd_inst(0x06);
	char_lcd_inst(0x0c);
	char_lcd_inst(0x80);
	char_lcd_inst(0x01);
	Delay_ms(2);
}

void char_lcd_cursor_set(char line, char pos)
{
	if(line == 0) char_lcd_inst(0x80 + pos);
	else if(line == 1) char_lcd_inst(0xC0 + pos);
}

void char_lcd_set_cursor_pos(int cursor_pos)
{
	if(cursor_pos == 0)
	{
		char_lcd_cursor_set(0, 0);
		char_lcd_data('>');
	}
	else if(cursor_pos == 1)
	{
		char_lcd_cursor_set(1, 0);
		char_lcd_data('>');
	}
}

void char_lcd_clear_cursor_pos(int cursor_pos)
{
	if(cursor_pos == 0)
	{
		char_lcd_cursor_set(0, 0);
		char_lcd_data(' ');
	}
	else if(cursor_pos == 1)
	{
		char_lcd_cursor_set(1, 0);
		char_lcd_data(' ');
	}
}


void char_lcd_func()
{
	if(right_state_changed == 1)
	{
		right_state_changed = 0;

		if(cursor_pos == 0) 
		{
			char_lcd_clear_cursor_pos(0);
			char_lcd_set_cursor_pos(1);
			cursor_pos = 1;
		}
		else if(cursor_pos == 1)
		{

			menu_idx++;
			if(menu_idx > 2) menu_idx = 2;

			char_lcd_cursor_set(0,0);
			printf_lcd(char_lcd_menu[menu_idx]);
			char_lcd_cursor_set(1,0);
			printf_lcd(char_lcd_menu[menu_idx+1]);
			char_lcd_set_cursor_pos(1);
		}
		select_index = menu_idx + cursor_pos;
	}
	else if(left_state_changed == 1)
	{
		left_state_changed = 0;

		if(cursor_pos == 1) 
		{
			char_lcd_clear_cursor_pos(1);
			char_lcd_set_cursor_pos(0);
			cursor_pos = 0;
		}
		else if(cursor_pos == 0)
		{

			menu_idx--;
			if(menu_idx < 0) menu_idx = 0;

			char_lcd_cursor_set(0,0);
			printf_lcd(char_lcd_menu[menu_idx]);
			char_lcd_cursor_set(1,0);
			printf_lcd(char_lcd_menu[menu_idx+1]);
			char_lcd_set_cursor_pos(0);
		}
		select_index = menu_idx + cursor_pos;
	}
	
}

void sub_menu_step_motor()
{
	char_lcd_cursor_set(0,0);
	char_lcd_string(char_lcd_submenu_step_motor);
	char_lcd_cursor_set(1,0);
	char_lcd_string("                ");
}

void button_func()
{
	if(button_pressed == 1)
	{
		button_pressed = 0;
		if(select_index == SELECT_INDEX_STEP_MOTOR)
		{
			sub_menu_step_motor();
			menu_step = MENU_STEP_STEP_MOTOR;
		}
	}
}

void char_lcd_step_motor_func()
{
char buf[20];
	if(right_state_changed == 1)
	{
		right_state_changed = 0;
		count++;
		GPIO_SetBits(GPIOD, GPIO_Pin_6); 
		step_flag = 1;
	}
	else if(left_state_changed == 1)
	{
		left_state_changed = 0;
		count--;
		GPIO_ResetBits(GPIOD, GPIO_Pin_6); 
		step_flag = 1;
	}

	char_lcd_cursor_set(0,12);
	if(count == 0) printf_lcd("   0");
	else
	{
		sprintf(buf, "%4.d", count);
		printf_lcd(buf);
	}
	
	// if(count > 0) GPIO_ResetBits(GPIOD, GPIO_Pin_6); 
	// else if(count < 0) GPIO_SetBits(GPIOD, GPIO_Pin_6); 
}


int main(void)
{
	int i;
	char buff[50];
	//int count = 0;
	unsigned long c_millis = 0, p_millis = 0;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	systick_func();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*
	while(1)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_12); 
		Delay(20);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		Delay(20);
	}
	*/
	
	systick_func();
	gpio_set();
	uart_init();
	char_lcd_init();
	
	for(i=0;i<6500;i++)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_12); 
		Delay(20);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		Delay(20);
	}
	
	while(1);
	
	printf("hello world \r\n\n\n\n\n");
	//printf("%s\r\n", char_lcd_menu[0]);

	
	
	char_lcd_cursor_set(0,0);
	char_lcd_string(char_lcd_menu[menu_idx]);
	char_lcd_cursor_set(1,0);
	char_lcd_string(char_lcd_menu[menu_idx+1]);
	char_lcd_set_cursor_pos(0);

	while(1)
	{	
		c_millis = micros_10us();

		if(c_millis - p_millis > 100)
		{
			p_millis = c_millis;
			
			rotary_switch_func();
			if(menu_step == MENU_STEP_MAIN)
			{
				char_lcd_func();
			}
			else if(menu_step == MENU_STEP_STEP_MOTOR)
			{
				char_lcd_step_motor_func();
			}
			button_func();
		}
		if(step_flag == 1)
		{
			int i;
			step_flag = 0;
			for(i=0;i<400;i++)
			{
				GPIO_SetBits(GPIOD, GPIO_Pin_12); 
				Delay(20);
				GPIO_ResetBits(GPIOD, GPIO_Pin_12);
				Delay(20);
			}
			printf("hello\r\n");
		}
		//Delay_ms(1);
	}


//----------------------------------------------------------------------------


	while(1)
	{
		char_lcd_cursor_set(0,0);
		//sprintf(buff, "hello world : %d", count++);
		//char_lcd_string(buff);
		printf_lcd("hello world : %d", count++);
		Delay_ms(1000);
	}
	
	
	while(1)
	{
		rotary_switch_func();
		Delay_ms(1);
		
		
//---------------------------------------------------
//		if(prev_output_a == 0 && output_a > 0)
//		{
//			if(output_b == 0)
//			{
//				if(left_state_flag == 0) 
//				{
//					left_state_flag = 1;
//					printf("left \r\n");
//				}
//			}
//			else if(output_b > 0)
//			{
//				if(right_state_flag == 0) 
//				{
//					right_state_flag = 1;
//					printf("right \r\n");
//				}
//			}
//		}
//		else
//		{
//			right_state_flag = 0;
//			left_state_flag = 0;
//		}
//		
//		
//		prev_output_a = output_a;
//		
//		
//		Delay_ms(1);
//---------------------------------------------------
		
		
		
//---------------------------------------------------
//		if( output_a == 0 && output_b > 0 ) rotary_state = ROTARY_STATE_1;
//		else if( output_a == 0 && output_b == 0 ) rotary_state = ROTARY_STATE_2;
//		else if( output_a > 0 && output_b == 0 ) rotary_state = ROTARY_STATE_3;
//		else if( output_a > 0 && output_b > 0 ) rotary_state = ROTARY_STATE_4;
//		
//		if( rotary_state == ROTARY_STATE_3 && prev_rotary_state == ROTARY_STATE_2 ) 
//		{
//			if(right_state_flag == 0) 
//			{
//				right_state_flag = 1;
//				printf("right \r\n");
//			}
//		}
//		if( rotary_state == ROTARY_STATE_1 && prev_rotary_state == ROTARY_STATE_2 )
//		{
//			if(left_state_flag == 0) 
//			{
//				left_state_flag = 1;
//				printf("left \r\n");
//			}
//		}
//		else if(rotary_state == ROTARY_STATE_4 )
//		{
//				right_state_flag = 0;
//				left_state_flag = 0;
//		}
//		
//		//printf("%d %d", right_state_flag, left_state_flag);
//		//printf("%d\r\n", rotary_state);
//		
//		prev_rotary_state = rotary_state;
//		
//		
//		Delay_ms(1);
//---------------------------------------------------
		
		
		
//---------------------------------------------------
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
//---------------------------------------------------
		
		
		
//---------------------------------------------------
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
//---------------------------------------------------


		
//---------------------------------------------------
//		if( output_a == 0 && output_b > 0 )
//		{
//			if(rotary_state_right == ROTARY_STATE_RIGHT_2)
//			{
//				if(right_state_flag == 0) 
//				{
//					right_state_flag = 1;
//					printf("right : %5.d \r\n", ++rotary_count);
//				}
//			}
//			rotary_state_left = ROTARY_STATE_LEFT_1;
//			rotary_state_right = ROTARY_STATE_RIGHT_1;
//		}
//		else if( output_a == 0 && output_b == 0 )
//		{
//			rotary_state_left = ROTARY_STATE_LEFT_2;
//			rotary_state_right = ROTARY_STATE_RIGHT_2;
//		}
//		else if( output_a > 0 && output_b == 0 )
//		{
//			if(rotary_state_left == ROTARY_STATE_2)
//			{
//				if(left_state_flag == 0) 
//				{
//					left_state_flag = 1;
//					printf("left  : %5.d \r\n", --rotary_count);
//				}
//			}
//			rotary_state_left = ROTARY_STATE_LEFT_3;
//			rotary_state_right = ROTARY_STATE_RIGHT_3;
//		}
//		else if( output_a > 0 && output_b > 0 )
//		{
//			rotary_state_left = ROTARY_STATE_LEFT_4;
//			rotary_state_right = ROTARY_STATE_RIGHT_4;
//			
//			right_state_flag = 0;
//			left_state_flag = 0;
//		}
		
		
		//printf("a = %d b = %d\r\n", output_a, output_b);
		//printf("state = %d\r\n", rotary_state);
		//printf("\r\n");
		//Delay_ms(1);
//---------------------------------------------------

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
