#include"74595.h"

char clk_74595;
char data_74595;
char latch_74595;

void 74595_init(char clk_pin, char data_pin, char latch_pin)
{
  clk_74595 = clk_pin;
  data_74595 = data_pin;
  latch_74595 = latch_pin;

  pinMode(clk_74595, OUTPUT);
  pinMode(data_74595, OUTPUT);
  pinMode(latch_74595, OUTPUT);
}

void set_data_74595(char data)
{
  shift_out(data);
  digitalWrite(latch_74595, HIGH);
  digitalWrite(latch_74595, LOW);
}

void shift_out(char data)
{
  for (int i = 0; i < 8; i++)
  {
    if(data & (0x80 >> i))
    {
      digitalWrite(data_74595, HIGH);
    }
    else
    {
      digitalWrite(data_74595, LOW);
    }

    digitalWrite(clk_74595, HIGH);
    digitalWrite(clk_74595, LOW);
  }
}
