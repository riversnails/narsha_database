#include "tiger_128_96_16bit.h"
#include "num_2_16bit.h"
#include "num_3_16bit.h"
#include "font_2.h"

#define OLED_CS     4
#define OLED_RST    3
#define OLED_DC     5
// uno
//#define OLED_SCK    13
//#define OLED_DATA   11
// mega
#define OLED_SCK    52
#define OLED_DATA   51

void setup()
{
  //pinMode(10, OUTPUT); // uno
  pinMode(53, OUTPUT); // mega

  pinMode(OLED_CS, OUTPUT);
  pinMode(OLED_RST, OUTPUT);
  pinMode(OLED_DC, OUTPUT);
  pinMode(OLED_SCK, OUTPUT);
  pinMode(OLED_DATA, OUTPUT);

  digitalWrite(OLED_SCK, LOW);

  spi_init();
  oled_1351_init();
  Clear_Screen();
  Draw_Bitmap();

  for (int j = 0; j < 10; j++)  {
    for (int i = 0; i < 128; i++)  {
      put_pixel(i, j, 0x001f);
    }
  }

  //font_write(100, 100, 0x001f, '2');
  string_write(20, 100, 0x001f, "hello world");
}

void loop()
{

}

void spi_init()
{
  SPCR |= 0x50;
  SPSR |= 0x01;
}

void spi_write(char data)
{
  SPDR = data;
  while (!(SPSR & 0x80));
}

void shift_out(char data)
{
  for (int i = 0; i < 8; i++)
  {
    if (data & (0x80 >> i))
    {
      digitalWrite(OLED_DATA, HIGH);
    }
    else
    {
      digitalWrite(OLED_DATA, LOW);
    }
    digitalWrite(OLED_SCK, HIGH);
    digitalWrite(OLED_SCK, LOW);
  }
}

void Write_Command(char command)
{
  digitalWrite(OLED_CS, LOW); // CS LOW
  digitalWrite(OLED_DC, LOW); // DC LOW
  //shift_out(command);
  spi_write(command);
  digitalWrite(OLED_DC, HIGH); // DC HIGH
  digitalWrite(OLED_CS, HIGH); // CS HIGH
}

void Write_Data(char data)
{
  digitalWrite(OLED_CS, LOW); // CS LOW
  //shift_out(data);
  spi_write(data);
  digitalWrite(OLED_CS, HIGH); // CS HIGH
}

void oled_1351_init(void)
{
  digitalWrite(OLED_CS, LOW);
  digitalWrite(OLED_RST, LOW);
  delay(10);
  digitalWrite(OLED_RST, HIGH);
  delay(10);

  Write_Command(0xfd);  // command lock
  Write_Data(0x12);
  Write_Command(0xfd);  // command lock
  Write_Data(0xB1);

  Write_Command(0xae);  // display off

  Write_Command(0xB3);
  Write_Data(0xF1);

  Write_Command(0xa0);  //set re-map & data format
  Write_Data(0x74); //Horizontal address increment

  Write_Command(0xCA);
  Write_Data(0x7F);

  Write_Command(0xa2);  //set display offset
  Write_Data(0x00);

  Write_Command(0xB5);
  Write_Data(0x00);

  Write_Command(0xAB);
  Write_Data(0x01);

  Write_Command(0xB1);
  Write_Data(0x32);

  Write_Command(0xBE);
  Write_Data(0x05);

  Write_Command(0xA6);

  Write_Command(0xC1);
  Write_Data(0xC8);
  Write_Data(0x80);
  Write_Data(0xC8);

  Write_Command(0xC7);
  Write_Data(0x0F);

  Write_Command(0xB4);
  Write_Data(0xA0);
  Write_Data(0xB5);
  Write_Data(0x55);

  Write_Command(0xB6);
  Write_Data(0x01);

  Write_Command(0xAF);
}

void Clear_Screen(void)
{
  Write_Command(0x15); // Column
  Write_Data(0x00);
  Write_Data(0x7f);

  Write_Command(0x75); // Row
  Write_Data(0x00);
  Write_Data(0x7f);

  Write_Command(0x5C); // Write Ram

  for (int j = 0; j < 128; j++)  {
    for (int i = 0; i < 128; i++)  {
      Write_Data(0x00);
      Write_Data(0x00); // one pixel == 2byte
    }
  }
}

void Draw_Bitmap(void)
{
  Write_Command(0x15); // Column
  Write_Data(0x00);
  Write_Data(127);

  Write_Command(0x75); // Row
  Write_Data(0x00);
  Write_Data(95);

  Write_Command(0x5C); // Write Ram

  for (int j = 0; j < 96; j++)  {
    for (int i = 0; i < 128; i++)  {
      Write_Data(pgm_read_byte(&tiger_128_96_16bit[0x46 + (i * 2 + j * 128 * 2) + 1]));
      Write_Data(pgm_read_byte(&tiger_128_96_16bit[0x46 + (i * 2 + j * 128 * 2)]));
    }
  }
}

void put_pixel(char x, char y, unsigned short color)
{
  char first_byte = (color & 0xff00) >> 8;
  char second_byte = color & 0x00ff;
  Write_Command(0x15); // Column
  Write_Data(x);
  Write_Data(x);

  Write_Command(0x75); // Row
  Write_Data(y);
  Write_Data(y);

  Write_Command(0x5C); // Write Ram

  Write_Data(first_byte);
  Write_Data(second_byte);
}

void font_write(char x, char y, int color, char font)
{
  for (int j = 0; j < 16; j++) {
    for (int i = 0; i < 8; i++)  {
      if (ascii_8x16[font - 0x20][j] & (0x80 >> i))
      {
        put_pixel(x + i, y + j, color);
      }
    }
  }
}

void string_write(char x, char y, int color, char *str)
{
  char font;
  int str_len = strlen(str);

  for (int i = 0; i < str_len; i++)
  {
    font_write(x + i * 8, y, color, *(str + i));
  }

}
