#ifndef ADC_H
#define ADC_H

#include "Arduino.h"

#define REF_AREF 0
#define REF_AVCC 1

#define AD_0 0
#define AD_1 1
#define AD_2 2
#define AD_3 3
#define AD_4 4
#define AD_5 5

void adc_init(char ref);
void adc_init_1(char ref, char pre_scale);
unsigned short analog_read(char channel);

#endif
