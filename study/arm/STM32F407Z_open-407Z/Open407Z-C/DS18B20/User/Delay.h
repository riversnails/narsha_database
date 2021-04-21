#ifndef _TIMER_H
#define _TIMER_H

#include "stm32f4xx.h"

void Delay_TimerIRQ (void);
void TIM_Config(void);
void uDelay (const uint32_t msec);
void mDelay (const uint32_t msec);

#endif 
