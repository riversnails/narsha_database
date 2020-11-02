#ifndef _74595_H
#define _74595_H

#include "Arduino.h"

void init_74595(char clk_pin, char data_pin, char latch_pin);
void init_lcd_74595(char clk_pin, char data_pin, char latch_pin);
void set_data_74595(char data);
void shift_out(char data);
void pwm_74595(unsigned long c_millis, unsigned long c_micros, char pwm_pin, unsigned int cycle);
void lcd_inst_8bit_74595(char inst);
void lcd_inst_4bit_74595(char inst);
void lcd_inst_4bit_high_74595(char inst_high);
void lcd_inst_4bit_low_74595(char inst_low);
void lcd_data_4bit_74595(char data);
void lcd_data_4bit_high_74595(char data_high);
void lcd_data_4bit_low_74595(char data_low);
#endif
