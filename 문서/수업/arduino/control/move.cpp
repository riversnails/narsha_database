#include "move.h"
#include "oled.h"

char move_c_location = 0;
char joy_toggle = 0;

char move_print()
{
  if (Serial.available()) {
    char dir = Serial.read();
    if (dir == ' ')
    {
      make_rect(arr[move_c_location][0], arr[move_c_location][1], WHITE);
      move_c_location++;
      if (move_c_location == 6) move_c_location = 0;
    }
  }

  if (adc_read(0) < 300)
  {
    if (joy_toggle == 0)
    {
      make_rect(arr[move_c_location][0], arr[move_c_location][1], WHITE);
      joy_toggle = 1;
      move_c_location--;
      if (move_c_location == -1) c_location = 5;
    }

  }
  else if (adc_read(0) > 700)
  {
    if (joy_toggle == 0)
    {
      make_rect(arr[move_c_location][0], arr[move_c_location][1], WHITE);
      joy_toggle = 1;
      move_c_location++;
      if (move_c_location == 6) move_c_location = 0;
    }
  }
  else
  {
    joy_toggle = 0;
  }

  return (move_c_location);
}

unsigned short adc_read(char channel)
{
  unsigned short adc_value = 0;

  ADMUX &= ~0x0f;
  ADMUX |= channel;

  ADCSRA |= 0x40;

  while (!(ADCSRA & 0x10));

  adc_value = ADC;
  return adc_value;
}
