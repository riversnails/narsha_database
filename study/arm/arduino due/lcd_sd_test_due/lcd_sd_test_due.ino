#include<SPI.h>
#include<SD.h>

#define CLK 13
#define MOSI 11
#define RES 3
#define CS 10
#define DS 5

#define CSPIN 4

#define RED 0xf800
#define BLUE 0x001f
#define GREEN 0x7E00
#define YELLOW 0xFFE0
#define WHITE 0xffff
#define BLACK 0x0000

#define APPLY_MODE 2

#define WIDTH 128
#define HEIGET 64

#define size_arr WIDTH*HEIGET*3

char pin_num[5] = {CLK, MOSI, RES, CS, DS};
//unsigned char arr[size_arr];
unsigned char arr[size_arr];
File myFile;

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  if (!SD.begin(CSPIN)) {
    Serial.println(F("Card Initializing failed, or not present"));
    while (1);
  }
  
  Serial.println("START");
  //  pinMode(CLK, OUTPUT);
  //  pinMode(MOSI, OUTPUT);
  //  pinMode(RES, OUTPUT);
  //  pinMode(CS, OUTPUT);
  //  pinMode(DS, OUTPUT);
  pinMode(CSPIN, OUTPUT);
  //myFile = SD.open("tiger.bmp", FILE_READ);
  //myFile = SD.open("test16.bmp", FILE_READ);

  for (int i = 0; i < 5; i++)
  {
    pinMode(pin_num[i], OUTPUT);
    digitalWrite(pin_num[i], LOW);
  }
  digitalWrite(RES, HIGH);
  //----------------------------------

  //----------------------------------
  // Arduino due SPI
  SPI.begin(10);
  SPI.setClockDivider(10, 5);
  //----------------------------------

  oled_1351_init();
  clear_screen(WHITE);
  delay(1000);
  clear_screen(BLACK);

  //  myFile = SD.open("test24.bmp", FILE_READ);
  //
  //  if (myFile)       //file has really been opened
  //  {
  //    Serial.println("file_open");
  //    Serial.println(myFile.name());
  //    Serial.println(myFile.size());
  //
  //    myFile.seek(0x36);
  //    myFile.read(arr, size_arr);
  //  }
  //  else
  //  {
  //    Serial.println("open_failed!");
  //    while (1);
  //  }
  //  myFile.close();
  //  draw_bitmap();
  //  while(1);
}

String fname;
int count = 1;

void loop()
{
  if (count == 2445) while (1);
  fname = "amz";
  if ((float)(count / 10) < 1)
    fname += "000";
  else if ((float)(count / 10) < 10)
    fname += "00";
  else if ((float)(count / 10) < 100)
    fname += "0";

  fname += count;
  fname += ".bmp";
  count++;

  read_sdcard(fname);

  //delay(30);
  draw_bitmap();
}

void read_sdcard(String fname)
{
  myFile = SD.open(fname, FILE_READ);

  if (myFile)       //file has really been opened
  {
    myFile.seek(0x36);
    myFile.read(arr, size_arr);
  }
  else
  {
    Serial.println("open_failed!");
    Serial.println(fname);
    Serial.println((float)(count / 10));
    myFile.close();
    while (1);
  }
  myFile.close();
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
  SPI.transfer(10, cmd);
  //shift_out(cmd);
  digitalWrite(CS, HIGH);
  digitalWrite(DS, HIGH);
}

void oled_1351_data(unsigned char data)
{
  digitalWrite(CS, LOW);
  SPI.transfer(10, data);
  //shift_out(data);
  digitalWrite(CS, HIGH);
}

void oled_1351_data_1byte(unsigned char data)
{
  SPI.transfer(10, data);
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
      oled_1351_data_1byte(first_byte);
      oled_1351_data_1byte(second_byte);
    }
  }
  digitalWrite(CS, HIGH);
}

unsigned char first_byte;
unsigned char second_byte;

unsigned char red;
unsigned char green;
unsigned char blue;

unsigned char r;
unsigned char g;
unsigned char b;

void draw_bitmap()
{
  oled_1351_command(0x15);
  oled_1351_data(0x00);
  oled_1351_data(WIDTH - 1);

  oled_1351_command(0x75);
  oled_1351_data(0x00);
  oled_1351_data(HEIGET - 1);

  oled_1351_command(0x5C);

  digitalWrite(CS, LOW);

  for (int j = 0; j < HEIGET; j++)
  {
    int y_path = 384 + (j * WIDTH * 3) - 3;
    for (int i = 0; i < WIDTH; i++)
    {
      //      first_byte = arr[1 + (i * 2) + (j * WIDTH * 2)];
      //      second_byte = arr[(i * 2) + (j * WIDTH * 2)];
      int x_path = (i * 3);
      b = arr[size_arr - (0 + y_path) + (x_path)];
      g = arr[size_arr - (2 + y_path) + (x_path)];
      r = arr[size_arr - (1 + y_path) + (x_path)];

      //first_byte = ((r >> 3) << 3) | (g >> 5);
      //second_byte = ((g >> 5) << 5) | (b >> 3);

      first_byte = (((r & 0b11111000) >> 3) << 3) | (((g & 0b11100000) >> 5));
      second_byte = (((g & 0b00011100) >> 2) << 5) | (((b & 0b11111000) >> 3));

      //      first_byte = myFile.read();
      //      second_byte = myFile.read();
      //      Serial.print(first_byte, HEX);
      //      Serial.print(" ");
      //      Serial.println(second_byte, HEX);

      oled_1351_data_1byte(first_byte);
      oled_1351_data_1byte(second_byte);
    }
  }

  digitalWrite(CS, HIGH);
  myFile.close();
}
