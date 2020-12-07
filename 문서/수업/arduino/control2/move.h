#ifndef MOVE_H
#define MOVE_H

#include "Arduino.h"

char move_print(char loc, char arr[][2]);
unsigned short adc_read(char channel);
void pointor_reload();

#endif
