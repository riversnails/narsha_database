#ifndef PCF8574_H
#define PCF8574_H

#include "Arduino.h"

void pcf8574_lcd_init();
void pcf8574_lcd_backlight_on_off(char on_off);
void pcf8574_lcd_backlight_pwm(unsigned long c_millis, unsigned long c_micros, int pwm_cycle);
void pcf8574_lcd_inst_8bit(char inst);
void pcf8574_lcd_inst_4bit(char inst);
void pcf8574_lcd_inst_4bit_high(char inst_high);
void pcf8574_lcd_inst_4bit_low(char inst_low);
void pcf8574_lcd_data_4bit(char data);
void pcf8574_lcd_data_4bit_high(char data_high);
void pcf8574_lcd_data_4bit_low(char data_low);

#endif
