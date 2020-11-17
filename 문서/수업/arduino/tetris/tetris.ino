#include "tiger_128_96_16bit.h"
#include "num_2_16bit.h"
#include "num_3_16bit.h"
#include "font_2.h"

#define RED     0xf800
#define BLUE    0x001f
#define GREEN   0x7E00
#define YELLOW  0x5464
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

int x = 3;
int y = 0;
char overlapped = 0;
char rotate = 0;
char block_num = 1;
char background[22][12] = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

char block_L[7][4][4][4] = {
  {
    {
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 1, 0}
    },
    {
      {0, 0, 0, 0},
      {0, 1, 1, 1},
      {0, 1, 0, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 0, 1, 0},
      {0, 0, 1, 0}
    },
    {
      {0, 0, 0, 0},
      {0, 0, 0, 1},
      {0, 1, 1, 1},
      {0, 0, 0, 0}
    }
  },
  {
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0}
    },
    {
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0},
      {0, 0, 0, 0}
    }
  }
};

char pixel_offset_x = 1;
char pixel_offset_y = 1;

void setup()
{
  Serial.begin(115200);
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

  draw_grid();
  print_background();
  redraw_background();
  make_block(x, y);
}

int count = 0;
int key = 0;

void loop()
{
  if (count == 20)
  {
    //print_background();
    overlapped = overlap_check(x, y + 1);
    if (overlapped == 0)
    {
      delete_block(x, y);
      y++;
      make_block(x, y);
    }
    else
    {
      //Serial.printf("number of rect : %d", line_check(20));
      insert_block(x, y);

      for (int k = 20; k > 1; k--)
      {
        if (line_check(k))
        {
          for (int j = k - 1; j >= 1; j--) {
            for (int i = 0; i < 10; i++) {
              background[j + 1][i + 1] = background[j][i + 1];
            }
          }
          k++;
        }
      }
      print_background();
      redraw_background();

      x = 3;
      y = 0;
      rotate = 0;
    }
    count = 0;
  }

  if (Serial.available())
  {
    key = Serial.read();
    if (key == 'a')
    {
      overlapped = overlap_check(x - 1, y);
      if (overlapped == 0)
      {
        delete_block(x, y);
        x--;
        make_block(x, y);
      }
    }
    if (key == 'd')
    {
      overlapped = overlap_check(x + 1, y);
      if (overlapped == 0)
      {
        delete_block(x, y);
        x++;
        make_block(x, y);
      }
    }
    if (key == 's')
    {
      overlapped = overlap_check(x, y + 1);
      if (overlapped == 0) {
        delete_block(x, y);
        y++;
        make_block(x, y);
      }
    }
    if (key == 'w')
    { overlapped = overlap_check(x, y - 1);
      if (overlapped == 0) {
        delete_block(x, y);
        y--;
        make_block(x, y);
      }
    }
    if (key == 'r')
    {
      overlapped = overlap_check_rotate(x, y, rotate);
      if (overlapped == 0) {
        delete_block(x, y);
        rotate++;
        if (rotate == 4) rotate = 0;
        make_block(x, y);
      }
    }
    if (key == 't')
    {
      delete_block(x, y);
      while (1)
      {
        overlapped = overlap_check(x, y+1);
        if (overlapped == 0) {
          y++;
        }
        else break;
      }
      make_block(x, y);
      delay(100);
    }
  }

  count++;
  delay(50);
}

void draw_grid()
{
  for (int i = 0; i < 21; i++)
  {
    draw_line_hori(0, 60, i * 6);
  }
  for (int i = 0; i < 11; i++)
  {
    draw_line_vert(0, 121, i * 6);
  }
}

void redraw_background()
{
  for (int j = 0; j < 20; j++) {
    for (int i = 0; i < 10; i++) {
      if (background[j + 1][i + 1] > 0)
      {
        make_rect(pixel_offset_x + i * 6, pixel_offset_y + j * 6, check_color(background[j + 1][i + 1] - 1));
      }
      else
      {
        make_rect(pixel_offset_x + i * 6, pixel_offset_y + j * 6, WHITE);
      }
    }
  }
}

void print_background()
{
  for (int j = 0; j < 22; j++) {
    for (int i = 0; i < 12; i++) {
      Serial.printf("%d ", background[j][i]);
    }
    Serial.printf("\n");
  }
  Serial.printf("\n");
}

char line_check(char line_num)
{
  char line_full = 0;
  char count = 0;
  for (int i = 0; i < 10; i++) {
    if (background[line_num][i + 1] > 0)
    {
      count++;
    }
  }
  if (count == 10)
  {
    line_full = 1;
  }
  return line_full;
}

char insert_block(char x, char y)
{
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if (block_L[block_num][rotate][j][i] == 1)
      {
        background[j + y][i + x] = block_num + 1;
      }
    }
  }
}

char overlap_check_rotate(char x, char y, char over_rotate)
{
  char tmp = over_rotate;
  tmp++;
  if (tmp == 4) tmp = 0;
  char overlap_count = 0;
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if (block_L[block_num][tmp][j][i] == 1 && background[j + y][i + x] > 0)
      {
        overlap_count++;
      }
    }
  }
  return overlap_count;
}

char overlap_check(char x, char y)
{
  char overlap_count = 0;
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if (block_L[block_num][rotate][j][i] == 1 && background[j + y][i + x] > 0)
      {
        overlap_count++;
      }
    }
  }
  return overlap_count;
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

void make_rect(char xx, char yy, unsigned short color)
{
  for (int j = 0; j < 5; j++) {
    for (int i = 0; i < 5; i++) {
      put_pixel(xx + i, yy + j, color);
    }
  }
}

unsigned short check_color(int color)
{
  unsigned short rect_color = 0;
  if (color == 0) rect_color = RED;
  else if (color == 1) rect_color = BLUE;
  else if (color == 2) rect_color = YELLOW;
  else if (color == 3) rect_color = GREEN;
  else if (color == 4) rect_color = BLUE;
  else if (color == 5) rect_color = BLUE;
  else if (color == 6) rect_color = BLUE;
  return rect_color;
}

void make_block(char x, char y)
{

  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if (block_L[block_num][rotate][j][i] == 1) make_rect(pixel_offset_x + (x + i - 1) * 6, pixel_offset_y + (y + j - 1) * 6, check_color(block_num));
    }
  }
}

void delete_block(char x, char y)
{
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if (block_L[block_num][rotate][j][i] == 1) make_rect(pixel_offset_x + (x + i - 1) * 6, pixel_offset_x + (y + j - 1) * 6, WHITE);
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
