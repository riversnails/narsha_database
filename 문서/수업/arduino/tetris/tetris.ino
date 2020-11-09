#include "tiger_128_96_16bit.h"
#include "num_2_16bit.h"
#include "num_3_16bit.h"
#include "font_2.h"

#define RED     0xf800
#define BLUE    0x001f
#define GREEN   0x7E00
#define WHITE   0xffff
#define BLACK   0x0000

#define OLED_CS     4
#define OLED_RST    3
#define OLED_DC     5
// uno
//#define OLED_SCK    13
//#define OLED_DATA   11
// mega
#define OLED_SCK    52
#define OLED_DATA   51

int x = 0;
int y = 0;

char block_L[4][4] = {
  {0, 0, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0}
};

char pixel_offset_x = 1;
char pixel_offset_y = 1;

void setup()
{
  Serial.begin(9600);
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
  Clear_Screen(0xffff);
  //Draw_Bitmap();

  /*
    for (int j = 0; j < 10; j++)  {
      for (int i = 0; i < 128; i++)  {
        put_pixel(i, j, 0x001f);
      }
    }*/
  //font_write(100, 100, 0x001f, '2');
  //string_write(20, 100, 0x001f, "hello world");

  for (int i = 0; i < 21; i++)
  {
    draw_line_hori(0, 60, i * 6);
  }
  for (int i = 0; i < 11; i++)
  {
    draw_line_vert(0, 121, i * 6);
  }
  make_block(x, y);
}

int count = 0;
int key = 0;

void loop()
{
  if (count == 20)
  {
    count = 0;
    delete_block(x, y);
    y++;
    make_block(x, y);
  }

  if (Serial.available())
  {
    key = Serial.read();
    if (key == 'a')
    {
      delete_block(x, y);
      x--;
      make_block(x, y);
    }
    if (key == 'd')
    {
      delete_block(x, y);
      x++;
      make_block(x, y);
    }
    if (key == 's')
    {
      delete_block(x, y);
      y++;
      make_block(x, y);
    }
    if (key == 'w')
    {
      delete_block(x, y);
      y--;
      make_block(x, y);
    }
  }

  count++;
  delay(50);
}

unsigned short color_value(char r, char g, char b)
{
  char first_byte = 0;
  char second_byte = 0;

  first_byte |= (r & 0xf8) << 3;
  first_byte |= (g & 0xE0) >> 5;
  second_byte |= ((g & 0x1c) >> 2) << 5;
  second_byte |= (b & 0xf1) >> 3;
  return ((first_byte << 8) + second_byte);
}

void draw_line_hori(int x_start, int x_end, int y)
{
  for (int i = 0; i < x_end - x_start; i++)
  {
    put_pixel(x_start + i, y, color_value(200, 200, 200));
  }
}

void draw_line_vert(int y_start, int y_end, int x)
{
  for (int i = 0; i < y_end - y_start; i++)
  {
    put_pixel(x, y_start + i, color_value(200, 200, 200));
  }
}

void make_rect(char x, char y, unsigned short color)
{
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < 5; i++) {
      put_pixel(x + i, y + j, color);
    }
  }
}

void make_block(char x, char y)
{
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if (block_L[j][i] == 1) make_rect(pixel_offset_x + (x + i) * 6, pixel_offset_y + (y + j) * 6, RED);
    }
  }
}

void delete_block(char x, char y)
{
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if (block_L[j][i] == 1) make_rect(pixel_offset_x + (x + i) * 6, pixel_offset_x + (y + j) * 6, WHITE);
    }
  }
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

  for (int i = 0; i < str_len; i++) {
    font_write(x + i * 8, y, color, *(str + i));
  }

}
