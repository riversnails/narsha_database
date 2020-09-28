#include "remocon.h"
#include "ultrasonic.h"
#include "i2c.h"
#include "servo.h"

#define REMOCON_PIN 2
#define ULTRASONIC_ECHO_PIN 3
#define ULTRASONIC_TRIGGER_PIN 4
#define SERVO_PORT 4

float ultrasonic_dist = 0;

char remocon_number = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;
unsigned long c_micros = 0;
unsigned long p_millis_ultra = 0;

int servo_degree = 0; // 0 ~ 180
int servo_duty = 22; // 22 ~ 72
int c = 0;

void setup()
{
  Serial.begin(9600);

  servo_init(SERVO_PORT);
}

void loop()
{
  c_micros = micros();
  c_millis = millis();

  //  servo_position(servo_position1);
  //  servo_position1++;
  //  if(servo_position1 == 180) servo_position1 = 0;

  if (c_millis - p_millis > 20)
  {
    p_millis = c_millis;

    //    if (servo_degree++ == 180) servo_degree = 0;
    //    servo_duty = (int)((700 + (((float)(180 - servo_degree) / 180.0) * 1600)) / 32 + 0.5);

    if (servo_duty++ == 72) servo_duty = 22;

  }

  // servo_degree : 22 ~ 72
  servo_position_micros(servo_duty, c_micros);
}
