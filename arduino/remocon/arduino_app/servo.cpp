#include "servo.h";

// 20ms 주기, 0.7~2.3 duty, 1단계 : 32us, 총 50단계(20ms)
#define SERVO_NUM 10

int duty_servo_lib[SERVO_NUM];
char servo_port_many[SERVO_NUM];
char servo_port_count = 0;

unsigned long p_micros_servo = 0;
char servo_port = 0;
unsigned int servo_count = 0;

void servo_init(char port)
{
  pinMode(port, OUTPUT);
  servo_port = port;
}

void servo_init_many(char* port, int port_count)
{
  for (int i = 0; i < port_count; i++) {
    pinMode(port[i], OUTPUT);
    servo_port_many[i] = port[i];
  }
  servo_port_count = port_count;
}

void servo_position(unsigned char degree)
{
  if (degree > 180) degree = 180;
  if (degree < 0) degree = 0;

  int duty = 700 + (int)(((float)(180 - degree) / 180.0) * 1600);

  digitalWrite(servo_port, HIGH);
  delayMicroseconds(duty);
  digitalWrite(servo_port, LOW);
  delay(17);
  delayMicroseconds(3000 - duty);
}

void servo_position_micros(unsigned char degree, unsigned long c_micros)
{
  if (c_micros - p_micros_servo > 32)
  {
    p_micros_servo = c_micros;

    if (degree > 72) degree = 72;
    if (degree < 22) degree = 22;
    //int duty = (int)((700 + (((float)(180 - degree) / 180.0) * 1600)) / 32 + 0.5);

    if (servo_count == 625)
    {
      digitalWrite(servo_port, HIGH);
      servo_count = 0;
    }
    else if (servo_count == degree)
    {
      digitalWrite(servo_port, LOW);
    }
    servo_count++;
  }
}

void servo_position_micros_many(unsigned char* degree, unsigned long c_micros)
{
  if (c_micros - p_micros_servo > 32)
  {
    p_micros_servo = c_micros;
    
    for (int i = 0; i < servo_port_count; i++) {
      if (degree[i] > 72) degree[i] = 72;
      if (degree[i] < 22) degree[i] = 22;
      duty_servo_lib[i] = degree[i];
    }

    //    for (int i = 0; i < servo_port_count; i++) Serial.println(c_micros);
    //    Serial.println();

    if (servo_count == 625)
    {
      for (int i = 0; i < servo_port_count; i++) digitalWrite(servo_port_many[i], HIGH);
      servo_count = 0;
    }
    for (int i = 0; i < servo_port_count; i++)
    {
      if (servo_count == duty_servo_lib[i])
      {
        digitalWrite(servo_port_many[i], LOW);
      }
    }

    servo_count++;
  }
}
