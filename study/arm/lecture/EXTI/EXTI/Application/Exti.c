
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization

int ledLight = 0;
int ledLight1 = 0;
int Alarm = 0;
int count=0;

#define __WFI                             __wfi

#define  PWR_CR_PDDS                         ((unsigned short)0x0002)     /*!< Power Down Deepsleep */
#define  PWR_CR_CWUF                         ((unsigned short)0x0004)     /*!< Clear Wakeup Flag */
#define  SCB_SCR_SLEEPDEEP                   ((unsigned char)0x04)               /*!< Sleep deep bit */

void PWR_EnterSTANDBYMode(void)
{
  /* Clear Wake-up flag */
  PWR->CR |= PWR_CR_CWUF;
  /* Select STANDBY mode */
  PWR->CR |= PWR_CR_PDDS;
  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR |= SCB_SCR_SLEEPDEEP;
/* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM   )
  __force_stores();
#endif
  /* Request Wait For Interrupt */
  __WFI();
}

/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
//#define LED             ( 1 << 5 )                // PB5: LED D2
#define LED             ( 1 << 6 )                // PB5: LED D2
/*----------------------------------------------------------*\
 | EXTI0 Interrupt Handler                                  |
\*----------------------------------------------------------*/
void EXTI0_IRQHandler(void)
{
  if (EXTI->PR & (1<<0)) {                        // EXTI0 interrupt pending?
		
		
		
    if ((ledLight ^=1) == 0) 
    	GPIOA->ODR &= ~(0x01 << 5);                           // switch on LED
    else
    	GPIOA->ODR |=  (0x01 << 5);                           // switch off LED
		

    EXTI->PR = (1<<0);                           // clear pending interrupt
  }
}

/*----------------------------------------------------------*\
 | EXTI1 Interrupt Handler                                  |
\*----------------------------------------------------------*/
void EXTI1_IRQHandler(void)
{
  if (EXTI->PR & (1<<1)) {                        // EXTI1 interrupt pending?
    if ((ledLight1 ^=1) == 0) 
    	GPIOA->ODR &= ~(0x01 << 8);                           // switch on LED
    else
    	GPIOA->ODR |=  (0x01 << 8);                           // switch off LED

    EXTI->PR = (1<<1);                           // clear pending interrupt
  }

	   count = 1;
}



/*----------------------------------------------------------*\
 | EXTI15..10 Interrupt Handler                             |
\*----------------------------------------------------------*/
void EXTI9_5_IRQHandler(void)
{
  if (EXTI->PR & (1<<5)) {                       // EXTI0 interrupt pending?
    if ((ledLight ^=1) == 0) 
    	GPIOC->ODR &= ~(0x01 <<8);                           // switch on LED
    else
    	GPIOC->ODR |=  (0x01 << 8);                           // switch off LED

    EXTI->PR = (1<<5);                          // clear pending interrupt
  }
	
	
	else if (EXTI->PR & (1<<6)) {                       // EXTI0 interrupt pending?
    if ((ledLight1 ^=1) == 0) 
    	GPIOB->ODR &= ~(0x01 << 7);                           // switch on LED
    else
    	GPIOB->ODR |=  (0x01 << 7);                           // switch off LED

    EXTI->PR = (1<<6);                          // clear pending interrupt
  }
	
	
	
}


/*----------------------------------------------------------*\
 | EXTI15..10 Interrupt Handler                             |
\*----------------------------------------------------------*/
void EXTI15_10_IRQHandler(void)
{
  if (EXTI->PR & (1<<13)) {                       // EXTI0 interrupt pending?
    if ((ledLight ^=1) == 0) 
    	GPIOA->ODR &= ~(0x01 << 5);                           // switch on LED
    else
    	GPIOA->ODR |=  (0x01 << 5);                           // switch off LED

    EXTI->PR = (1<<13);                          // clear pending interrupt
  }
}

void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }

	

/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
	
	int a;
	

  stm32_Init ();                                // STM32 setup

//while(1);
	
	//////////////// input : PA1, output:PA8 ///////////////////////////////////
//	*(volatile unsigned int *)(0x40021018) |= 0x04; // APB2ENR
//	*(volatile unsigned int *)(0x40010800) = 0x40; // GPIOA->CRL 
//		*(volatile unsigned int *)(0x40010804) = 0x03;  // GPIOA->CRH
//	
//		*(volatile unsigned int *)(0x4001040c) = 0x02;	// EXTI->FTSR
//			*(volatile unsigned int *)(0x40010400) = 0x02;	// EXTI->IMR
//	
//			*(volatile unsigned int *)(0xE000E100) |= 0x80;	// NVIC->ISER0
	///////////////////////////////////////////////////////////////////////
	

	*(volatile unsigned int *)(0x40021018) |= 0x1C; // APB2ENR
		*(volatile unsigned int *)(0x40010800) = (0x04 << 20) ;  //0x00400000; // GPIOA.5->CRL 
	//*(volatile unsigned int *)(0x40010800) |= (0x04 << 20) ;  //0x00400000; // GPIOA.5->CRL 
		*(volatile unsigned int *)(0x40011004) = 0x03;  // GPIOC.8->CRH
//		*(volatile unsigned int *)(0x40010804) = 0x03;  // GPIOA.8->CRH
	
		*(volatile unsigned int *)(0x4001040c) = 0x20;	// EXTI->FTSR
			*(volatile unsigned int *)(0x40010400) = 0x20;	// EXTI->IMR
	
			*(volatile unsigned int *)(0xE000E100) |= 0x00800000;	// NVIC->ISER0.23	
	
	
	
// Interrupt Group Priority
//SCB->AIRCR |=0x05FA0300; 
// =>  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup; 

//NVIC->IPR[1] |=0x00;

// Interrupt enable
//NVIC->ISER[0]=0x00;

//Port enable
//RCC->APB2ENR |= 0x00;

// Port Select
//AFIO->EXTICR[0]=0x00;

// External Interrupt
//EXTI->IMR = 0x00;
//EXTI->RTSR      |= 0x00;
//EXTI->FTSR      |= 0x00;


	for(;;) {
		
//		a= GPIOA->IDR;
//		
//		
//		if(a& 0x01)
//			 GPIOA->ODR |= (0x01 << 8); 
//		else
//			GPIOA->ODR &= ~(0x01 << 8); 
//		
//		
//		
//#if 0 
//      GPIOB->ODR &= ~(0x01 << 5);                           // switch on LED
//    GPIOB->ODR &= ~(0x01 << 8);                          // switch on LED
//    GPIOB->ODR |= (0x01 << 9);                         // switch on LED
//	Delay(2000000);
//    GPIOB->ODR |=  (0x01 << 5);                           // switch off LED
//    GPIOB->ODR |=  (0x01 << 8);                           // switch on LED
//    GPIOB->ODR &=  ~(0x01 << 9);                            // switch on LED
//	Delay(2000000);

//	if(count == 1){
//		count = 0;
//  		//__WFI();  // Sleep Mode Test
//		PWR_EnterSTANDBYMode();	  // Standby Mode Test
//	 }

//#endif
  	}
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
