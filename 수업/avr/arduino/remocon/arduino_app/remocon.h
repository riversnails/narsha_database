#ifndef REMOCON_H
#define REMOCON_H

#include "Arduino.h"

void remocon_init(char int_pin);
void remocon_proc();
void int_callback_flag_clear();
char remocon_callback(char *rem_num);
void remocon_ISR();

#endif
