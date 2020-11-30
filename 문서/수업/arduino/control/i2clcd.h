#ifndef I2CLCD_H
#define I2CLCD_H

#include "Arduino.h"

void i2c_init();
void i2c_start();
void i2c_stop();
void data_8bit_set(char data);
void i2c_ack();
void i2c_byte_write(char addr, char data);

#endif
