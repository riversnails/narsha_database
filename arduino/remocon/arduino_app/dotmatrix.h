#ifndef DOTMATRIX_H
#define DOTMATRIX_H

#include "Arduino.h"

void dotmatrix_init(char *pin_num);
void set_line(char line_num, char value);
void set_num_delay(char *dot_num);
void set_num_millis(char *dot_num, unsigned long dot_curr_millis);
void dotmatrix_init_74595(char *pin_num);
void set_74595_value(char value);
void set_line_74595(char line_num, char value);
void set_num_74595_delay(char *dot_num);
void set_num_74595_millis(char *dot_num, unsigned long dot_curr_millis);

#endif
