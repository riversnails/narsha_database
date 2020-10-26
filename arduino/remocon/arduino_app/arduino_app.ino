#include "keymatrix.h"

char key_pin[8] = {2, 3, 4, 5, 6, 7, 8, 9};

void setup()
{
  Serial.begin(9600);
  keymatrix_init(key_pin);
  pinMode(13, OUTPUT);
}

unsigned long c_micros = 0;
unsigned long p_micros = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;

unsigned long p_micros_pwm = 0;
int pwm_count = 0;
int pwm_duty = 10;

void loop()
{
  char key_in = 0;
  c_millis = millis();
  c_micros = micros();

  keymatrix_input_millis(&key_in, c_millis);
  if (key_in != 0)
  {
    pwm_duty = key_in;
    Serial.printf("key %d\n", key_in);
  }

  // LED PWM
  if (c_micros - p_micros_pwm > 100)
  {
    p_micros_pwm = c_micros;



    pwm_count++;
    if (pwm_count == 100)
    {
      pwm_count = 0;
      digitalWrite(13, HIGH);
    }
    else if (pwm_count == pwm_duty * 6)
    {
      digitalWrite(13, LOW);
    }
  }
}
