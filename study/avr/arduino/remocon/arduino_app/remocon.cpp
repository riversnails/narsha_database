#include "remocon.h";
#include "exti.h"

char rem_int_count = 0;
unsigned long int_time[40];
int diff_time[33];
char rem_bit[32];
char remocon_num = 0x00;
int remocon_finish_flag = 0;

char rem_8bit_data[2][8] = {
  {0, 1, 1, 0, 1, 0, 0, 0}, // 0
  {0, 0, 1, 1, 0, 0, 0, 0} // 1
};

void remocon_init(char int_pin)
{
  exti_init(int_pin - 2, EXTI_FALLING, remocon_ISR);

  //  pinMode(2, INPUT);
  //  DDRD &= ~(0x01 << int_pin);
  //  attachInterrupt(0, remoconISR, FALLING);
  //  EICRA = 0x02 << (int_pin - 2) * 2;
  //  EIMSK = 0x01 << (int_pin - 2);
}

void remocon_proc()
{

}

void int_callback_flag_clear()
{
  remocon_finish_flag = 0;
}

char remocon_callback(char *rem_num)
{
  *rem_num = remocon_num;
  return remocon_finish_flag;
}

//SIGNAL(INT0_vect) // == remoconISR

void remocon_ISR()
{
  int_time[rem_int_count] = micros();

  if (rem_int_count > 0)
  {
    if ((int_time[rem_int_count] - int_time[rem_int_count - 1]) > 13000 && (int_time[rem_int_count] - int_time[rem_int_count - 1]) < 14000)
    {
      rem_int_count = 1;
      //Serial.println("Lead Code");
    }
    else if ((int_time[rem_int_count] - int_time[rem_int_count - 1]) > 11000 && (int_time[rem_int_count] - int_time[rem_int_count - 1]) < 12000)
    {
      //Serial.println("R");
      //Serial.println(remocon_num);
      remocon_finish_flag = 1;
    }

    diff_time[rem_int_count - 1] = int_time[rem_int_count] - int_time[rem_int_count - 1];
  }

  rem_int_count++;
  if (rem_int_count == 34)
  {
    rem_int_count = 0;
    for (int i = 0; i < 32; i++)
    {
      if (diff_time[i + 1] > 1000 && diff_time[i + 1] < 1500)
      {
        rem_bit[i] = 0;
      }
      else if (diff_time[i + 1] > 2000 && diff_time[i + 1] < 2500)
      {
        rem_bit[i] = 1;
      }
    }

    remocon_num = 0x00;

    for (int j = 0; j < 2; j++)
    {
      int count = 0;
      for (int i = 0; i < 8; i++)
      {
        if (rem_bit[i + 16] == rem_8bit_data[j][i]) count++;
        //Serial.println(rem_bit[i], HEX);
      }
      if (count == 8)
      {
        remocon_num = j + 0x30;
        remocon_finish_flag = 1;
        //Serial.println(remocon_num);
        break;
      }
    }

  }
}
