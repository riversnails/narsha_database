/*

                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                 智林STM32开发板试验程序                   |
|                  Timer1 定时器中断实验                    |
|                 刘笑然 by Xiaoran Liu                     |
|                        2008.4.16                          |
|                                                           |
|           智林测控技术研究所 ZERO research group          |
|                      www.the0.net                         |
|                             Oooo                          |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)     

*/
/*----------------------------------------------------------*\
 |  引入相关芯片的头文件                                    |
\*----------------------------------------------------------*/
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization
#include "TFT018.h"

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
#define LED1             ( 1 << 13 )                // PB5: LED D2
/*----------------------------------------------------------*\
 | Timer1 Update Interrupt Handler                          |
\*----------------------------------------------------------*/
void TIM1_UP_IRQHandler (void) {

  if ((TIM1->SR & 0x0001) != 0) {                 // check interrupt source

	ledLight = ~ledLight;
	if( ledLight )
    	GPIOB->ODR &= ~LED;                           // switch on LED
	else
    	GPIOB->ODR |=  LED;                           // switch off LED

    TIM1->SR &= ~(1<<0);                          // clear UIF flag
 }
} // end TIM1_UP_IRQHandler

/*----------------------------------------------------------*\
                        |
\*----------------------------------------------------------*/
void TIM2_IRQHandler (void) {

  if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source

	ledLight2 = ~ledLight2;
	if( ledLight2 )
    	GPIOB->ODR &= ~LED1;                           // switch on LED
	else
    	GPIOB->ODR |=  LED1;                           // switch off LED

    TIM2->SR &= ~(1<<0);                          // clear UIF flag
 }
} // end TIM2_UP_IRQHandler


unsigned long micros_10us()
{
	
	return count;
}

void SysTick_Handler(void)
 {
//	 GPIOB->ODR =0x01; 
	 
//	ledLight1 = ~ledLight1;
//	if( ledLight1 )
//    	GPIOC->ODR &= ~(0x01 << 13);                           // switch on LED
//	else
//    	GPIOC->ODR |=  (0x01 << 13); 				
		
	// 		GPIOB->ODR =0x00;	
	
//GPIOB->ODR =0x01; 

	 
	count++;
	 
	 
	 
	 
	//GPIOB->ODR =0x00;
	 
	 
	 
	 
	 
	 
	 
	 
	 
//	if(count== 1000){
//		count=0;
//		ledLight3 = ~ledLight3;
//		if( ledLight3 )
//	    	GPIOB->ODR &= ~(0x01 << 4);                           // switch on LED
//		else
//	    	GPIOB->ODR |=  (0x01 << 4); 
//	}
			
		                          // switch off LED
 }

 unsigned long c=0;
 
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
	
	unsigned long c_micro=0;
		unsigned long p_micro=0;
		unsigned long p_micro1=0;
	
  stm32_Init ();                                // STM32 setup
	
	
//  LCD_Init();
//  LCD_Clear_Screen(Blue);

//	GPIOA->CRH &= ~(0x0f << 20);  // GPIOA.13
//	GPIOA->CRH |= (0x03 << 20);	
	
//	SysTick->LOAD = ....;
//	SysTick->CTRL = ....;
	
  for(;;) {
		
		c++;
		
//		Delay(100);
//		
//		GPIOA->ODR |= (0x01 << 13);
//		
//			Delay(100);
//		
//		GPIOA->ODR &= ~(0x01 << 13);

		
//		
//		c_micro = micros_10us();
//		
//		if(c_micro - p_micro > 100){
//				p_micro = c_micro;
//	
//				ledLight1 = ~ledLight1;
//				if( ledLight1 )
//						GPIOC->ODR &= ~(1<<13);                           // switch on LED
//				else
//						GPIOC->ODR |=  (1<<13); 	
//		
//		}
//		
//		if(c_micro - p_micro1 > 1000){
//				p_micro1 = c_micro;
//	
//				ledLight2 = ~ledLight2;
//				if( ledLight2 )
//						GPIOC->ODR &= ~(1<<12);                           // switch on LED
//				else
//						GPIOC->ODR |=  (1<<12); 	
//		
//		}		
//		
 	}
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
