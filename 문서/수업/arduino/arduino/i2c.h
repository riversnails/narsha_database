#ifndef I2C_H
#define I2C_H

#include "Arduino.h"

void i2c_init(char scl, char sca);
void i2c_start();
void i2c_stop();
void i2c_ack();
void i2c_8bit(char data);
void i2c_8bit_data_write(char addr, char data);

#endif
