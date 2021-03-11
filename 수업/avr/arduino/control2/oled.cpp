#include "oled.h"
#include "main1.h"
#include "main2.h"
#include "font_2.h"


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

void Clear_Screen(unsigned short color)
{
  char first_byte = (color & 0xff00) >> 8;
  char second_byte = color & 0x00ff;
  Write_Command(0x15); // Column
  Write_Data(0x00);
  Write_Data(0x7f);

  Write_Command(0x75); // Row
  Write_Data(0x00);
  Write_Data(0x7f);

  Write_Command(0x5C); // Write Ram

  for (int j = 0; j < 128; j++)  {
    for (int i = 0; i < 128; i++)  {
      Write_Data(first_byte);
      Write_Data(second_byte); // one pixel == 2byte
    }
  }
}

void Draw_Bitmap(int ch)
{
  Write_Command(0x15); // Column
  Write_Data(0x00);
  Write_Data(127);

  Write_Command(0x75); // Row
  Write_Data(0x00);
  Write_Data(127);

  Write_Command(0x5C); // Write Ram

  if (ch == 0)
  {
    for (int j = 0; j < 128; j++)  {
      for (int i = 0; i < 128; i++)  {
        if (j < 64)
        {
          Write_Data(pgm_read_byte(&main1[0x46 + (i * 2 + j * 128 * 2) + 1]));
          Write_Data(pgm_read_byte(&main1[0x46 + (i * 2 + j * 128 * 2)]));
        }
        else
        {
          Write_Data(pgm_read_byte(&main2[0x46 + (i * 2 + (j - 64) * 128 * 2) + 1]));
          Write_Data(pgm_read_byte(&main2[0x46 + (i * 2 + (j - 64) * 128 * 2)]));
        }
      }
    }
  }
  else if(ch == 1)
  {
    for (int j = 0; j < 128; j++)  {
      for (int i = 0; i < 128; i++)  {
        if (j <= 32) put_pixel(i, j, RED);
        else if(j <= 64) put_pixel(i, j, BLUE);
        else if(j <= 96) put_pixel(i, j, GREEN);
        else if(j <= 128) put_pixel(i, j, YELLOW);
      }
    }
    string_write(20, 10, BLACK, "variable");
    string_write(20, 40, BLACK, "left");
    string_write(20, 70, BLACK, "left");
    string_write(20, 100, BLACK, "back");
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

void put_pixel_two(char x, char y, char color1, char color2)
{
  char first_byte = color1;
  char second_byte = color2;
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

void make_rect(char xx, char yy, unsigned short color)
{
  for (int j = 0; j < 10; j++) {
    for (int i = 0; i < 10; i++) {
      put_pixel(xx + i, yy + j, color);
    }
  }
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

  for (int i = 0; i < str_len; i++) {
    font_write(x + i * 8, y, color, *(str + i));
  }
}
