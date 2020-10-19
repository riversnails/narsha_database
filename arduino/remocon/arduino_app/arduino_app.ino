#include"dotmatrix.h"

// row 1, 2, 3, ... row 7, col 0, 1, 2, ... col 7
char pin_num_dot[16] = {12, 13, 14, 15, 16, 17, 18, 19, 4, 5, 6, 7, 8, 9, 10, 11}; // 도트 메트릭스 차이로 배열변경
// row 1, 2, 3, ... row 7, data, clock, latch
char pin_num_dot_74595[11] = {12, 13, 14, 15, 16, 17, 18, 19, 9, 10, 11}; // 도트 메트릭스 차이로 배열변경

// {12, 13, 14, 15, 16, 17, 18, 19, 11, 10, 9, 8, 7, 6, 5, 4};

char asdf[8] = {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c};

char dot_value[10][8] = {
  {0x00, 0x38, 0x44, 0x4C, 0x54, 0x64, 0x44, 0x38},
  {0x00, 0x10, 0x30, 0x50, 0x10, 0x10, 0x10, 0x7c},
  {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c},
  {0x00, 0x38, 0x44, 0x04, 0x18, 0x04, 0x44, 0x38},
  {0x00, 0x08, 0x18, 0x28, 0x48, 0x7C, 0x08, 0x08},
  {0x00, 0x7C, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38},
  {0x00, 0x38, 0x40, 0x40, 0x78, 0x44, 0x44, 0x38},
  {0x00, 0x7C, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20},
  {0x00, 0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38},
  {0x00, 0x38, 0x44, 0x44, 0x3C, 0x04, 0x44, 0x38}
};

void setup()
{
  Serial.begin(9600);
  dotmatrix_init_74595(pin_num_dot_74595);
}

unsigned long c_micros = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;
int count = 0;

void loop()
{
  c_micros = micros();
  c_millis = millis();
  //  if(c_millis - p_millis > 1000)
  //  {
  //    p_millis = c_millis;
  //    count++;
  //    if(count == 10) count = 0;
  //  }
  //  //set_num_millis(asdf, c_millis);
  //  //set_num_delay(asdf);
  //  //set_line_74595(0, 0x23);
  //  //set_num_74595_delay(asdf);
  //  set_num_74595_millis(dot_value[count], c_millis);
  //dot_led_74595_toggle(9);
  //  if (c_millis - p_millis > 5000)
  //  {
  //    p_millis = c_millis;
  //    led_pwm_value(1, 99);
  //  }

  char in_num = 0;
  if (Serial.available())
  {
    int in_data = Serial.read();
    Serial.printf("%c", in_data);
    //    if(in_data == '1')
    //    {
    //      led_pwm_value(1, 99);
    //    }
    //    else if(in_data == '2')
    //    {
    //      led_pwm_value(2, 99);
    //    }

    in_num = in_data - 0x30;
    led_pwm_value(in_num, 99);
  }

  dot_led_74595_pwm(in_num, c_micros, c_millis);
}
