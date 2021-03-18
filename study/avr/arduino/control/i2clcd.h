#ifndef I2CLCD_H
#define I2CLCD_H

#define PCF_8574_ADDR 0x27

#include "Arduino.h"

void i2c_start();
void i2c_8bit(char data);
void i2c_stop();
void i2c_ack();
void lcd_data_set(char data);
void i2c_init();

#endif
