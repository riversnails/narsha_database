#ifndef EXTI_H
#define EXTI_H

#include "Arduino.h"

#define EXTI_CHANGE 0x01
#define EXTI_FALLING 0x02
#define EXTI_RISING 0x03

void exti_init(char int_num, char mode, void (*isr_ptr)());

#endif
