#include "pwm.h"

int pwm_count = 0;
int pwm_duty = 1;

void pwm_init()
{
  DDRH |= PWM_PIN;
  TCCR2A = 0x03;
  TCCR2B = 0x02;
  OCR2A = 40;
  TIMSK2 = 0x00;
}

void pwm_start()
{
  TIMSK2 = 0x02;
}

void pwm_stop()
{
  pwm_count = 0;
  pwm_duty = 1;
  TIMSK2 = 0x00;
}

void pwm_change(char get_duty)
{
  pwm_duty = get_duty;
}

SIGNAL(TIMER2_COMPA_vect)
{
  pwm_count++;
  if(pwm_count == 100)
  {
    pwm_count = 0;
    PORTH |= PWM_PIN;
  }
  else if(pwm_count == pwm_duty)
  {
    PORTH &= ~PWM_PIN;
  }
}
