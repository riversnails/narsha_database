
#include <stdio.h>

#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 5 )              // PB5: LED D2
/*----------------------------------------------------------*\
 | SOFTWARE DATA                                            |
\*----------------------------------------------------------*/


unsigned short  analog [3];


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


void adc_Init (void) {

//  GPIOA->CRL &= ~0x0000000F;                   /* set PIN1 analog input (see stm32_Init.c) */

  RCC->AHBENR |= (1<<0);                       /* enable periperal clock for DMA       */

//  DMA1_Channel1->CMAR  = (unsigned long)&analog; /* set chn1 memory address     */
  DMA1_Channel1->CMAR  = (unsigned long)analog; /* set chn1 memory address     */
  DMA1_Channel1->CPAR  = (unsigned long)&(ADC1->DR);     /* set chn1 peripheral address */
  DMA1_Channel1->CNDTR = 3;                     /* transmit 3 words                     */
//  DMA_Channel1->CCR   = 0x00002520;            /* configure DMA channel 1              */
  DMA1_Channel1->CCR   = 0x000025A0;            /* configure DMA channel 1              */
                                               /* circular mode, memory increment mode */
                                               /* memory & peripheral size 16bit       */
                                               /* channel priotity high                */
  DMA1_Channel1->CCR  |= (1 << 0);              /* enable DMA Channe1                   */

  RCC->APB2ENR |= (1<<9);                      /* enable periperal clock for ADC1      */

  ADC1->SQR1  = 0x00200000;                    /* three conversions                    */
  ADC1->SQR3  = (3<<10) | (2<<5) | (1<<0);     /* set order to chn1 - chn2 - chn3      */
  ADC1->SMPR2 = (5<< 9) | (5<<6) | (5<<3);     /* set sample time (55,5 cycles)        */ 

  ADC1->CR1   =  0x00000100;                   /* use independant mode, SCAN mode      */
  ADC1->CR2   =  0x000E0103;                   /* data align right, cont. conversion   */
                                               /* EXTSEL = SWSTART                     */ 
                                               /* enable ADC, DMA mode                 */
  ADC1->CR2  |=  0x00500000;                   /* start SW conversion                  */
  }


void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }

		
unsigned int *addr_var;
unsigned int test_var = 0x12345678;
	
	
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
	
int main (void) {
  	char s[20];
	int i;
	
	unsigned int *aa;
	unsigned short *bb;
	
	
	
	
	stm32_Init ();                                // STM32 setup

	addr_var = &test_var;
	
	
	
	while(1)
		{
			
		}
	
	
  //adc_Init();
	
	//*(volatile unsigned int *)(0x20002000) = 0x12345678;

//	aa = (unsigned int *)0x20002000;
//	bb = (unsigned short *)0x20002030;
//	
//	*aa=0x12345678;
//	
//	*(unsigned int *)bb =0xabcdef;
	
	for(;;) {
		printf("% 4.2fV \r\n", (float)(analog[0] * 3.3 / 0xFFF));
		//printf("% 4.2fV , p=%p\r\n", (float)(analog[0] * 3.3 / 0xFFF), &analog[0]);
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
