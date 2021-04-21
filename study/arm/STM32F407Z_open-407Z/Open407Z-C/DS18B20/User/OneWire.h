#ifndef _ONEWIRE_H
#define _ONEWIRE_H

#include "stm32f4xx.h"

#define DS18B20_Port	GPIOG
#define DS18B20_Pin		GPIO_Pin_8
#define DS18B20_PortRCC	RCC_AHB1Periph_GPIOG	
#define DS18B20_RCC_AHB	RCC_AHB1PeriphClockCmd		

#define SET_DQ() 		GPIO_SetBits(DS18B20_Port,DS18B20_Pin)
#define CLR_DQ() 		GPIO_ResetBits(DS18B20_Port,DS18B20_Pin)

#define GET_DQ() 		GPIO_ReadInputDataBit(DS18B20_Port,DS18B20_Pin)

void Onewire_Enable_GPIO_Port(void);
void Onewire_OUT_PULL_UP(void);
void Onewire_OUT_FLOATING(void);
void Onewire_IN_FLOATING(void);
void Onewire_IN_PULL_UP(void);
void _delay_us(u8 us);
void resetOnewire(void) ;
u8 rOnewire(void);
void wOnewire(u8 data);

#endif /*_ONEWIRE_H*/
