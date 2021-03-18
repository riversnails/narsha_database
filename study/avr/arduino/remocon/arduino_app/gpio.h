#ifndef GPIO_H
#define GPIO_H

#include "Arduino.h"

void pin_mode(char pin_num, char direction);
void digital_write(char pin_num, char high_low);
int digital_read(char pin_num);

#endif
