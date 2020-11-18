#ifndef OLED_H
#define OLED_H

#include "Arduino.h"

void init_oled_first();
void oled_on(double x, double y, double z, int e, int b, unsigned long c_millis);
void X_location_status(int pre_x, int cur_x);
void Y_location_status(int pre_y, int cur_y);
void Z_location_status(int pre_z, int cur_z);
void draw_X_location(int cur_x);
void draw_Y_location(int cur_y);
void draw_Z_location(int cur_z);
void nozzle_temp_status(int pre_N, int cur_N);
void bed_temp_status(int pre_B, int cur_B);
void draw_nozzle_temp(int temp);
void draw_bed_temp(int temp);
int exact_temp(int temp_value);
int temp_check(int temp_value);
void spi_init();
void spi_write(char data);
void Write_Command(char command);
void Write_Data(char data);
void oled_init();
void clear_screen(unsigned short color);
void put_pixel(char x, char y, unsigned short color);
void draw_bitmap();
void font_write(char x, char y, unsigned short color, char font);
void string_write(char x, char y, unsigned short color, char *str);

#endif
