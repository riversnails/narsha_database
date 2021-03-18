#include "remocon.h"

char rem_int_count = 0;
volatile unsigned long int_time[40];
int diff_time[33];
char rem_bit[32];
char remocon_num = 0x00;
int remocon_finish_flag = 0;

char remocon_number[21][8] = {
  {0, 1, 1, 0, 1, 0, 0, 0}, // 0
  {0, 0, 1, 1, 0, 0, 0, 0}, // 1
  {0, 0, 0, 1, 1, 0, 0, 0}, // 2
  {0, 1, 1, 1, 1, 0, 1, 0}, // 3
  {0, 0, 0, 1, 0, 0, 0, 0}, // 4
  {0, 0, 1, 1, 1, 0, 0, 0}, // 5
  {0, 1, 0, 1, 1, 0, 1, 0}, // 6
  {0, 1, 0, 0, 0, 0, 1, 0}, // 7
  {0, 1, 0, 0, 1, 0, 1, 0}, // 8
  {0, 1, 0, 1, 0, 0, 1, 0}, // 9
  {1, 0, 0, 1, 1, 0, 0, 0}, // 100+
  {1, 0, 1, 1, 0, 0, 0, 0}, // 200+
  {1, 1, 1, 0, 0, 0, 0, 0}, // -
  {1, 0, 1, 0, 1, 0, 0, 0}, // +
  {1, 0, 0, 1, 0, 0, 0, 0}, // EQ
  {0, 0, 1, 0, 0, 0, 1, 0}, // PREV
  {0, 0, 0, 0, 0, 0, 1, 0}, // NEXT
  {1, 1, 0, 0, 0, 0, 1, 0}, // PLAY/PAUSE
  {1, 0, 1, 0, 0, 0, 1, 0}, // CH-
  {0, 1, 1, 0, 0, 0, 1, 0}, // CH
  {1, 1, 1, 0, 0, 0, 1, 0} // CH+

};

void remocon_init()
{
  pinMode(REMOCON_PIN, INPUT);
  EICRB |= 0x08;
  EIMSK |= 0x20;
}

char c_remocon()
{
  return remocon_num;
}

void set_remocon(char len)
{
  remocon_num = len;
}

void remoconISR()
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
    }

    diff_time[rem_int_count - 1] = int_time[rem_int_count] - int_time[rem_int_count - 1];
  }

  rem_int_count++;
  if (rem_int_count == 34)
  {
    //Serial.println();
    rem_int_count = 0;

    for (int i = 0; i < 32; i++)
    {
      if (diff_time[i + 1] > 1000 && diff_time[i + 1] < 1500)
      {
        rem_bit[i] = 0;
      }
      else if (diff_time[i + 1] > 2000 && diff_time[i + 1] < 2500) {
        rem_bit[i] = 1;
      }
      //Serial.println(diff_time[i + 1]);
    }


//    for (int i = 0; i < 32; i++)
//    {
//      Serial.print(int(rem_bit[i]));
//    }
//    Serial.println();

    remocon_num = 0x00;
    for (int j = 0; j < 21; j++)
    {
      int count = 0;
      for (int i = 0; i < 8; i++)
      {
        if (rem_bit[i + 16] == remocon_number[j][i]) count++;

      }
      if (count == 8) {
        if(j == 15) // prev
        {
          remocon_num = 'p';
        }
        else if(j == 16)// next
        {
          remocon_num = 'n';
        }
        else if(j == 17)
        {
          remocon_num = 'l';
        }
        else remocon_num = j + 0x30;
        remocon_finish_flag = 1;
//        Serial.print("Data : ");
//        Serial.println(remocon_num);
        break;
      }
    }
  }
}

SIGNAL(INT5_vect)
{
  remoconISR();
}
