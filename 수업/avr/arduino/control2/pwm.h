#ifndef PWM_H
#define PWM_H

#define PWM_PIN 0x40 // H

#include "Arduino.h"

void pwm_init();
void pwm_start();
void pwm_stop();
void pwm_change(char get_duty);

#endif
