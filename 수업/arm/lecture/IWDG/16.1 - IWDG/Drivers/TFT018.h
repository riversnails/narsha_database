/*

                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                                                           |
|                         CPU.c                             |
|                     by Xiaoran Liu                        |
|                        2005.3.16                          |
|                                                           |
|                    ZERO research group                    |
|                        www.the0.net                       |
|                                                           |
|                            Oooo                           |
+----------------------oooO--(   )--------------------------+
                      (   )   ) /
                       \ (   (_/
                        \_)     

*/

#ifndef __TFT018_H
#define __TFT018_H
#include "MyType.h"

#define LCD_WIDTH	160
#define LCD_HIGH	128

extern volatile U8 Font;

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0


/*----------------------------------------------------------*\
 | BUS simulate                                             |
\*----------------------------------------------------------*/

/* ------ LOW LEVEL OPERATE ------ */
void LCD_Bus_Write_Byte( unsigned char Data );
void LCD_Reg_Set(unsigned int Cmd,unsigned int Data);


/*----------------------------------------------------------*\
 | HIGHLEVEL OPERATION                                      |
\*----------------------------------------------------------*/
void LCD_Init( void );
void LCD_SetArea( U8 x0, U8 y0, U8 x1, U8 y1 );

void LCD_Clear_Screen( unsigned int Data );
void LCD_Rectangle( unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned int Data );
void LCD_SetPixel(unsigned short x, unsigned short y, unsigned short color);

void LCD_PutChar(unsigned short x, unsigned short y, char c, unsigned int f, unsigned int b);
void LCD_PutString(unsigned short x, unsigned short y, char *s, unsigned int f, unsigned int b);

void PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int f,unsigned int b);

void LCD_BMP( U8 x0, U8 y0, U8 x1, U8 y1 , U8 * Bmp );

#endif

