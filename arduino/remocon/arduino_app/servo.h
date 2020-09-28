#ifndef SERVO_H
#define SERVO_H

#include "Arduino.h"

void servo_init(char port);
void servo_position(unsigned char degree);

#endif
