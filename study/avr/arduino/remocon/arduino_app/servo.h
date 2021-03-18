#ifndef SERVO_H
#define SERVO_H

#include "Arduino.h"

void servo_init(char port);
void servo_init_many(char* port, int port_count);
void servo_position(unsigned char degree);
void servo_position_micros(unsigned char degree, unsigned long c_micros);
void servo_position_micros_many(unsigned char* degree, unsigned long c_micros);

#endif
