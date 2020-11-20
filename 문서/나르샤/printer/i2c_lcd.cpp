#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "I2c_lcd.h"
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 100k bed thermistor
const short temptable_1[][2] = {
  { 23, 300 },
  { 25, 295 },
  { 27, 290 },
  { 28, 285 },
  { 31, 280 },
  { 33, 275 },
  { 35, 270 },
  { 38, 265 },
  { 41, 260 },
  { 44, 255 },
  { 48, 250 },
  { 52, 245 },
  { 56, 240 },
  { 61, 235 },
  { 66, 230 },
  { 71, 225 },
  { 78, 220 },
  { 84, 215 },
  { 92, 210 },
  { 100, 205 },
  { 109, 200 },
  { 120, 195 },
  { 131, 190 },
  { 143, 185 },
  { 156, 180 },
  { 171, 175 },
  { 187, 170 },
  { 205, 165 },
  { 224, 160 },
  { 245, 155 },
  { 268, 150 },
  { 293, 145 },
  { 320, 140 },
  { 348, 135 },
  { 379, 130 },
  { 411, 125 },
  { 445, 120 },
  { 480, 115 },
  { 516, 110 },
  { 553, 105 },
  { 591, 100 },
  { 628,  95 },
  { 665,  90 },
  { 702,  85 },
  { 737,  80 },
  { 770,  75 },
  { 801,  70 },
  { 830,  65 },
  { 857,  60 },
  { 881,  55 },
  { 903,  50 },
  { 922,  45 },
  { 939,  40 },
  { 954,  35 },
  { 966,  30 },
  { 977,  25 },
  { 985,  20 },
  { 993,  15 },
  { 999,  10 },
  { 1004,  5 },
  { 1008,  0 },
  { 1012, -5 },
  { 1016, -10 },
  { 1020, -15 }
};

int current_nozzle_temp = 0;
int current_bed_temp = 0;
unsigned long i2c_p_millis = 0;

int N = 120;
int B = 860;

void i2c_init()
{
  lcd.begin();
  lcd.backlight();

  lcd.print("Nozzle: ");
  lcd.setCursor(0, 1);
  lcd.print("Bed   : ");
}

void i2c_write(int e, int b, unsigned long i2c_c_millis)
{
  if(i2c_c_millis - i2c_p_millis > 1000)
  {
    i2c_p_millis = i2c_c_millis;

    N = e;
    B = b;
    current_nozzle_temp = temp_check(N);
    current_bed_temp = temp_check(B);

    lcd.setCursor(8, 0);
    lcd.printf("%d C", current_nozzle_temp);
    lcd.setCursor(8, 1);
    lcd.printf("%d C", current_bed_temp);
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
