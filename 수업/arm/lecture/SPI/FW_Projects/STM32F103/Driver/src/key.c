#include "stm32f10x.h"
#include <STM32F10X_GPIO.h>
#include <STM32F10X_RCC.h>
#include "key.h"

u8 ReadKeyDown(void)
{
	/* 1 key is pressed */
	if(!GPIO_ReadInputDataBit(GPIO_KEY_PORT, GPIO_KEY1))
	{
		while(!GPIO_ReadInputDataBit(GPIO_KEY_PORT, GPIO_KEY1))
		{
			;//reserved
		}
		return KEY1; 
	}	
	/* 2 key is pressed */
	if(!GPIO_ReadInputDataBit(GPIO_KEY_PORT, GPIO_KEY2))
	{
		while(!GPIO_ReadInputDataBit(GPIO_KEY_PORT, GPIO_KEY2))
		{
			;//reserved
		}
		return KEY2; 
	}

	/* No key is pressed */
	else 
	{
		return NOKEY;
	}
}

void GPIO_KEY_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure KEY Button */
	RCC_APB2PeriphClockCmd(RCC_KEY, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_KEY1;
	GPIO_Init(GPIO_KEY_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_KEY2;
	GPIO_Init(GPIO_KEY_PORT, &GPIO_InitStructure);
}
