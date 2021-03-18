#include "keymatrix.h"

#define ROW_PTR 0
#define COL_PTR 4

char curr_column = 0;
char *keymatrix_pin;
char flag_row[4][4];

void keymatrix_init(char *pin_num)
{
  keymatrix_pin = pin_num;

  for (int i = 0; i < 4; i++) pinMode(keymatrix_pin[i], INPUT); // row
  for (int i = 0; i < 4; i++) pinMode(keymatrix_pin[i + 4], OUTPUT); // col

  for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++)flag_row[i][j] = 0;
}

void keymatrix_input_delay(char *in_value)
{
  char key_input = 0;

  //1. all column off : LOW

  for (int i = 0; i < 4; i++) digitalWrite(keymatrix_pin[COL_PTR + i], LOW);

  //1. one column on : HIGH

  digitalWrite(keymatrix_pin[COL_PTR + curr_column], HIGH);

  //3. Row read
  for (int i = 0; i < 4; i++ )
  {
    key_input = digitalRead(keymatrix_pin[ROW_PTR + i]);

    if (key_input == 1 && flag_row[i][curr_column] == 0)
    {
      flag_row[i][curr_column] = 1;
      *in_value = key_input + curr_column + i * 4;
      //Serial.printf("key %d\n", *in_value);
    }
    else if (key_input == 0 && flag_row[i][curr_column] == 1)
    {
      flag_row[i][curr_column] = 0;
      *in_value = 0;
    }
  }
  //4. delay : 10ms

  curr_column++;
  if (curr_column == 4) curr_column = 0;

  delay(10);
}

unsigned long keymatrix_p_millis = 0;

void keymatrix_input_millis(char *in_value, unsigned long c_millis)
{
  if (c_millis - keymatrix_p_millis > 10)
  {
    keymatrix_p_millis = c_millis;    
    char key_input = 0;

    //1. all column off : LOW
    for (int i = 0; i < 4; i++) digitalWrite(keymatrix_pin[COL_PTR + i], LOW);

    //1. one column on : HIGH
    digitalWrite(keymatrix_pin[COL_PTR + curr_column], HIGH);

    //3. Row read
    for (int i = 0; i < 4; i++ )
    {
      key_input = digitalRead(keymatrix_pin[ROW_PTR + i]);

      if (key_input == 1 && flag_row[i][curr_column] == 0)
      {
        flag_row[i][curr_column] = 1;
        *in_value = key_input + curr_column + i * 4;
        //Serial.printf("key %d\n", *in_value);
      }
      else if (key_input == 0 && flag_row[i][curr_column] == 1)
      {
        flag_row[i][curr_column] = 0;
        *in_value = 0;
      }
    }
    
    //4. delay : 10ms
    curr_column++;
    if (curr_column == 4) curr_column = 0;
  }
}
