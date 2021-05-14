#include<SPI.h>
#include"tiger_128_128_16bit.h"

#define CLK 13
#define MOSI 11
#define RES 3
#define CS 10
#define DS 5

#define RED 0xf800
#define BLUE 0x001f
#define GREEN 0x7E00
#define YELLOW 0xFFE0
#define WHITE 0xffff
#define BLACK 0x0000

char pin_num[5] = {CLK, MOSI, RES, CS, DS};

void setup()
{
  while (!Serial);


  //  pinMode(CLK, OUTPUT);
  //  pinMode(MOSI, OUTPUT);
  //  pinMode(RES, OUTPUT);
  //  pinMode(CS, OUTPUT);
  //  pinMode(DS, OUTPUT);
  for (int i = 0; i < 5; i++)
  {
    pinMode(pin_num[i], OUTPUT);
    digitalWrite(pin_num[i], LOW);
  }
  digitalWrite(RES, HIGH);
  //----------------------------------

  //----------------------------------
  // Arduino due SPI
  SPI.begin(4);
  SPI.setClockDivider(4, 5);
  //----------------------------------

  oled_1351_init();
  clear_screen(BLACK);

  //  for (int j = 0; j < 127; j++)
  //  {
  //    for (int i = 0; i < 127; i++)
  //    {
  //      put_pixel(j, i, RED);
  //    }
  //  }

  draw_bitmap();
}

void loop()
{
  //clear_screen(BLACK);
  draw_bitmap();
  delay(7);
  //delay(16);
//  clear_screen(BLACK);
//  delay(16);
}

void shift_out(unsigned char data)
{
  for (int i = 0; i < 8; i++)
  {
    if (data & (0x80 >> i))
    {
      digitalWrite(MOSI, HIGH);
    }
    else
    {
      digitalWrite(MOSI, LOW);
    }
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
}

void oled_1351_command(unsigned char cmd)
{
  digitalWrite(CS, LOW);
  digitalWrite(DS, LOW);
  SPI.transfer(4, cmd);
  //shift_out(cmd);
  digitalWrite(CS, HIGH);
  digitalWrite(DS, HIGH);
}

void oled_1351_data(unsigned char data)
{
  digitalWrite(CS, LOW);
  SPI.transfer(4, data);
  //shift_out(data);
  digitalWrite(CS, HIGH);
}

void oled_1351_data_1byte(unsigned char data)
{
  SPI.transfer(4, data);
}

void oled_1351_init(void) {
  digitalWrite(CS, LOW);

  digitalWrite(RES, LOW);
  delay(100);
  digitalWrite(RES, HIGH);
  delay(100);

  oled_1351_command(0xfd);  // command lock
  oled_1351_data(0x12);
  oled_1351_command(0xfd);  // command lock
  oled_1351_data(0xB1);

  oled_1351_command(0xae);  // display off

  oled_1351_command(0xB3);
  oled_1351_data(0xF1);

  oled_1351_command(0xa0);  //set re-map & data format
  oled_1351_data(0x74); //Horizontal address increment

  oled_1351_command(0xCA);
  oled_1351_data(0x7F);

  oled_1351_command(0xa2);  //set display offset
  oled_1351_data(0x00);

  oled_1351_command(0xB5);
  oled_1351_data(0x00);

  oled_1351_command(0xAB);
  oled_1351_data(0x01);

  oled_1351_command(0xB1);
  oled_1351_data(0x32);

  oled_1351_command(0xBE);
  oled_1351_data(0x05);

  oled_1351_command(0xA6);

  oled_1351_command(0xC1);
  oled_1351_data(0xC8);
  oled_1351_data(0x80);
  oled_1351_data(0xC8);

  oled_1351_command(0xC7);
  oled_1351_data(0x0F);

  oled_1351_command(0xB4);
  oled_1351_data(0xA0);
  oled_1351_data(0xB5);
  oled_1351_data(0x55);

  oled_1351_command(0xB6);
  oled_1351_data(0x01);

  oled_1351_command(0xAF);
}

void put_pixel(char x, char y, unsigned short color)
{
  char first_byte = (color & 0xff00) >> 8;
  char second_byte = color & 0xff;

  oled_1351_command(0x15);
  oled_1351_data(x);
  oled_1351_data(x);

  oled_1351_command(0x75);
  oled_1351_data(y);
  oled_1351_data(y);

  oled_1351_command(0x5C);

  oled_1351_data(first_byte);
  oled_1351_data(second_byte);
}

void clear_screen(unsigned short color)
{
  char first_byte = (color >> 8) & 0xff;
  char second_byte = color & 0xff;

  oled_1351_command(0x15);
  oled_1351_data(0x00);
  oled_1351_data(0x7f);

  oled_1351_command(0x75);
  oled_1351_data(0x00);
  oled_1351_data(0x7f);

  oled_1351_command(0x5C);

  digitalWrite(CS, LOW);
  for (int j = 0; j < 128; j++)
  {
    for (int i = 0; i < 128; i++)
    {
      oled_1351_data_1byte(first_byte);
      oled_1351_data_1byte(second_byte);
    }
  }
  digitalWrite(CS, HIGH);
}

unsigned char first_byte;
unsigned char second_byte;
int ydiff = 0;
int xdiff = 0;

void draw_bitmap()
{
  oled_1351_command(0x15);
  oled_1351_data(0x00);
  oled_1351_data(127);

  oled_1351_command(0x75);
  oled_1351_data(0x00);
  oled_1351_data(127);

  oled_1351_command(0x5C);

  digitalWrite(CS, LOW);

  for (int j = 0; j < 128; j++)
  {
    for (int i = 0; i < 128; i++)
    {
      first_byte = tiger_128_128_16bit[0x46 + ((xdiff + i)*2 + 1) + ydiff*128*2];
      second_byte = tiger_128_128_16bit[0x46 + ((xdiff + i)*2 + 0) + ydiff*128*2];
      oled_1351_data_1byte(first_byte);
      oled_1351_data_1byte(second_byte);
      if(xdiff >= 127) xdiff = 0;
    }
    ydiff++;
    if(ydiff >= 127) ydiff = 0;
  }
  xdiff++;
  ydiff++;
  
  //  for (int j = 0; j < 42; j++)
  //  {
  //    for (int i = 0; i < 128; i++)
  //    {
  //      //      oled_1351_data(pgm_read_byte(&tiger_128_96_16bit_565[0x46 + 1 + i*2 + j*128*2]));
  //      //      oled_1351_data(pgm_read_byte(&tiger_128_96_16bit_565[0x46 + i*2 + j*128*2]));
  //      oled_1351_data_1byte(0xF8);
  //      oled_1351_data_1byte(0x00);
  //    }
  //  }
  //  for (int j = 0; j < 42; j++)
  //  {
  //    for (int i = 0; i < 128; i++)
  //    {
  //
  //      oled_1351_data_1byte(0x07);
  //      oled_1351_data_1byte(0xE0);
  //    }
  //  }
  //  for (int j = 0; j < 42; j++)
  //  {
  //    for (int i = 0; i < 128; i++)
  //    {
  //      oled_1351_data_1byte(0x00);
  //      oled_1351_data_1byte(0x1F);
  //    }
  //  }

  digitalWrite(CS, HIGH);
}
