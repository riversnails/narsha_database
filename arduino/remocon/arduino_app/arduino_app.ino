#include"gpio.h"
#include "adc.h"

void setup()
{
  Serial.begin(9600);
  adc_init(REF_AVCC);
}


void loop()
{
  unsigned short joystick_x = 0;
  unsigned short joystick_y = 0;

  joystick_x = analog_read(AD_0);
  joystick_y = analog_read(AD_1);

  Serial.printf("x = %d y = %d\n", joystick_x, joystick_y);
}
