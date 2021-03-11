#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
//#include "usart.h"
#include <stdio.h>

vu16 ADC2ConvertedValue;

void ADCIn_Init(void)//PC0/ADC10, ADC12_IN10
{
	NVIC_InitTypeDef NVIC_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//DISABLE;//
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigInjecConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_28Cycles5);

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	
	ADC_Cmd(ADC1, ENABLE);

	
  /* Enable ADC2 reset calibaration register */   
  ///ADC_ResetCalibration(ADC2);
  /* Check the end of ADC2 reset calibration register */
  ///while(ADC_GetResetCalibrationStatus(ADC2));
  /* Start ADC2 calibaration */
  ///ADC_StartCalibration(ADC2);
  /* Check the end of ADC2 calibration */
  ///while(ADC_GetCalibrationStatus(ADC2));
/* Start ADC2 Software Conversion */ 
  ///ADC_SoftwareStartConvCmd(ADC2, ENABLE);


	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void ADC1_2_IRQHandler(void)
{
  /* Get injected channel13 converted value */
  ADC2ConvertedValue = ADC_GetConversionValue(ADC1);
	
	printf("\r\nADC Value : %d",ADC2ConvertedValue);

  /*if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) == 0x00)
      GPIO_SetBits(GPIOE, GPIO_Pin_13);
    else
      GPIO_ResetBits(GPIOE, GPIO_Pin_13);*/
}

