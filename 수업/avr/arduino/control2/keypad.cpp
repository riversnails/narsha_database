#include "keypad.h"

char key_num[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
int key_col[4] = {30, 31, 32, 33};
int key_row[4] = {34, 35, 36, 37};
char rem;
int key_toggle = 0;

void keypad_init()
{
  for (int i = 0; i < 4; i++)
  {
    pinMode(key_col[i], OUTPUT);
    pinMode(key_row[i], INPUT);
    digitalWrite(key_col[i], HIGH);
  }
}

char keypad_print(char loc_num)
{
  rem = 'k';
  for (int j = 0; j < 4; j++)
  {
    digitalWrite(key_col[j], LOW);
    for (int i = 0; i < 4; i++)
    {
      if (digitalRead(key_row[i]) == 0) rem = key_num[i * 4 + j];
    }
    digitalWrite(key_col[j], HIGH);
  }
  //  if(rem == '*' || rem == '#') Serial.println("ok");
  //  else Serial.println(rem);
  for (int i = 0; i < 16; i++)
  {
    if (rem == '1' && key_toggle == 0)
    {
      key_toggle = 1;
      return 0;
    }
    else if (rem == '2' && key_toggle == 0)
    {
      key_toggle = 1;
      return 1;
    }
    else if (rem == '3' && key_toggle == 0)
    {
      key_toggle = 1;
      return 2;
    }
    else if (rem == '4' && key_toggle == 0)
    {
      key_toggle = 1;
      return 3;
    }
    else if (rem == '5' && key_toggle == 0)
    {
      key_toggle = 1;
      return 4;
    }
    else if (rem == '6' && key_toggle == 0)
    {
      key_toggle = 1;
      return 5;
    }
    else if (rem == '*' && key_toggle == 0)
    {
      key_toggle = 1;
      int a = loc_num -= 1;
      if (a == -1) a = 5;
      return a;
    }
    else if (rem == '#' && key_toggle == 0)
    {
      key_toggle = 1;
      int a = loc_num += 1;
      if (a == 6) a = 0;
      return a;
    }
    else if (rem == '0' && key_toggle == 0)
    {
      key_toggle = 1;
      return -2;
    }
    else if (rem == 'k' && key_toggle == 1) key_toggle = 0;
    else return loc_num;
  }
}
