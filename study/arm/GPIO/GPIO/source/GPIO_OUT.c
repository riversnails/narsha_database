
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 5 )                // PB5: LED D2
#define LED6            ( 1 << 6 )                // PB5: LED D2
#define LED7            ( 1 << 7 )                // PB5: LED D2

int i = 0;
int j = 0;
unsigned int count = 0;
unsigned int count_10ms = 0;
unsigned int var_pwm = 0;
int count_isr = 0;
int var_timer_pwm = 0;
char toggle_flag = 0;
volatile unsigned int count_exti = 0;
#define GPIOA_CRL *((volatile int *)(0x40010800))
#define GPIOA_ODR *((volatile int *)(0x4001080C))
#define GPIOA_IDR  *((volatile int *)(0x40010808))


void Delay_count(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }

	void Delay_us(int count_us) {
		int i, j;
		for(j=0; j < count_us; j++)
			for(i=0; i < 14; i++);
  }
	
	void Delay_ms(int count_ms) {
		int i;
		for(i=0; i < count_ms; i++)
			Delay_us(1000);
  }

unsigned 	int test=5;
	
//===============================
int i,j;

volatile unsigned int TimingDelay;	
volatile unsigned long sys_count;

unsigned long c_mic;
unsigned long p_mic1;
unsigned long p_mic2;

int led_toggle = 0;

int count_pwm = 0;
int pwm_duty = 20;

int count_servo = 0;
int servo_duty = 700;

unsigned long rising_time = 0;
unsigned long falling_time = 0;
unsigned long diff_time = 0;

unsigned long remocon_time[40];
unsigned int remocon_diff_time[40];
unsigned int remocon_data_bit[40];

int rem_count = 0;

int remocon_hex_data;
int remocon_flag = 0;
char remocon_data=0;

char rem_data[10] = {0x68,0x30,0x18,0x7a,0x10,0x38,0x5a,0x42,0x4a,0x52};
int remocon_value = 0;
int remocon_pre_value = 0;

unsigned short adc_value_x = 0;



unsigned long micros_10us()
{
	return sys_count;
}


void Delay(unsigned int nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}


void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


void static_led_pwm()
{
	if(count_pwm == 100)
	{
		count_pwm = 0;
		GPIOA->BSRR = 0x01 << 5;
	}
	else if(count_pwm == pwm_duty)
	{
		GPIOA->BRR = 0x01 << 5;
	}
	
	count_pwm++;
}

void static_servo()
{
	if(count_servo == 625)
	{
		count_servo = 0;
		GPIOA->BSRR = 0x01 << 8;
	}
	else if(count_servo == servo_duty)
	{
		GPIOA->BRR = 0x01 << 8;
	}
	
	count_servo++;
}

// ISR Routine
//------------------------------------
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
	sys_count++;
}

//void EXTI0_IRQHandler(void)
//{
//	if (EXTI->PR & (1<<0)) {                       // EXTI0 interrupt pending?

//		remocon_time[rem_count] = micros_10us() * 10;
//		
//		if(rem_count > 0){
//			remocon_diff_time[rem_count-1] = remocon_time[rem_count] - remocon_time[rem_count-1];
//			
//			if(remocon_diff_time[rem_count-1] > 13000 && remocon_diff_time[rem_count-1] < 14000)
//			{
//				rem_count = 1;
//			}
//			if(remocon_diff_time[rem_count-1] > 11000 && remocon_diff_time[rem_count-1] < 12000)
//			{
//				rem_count = -1;
//				remocon_value = remocon_pre_value;
//			}			
//		}

//		
//		rem_count++;
//		if(rem_count == 34)
//		{
//			rem_count = 0;
//			remocon_flag = 1;
//		}
//		
//		EXTI->PR = (1<<0);                          // clear pending interrupt
//	}
//}


//void EXTI0_IRQHandler(void)
//{
//	
//	if(toggle_flag == 0) 
//		 {
//			 toggle_flag = 1;
//			 GPIOA_ODR |= 0x02;
//		 }
//		 else 
//		 {
//			 toggle_flag = 0;
//			 GPIOA_ODR &= ~0x02;
//		 }
//	 
//	EXTI->PR = (1<<0);                          // clear pending interrupt
//}

unsigned int first_time = 0;
unsigned int last_time = 0;
unsigned int difference_time = 0;

void EXTI0_IRQHandler(void)
{
	if (EXTI->PR & (1<<0)) {   
		if(GPIOA_IDR & 0x01)
		{
			first_time = count_exti;
		}
		else
		{
			last_time = count_exti;
			
			difference_time = last_time - first_time;
		}
		 
		EXTI->PR = (1<<0);                          // clear pending interrupt
	}
}


int toggle1 = 0;
int toggle2 = 0;


void EXTI1_IRQHandler(void)
{
	if (EXTI->PR & (1<<1)) {                       // EXTI0 interrupt pending?
		if(toggle1)
			GPIOA->ODR |= 0x01 << 11;
		else 
			GPIOA->ODR &= ~(0x01 << 11);
		
		toggle1 = !toggle1;
		
		//EXTI->PR = (1<<1);                          // clear pending interrupt
	}
}


void EXTI2_IRQHandler(void)
{
	if (EXTI->PR & (1<<2)) {                       // EXTI0 interrupt pending?
		if(toggle2)
			GPIOA->ODR |= 0x01 << 12;
		else 
			GPIOA->ODR &= ~(0x01 << 12);
		
		toggle2 = !toggle2;
		
		EXTI->PR = (1<<2);                          // clear pending interrupt
	}
}


		
void EXTI15_10_IRQHandler(void)
{
	if (EXTI->PR & (1<<13)) {                       // EXTI0 interrupt pending?

		if( GPIOC->IDR & (0x01 << 13) )
		{
			rising_time = micros_10us();
		}
		else
		{
			falling_time = micros_10us();
			diff_time = (falling_time - rising_time) * 10;
		}
		
		
		EXTI->PR = (1<<13);                          // clear pending interrupt
	}
}

//void TIM2_IRQHandler (void) {

//	 if ((TIM2->SR & 0x0001) != 0) { // check update interrupt source
//			
//			//static_led_pwm();
//			GPIOA->BSRR = 0x01 << 5;
//	    
//			TIM2->SR &= ~(1<<0); // clear UIF flag
//	 }
//	 else if ((TIM2->SR & 0x0002) != 0) { // check capture compare interrupt source
//			
//			//static_led_pwm();
//			GPIOA->BRR = 0x01 << 5;
//	    
//			TIM2->SR &= ~(1<<1); // clear UIF flag
//	 }		
//}


//char toggle_flag = 0;

//void TIM2_IRQHandler (void) {

//	 if ((TIM2->SR & 0x0001) != 0) { 
//		 
//		 if(toggle_flag == 0) 
//		 {
//			 toggle_flag = 1;
//			 GPIOA_ODR |= 0x02;
//		 }
//		 else 
//		 {
//			 toggle_flag = 0;
//			 GPIOA_ODR &= ~0x02;
//		 }
//			
//			TIM2->SR &= ~(1<<0); // clear UIF flag
//	 }
//	 
//}


//void TIM2_IRQHandler (void) {

//	 if ((TIM2->SR & 0x0001) != 0) { 
//		 
//		 if(count_isr == 100)
//		 {
//			 GPIOA_ODR |= 0x02;
//			 count_isr = 0;
//			 
//		 }
//		 else if(count_isr == var_timer_pwm)
//		 {
//			 GPIOA_ODR &= ~0x02;
//		 }
//			
//		 count_isr++;
//		 TIM2->SR &= ~(1<<0); // clear UIF flag
//	 }
//	 
//}



void TIM2_IRQHandler (void) {

	 if ((TIM2->SR & 0x0001) != 0) { 
		 count_exti++;
		 
		 TIM2->SR &= ~(1<<0); // clear UIF flag
	 }
	 
}






//void TIM3_IRQHandler (void) {

//	 if ((TIM3->SR & 0x0001) != 0) { // check update interrupt source
//			
//			//static_servo();
//			GPIOA->BSRR = 0x01 << 8;
//	    
//			TIM3->SR &= ~(1<<0); // clear UIF flag
//	 }	
//	 else if ((TIM3->SR & 0x0002) != 0) { // check capture compare interrupt source
//			
//			GPIOA->BRR = 0x01 << 8;
//	    
//			TIM3->SR &= ~(1<<1); // clear UIF flag
//	 }		 
//} 


void TIM3_IRQHandler (void) {

	 if ((TIM3->SR & 0x0001) != 0) {
		 
		 if(var_timer_pwm == 99) var_timer_pwm = 1;
		var_timer_pwm++;
		 
		TIM3->SR &= ~(1<<0); // clear UIF flag
	 }
} 


//------------------------------------




/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/


int main (void) {
//  	int a =5;
	
	stm32_Init ();                                  // STM32 setup
	
	/*
	//PA0
	*((volatile int *)(0x40010800)) = 0x44444443;
	
	// PA1
	*((volatile int *)(0x40010800)) &= ~(0x0f << 4);
	*((volatile int *)(0x40010800)) |= (0x03 << 4);
	
	
	*((volatile int *)(0x4001080C)) = 0x01;
	
	while(1) {
		if(j == 0) {
			*((volatile int *)(0x4001080C)) |= 0x01;
			j  =1;
		}
		else if(j == 1){
			*((volatile int *)(0x4001080C)) &= ~0x01;
			j = 0;
		}
		
		if(i == 0)
		{
			*((volatile int *)(0x4001080C))|= 0x02;
			i++;
		}
		else if(i == 2)
		{
			*((volatile int *)(0x4001080C)) &= ~0x02;
			i++;
		}
		else if(i == 3) i = 0;
		else i++;
		Delay_count(1000);
	}
	*/
	
	/*
	//PA0
	*((volatile int *)(0x40010800)) = 0x44444443;
	
	while(1) {
		*((volatile int *)(0x4001080C)) |= 0x01;
		Delay_ms(1);
		*((volatile int *)(0x4001080C)) &= ~0x01;
		Delay_ms(9);
	}
	*/
	
	/*
	GPIOA_CRL = 0x44444400;
	GPIOA_CRL |= (0x04 << 0); // Float Input
	GPIOA_CRL |= (0x03 << 4); // Output 50MHz Push-Pull
	
	while(1)
	{
		if(GPIOA_IDR & 0x01)
		{
			GPIOA_ODR |= 0x02;
		}
		else
		{
			GPIOA_ODR &= ~0x02;
		}
	}
	*/
	
	
	/*
	GPIOA_CRL = 0x44444400;
	GPIOA_CRL |= (0x03 << 4);
	
	while(1) {
		if(count == var_pwm)
		GPIOA_ODR &= ~0x02;
		else if(count == 100)
		{
			GPIOA_ODR |= 0x02;
			count = 0;
		}
		
		if(count_10ms > 100)
		{
			count_10ms = 0;
			var_pwm++;
			if(var_pwm == 99)	var_pwm = 1;
		}
			
		count++;
		count_10ms++;
		Delay_us(100);
	}
	*/
	
	
	GPIOA_CRL = 0x44444400;
	GPIOA_CRL |= (0x04 << 0); // floating input
	GPIOA_CRL |= (0x03 << 4); // 50mhz pushpull
	
	
	
	
	
	// systick
	SysTick->LOAD = 720;
	SysTick->CTRL = 0x07;

	//clock
	RCC->APB2ENR |= 0x01 << 2;  // GPIOA clock enable
	RCC->APB2ENR |= 0x01 << 4;  // GPIOC clock enable
	RCC->APB2ENR |= 0x01 << 9;  // ADC1 clock enable	
	RCC->APB1ENR |= 0x01 << 0; // timer2 clock enable
	RCC->APB1ENR |= 0x01 << 1; // timer3 clock enable
	RCC->AHBENR |= 0x01 << 0; // DMA1 clock enable
	
	//GPIOA.5 : GPIO output push/pull 50MHz
	GPIOA->CRL &= ~(0x0f << 20);
	GPIOA->CRL |= 0x03 << 20;
	//GPIOA.8 : GPIO output push/pull 50MHz
	GPIOA->CRH &= ~(0x0f << 0);
	GPIOA->CRH |= 0x03 << 0;
	//GPIOC.0 : GPIO floating Input mode 
	GPIOC->CRL &= ~(0x0f << 0);
	GPIOC->CRL |= 0x04 << 0;	
	//GPIOC.13 : GPIO floating Input mode 
	GPIOC->CRH &= ~(0x0f << 24);
	GPIOC->CRH |= 0x04 << 24;		
	//GPIOA.0 : GPIO analog input
	GPIOA->CRL &= ~(0x0f << 0);
	GPIOA->CRL |= 0x00 << 0;	
	
	
	//EXTI 0
	EXTI->IMR |= 0x01;
	EXTI->FTSR |= 0x01;
	AFIO->EXTICR[0] |= 0x02;  // PC.0 interrupt

	// EXTI 13
	EXTI->IMR |= 0x01 << 13;
	EXTI->FTSR |= 0x01 << 13;
	EXTI->RTSR |= 0x01 << 13;
	AFIO->EXTICR[3] |= 0x20;  // PC.13 interrupt

	//timer2
	TIM2->CR1 |= 0x01;  // timer2 count enable
	TIM2->PSC = 72;  // 72MHz / 72 = 1MHz
	TIM2->ARR = 2000;
	TIM2->CNT = 0x00;
	TIM2->DIER |= 0x03; // update/capture compare interrupt enable
	TIM2->CCMR1 = 0x60;
	TIM2->CCR1 = 200;

	//timer3
	TIM3->CR1 |= 0x01;  // timer3 count enable
	TIM3->PSC = 72;  // 72MHz / 72 = 1MHz
	TIM3->ARR = 20000;
	TIM3->CNT = 0x00;
	TIM3->DIER |= 0x03; // update/capture compare interrupt enable
	TIM3->CCMR1 = 0x60;
	TIM3->CCR1 = 2000;
	
	
	//DMA
	DMA1_Channel1->CNDTR = 0x01;
	DMA1_Channel1->CPAR = 0x4001244c;  // adc data register
	DMA1_Channel1->CMAR = (unsigned int)&adc_value_x;
	DMA1_Channel1->CCR = 0x521;
	
	// ADC
	ADC1->CR1 = 0x100;
	ADC1->CR2 = 0x1E0103;
	ADC1->SMPR1 = 0x00;
	ADC1->SMPR2 = 0x05;
	
	
	//NVIC
	NVIC->ISER[0] |= 0x01 << 28; //timer2 interrupt enable
	NVIC->ISER[0] |= 0x01 << 29; //timer3 interrupt enable
	
	NVIC->ISER[0] |= 0x01 << 6; //EXTI0 interrupt enable
	NVIC->ISER[1] |= 0x01 << (40-32); //EXTI15_10 interrupt enable

//=================================
// Servo/LED pwm variable duty

	while(1)
	{
		c_mic = micros_10us();
		if(c_mic - p_mic1 > 1000)
		{
			p_mic1 = c_mic;
			
			pwm_duty+=20;
			if(pwm_duty > 1999) pwm_duty = 20;	

			TIM2->CCR1 = pwm_duty;			
		}	

		if(c_mic - p_mic2 > 2000)
		{
			p_mic2 = c_mic;
			
			servo_duty+=16;
			if(servo_duty > 2300) servo_duty = 700;	
			TIM3->CCR1 = servo_duty;
			
		}			
		
		
		//------------------------------------------
		// Remocon
			if(remocon_flag == 1)
			{
				remocon_flag = 0;
				
				for(i=0;i<32;i++){
					if(remocon_diff_time[i+1] > 1000 && remocon_diff_time[i+1] < 1500)
					{
						remocon_data_bit[i] = 0;
					}
					else if(remocon_diff_time[i+1] > 2000 && remocon_diff_time[i+1] < 2500)
					{
						remocon_data_bit[i] = 1;
					}						
					
				}
				
				remocon_hex_data = 0;
				for(i=0;i<8;i++)
				{
					remocon_hex_data = remocon_hex_data << 1;
					if(remocon_data_bit[16+i]  == 1){
						remocon_hex_data |= 0x01;
					}
				}
				
				remocon_data = (char)remocon_hex_data;
				
				remocon_value = 0;
				for(i=0;i<10;i++)
				{
					if(remocon_data == rem_data[i])
					{
						remocon_value = i;
						remocon_pre_value= i;
						break;
					}						
				}
			}
		
	}

//=================================
// LED pwm variable duty

//	while(1)
//	{
//		c_mic = micros_10us();
//		if(c_mic - p_mic1 > 1000)
//		{
//			p_mic1 = c_mic;
//			
//			pwm_duty+=20;
//			if(pwm_duty > 1999) pwm_duty = 20;	

//			TIM2->CCR1 = pwm_duty;			
//		}		
//	}

//=================================
// LED pwm variable duty

//	while(1)
//	{
//		c_mic = micros_10us();
//		if(c_mic - p_mic1 > 1000)
//		{
//			p_mic1 = c_mic;
//			
//			pwm_duty++;
//			if(pwm_duty == 99) pwm_duty = 1;				
//		}		
//	}
	
//=================================
// LED toggle

//	while(1)
//	{
//		c_mic = micros_10us();
//		if(c_mic - p_mic1 > 10000)
//		{
//			p_mic1 = c_mic;
//			
//			if(led_toggle == 0){
//				led_toggle = 1;
//				GPIOA->BSRR = 0x01 << 5;
//			}
//			else if(led_toggle == 1){
//				led_toggle = 0;
//				GPIOA->BRR = 0x01 << 5;
//			}
//				
//		}
//	}
	
//=====================================================	
	
//	while(1)
//	{
//		GPIOA->ODR |= 0x01 << 5;
//		//GPIOA->BSRR = 0x01 << 5;
//		
//		Delay(100);
//		
//		//GPIOA->ODR &= ~(0x01 << 5);
//		GPIOA->BRR = 0x01 << 5;
//		
//		Delay(100);		
//	}

	
	
	//====================================================
	
	


	
	*(volatile unsigned int *)0x40021018 |= 0x08;
	
	RCC->APB2ENR |= 0x04;
	
		GPIOB->CRL = 0x33333333;
		GPIOB->ODR = 0x20;
	
		GPIOB->ODR |= 0xC0;
	
	
//				*(volatile unsigned int *)0x40011004 &= ~(0x0f << 20);
//	 *(volatile unsigned int *)0x40011004 |= (0x04 << 20);
	
//	
//				*(volatile unsigned int *)0x40010800 = 0x03;
	
	//GPIOB->CRL &= ~(0x0f << 28);
	//GPIOB->CRL |= (0x04 << 28);
	
 for(;;) {


//			*(volatile unsigned int *)0x4001100c = 0x00;	 
//			Delay(20000);
//			*(volatile unsigned int *)0x4001100c = 0x2000;
			Delay(20000);		
		
	// test=GPIOB->IDR;
		
//    GPIOB->ODR &= ~LED;                           // switch on LED
//    GPIOB->ODR &= ~LED6;                           // switch on LED
//    GPIOB->ODR |= LED7;                           // switch on LED
//	Delay(2000000);
//    GPIOB->ODR |=  LED;                           // switch off LED
//    GPIOB->ODR |=  LED6;                           // switch on LED
//    GPIOB->ODR &=  ~LED7;                           // switch on LED
	//Delay(2000000);

    }
   
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
