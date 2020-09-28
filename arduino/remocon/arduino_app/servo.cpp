#include "servo.h";

char servo_port = 0;

void servo_init(char port)
{
  pinMode(port, OUTPUT);
  servo_port = port;
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
