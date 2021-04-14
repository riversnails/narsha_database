#ifndef _KEY_LED_H
#define _KEY_LED_H

#include <stdio.h>
#include "stm32f4xx.h"

void JOYState_LED_GPIO_Init(void);
uint8_t Read_JOYState(void);
void Led_Toggle(uint8_t key);

#endif /*_KEY_LED_H*/
