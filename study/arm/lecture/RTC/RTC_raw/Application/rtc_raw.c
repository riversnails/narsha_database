
#include <stdio.h>

#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 5 )              // PB5: LED D2
#define LED1             ( 1 << 7 )                // PB5: LED D2
/*----------------------------------------------------------*\
 | SOFTWARE DATA                                            |
\*----------------------------------------------------------*/

unsigned short  analog [3];

int ledLight = 0;


/*----------------------------------------------------------*\
 | SendChar                                                 |
 | Write character to Serial Port.                          |
\*----------------------------------------------------------*/
int SendChar (int ch)  {

  while (!(USART1->SR & USART_FLAG_TXE));
  USART1->DR = (ch & 0x1FF);

  return (ch);
}
/*----------------------------------------------------------*\
 | GetKey                                                   |
 | Read character to Serial Port.                           |
\*----------------------------------------------------------*/
int GetKey (void)  {

  while (!(USART1->SR & USART_FLAG_RXNE));

  return ((int)(USART1->DR & 0x1FF));
}


void USART1_IRQHandler (void) {
  volatile unsigned int IIR;
  char receive_char;

    IIR = USART1->SR;
    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
      USART1->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
	receive_char =  (USART1->DR & 0x1FF);
//	printf("received data is %c..\n\r",receive_char);
	printf("uart 1 : %c",receive_char);

    }
 /*
    if (IIR & USART_FLAG_TXE) {
      USART1->SR &= ~USART_FLAG_TXE;	          // clear interrupt
	//USART1->DR = '5';
	printf("sent data is %c..\n\r",USART1->DR);
    }
 */
}


void USART2_IRQHandler (void) {
  volatile unsigned int IIR;
  char receive_char;

    IIR = USART2->SR;
    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
      USART2->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
	receive_char =  (USART2->DR & 0x1FF);
//	printf("received data is %c..\n\r",receive_char);
	printf("uart2 : %c",receive_char);

    }
}


void EXTI0_IRQHandler(void)
{
  if (EXTI->PR & (1<<0)) {                        
	printf("in exti0 irq handler...\r\n");
    EXTI->PR |= (1<<0);                           // clear pending interrupt
  }
}

void EXTI1_IRQHandler(void)
{
  if (EXTI->PR & (1<<1)) {                        
	printf("in exti0 irq handler...\r\n");
	PWR_EnterSTANDBYMode();

    EXTI->PR |= (1<<1);                           // clear pending interrupt
  }
}


void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }

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

  printf("%2d : %2d : %2d \r",THH, TMM, TSS);
  }

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  	char s[20];
	int i;
	
	stm32_Init ();                                // STM32 setup


	for(;;) {
		DisplayTime();	
	}

  }
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
