#include <avr/pgmspace.h>
#include "font.h"
#include "background.h"
#include "temptable_1.h"
#include "oled.h"
#include <stdio.h>

#define OLED_CS   6
#define OLED_RST  4
#define OLED_DC   5

#define OLED_SCK  52
#define OLED_DATA 51

#define RED    0xf800
#define GREEN  0x07e0
#define BLUE   0x001f
#define WHITE  0xffff
#define BLACK  0x0000

#define NOZZLE_PIN A13
#define BED_PIN    A14

int current_nozzle_temp = 0;
int current_bed_temp = 0;

int before_nozzle_temp = 0;
int before_bed_temp = 0;

double current_oled_x = 0;
double current_oled_y = 0;
double current_oled_z = 0;

int before_X_location = 0;
int before_Y_location = 0;
int before_Z_location = 0;

char nozzle_temp_buf[100];
char bed_temp_buf[100];

char X_location_buf[100];
char Y_location_buf[100];
char Z_location_buf[100];

void init_oled_first() {
  pinMode(53, OUTPUT); // Mega

  pinMode(OLED_RST, OUTPUT);
  pinMode(OLED_CS, OUTPUT);
  pinMode(OLED_DC, OUTPUT);
  pinMode(OLED_SCK, OUTPUT);
  pinMode(OLED_DATA, OUTPUT);

  digitalWrite(OLED_SCK, LOW);

  spi_init();
  oled_init();

  clear_screen(BLACK);

  draw_bitmap();
}

int oled_end_analog_value = 23;
int oled_bed_analog_value = 1008;
unsigned long p_oled_millis = 0;

void oled_on(double x, double y, double z, int e, int b, unsigned long c_oled_millis)
{
  if (c_oled_millis - p_oled_millis > 1500)
  {
    p_oled_millis = c_oled_millis;

    //이전값
    before_nozzle_temp = current_nozzle_temp;
    before_bed_temp = current_bed_temp;

    before_X_location = current_oled_x;
    before_Y_location = current_oled_y;
    before_Z_location = current_oled_z;
  
    /* 저항값 & 위치값 읽기*/
    /* analogRead 를 넣어주세요^^*/
    current_nozzle_temp = temp_check(oled_end_analog_value);
    current_bed_temp = temp_check(oled_bed_analog_value);

    oled_end_analog_value = e;
    oled_bed_analog_value = b;
    current_oled_x = x;
    current_oled_y = y;
    current_oled_z = z;
    
    // status 출력
    nozzle_temp_status(before_nozzle_temp, current_nozzle_temp);
    bed_temp_status(before_bed_temp, current_bed_temp);

    X_location_status(before_X_location, current_oled_x);
    Y_location_status(before_Y_location, current_oled_y);
    Z_location_status(before_Z_location, current_oled_z);
  }
}

void X_location_status(int pre_x, int cur_x)
{
  if (pre_x != cur_x)
  {
    for (int j = 75; j < 92; j++)
    {
      for (int i = 30; i < 60; i++)
      {
        put_pixel(i, j, BLACK);
      }
    }
  }
  draw_X_location(cur_x);
}

void Y_location_status(int pre_y, int cur_y)
{
  if (pre_y != cur_y)
  {
    for (int j = 92; j < 109; j++)
    {
      for (int i = 30; i < 60; i++)
      {
        put_pixel(i, j, BLACK);
      }
    }
  }
  draw_Y_location(cur_y);
}

void Z_location_status(int pre_z, int cur_z)
{
  if (pre_z != cur_z)
  {
    for (int j = 109; j < 124; j++)
    {
      for (int i = 30; i < 60; i++)
      {
        put_pixel(i, j, BLACK);
      }
    }
  }
  draw_Z_location(cur_z);
}

void draw_X_location(int cur_x)
{
  sprintf(X_location_buf, "%d", cur_x);
  string_write(30, 75, WHITE, X_location_buf);
}

void draw_Y_location(int cur_y)
{
  sprintf(Y_location_buf, "%d", cur_y);
  string_write(30, 92, WHITE, Y_location_buf);
}

void draw_Z_location(int cur_z)
{
  sprintf(Z_location_buf, "%d", cur_z);
  string_write(30, 109, WHITE, Z_location_buf);
}

void nozzle_temp_status(int pre_N, int cur_N)
{
  if (pre_N != cur_N)
  {
    for (int j = 16; j < 60; j++)
    {
      for (int i = 54; i < 70; i++)
      {
        put_pixel(j, i, BLACK);
      }
    }
  }
  draw_nozzle_temp(cur_N);
}

void bed_temp_status(int pre_B, int cur_B)
{
  if (pre_B != cur_B)
  {
    for (int j = 79; j < 120; j++)
    {
      for (int i = 54; i < 70; i++)
      {
        put_pixel(j, i, BLACK);
      }
    }
  }
  draw_bed_temp(cur_B);
}

void draw_nozzle_temp(int temp)
{
  if (temp >= 100)
  {
    sprintf(nozzle_temp_buf, "%d", temp);
    string_write(16, 54, WHITE, nozzle_temp_buf);
    string_write(47, 54, WHITE, "C");
  }
  else if (temp >= 10)
  {
    sprintf(nozzle_temp_buf, "%d", temp);
    string_write(20, 54, WHITE, nozzle_temp_buf);
    string_write(43, 54, WHITE, "C");
  }
  else if (temp >= 0)
  {
    sprintf(nozzle_temp_buf, "%d", temp);
    string_write(24, 54, WHITE, nozzle_temp_buf);
    string_write(38, 54, WHITE, "C");
  }
}

void draw_bed_temp(int temp)
{
  if (temp >= 100)
  {
    sprintf(bed_temp_buf, "%d", temp);
    string_write(79, 54, WHITE, bed_temp_buf);
    string_write(110, 54, WHITE, "C");
  }
  else if (temp >= 10)
  {
    sprintf(bed_temp_buf, "%d", temp);
    string_write(82, 54, WHITE, bed_temp_buf);
    string_write(105, 54, WHITE, "C");
  }
  else if (temp >= 0)
  {
    sprintf(bed_temp_buf, "%d", temp);
    string_write(86, 54, WHITE, bed_temp_buf);
    string_write(100, 54, WHITE, "C");
  }
}

int exact_temp(int temp_value)
{
  float tmp1 = 0;
  float tmp2 = 0;
  float tmp3 = 0;

  int row = sizeof(temptable_1) / sizeof(temptable_1[0]);

  for (int i = 1; i < row; i++)
  {
    if (temptable_1[i][0] > temp_value && temptable_1[i - 1][0] < temp_value)
    {
      tmp1 = (temptable_1[i][0] - temptable_1[i - 1][0]) / 5;
      tmp2 = temp_value - temptable_1[i - 1][0];
      tmp3 = ceil(tmp2 / tmp1);

      return (int)tmp3;
    }
  }
}

int temp_check(int temp_value)
{
  int diff_temp = 0;
  int real_temp = 0;
  int row = sizeof(temptable_1) / sizeof(temptable_1[0]);

  for (int i = 0 ; i < row; i++)
  {
    if (temp_value == temptable_1[i][0])
    {
      real_temp = temptable_1[i][1];
      return real_temp;
    }
    else if (temptable_1[i][0] > temp_value && temptable_1[i - 1][0] < temp_value)
    {
      diff_temp = exact_temp(temp_value);
      real_temp = temptable_1[i - 1][1] - diff_temp;
      return real_temp;
    }
  }
}

void spi_init()
{
  SPCR = 0x50;
  SPSR = 0x01;
}

void spi_write(char data)
{
  SPDR = data;
  while (!(SPSR & 0x80));
}

void Write_Command(char command)
{
  digitalWrite(OLED_CS, LOW);   // CS LOW
  digitalWrite(OLED_DC, LOW);   // DC LOW

  //shift_out(command);
  spi_write(command);

  digitalWrite(OLED_CS, HIGH);  // CS HIGH
  digitalWrite(OLED_DC, HIGH);  // DC HIGH
}


void Write_Data(char data)
{
  digitalWrite(OLED_CS, LOW);   // CS LOW
  //shift_out(data);
  spi_write(data);
  digitalWrite(OLED_CS, HIGH);  // CS HIGH
}

void oled_init()
{
  digitalWrite(OLED_CS, LOW);
  digitalWrite(OLED_RST, LOW);
  delay(100);
  digitalWrite(OLED_RST, HIGH);
  delay(100);

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

void clear_screen(unsigned short color)
{
  char first_byte = (color & 0xff00) >> 8;
  char second_byte = color & 0xff;
  Write_Command(0x15);
  Write_Data(0x00);
  Write_Data(0x7f);

  Write_Command(0x75);
  Write_Data(0x00);
  Write_Data(0x7f);

  Write_Command(0x5c);

  for (int j = 0; j < 128; j++)
  {
    for (int i = 0; i < 128; i++)
    {
      //Write_Data(0xF8); //RED
      Write_Data(first_byte);
      Write_Data(second_byte);
    }
  }

}

void put_pixel(char x, char y, unsigned short color)
{
  char first_byte = (color & 0xff00) >> 8;
  char second_byte = color & 0xff;

  //Column
  Write_Command(0x15);
  Write_Data(x);
  Write_Data(x);

  //Row
  Write_Command(0x75);
  Write_Data(y);
  Write_Data(y);

  Write_Command(0x5c);

  Write_Data(first_byte);
  Write_Data(second_byte);
}

void draw_bitmap()
{
  Write_Command(0x15);
  Write_Data(0x00);
  Write_Data(0x7f);

  Write_Command(0x75);
  Write_Data(0x00);
  Write_Data(0x7f);

  Write_Command(0x5c);

  for (int j = 0; j < 128; j++)
  {
    for (int i = 0; i < 128; i++)
    {
      Write_Data(pgm_read_byte(&background[0x46 + 1 + i * 2 + j * 128 * 2]));
      Write_Data(pgm_read_byte(&background[0x46 + i * 2 + j * 128 * 2]));
    }
  }
}

void font_write(char x, char y, unsigned short color, char font)
{
  for (int j = 0; j < 16; j++)
  {
    for (int i = 0; i < 8; i++)
    {
      if (ascii_8x16[font - 0x20][j] & (0x80 >> i))
      {
        put_pixel(x + i, y + j, color);
      }
    }
  }
}

void string_write(char x, char y, unsigned short color, char *str)
{
  char font;
  int str_len = strlen(str);

  for (int i = 0; i < str_len; i++)
  {
    font_write(x + (i * 10), y, color, *(str + i));
  }
}
