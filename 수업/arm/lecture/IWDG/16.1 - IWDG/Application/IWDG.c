
#include <stdio.h>
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization
#include "TFT018.h"

int ledLight = 0;
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 6 )                // PB5: LED D2

void Delay(unsigned int nCount) {
  for(; nCount != 0; nCount--);
  }
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  int i;

  stm32_Init ();                                // STM32 setup


  if (RCC->CSR & (1<<29)) {                       // IWDG Reset Flag set
    RCC->CSR |= (1<<24);                          // Clear Reset Flags
    GPIOB->ODR |=  LED;                           // switch on LED
    }
  else {
    GPIOB->ODR &= ~LED;                           // switch off LED
    }

 /*
  for (i = 0; i < 2; i++) {
    Delay(1000000);                               // wait less than watchdog interval
    IWDG->KR  = 0xAAAA;                           // reload the watchdog
  }
*/
  GPIOB->ODR |=  LED;                           // switch on LED

  for(;;) {
 //    IWDG->KR  = 0xAAAA; 
  	}
  }
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
