#ifndef _USART_H
#define _USART_H

#include <stdio.h>
#include "stm32f4xx.h"

/**
 * @brief Definition for COM port1, connected to USART3
 */  
#define Open_USART                        USART2
	#define Open_USART                        USART2
	#define Open_USART_CLK                    RCC_APB1Periph_USART2
	
	#define Open_USART_TX_PIN                 GPIO_Pin_5
	#define Open_USART_TX_GPIO_PORT           GPIOD
	#define Open_USART_TX_GPIO_CLK            RCC_AHB1Periph_GPIOD
	#define Open_USART_TX_SOURCE              GPIO_PinSource5
	#define Open_USART_TX_AF                  GPIO_AF_USART2
	
	
	#define Open_USART_RX_PIN                 GPIO_Pin_6
	#define Open_USART_RX_GPIO_PORT           GPIOD
	#define Open_USART_RX_GPIO_CLK            RCC_AHB1Periph_GPIOD
	#define Open_USART_RX_SOURCE              GPIO_PinSource6
	#define Open_USART_RX_AF                  GPIO_AF_USART2
	
	#define Open_USART_IRQn                   USART2_IRQn

void USART_Configuration(void);
void USART_NVIC_Config(void);

#endif /*_USART_H*/
