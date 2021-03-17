
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization


volatile unsigned long count=0;

int ledLight = 0;
int ledLight1 = 0;
int ledLight2 = 0;
int ledLight3 = 0;
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
//#define LED             ( 1 << 5 )                // PB5: LED D2
#define LED             ( 1 << 6 )                // PB5: LED D2
#define LED1             ( 1 << 7 )                // PB5: LED D2
#define LED2             ( 1 << 8 )                // PB5: LED D2


unsigned int TimingDelay;

void Delay(unsigned int nTime){
	TimingDelay=nTime;
	while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0x00)
		TimingDelay--;
}



/*----------------------------------------------------------*\
 | Timer1 Update Interrupt Handler                          |
\*----------------------------------------------------------*/
void TIM1_UP_IRQHandler (void) {

  if ((TIM1->SR & 0x0001) != 0) {                 // check interrupt source

//	ledLight = ~ledLight;
//	if( ledLight )
//    	GPIOC->ODR &= ~(0x01 << 13);                           // switch on LED
//	else
//    	GPIOC->ODR |=  0x01 << 13;                           // switch off LED

//		
//		GPIOC->ODR |= 0x01 << 13; 
//		
		
		GPIOA->ODR |=   (0x01 << 5);
		
    TIM1->SR &= ~(1<<0);                          // clear UIF flag
		

 }
} // end TIM1_UP_IRQHandler


void TIM1_CC_IRQHandler (void) {

  if ((TIM1->SR & 0x0002) != 0) {                 // check interrupt source

//	ledLight = ~ledLight;
//	if( ledLight )
    	//GPIOC->ODR &= ~(0x01 << 13);                           // switch on LED
//	else
//    	GPIOB->ODR |=  LED;                           // switch off LED

		
		GPIOA->ODR &= ~(0x01 << 5);
		
    TIM1->SR &= ~(1<<1);                          // clear UIF flag
 }
} // end TIM1_UP_IRQHandler


int timer_count=0;
int pwm_var = 0;
void set_74595(unsigned short value);
unsigned short value_74595=0x0000;


 unsigned long c_micros;
  unsigned long p_micros;
   unsigned long p_micros1;
 
 int count_pwm;
 int pwm_duty = 1;
 int i;
 int reg_data = 0;
 int row_num = 0;
 
 unsigned char num[8] = {0x00,0x38,0x44,0x04,0x18,0x04,0x44,0x38};
 


/*----------------------------------------------------------*\
                        |
\*----------------------------------------------------------*/
//void TIM2_IRQHandler (void) {

//  if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source

//		// all row off
//		value_74595 = 0x0000;
//		set_74595(value_74595);  // 
//		// column value
//		value_74595 |= num[row_num] << 8;
//		set_74595(value_74595);  // 
//		// one row on
//		value_74595 |= 0x01 << row_num;
//		set_74595(value_74595);  //
//		// row value
//		row_num++;
//		if(row_num == 8) row_num = 0;
//		
//		
//	
//		
//	//	GPIOA->ODR |=   (0x01 << 5);
//		
////	ledLight2 = ~ledLight2;
////	if( ledLight2 )
////    	GPIOA->ODR &= ~(0x01 << 5);                           // switch on LED
////	else
////    	GPIOA->ODR |=   (0x01 << 5);                          // switch off LED

////		timer_count++;
////		if(timer_count == 100){
////			timer_count = 0;
////			GPIOA->ODR |=   (0x01 << 5);
////		}
////		else if(timer_count == pwm_var)
////			GPIOA->ODR &= ~(0x01 << 5);
////	
//	
//    TIM2->SR &= ~(1<<0);                          // clear UIF flag
// }
//	
////   if ((TIM2->SR & 0x0002) != 0) {                 // check interrupt source

////			GPIOA->ODR &= ~(0x01 << 5);
////			
////    TIM2->SR &= ~(1<<1);                          // clear UIF flag
//// }
// 
//} // end TIM2_UP_IRQHandler



volatile int timer2_toggle = 0;
volatile int servo_count = 0;
int servo_diff = 44;

void TIM2_IRQHandler (void) {

	 if ((TIM2->SR & 0x0001) != 0) { 
		 
		 if(timer2_toggle == 0) {
			 //GPIOA->ODR |= 0x02;
			 timer2_toggle = 1;
		 }
		 else {
			 //GPIOA->ODR &= ~0x02;
			 timer2_toggle = 0;
		 }
		servo_count++;
		 
		 TIM2->SR &= ~(1<<0); // clear UIF flag
	 }
	 
}


void TIM3_IRQHandler (void) {

	 if ((TIM3->SR & 0x0001) != 0) { 
		 
		 if(servo_diff >= 143) {
			 servo_diff = 44;
		 }
		 servo_diff++;
		 
		 TIM3->SR &= ~(1<<0); // clear UIF flag
	 }
	 
}


unsigned long micros_10us()
{
		return count;
}


void SysTick_Handler(void)
 {
//		count++;
	 
		TimingDelay_Decrement();
	 
	 
//if(	 ledLight1 = !ledLight1){
	 
//	ledLight1 = ~ledLight1;
//	if( ledLight1 )
//    	GPIOA->ODR &= ~0x20;                           // switch on LED
//	else
//    	GPIOA->ODR |=  0x20; 	
	 
//	 count++;
	 
//	 #if 0
//	ledLight1 = ~ledLight1;
//	if( ledLight1 )
//    	GPIOB->ODR &= ~LED1;                           // switch on LED
//	else
//    	GPIOB->ODR |=  LED1; 				
//		
//	if(count++ == 1000){
//		count=0;
//		ledLight3 = ~ledLight3;
//		if( ledLight3 )
//	    	GPIOB->ODR &= ~(0x01 << 4);                           // switch on LED
//		else
//	    	GPIOB->ODR |=  (0x01 << 4); 
//	}
//			#endif
		                          // switch off LED
 }

 //------------------------
 void delay_for()
 {
	 int i;
	 for(i=0;i<600;i++);  // 600 : 1ms
 }
 
 void set_74595(unsigned short value)
 {
	 for(i=0;i<16;i++){
		// data 
		if(value & (0x8000 >> i))
		{
			GPIOC->ODR |= 0x01; 
		}
		else{
			GPIOC->ODR &= ~0x01; 
		}
		// clock
		GPIOC->ODR |= 0x02;
		delay_for();
		GPIOC->ODR &= ~0x02;
		delay_for();
	}
	// latch
	GPIOC->ODR |= 0x04;
	delay_for();
	GPIOC->ODR &= ~0x04;
	delay_for();
	
 }
 
 void latch_74595()
 {
	 GPIOC->ODR |= 0x04;
	 delay_for();
	 GPIOC->ODR &= ~0x04;	 
 }
 
 void charLCD_set_inst(char data)
 {
	//1)
	while(!(SPI1->SR & 0x02));
	SPI1->DR = 0x00;  // RS LOW
	delay_for();
	latch_74595();

	//2)
	while(!(SPI1->SR & 0x02));
	SPI1->DR = data;  // data 0x30
	delay_for();
	latch_74595();

	//3)
	while(!(SPI1->SR & 0x02));
	data |= 0x04;
	SPI1->DR = data;  // Enable high : 0x04
	delay_for();
	latch_74595();

	//4)
	while(!(SPI1->SR & 0x02));
	data &=~0x04;
	SPI1->DR = data;  // Enable low : 0x00
	delay_for();
	latch_74595();

	//5)
	while(!(SPI1->SR & 0x02));
	data |= 0x01;
	SPI1->DR = data;  // RS high : 0x01
	delay_for();
	latch_74595();

	//6)
	delay_for();  // delay 50 us
}

void charLCD_set_data(char data)
 {
	//1)
	//RS LOW

	//2)
	data |= 0x01;
	while(!(SPI1->SR & 0x02));
	SPI1->DR = data;  // data 0x30
	delay_for();
	latch_74595();

	//3)
	data |= 0x04;
	while(!(SPI1->SR & 0x02));
	SPI1->DR = data;  // Enable high : 0x04
	delay_for();
	latch_74595();

	//4)
	data &=~0x04;
	while(!(SPI1->SR & 0x02));
	SPI1->DR = data;  // Enable low : 0x00
	delay_for();
	latch_74595();

	//5)
	// RS HIGH

	//6)
	delay_for();  // delay 50 us
}
 
 unsigned long ii;
 char data;
 
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  stm32_Init ();                                // STM32 setup
 
	
	//*((volatile unsigned int *)0xE000E100) |= 0x01 << 28; // NVIC TIM2
//	*((volatile unsigned int *)0x40000000) |= 0x01 << 0; // TIM2_CR1 cen
//	*((volatile unsigned int *)0x4000000C) |= 0x01 << 0; // TIM2_DIER uie
	
	//GPIOA.1
	//*((volatile unsigned int *)0x40021018) |= 0x01 << 2; // RCC base 0x40021000
	RCC->APB2ENR |= (0x01 << 2); // GPIOA enable
	GPIOA->CRL = (0x03 << 4);
	
	//TIMER2
	RCC->APB1ENR |= 0x01;
	TIM2->CR1 = 0x01;
	TIM2->DIER = 0x01;
	TIM2->PSC = 71;
	TIM2->ARR = 15;
	
	//TIMER3
	RCC->APB1ENR |= 0x01 << 1;
	TIM3->CR1 = 0x01;
	TIM3->DIER = 0x01;
	TIM3->PSC = 710;
	TIM3->ARR = 1000;
	
	//NVIC
	NVIC->ISER[0] |= (0x01 << 28); // timer2 UIE
	NVIC->ISER[0] |= (0x01 << 29); // timer3 UIE
	
	while(1){
		if(servo_count >= servo_diff) {
			GPIOA->ODR &= ~0x02;
		}
		if(servo_count >= 1250) {
			GPIOA->ODR |= 0x02;
			servo_count = 0;
		}
	}
	
		NVIC->ICER[0] |= (0x01 << 25);  // timer1 update interrupt clear
		RCC->APB2ENR &= ~(0x01 << 11);  // timer 1 clock disable
	
		RCC->APB1ENR = 0x01;
		RCC->APB2ENR |= 0x04;  // PORTA clock enable

	
		GPIOA->CRL &= ~(0xff << 5*4);
		GPIOA->CRL |= (0x03 << 5*4);
	
	RCC->APB2ENR |= 0x08;  // PORTB clock enable
	RCC->APB2ENR |= 0x10;  // PORTC clock enable
	GPIOC->CRL = 0x33333333;  // ROW : PC0 ~ PC7
	GPIOB->CRH = 0x33333333;   // Column : PB7 ~ PB15
	
	GPIOC->ODR = 0x00;  // all row off
	
//--------------------------------
	//SPI register setting
	RCC->APB2ENR = 0x501C;
	GPIOC->CRL = 0xB4BB3333;
	GPIOA->CRL = 0xB4BB0B04;
	
	SPI1->CR1 = 0x037C;
	SPI1->CR2 = 0x00;

//-----------------------------
//while(1){
//	SPI1->DR = 0x31;  // data 0x30
//	//delay_for();
//	while(!(SPI1->SR & 0x02));
//	latch_74595();
//	//Delay(1000);
//	for(ii=0;ii<1000;ii++)  // 
//		delay_for();
//	
//	SPI1->DR = 0x30;  // data 0x30
//	//delay_for();
//	while(!(SPI1->SR & 0x02));
//	latch_74595();
//	//Delay(1000);
//	for(ii=0;ii<1000;ii++)  // 2ms delay
//		delay_for();	
//}
//-----------------------------


//======================================
// 8bit mode
charLCD_set_inst(0x30);  // 0x30;
charLCD_set_inst(0x30);  // 0x30;
charLCD_set_inst(0x30);  // 0x30;
charLCD_set_inst(0x28);  // 0x28;
// 4bit mode
charLCD_set_inst(0x20);  // 0x28;
charLCD_set_inst(0x80);  // 0x28;

charLCD_set_inst(0x00);  // 0x06;
charLCD_set_inst(0x60);  // 0x06;

charLCD_set_inst(0x00);  // 0x0C;
charLCD_set_inst(0xC0);  // 0x0C;

charLCD_set_inst(0x80);  // 0x80;
charLCD_set_inst(0x00);  // 0x80;

charLCD_set_inst(0x00);  // 0x01;
charLCD_set_inst(0x10);  // 0x01;
for(i=0;i<5;i++)  // 2ms delay
	delay_for();
	
	
	while(1){
		charLCD_set_data(0x30);  // 0x31;
		charLCD_set_data(0x10);  // 0x31;
		for(ii=0;ii<1000;ii++)  // 2ms delay
			delay_for();
	}
//--------------------------------------






//while(1){
//	SPI1->DR = 0xaaaa;

//	// 74595 latch enable
//	delay_for();
//	GPIOC->ODR = 0xff;
//	delay_for();
//	GPIOC->ODR = 0x00;
//	
//	for(ii=0;ii<300000;ii++)
//		delay_for();

////---------

//	SPI1->DR = 0x5555;

//	// 74595 latch enable
//	delay_for();
//	GPIOC->ODR = 0xff;
//	delay_for();
//	GPIOC->ODR = 0x00;
//	
//	for(ii=0;ii<300000;ii++)
//		delay_for();
//		
//}
//--------------------------------


// 74595
//RCC->APB2ENR |= 0x04;  // PORTA clock enable
//GPIOC->CRL = 0x33333333;

	//set_74595(0x23ff);
	
	for(;;) {
		
//		
//		ledLight1 = ~ledLight1;
//		if( ledLight1 )
//				GPIOA->ODR &= ~0x20;                           // switch on LED
//		else
//				GPIOA->ODR |=  0x20; 		
//			
//		Delay(1000);	
		
//----------------------------------		
		
//		c_micros = micros_10us();
//		if(c_micros - p_micros > 250)  // 2.5 ms millis
//		{
//			p_micros = c_micros;	
//			
//			//-----------------
//			//[1] all row off 
//			GPIOB->ODR = 0x0000;  // all row off
//			//-----------------
//			//[2] setting one column value
//			for(i=0;i<8;i++){	
//				if( !(num[row_num] & (0x80>>i))){
//					reg_data |= 0x01 << i;
//				}
//			}
//			GPIOC->ODR = reg_data;
//			
//			//-----------------
//			//[3] one row on
//			GPIOB->ODR = 0x0100 << row_num;	
//			
//			//-----------------
//			//[4] row number change
//			row_num++;
//			if(row_num == 8) row_num = 0;
//			
//		}
		
		
//----------------------------------

			
//		c_micros = micros_10us();
//		if(c_micros - p_micros > 10)
//		{
//			p_micros = c_micros;

//			count_pwm++;
//			if(count_pwm == 100){
//				count_pwm = 0;
//				GPIOA->ODR |=  0x20;
//			}
//			else if(count_pwm == pwm_duty){
//				GPIOA->ODR &= ~0x20;
//			}
			
//			ledLight1 = ~ledLight1;
//			if( ledLight1 )
//					GPIOA->ODR &= ~0x20;                           // switch on LED
//			else
//					GPIOA->ODR |=  0x20; 
			
//			pwm_var+=20;
//			if(pwm_var >1800) pwm_var = 20;
//			TIM2->CCR1 = pwm_var;
			
		}

//----------------------------------		
		
//		if(c_micros - p_micros1 > 1000)
//		{		
//				p_micros1 = c_micros;
//			
//				pwm_duty++;
//				if(pwm_duty == 99 ) pwm_duty = 0;
//		}
		
		
//----------------------------------
		
//  	}		
		

  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
