#include "remocon.h"
#include "ultrasonic.h"
#include "i2c.h"
#include "servo.h"

#define REMOCON_PIN 2
#define ULTRASONIC_ECHO_PIN 3
#define ULTRASONIC_TRIGGER_PIN 4
#define SERVO_PORT 4
#define SERVO_PORT_COUNT 3
char servo_port_num[SERVO_PORT_COUNT] = {4, 5 ,6};
unsigned char servo_duty_ptr[SERVO_PORT_COUNT] = {22, 22, 22};

char remocon_number = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;
unsigned long p_millis1 = 0;
unsigned long p_millis2 = 0;
unsigned long c_micros = 0;
unsigned long p_millis_ultra = 0;

int servo_degree = 0; // 0 ~ 180
int servo_duty = 22; // 22 ~ 72
int servo_duty1 = 22; // 22 ~ 72
int c = 0;

void setup()
{
  Serial.begin(9600);

  //servo_init(SERVO_PORT);
  servo_init_many(servo_port_num, SERVO_PORT_COUNT);
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

    if (servo_duty_ptr[0]++ == 72) servo_duty_ptr[0] = 22;
  }

  if (c_millis - p_millis1 > 30)
  {
    p_millis1 = c_millis;

    //    if (servo_degree++ == 180) servo_degree = 0;
    //    servo_duty = (int)((700 + (((float)(180 - servo_degree) / 180.0) * 1600)) / 32 + 0.5);

    if (servo_duty_ptr[1]++ == 72) servo_duty_ptr[1] = 22;
  }
  
  if (c_millis - p_millis2 > 40)
  {
    p_millis2 = c_millis;

    //    if (servo_degree++ == 180) servo_degree = 0;
    //    servo_duty = (int)((700 + (((float)(180 - servo_degree) / 180.0) * 1600)) / 32 + 0.5);

    if (servo_duty_ptr[2]++ == 72) servo_duty_ptr[2] = 22;
  }
  
  // servo_degree : 22 ~ 72
  servo_position_micros_many(servo_duty_ptr, c_micros);
}
