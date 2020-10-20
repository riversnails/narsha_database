#include "dotmatrix.h"

char *dot_pin;

void dotmatrix_init(char *pin_num)
{
  dot_pin = pin_num;
  for (int i = 0; i < 16; i++) pinMode(dot_pin[i], OUTPUT);
}

void set_line(char line_num, char value)
{
  for (int i = 0; i < 8; i++)
  {
    if ((value & (0x01 << i))) digitalWrite(dot_pin[8 + i], LOW);
    else digitalWrite(dot_pin[8 + i], HIGH);
  }

  digitalWrite(dot_pin[line_num], HIGH);
}

char dot_row_num = 0;

void set_num_delay(char *dot_num)
{
  // all off
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dot_pin[i], LOW);
  }
  // one column value set
  for (int i  = 0; i < 8; i++)
  {
    if ((dot_num[dot_row_num] & (0x80 >> i))) digitalWrite(dot_pin[8 + i], LOW);
    else digitalWrite(dot_pin[8 + i], HIGH);
  }
  // one row power on
  digitalWrite(dot_pin[dot_row_num], HIGH);
  dot_row_num++;
  if (dot_row_num == 8) dot_row_num = 0;
  // delay
  delay(1);
}

unsigned long pre_dot_millis = 0;

void set_num_millis(char *dot_num, unsigned long dot_curr_millis)
{
  if (dot_curr_millis - pre_dot_millis >= 1)
  {
    pre_dot_millis = dot_curr_millis;
    // all off
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(dot_pin[i], LOW);
    }
    // one column value set
    for (int i  = 0; i < 8; i++)
    {
      if ((dot_num[dot_row_num] & (0x80 >> i))) digitalWrite(dot_pin[8 + i], LOW);
      else digitalWrite(dot_pin[8 + i], HIGH);
    }
    // one row power on
    digitalWrite(dot_pin[dot_row_num], HIGH);
    dot_row_num++;
    if (dot_row_num == 8) dot_row_num = 0;
  }
}

//---------------------------------------------------------------------------------

char clock_74595_pin;
char data_74595_pin;
char latch_74595_pin;

void dotmatrix_init_74595(char *pin_num)
{
  dot_pin = pin_num;

  for (int i = 0; i < 11; i++) pinMode(dot_pin[i], OUTPUT);
  data_74595_pin = dot_pin[8];
  clock_74595_pin = dot_pin[9];
  latch_74595_pin = dot_pin[10];
}

void set_74595_value(char value)
{
  // column data
  for (int i = 0; i < 8; i++)
  {
    if ((value & (0x01 << i))) digitalWrite(data_74595_pin, LOW);
    else digitalWrite(data_74595_pin, HIGH);

    digitalWrite(clock_74595_pin, HIGH);
    digitalWrite(clock_74595_pin, LOW);
  }
  digitalWrite(latch_74595_pin, HIGH);
  digitalWrite(latch_74595_pin, LOW);
}

void set_line_74595(char line_num, char value)
{
  set_74595_value(value);
  //row power
  digitalWrite(dot_pin[line_num], HIGH);
}

void set_num_74595_delay(char *dot_num)
{
  // all off
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dot_pin[i], LOW);
  }
  // one column value set
  set_74595_value(dot_num[dot_row_num]);
  // one row power on
  digitalWrite(dot_pin[dot_row_num], HIGH);
  dot_row_num++;
  if (dot_row_num == 8) dot_row_num = 0;
  // delay
  delay(1);
}

void set_num_74595_millis(char *dot_num, unsigned long dot_curr_millis)
{
  if (dot_curr_millis - pre_dot_millis >= 1)
  {
    pre_dot_millis = dot_curr_millis;
    // all off
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(dot_pin[i], LOW);
    }
    // one column value set
    set_74595_value(dot_num[dot_row_num]);
    // one row power on
    digitalWrite(dot_pin[dot_row_num], HIGH);
    dot_row_num++;
    if (dot_row_num == 8) dot_row_num = 0;
  }
}

//---------------------------------------------------------------------------------

char col_value_74595 = 0;

void dot_led_74595_toggle(char led_num)
{
  char row_num = led_num / 8;
  char col_num = led_num % 8;

  digitalWrite(dot_pin[row_num], HIGH);
  col_value_74595 &= ~(0x01 << col_num);
  set_74595_value(col_value_74595);
  delay(1000);
  col_value_74595 |= 0x01 << col_num;
  set_74595_value(col_value_74595);
  delay(1000);
}

unsigned long pre_led_micros = 0;
unsigned long pre_led_millis = 0;
int led_pwm_count = 0;
char led_pwm_duty = 99;
char row_num;
char col_num;

void dot_led_74595_pwm(char led_num, unsigned long cur_led_micros, unsigned long cur_led_millis)
{
  if (cur_led_micros - pre_led_micros > 100)
  {
    row_num = led_num / 8;
    col_num = led_num % 8;
    pre_led_micros = cur_led_micros;

    if (led_pwm_count == 100)
    {
      col_value_74595 |= 0x01 << col_num;
      set_74595_value(col_value_74595);
      led_pwm_count = 0;
    }
    else if (led_pwm_count == led_pwm_duty)
    {
      col_value_74595 &= ~(0x01 << col_num);
      set_74595_value(col_value_74595);
    }
    led_pwm_count++;
  }

  if (cur_led_millis - pre_led_millis > 10)
  {
    pre_led_millis = cur_led_millis;

    if (led_pwm_duty >= 2) led_pwm_duty--;
    // if (led_pwm_duty == 0) led_pwm_duty = 99;
  }

  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dot_pin[i], LOW);
    if ( i == row_num) digitalWrite(dot_pin[row_num], HIGH);
  }
}

void led_pwm_value(char led_num, char value)
{
  row_num = led_num / 8;
  col_num = led_num % 8;

  led_pwm_duty = value;
}

void set_num_74595_millis_pwm(char led_num, char *dot_num, unsigned long dot_curr_micros, unsigned long dot_curr_millis)
{
  if (dot_curr_micros - pre_led_micros > 100)
  {
    row_num = led_num / 8;
    col_num = led_num % 8;
    pre_led_micros = dot_curr_micros;

    if (led_pwm_count == 100)
    {
      if (dot_row_num == 0)
      {
        dot_num[0] |= 0x80;
        set_74595_value(dot_num[dot_row_num]);
        digitalWrite(dot_pin[0], HIGH);
      }

      led_pwm_count = 0;
    }
    else if (led_pwm_count == 10)
    {
      if (dot_row_num == 0)
      {
        dot_num[0] &= ~0x80;
        set_74595_value(dot_num[dot_row_num]);
        digitalWrite(dot_pin[0], LOW);
        //col_value_74595 &= ~(0x01 << col_num);
      }
    }
    led_pwm_count++;
  }
  //---------------------------------------------
  if (dot_curr_millis - pre_dot_millis >= 1)
  {
    pre_dot_millis = dot_curr_millis;
    // all off
    for (int i = 0; i < 8; i++) digitalWrite(dot_pin[i], LOW);
    // one column value set
    set_74595_value(dot_num[dot_row_num]);
    // one row power on
    digitalWrite(dot_pin[dot_row_num], HIGH);
    dot_row_num++;
    if (dot_row_num == 8) dot_row_num = 0;
  }

  //  if (cur_led_millis - pre_led_millis > 10)
  //  {
  //    pre_led_millis = cur_led_millis;
  //
  //    if (led_pwm_duty >= 2) led_pwm_duty--;
  //  }
}
