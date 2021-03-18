#include "gpio.h"

void pin_mode(char pin_num, char direction)
{
  if (pin_num >= 0 && pin_num <= 7)
  {
    if (direction == OUTPUT)
      DDRD |= 0x01 << pin_num;
    else
      DDRD &= ~(0x01 << pin_num);
  }
  else if (pin_num >= 8 && pin_num <= 13)
  {
    if (direction == OUTPUT)
      DDRB |= 0x01 << (pin_num - 8);
    else
      DDRB &= ~(0x01 << (pin_num - 8));
  }
  else if (pin_num >= 14 && pin_num <= 19)
  {
    if (direction == OUTPUT)
      DDRC |= 0x01 << (pin_num - 14);
    else
      DDRC &= ~(0x01 << (pin_num - 14));
  }
}

void digital_write(char pin_num, char high_low)
{
  if (pin_num >= 0 && pin_num <= 7)
  {
    if (high_low == HIGH)
      PORTD |= 0x01 << pin_num;
    else
      PORTD &= ~(0x01 << pin_num);
  }
  else if (pin_num >= 8 && pin_num <= 13)
  {
    if (high_low == HIGH)
      PORTB |= 0x01 << (pin_num - 8);
    else
      PORTB &= ~(0x01 << (pin_num - 8));
  }
  else if (pin_num >= 14 && pin_num <= 19)
  {
    if (high_low == HIGH)
      PORTC |= 0x01 << (pin_num - 14);
    else
      PORTC &= ~(0x01 << (pin_num - 14));
  }
}

int digital_read(char pin_num)
{
  char in_value = 0;
  int return_value = 0;
  if (pin_num >= 0 && pin_num <= 7)
  {
    in_value = PIND;
    if (in_value & (0x01 << pin_num))
    {
      return_value = 1;
    }
    else
      return_value = 0;
  }
  else if (pin_num >= 8 && pin_num <= 13)
  {
    in_value = PINB;
    if (in_value & (0x01 << (pin_num - 8)))
    {
      return_value = 1;
    }
    else
      return_value = 0;
  }
  else if (pin_num >= 14 && pin_num <= 19)
  {
    in_value = PINC;
    if (in_value & (0x01 << (pin_num - 14)))
    {
      return_value = 1;
    }
    else
      return_value = 0;
  }

  return return_value;
}
