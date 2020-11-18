#ifndef I2C_LCD_H
#define I2C_LCD_H

#include "Arduino.h"

void i2c_init();
void i2c_write(int e, int b, unsigned long i2c_c_millis);
int exact_temp(int temp_value);
int temp_check(int temp_value);

#endif
