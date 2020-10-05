#include "fnd.h"
#include "gpio.h"

char fnd_number[10][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1, 0}, // 2
  {1, 1, 1, 1, 0, 0, 1, 0}, // 3
  {0, 1, 1, 0, 0, 1, 1, 0}, // 4
  {1, 0, 1, 1, 0, 1, 1, 0}, // 5
  {1, 0, 1, 1, 1, 1, 1, 0}, // 6
  {1, 1, 1, 0, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1, 0}, // 8
  {1, 1, 1, 1, 0, 1, 1, 0}  // 9
};

char fnd_pin_num[8];
char c_anode_cathode = 0;
char common_pin[4];
char fndCount = 0;
char current_digit = 0;
char digit_num[4];

void fnd_init_1(char *fnd_pin, char common)
{
  c_anode_cathode = common;
  for (int i = 0; i < 8; i++)
  {
    pin_mode(fnd_pin[i], OUTPUT);
    fnd_pin_num[i] = fnd_pin[i];
  }
}

void fnd_num_1(char num)
{
  for (int i = 0; i < 8; i++)
  {
    if (c_anode_cathode == C_CATHODE)
      digital_write(fnd_pin_num[i], fnd_number[num][i]);
    else
      digital_write(fnd_pin_num[i], !fnd_number[num][i]);
  }
}

//---------------------------------------------------------------

void fnd_init_2(char *fnd_pin, char common)
{
  c_anode_cathode = common;
  fndCount = 2;

  for (int i = 0; i < 8; i++)
  {
    pin_mode(fnd_pin[i], OUTPUT);
    fnd_pin_num[i] = fnd_pin[i];
  }

  for (int i = 0; i < fndCount; i++)
  {
    pin_mode(fnd_pin[i + 8], OUTPUT);
    common_pin[i] = fnd_pin[i + 8];
  }
}

void fnd_num_2(int num)
{
  // all power off
  for (int i = 0; i < 2; i++)
  {
    digital_write(common_pin[i], LOW);
  }
  // 오른쪽 숫자 입력
  char num_1 = num % 10;
  for (int i = 0; i < 8; i++)
  {
    if (c_anode_cathode == C_CATHODE)
      digital_write(fnd_pin_num[i], fnd_number[num_1][i]);
    else
      digital_write(fnd_pin_num[i], !fnd_number[num_1][i]);
  }
  // 오른쪽 power on
  digital_write(common_pin[0], HIGH);
  // delay 10
  delay(10);


  // all power off
  for (int i = 0; i < 2; i++)
  {
    digital_write(common_pin[i], LOW);
  }
  // 왼쪽 숫자 입력
  char num_10 = (num % 100) / 10;
  for (int i = 0; i < 8; i++)
  {
    if (c_anode_cathode == C_CATHODE)
      digital_write(fnd_pin_num[i], fnd_number[num_10][i]);
    else
      digital_write(fnd_pin_num[i], !fnd_number[num_10][i]);
  }
  // 왼쪽 power on
  digital_write(common_pin[1], HIGH);
  // delay 10
  delay(10);
}

//---------------------------------------------------------------

void fnd_init(char fnd_count, char *fnd_pin, char common)
{
  c_anode_cathode = common;
  fndCount = fnd_count;

  for (int i = 0; i < 8; i++)
  {
    pin_mode(fnd_pin[i], OUTPUT);
    fnd_pin_num[i] = fnd_pin[i];
  }

  for (int i = 0; i < fndCount; i++)
  {
    pin_mode(fnd_pin[i + 8], OUTPUT);
    common_pin[i] = fnd_pin[i + 8];
  }
}

void fnd_num_delay(int num)
{
  // all power off
  for (int i = 0; i < fndCount; i++)
  {
    digital_write(common_pin[i], LOW);
  }
  // 오른쪽 숫자 입력
  digit_num[0] = num % 10;
  digit_num[1] = (num % 100) / 10;
  digit_num[2] = (num % 1000) / 100;
  digit_num[3] = num / 1000;

  for (int i = 0; i < 8; i++)
  {
    if (c_anode_cathode == C_CATHODE)
      digital_write(fnd_pin_num[i], fnd_number[digit_num[current_digit]][i]);
    else
      digital_write(fnd_pin_num[i], !fnd_number[digit_num[current_digit]][i]);
  }
  // 오른쪽 power on
  digital_write(common_pin[current_digit], HIGH);
  // delay 10
  current_digit++;
  if (current_digit == 4) current_digit = 0;

  delay(10 / fndCount);
}

unsigned long previous_millis = 0;
void fnd_num_millis(unsigned int current_millis, int num)
{
  if (current_millis - previous_millis > 10 / fndCount)
  {
    previous_millis = current_millis;

    // all power off
    for (int i = 0; i < fndCount; i++)
    {
      digital_write(common_pin[i], LOW);
    }
    // 오른쪽 숫자 입력
    if(fndCount >= 1) digit_num[0] = num % 10;
    if(fndCount >= 2) digit_num[1] = (num % 100) / 10;
    if(fndCount >= 3) digit_num[2] = (num % 1000) / 100;
    if(fndCount >= 4) digit_num[3] = num / 1000;

    for (int i = 0; i < 8; i++)
    {
      if (c_anode_cathode == C_CATHODE)
        digital_write(fnd_pin_num[i], fnd_number[digit_num[current_digit]][i]);
      else
        digital_write(fnd_pin_num[i], !fnd_number[digit_num[current_digit]][i]);
    }
    // 오른쪽 power on
    digital_write(common_pin[current_digit], HIGH);
    // delay 10
    current_digit++;
    if (current_digit == 4) current_digit = 0;
  }


}
