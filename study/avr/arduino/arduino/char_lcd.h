#ifndef CHAR_LCD_H
#define CHAR_LCD_H

#include "Arduino.h"

void char_lcd_init_8bit(char *pin);
void char_lcd_inst_8bit(char inst);
void char_lcd_data_8bit(char data);
void char_lcd_init_4bit(char *pin);
void char_lcd_inst_4bit(char inst);
void char_lcd_inst_4bit_high(char inst_high);
void char_lcd_inst_4bit_low(char inst_low);
void char_lcd_data_4bit(char data);
void char_lcd_data_4bit_high(char data_high);
void char_lcd_data_4bit_low(char data_low);

#endif
