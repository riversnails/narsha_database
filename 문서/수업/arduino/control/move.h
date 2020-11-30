#ifndef MOVE_H
#define MOVE_H

char arr[6][2] =
{
  {40, 25},
  {70, 25},
  {100, 80},
  {70, 95},
  {40, 100},
  {10, 75}
};

#include "Arduino.h"

char move_print();
unsigned short adc_read(char channel);

#endif
