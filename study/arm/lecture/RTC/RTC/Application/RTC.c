
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization

int ledLight = 0;
int ledLight1 = 0;
int Alarm = 0;
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 6 )                // PB5: LED D2
#define LED1             ( 1 << 7 )                // PB5: LED D2
/*----------------------------------------------------------*\
 | RTC Interrupt Handler                                    |
\*----------------------------------------------------------*/
void RTC_IRQHandler(void) {
  	if (RTC->CRL & (1<<0) ) {                       // check second flag
	    RTC->CRL &= ~(1<<0);                          // clear second flag
	
		ledLight = ~ledLight;
		if( ledLight )
	    	GPIOB->ODR &= ~LED;                           // switch on LED
		else
	    	GPIOB->ODR |=  LED;                           // switch off LED
    }

   if (RTC->CRL & (1<<1) ) {                       // check alarm flag
	    RTC->CRL &= ~(1<<1);                          // clear alarm flag
	
    	GPIOB->ODR |=  LED1;                           // switch off LED
 	    GPIOB->ODR &= ~LED1;                           // switch on LED
   }

  } // end TIM1_UP_IRQHandler
/*----------------------------------------------------------*\
 | RTC Interrupt Handler                                    |
\*----------------------------------------------------------*/
void DisplayTime(void) {
  unsigned int TimeVar=0,  THH = 0, TMM = 0, TSS = 0;
  char s[30];

  TimeVar = RTC->CNTH << 16 | RTC->CNTL;
  /* Compute  hours */
  THH = TimeVar/3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600)/60;
  /* Compute seconds */
  TSS = (TimeVar % 3600)% 60;

  //printf("%2d : %2d : %2d \r",THH, TMM, TSS);
  }
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  stm32_Init ();                                // STM32 setup


  for(;;) {
  }
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
