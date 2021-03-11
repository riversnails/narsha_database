#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define ULTRASONIC_ECHO_PIN 2
#define ULTRASONIC_TRIGGER_PIN 10

#include "Arduino.h"

void ultrasonic_init();
void ultrasonic_trigger();
char ultrasonic_callback(float *distance);
void ultrasonic_callback_flag_clear();

#endif
