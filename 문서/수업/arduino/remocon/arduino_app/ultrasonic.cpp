#include "ultrasonic.h"
#include "exti.h"

char echo_pin_num = 0;
char trig_pin_num = 0;
char ultrasonic_finish_flag = 0;
float measured_distance = 0;

void ultrasonic_init(char int_pin_num, char trigger_pin_num)
{
  echo_pin_num = int_pin_num;
  trig_pin_num = trigger_pin_num;
  pinMode(trigger_pin_num, OUTPUT);
  exti_init(echo_pin_num - 2, EXTI_CHANGE, ultrasonic_ISR);
}

void ultrasonic_trigger()
{
  digitalWrite(trig_pin_num, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin_num, LOW);
}

void ultrasonic_callback_flag_clear()
{
  ultrasonic_finish_flag = 0;
}

char ultrasonic_callback(float *distance)
{
  *distance = measured_distance;
  return ultrasonic_finish_flag;
}

void ultrasonic_ISR()
{
  static unsigned long echo_time = 0;
  static unsigned int diff_time = 0;

  if (digitalRead(echo_pin_num) == 1) // rising edge
  {
    echo_time = micros();
  }
  else
  {
    diff_time = micros() - echo_time;
    measured_distance = diff_time * 0.017;
    ultrasonic_finish_flag = 1;
  }
}
