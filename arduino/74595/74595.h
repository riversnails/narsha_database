#ifndef 74595_H
#define 74595_H

#include "Arduino.h"

void 74595_init(char clk_pin, char data_pin, char latch_pin);
void set_data_74595(char data);
void shift_out(char data);


#endif
