/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "usart.h"
#include "PS2.h"

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

	//LED1 (PC9)  LED2 (PC10)  LED3 (PC11)  LED4 (PC12)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);	

}



int main(void)
{
    uint8_t count=0;
	uint8_t data=0;

	GPIO_Configuration();	
	USART_Configuration();
	USART_NVIC_Config();

	printf("\r\nWelcome to WaveShare STM32F4 series MCU Board Open407V\r\n");
	printf("\r\n*****************Now the PS2 experiments**************\r\n");
	printf("\r\nPress the button on the PS2:\r\n");
	
	PS2_Init();
    while (1)
    {
		data = PS2_Test();
		if(data!=0xff)
		{
			printf("Keyboard Input : %c\r\n",data);
	        if(count>=2)
	        {
				count=0;
				/*====LED-OFF=======*/ 
				GPIO_ResetBits(GPIOF , GPIO_Pin_6);
				GPIO_ResetBits(GPIOF , GPIO_Pin_7);
				GPIO_ResetBits(GPIOF , GPIO_Pin_8);
				GPIO_ResetBits(GPIOF , GPIO_Pin_9);
	        }
	        else
	        {
				/*====LED-ON=======*/
				GPIO_SetBits(GPIOF , GPIO_Pin_6);
				GPIO_SetBits(GPIOF , GPIO_Pin_7);
				GPIO_SetBits(GPIOF , GPIO_Pin_8);
				GPIO_SetBits(GPIOF , GPIO_Pin_9);
	        }
		}
	}

}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
