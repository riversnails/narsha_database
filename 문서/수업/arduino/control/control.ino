#include "i2clcd.h"
#include "oled.h"
#include "servo.h"
#include "pwm.h"
#include "move.h"

char c_location = 0;
char p_location = 0;
char i = 1, j = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(53, OUTPUT); // mega
  pinMode(OLED_CS, OUTPUT);
  pinMode(OLED_RST, OUTPUT);
  pinMode(OLED_DC, OUTPUT);
  pinMode(OLED_SCK, OUTPUT);
  pinMode(OLED_DATA, OUTPUT);
  digitalWrite(OLED_SCK, LOW);

  spi_init();
  oled_1351_init();
  Clear_Screen(WHITE);
  Draw_Bitmap();

  i2c_init();
  servo_init();
  pwm_init();

  ADMUX |= 0x40;
  ADCSRA = 0x87;
  ADCSRB = 0x00;
}

void loop()
{
  //  for(int i = 0; i < 6; i++)
  //  {
  //    make_rect(arr[i][0], arr[i][1], BLUE);
  //    delay(1000);
  //    make_rect(arr[i][0], arr[i][1], WHITE);
  //  }

  //  for (int i = 1; i < 9; i++)
  //  {
  //    delay(100);
  //    pwm_change(i * 10);
  //    servo_change(i * 2);
  //  }

  c_millis = millis();

  

  if (p_location != c_location)
  {
    stop_all();
    if (c_location == 0)
    {
      servo_start();
    }
    else if (c_location == 1)
    {
      pwm_start();
    }
    else if (c_location == 2)
    {

    }
    else if (c_location == 3)
    {

    }
    else if (c_location == 4)
    {

    }
    else if (c_location == 5)
    {

    }
    make_rect(arr[c_location][0], arr[c_location][1], BLUE);
  }

  if (c_millis - p_millis > 100)
  {
    p_millis = c_millis;

    if (c_location == 0)
    {
      servo_change(j++);
    }
    else if (c_location == 1)
    {
      pwm_change((i++) * 5);
    }
    else if (c_location == 2)
    {

    }
    else if (c_location == 3)
    {

    }
    else if (c_location == 4)
    {

    }
    else if (c_location == 5)
    {

    }
  }
  if (j == 25) j = 1;
  if (i == 20) i = 1;

  p_location = c_location;
}

void stop_all()
{
  servo_stop();
  pwm_stop();
}
