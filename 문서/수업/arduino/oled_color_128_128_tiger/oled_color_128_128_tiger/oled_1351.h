#ifndef __OLED_1351_H
#define __OLED_1351_H

#include "Arduino.h"

#include "SPI.h"


#include "stdlib.h"


#include "num0_40_48_16bit.h"
//	#include "num1_40_48_16bit.h"
//	#include "num2_40_48_16bit.h"

#include "tiger_128_96_16bit_565.h"



#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define NUM_POSITION_OFFSET		4


#define NUM_IMAGE_WIDTH			40
#define NUM_IMAGE_HEIGHT		48

#define BITMAP_IMAGE_WIDTH		128
#define BITMAP_IMAGE_HEIGHT		96



#define SSD1351_WIDTH   128
#define SSD1351_HEIGHT  128

#define SSD1351_CMD_SETCOLUMN       0x15
#define SSD1351_CMD_SETROW          0x75
#define SSD1351_CMD_WRITERAM        0x5C
#define SSD1351_CMD_READRAM         0x5D
#define SSD1351_CMD_SETREMAP        0xA0
#define SSD1351_CMD_STARTLINE       0xA1
#define SSD1351_CMD_DISPLAYOFFSET 	0xA2
#define SSD1351_CMD_DISPLAYALLOFF 	0xA4
#define SSD1351_CMD_DISPLAYALLON  	0xA5
#define SSD1351_CMD_NORMALDISPLAY 	0xA6
#define SSD1351_CMD_INVERTDISPLAY 	0xA7
#define SSD1351_CMD_FUNCTIONSELECT 	0xAB
#define SSD1351_CMD_DISPLAYOFF      0xAE
#define SSD1351_CMD_DISPLAYON     	0xAF
#define SSD1351_CMD_PRECHARGE       0xB1
#define SSD1351_CMD_DISPLAYENHANCE  0xB2
#define SSD1351_CMD_CLOCKDIV        0xB3
#define SSD1351_CMD_SETVSL          0xB4
#define SSD1351_CMD_SETGPIO 		    0xB5
#define SSD1351_CMD_PRECHARGE2      0xB6
#define SSD1351_CMD_SETGRAY         0xB8
#define SSD1351_CMD_USELUT          0xB9
#define SSD1351_CMD_PRECHARGELEVEL 	0xBB
#define SSD1351_CMD_VCOMH           0xBE
#define SSD1351_CMD_CONTRASTABC     0xC1
#define SSD1351_CMD_CONTRASTMASTER  0xC7
#define SSD1351_CMD_MUXRATIO        0xCA
#define SSD1351_CMD_COMMANDLOCK     0xFD
#define SSD1351_CMD_HORIZSCROLL     0x96
#define SSD1351_CMD_STOPSCROLL      0x9E
#define SSD1351_CMD_STARTSCROLL     0x9F

// uno
  #define oled_cs   4
  #define oled_rst  3
  #define oled_dc   5
  #define oled_sck  13
  #define oled_din  11

// mega
//	#define oled_cs	4
//	#define oled_rst	3
//	#define oled_dc	5
//	#define oled_sck	52
//	#define oled_din	51



void OLED_CS(uint8_t x);
void OLED_RST(uint8_t x);
void OLED_DC(uint8_t x);
void OLED_SCK(uint8_t x);
void OLED_DIN(uint8_t x);
void Write_Command(uint8_t cmd) ;
void Write_Data(uint8_t dat) ;
void Write_Data_bulk(uint8_t dat) ;
void oled_1351_init(void) ;
void RAM_Address(void) ;
void Clear_Screen(void) ;
void Set_Address(uint8_t column, uint8_t row)  ;
void Put_Pixel(int16_t x, int16_t y, unsigned int color);
void Draw_Num(uint16_t x, uint16_t y, uint8_t num) ;
void Draw_Bitmap(uint16_t x, uint16_t y);
void Set_Init_Value();
void Set_Value(int num);



#endif
