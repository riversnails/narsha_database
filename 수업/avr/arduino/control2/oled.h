#ifndef OLED_H
#define OLED_H

#define OLED_CS     5
#define OLED_RST    4
#define OLED_DC     6
#define OLED_SCK    52
#define OLED_DATA   51

#define RED     0xf800
#define BLUE    0x001f
#define GREEN   0x7E00
#define YELLOW  0x5464
#define WHITE   0xffff
#define BLACK   0x0000

#include "Arduino.h"

void spi_init();
void shift_out(char data);
void Write_Command(char command);
void Write_Data(char data);
void oled_1351_init(void);
void Clear_Screen(unsigned short color);
void Draw_Bitmap(int ch);
void put_pixel(char x, char y, unsigned short color);
void put_pixel_two(char x, char y, char color1, char color2);
void make_rect(char xx, char yy, unsigned short color);
void font_write(char x, char y, int color, char font);
void string_write(char x, char y, int color, char *str);

#endif
