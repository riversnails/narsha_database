#include"i2c.h"

char scl;
char sda;

void i2c_init(char i2c_scl, char i2c_sda)
{
  scl = i2c_scl;
  sda = i2c_sda;
  pinMode(scl, OUTPUT);
  pinMode(sda, OUTPUT);

  digitalWrite(scl, HIGH); // pull up
  digitalWrite(sda, HIGH);
}

void i2c_start()
{
  digitalWrite(scl, HIGH);
  digitalWrite(sda, LOW);
  digitalWrite(scl, LOW);
}

void i2c_stop()
{
  digitalWrite(sda, LOW);
  digitalWrite(scl, HIGH);
  digitalWrite(sda, HIGH);
}

void i2c_ack()
{
  digitalWrite(scl, HIGH);
  digitalWrite(scl, LOW);
}

void i2c_8bit(char data)
{
  for (int i = 0; i < 8; i++)
  {
    if (data & (0x80 >> i))
    {
      digitalWrite(sda, HIGH);
    }
    else
    {
      digitalWrite(sda, LOW);
    }
    digitalWrite(scl, HIGH);
    digitalWrite(scl, LOW);
  }
}

void i2c_8bit_data_write(char addr, char data)
{
  i2c_start();
  i2c_8bit(addr << 1);
  i2c_ack();
  i2c_8bit(data);
  i2c_ack();
  i2c_stop();
}
