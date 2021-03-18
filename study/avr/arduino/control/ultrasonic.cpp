#include "ultrasonic.h"

char ultrasonic_finish_flag = 0;
float measured_distance = 0;

void ultrasonic_init()
{
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  EICRB |= 0x01;
  EIMSK |= 0x10;
}

void ultrasonic_trigger()
{
  digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
}

char ultrasonic_callback(float *distance)
{
  *distance = measured_distance;
  return ultrasonic_finish_flag;
}

void ultrasonic_callback_flag_clear()
{
  ultrasonic_finish_flag = 0;
}

SIGNAL(INT4_vect)
{
  static unsigned long echo_time = 0;
  static unsigned int diff_time = 0;

  if (digitalRead(ULTRASONIC_ECHO_PIN)) { //rising edge
    echo_time = micros();
  }
  else {
    diff_time = micros() - echo_time;
    measured_distance = diff_time * 0.017;
    ultrasonic_finish_flag = 1;
  }
}
