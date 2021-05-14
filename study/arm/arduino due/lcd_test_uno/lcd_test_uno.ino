#define CLK 13
#define MOSI 11
#define RES 3
#define CS 4
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
  //while (!Serial);
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
  //-----------------------------------

  oled_1351_init();
  clear_screen(BLACK);

  for (int j = 0; j < 127; j++)
  {
    for (int i = 0; i < 127; i++)
    {
      put_pixel(j, i, RED);
    }
  }
}

void loop()
{

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
  shift_out(cmd);
  digitalWrite(CS, HIGH);
  digitalWrite(DS, HIGH);
}

void oled_1351_data(unsigned char data)
{
  digitalWrite(CS, LOW);
  shift_out(data);
  digitalWrite(CS, HIGH);
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

  for (int j = 0; j < 128; j++)
  {
    for (int i = 0; i < 128; i++)
    {
      oled_1351_data(first_byte);
      oled_1351_data(second_byte);
    }
  }
}
