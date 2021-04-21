/*********************************************************************************************************
*
* File                : PS2.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM Version:4.14
* Version             : V1.0
* By                  : Xiao xian hui
*
*                                  (c) Copyright 2005-2010, WaveShare
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#ifndef _PS2_H
#define _PS2_H

#include "stm32f4xx.h"
/*PS2_DATA->PF10    PS2_CLK->PF11*/

#define PS2_DATA_PORT		GPIOC
#define PS2_DATA_PIN		GPIO_Pin_7
#define PS2_DATA_RCC		RCC_AHB1Periph_GPIOC

#define PS2_CLK_PORT		GPIOC
#define PS2_CLK_PIN			GPIO_Pin_6
#define PS2_CLK_RCC			RCC_AHB1Periph_GPIOC

#define SET_DATA			GPIO_SetBits(PS2_DATA_PORT,PS2_DATA_PIN)
#define CLR_DATA			GPIO_ResetBits(PS2_DATA_PORT,PS2_DATA_PIN)
#define GET_DATA			GPIO_ReadInputDataBit(PS2_DATA_PORT,PS2_DATA_PIN)

#define SET_CLK				GPIO_SetBits(PS2_CLK_PORT,PS2_CLK_PIN)
#define CLR_CLK				GPIO_ResetBits(PS2_CLK_PORT,PS2_CLK_PIN)
#define GET_CLK				GPIO_ReadInputDataBit(PS2_CLK_PORT,PS2_CLK_PIN)

void PS2_OUT_SDA(void);
void PS2_IN_SDA(void);
void PS2_OUT_SCK(void);
void PS2_IN_SCK(void);
void PS2_Init(void);
u8 PS2_Test(void);
u8 keyHandle(u8 val);

#endif /*_PS2_H*/
