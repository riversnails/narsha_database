#ifndef SERVO_H
#define SERVO_H

#define SERVO_PIN 0x20 // H

#include "Arduino.h"

void servo_init();
void servo_start();
void servo_stop();
void servo_change(char get_duty);

#endif
