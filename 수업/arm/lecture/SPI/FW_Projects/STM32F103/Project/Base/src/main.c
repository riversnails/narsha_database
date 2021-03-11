#include "stm32f10x.h"
#include "flash.h" 
#include "delay.h"
#include "usart.h"
#include "key.h"
//#include "spisoft.h"
//#include "spi.h"
#include "adc.h"
#include <stdio.h>

const u8 TEXT_Buffer[]={"MiniSTM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)
extern vu16 ADC2ConvertedValue;

void Init(void);

int main(void)
{
	u8 key;
	u8 i=0;
	u8 datatemp[SIZE];

	Init();
	
	while(1);  // by joshua
	
	//SPI_SOFT_CS_H;
	//SW_SPI_MOSI_H;
	//SW_SPI_CLK_L;
	
	//Delay(0xaFFFF);
  		 
	while(SPI_Flash_ReadID()!=FLASH_ID)//while(SPISoft_Flash_ReadID()!=FLASH_ID)//
	{	   
		i=SPI_Flash_ReadID();//i=SPISoft_Flash_ReadID();//
		printf("\n\r W25X16 ID:%d",i);
		Delay(0xaFFFF);
		Delay(0xaFFFF);
	}
	
	printf("\n\r W25X16 ID:%x", SPI_Flash_ReadID());//printf("\n\r W25X16 ID:%x", SPISoft_Flash_ReadID());//

	
	//while(1);
	
	
	while(1)
	{
		key=ReadKeyDown();
		if(key==KEY1)
		{
			printf("\n\rWrite W25X16 SPI FLASH");
			SPI_Flash_Write((u8*)TEXT_Buffer,1000,SIZE);//SPISoft_Flash_Write((u8*)TEXT_Buffer,1000,SIZE);//
			printf("\n\r Finsh!");
		}
		if(key==KEY2)
		{
 			printf("\n\rRead W25X16 SPI FLASH");
			SPI_Flash_Read(datatemp,1000,SIZE);//SPISoft_Flash_Read(datatemp,1000,SIZE);//
			printf("\n\rRead Data : %s", datatemp);
		}

		i++;
		Delay(0xFFFF);//Delay(0x00FF);//
		if(i>0&& i<100)//if(i>0&& i<100)
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_8);
			GPIO_ResetBits(GPIOD, GPIO_Pin_2);
		}
		else if(i >= 100 && i < 200)//else if(i >= 100 && i < 200)
		{
			
			GPIO_SetBits(GPIOA, GPIO_Pin_8);
			GPIO_SetBits(GPIOD, GPIO_Pin_2);
		}	
		//i = i % 200;	   
		if(i >= 200)
		{
			i = 0;
			//printf("\r\nADC Value : %d",ADC2ConvertedValue);//*3300/4096);
		}
	}  
}

void Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);//(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
	//GPIO_SetBits(GPIOF, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
	
	
	STM32_Shenzhou_COMInit();
	GPIO_KEY_Config();
	SPI_Flash_Init();//SPISoft_Init();//
	ADCIn_Init();
}
