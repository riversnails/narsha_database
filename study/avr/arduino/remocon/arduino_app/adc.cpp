#include "adc.h"

void adc_init(char ref)
{
  if (ref == REF_AREF)
  {
    ADMUX = 0x00;
    ADCSRA = 0x87;
  }
  else
  {
    ADMUX = 0x40;
    ADCSRA = 0x87;
  }
}

void adc_init_1(char ref, char pre_scale)
{
  if (ref == REF_AREF)
  {
    ADMUX = 0x00;
  }
  else
  {
    ADMUX = 0x40;
  }

  ADCSRA &= ~0x07;
  ADCSRA = 0x87;
  ADCSRA |= pre_scale;
}

unsigned short analog_read(char channel)
{
  unsigned short adc_value = 0;

  ADMUX &= ~0x0f;
  ADMUX |= channel;

  ADCSRA |= 0x40;

  while (!(ADCSRA & 0x10));
  
  adc_value = ADC;
  return adc_value;
}
