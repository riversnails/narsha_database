#include <avr/pgmspace.h>
#include "font.h"
#include "background.h"
#include "main_menu.h"
//#include "move.h"
//#include "move_axis.h"
//#include "option.h"
//#include "temp.h"
#include "temptable_1.h"
#include<stdio.h>

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
#define YELLOW 0xffE0

#define NOZZLE_PIN A0
#define BED_PIN    A1

int current_nozzle_temp = 0;
int current_bed_temp = 0;

void setup() {
  Serial.begin(115200);
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
  draw_bitmap(background);

  //1의 자리수만
  //string_write(24, 54, WHITE, "0C");
  //string_write(87, 54, WHITE, "0C");

  //10의 자리수 까지
  //string_write(20, 54, WHITE, "00C");
  //

  //100의 자리수 까지
  //string_write(16, 54, WHITE, "000C");
  //string_write(79, 54, WHITE, "000C");

//  string_write(6, 75, WHITE, "X:");
//  string_write(6, 92, WHITE, "Y:");
//  string_write(6, 109, WHITE, "Z:");
}

int a = 95;

char nozzle_temp_buf[100];
char bed_temp_buf[100];
char X_location_buf[100];
char Y_location_buf[100];
char Z_location_buf[100];

void loop() 
{
  //int N_temp = analogRead(NOZZLE_PIN);
  //int B_temp = analogRead(BED_PIN);

  //temp_check(NOZZLE_PIN, N_temp);
  //temp_check(BED_PIN, B_temp);
  
//  draw_bitmap(background);
//  draw_bed_temp(a);
//  a++;

  draw_bitmap(background);
  draw_bed_temp(a);
  a++;
  delay(1000);
  //draw_bitmap(main_menu);
  //delay(1000);
}

void draw_bed_temp(int temp)
{
  if(a >= 100)
  {
    sprintf(bed_temp_buf, "%d", a);
    string_write(79, 54, WHITE, bed_temp_buf);
    string_write(110, 54, WHITE, "C");
  }
  else if(a >= 10)
  {
    sprintf(bed_temp_buf, "%d", a);
    string_write(83, 54, WHITE, bed_temp_buf);
    string_write(105, 54, WHITE, "C");
  }
  else if(a >= 0)
  {
    sprintf(bed_temp_buf, "%d", a);
    string_write(87, 54, WHITE, bed_temp_buf);
    string_write(100, 54, WHITE, "C");
  }
}

int exact_temp(int temp_value)
{
  int tmp1 = 0, tmp2 = 0, tmp3 = 0;
  
  for(int i = 0; i < (sizeof(temptable_1) / sizeof(temptable_1[0])); i++)
  {
    if(temptable_1[i][0] > temp_value && temptable_1[i-1][0] < temp_value)
      tmp1 = (temptable_1[i][0] - temptable_1[i-1][0]) / 5;
      tmp2 = temp_value - temptable_1[i-1][0];
      if(tmp1 == 0) tmp3 = 0;
      else tmp3 = tmp2 / tmp1;
      return tmp3;
  }
}

void temp_check(int PIN, int temp_value)
{ 
  int diff_temp = 0;
  if(PIN == NOZZLE_PIN)
  {
    for(int i = 0; i < (sizeof(temptable_1) / sizeof(temptable_1[0])); i++)
    {
      if(temp_value == temptable_1[i][0])
      {
        current_nozzle_temp = temptable_1[i][1];
      }
      else
      {
        diff_temp = exact_temp(temp_value);
        current_nozzle_temp = temptable_1[i-1][1] + diff_temp;
      }
    }
  }
  else if(PIN == BED_PIN)
  {
    for(int i = 0; i < (sizeof(temptable_1) / sizeof(temptable_1[0])); i++)
    {
      if(temp_value == temptable_1[i][0])
      {
        current_bed_temp = temptable_1[i][1];
      }
      else
      {
      current_bed_temp = exact_temp(temp_value);
      }
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
  while(!(SPSR & 0x80)); 
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

  for(int j = 0; j < 128; j++)
  {
    for(int i = 0; i < 128; i++)
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

void draw_bitmap(unsigned char *back)
{
  Write_Command(0x15);
  Write_Data(0); 
  Write_Data(127);

  Write_Command(0x75);
  Write_Data(0);
  Write_Data(127);

  Write_Command(0x5c);

  for(int j = 0; j < 128; j++)
  {
    for(int i = 0; i < 128; i++)
    {
      Serial.print(j);
      Serial.print(" ");
      Serial.println(i);
      Write_Data(pgm_read_byte(&back[0x46 + 1 + (i*2 + j*128*2)]));
      Write_Data(pgm_read_byte(&back[0x46 + (i*2 + j*128*2)]));
    }
  }
}

void font_write(char x, char y, unsigned short color, char font)
{
  for(int j = 0; j < 16; j++)
  {
    for(int i = 0; i < 8; i++)
    {
      if(ascii_8x16[font - 0x20][j] & (0x80 >> i))
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
  
  for(int i = 0; i < str_len; i++)
  {
    font_write(x + (i * 10), y, color, *(str + i));
  }
}

void draw_line_hori(int x_start, int x_end, int y, unsigned short color)
{
  for(int i = x_start; i < x_end - x_start; i++) put_pixel(x_start + i, y, color);
}

void draw_line_vert(int y_start, int y_end, int x, unsigned short color)
{
  for(int i = y_start; i < y_end - y_start; i++) put_pixel(x, y_start + i, color);
}

void draw_selection_box(char *bit_map, int joy_value)
{
  if(/*조이스틱 값에따라*/ joy_value == 0)
  {
    draw_line_hori(0, 127, 0, WHITE);
    draw_line_hori(0, 127, 1, WHITE);
    
    draw_line_hori(0, 127, 30, WHITE);
    draw_line_hori(0, 127, 31, WHITE);
  }
  else if(/*조이스틱 값에따라*/ joy_value == 1)
  {
    draw_line_hori(0, 127, 32, WHITE);
    draw_line_hori(0, 127, 33, WHITE);
    
    draw_line_hori(0, 127, 62, WHITE);
    draw_line_hori(0, 127, 63, WHITE);
  }
  else if(/*조이스틱 값에따라*/ joy_value == 2)
  {
    draw_line_hori(0, 127, 64, WHITE);
    draw_line_hori(0, 127, 65, WHITE);
  
    draw_line_hori(0, 127, 94, WHITE);
    draw_line_hori(0, 127, 95, WHITE);
  }
  else if(/*조이스틱 값에따라*/ joy_value == 3)
  {
    draw_line_hori(0, 127, 96, WHITE);
    draw_line_hori(0, 127, 97, WHITE);

    draw_line_hori(0, 127, 126, WHITE);
    draw_line_hori(0, 127, 127, WHITE);
  }
  
}
