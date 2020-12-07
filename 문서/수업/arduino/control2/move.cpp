#include "move.h"
#include "oled.h"
#include "main1.h"
#include "main2.h"

char move_c_location = 0;
char joy_toggle = 0;
int move_x = 0, move_y = 0;
int p_move_x = 0, p_move_y = 0;
boolean pointor[17][11] = {
  {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0},
  {1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0},
  {1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
};
char move_toggle = 0;

char move_print(char loc, char arr[][2])
{
  move_c_location = loc;
  if (Serial.available()) {
    char dir = Serial.read();
    if (dir == ' ')
    {
      make_rect(arr[move_c_location][0], arr[move_c_location][1], WHITE);
      move_c_location++;
      if (move_c_location == 6) move_c_location = 0;
      make_rect(arr[move_c_location][0], arr[move_c_location][1], BLUE);
    }
  }

  if (adc_read(0) < 300)
  {
    if ((move_x - 1) != -1)
    {
      p_move_x = move_x;
      move_x--;
      move_toggle = 1;
    }
  }
  else if (adc_read(0) > 700)
  {
    if ((move_x + 1) != 118)
    {
      p_move_x = move_x;
      move_x++;
      move_toggle = 1;
    }
  }
  else if (adc_read(1) < 300)
  {
    if ((move_y - 1) != -1)
    {
      p_move_y = move_y;
      move_y--;
      move_toggle = 1;
    }

  }
  else if (adc_read(1) > 700)
  {
    if ((move_y + 1) != 118)
    {
      p_move_y = move_y;
      move_y++;
      move_toggle = 1;
    }
  }
  else
  {
    joy_toggle = 0;
  }
  pointor_reload();

    Serial.print(move_x);
    Serial.print(" ");
    Serial.print(move_y);
    Serial.print("  ");
    Serial.print(p_move_x);
    Serial.print(" ");
    Serial.println(p_move_y);

  if(move_x >= 42 && move_y >= 23)
  else if()
  else if()
  else if()
  else if()
  else if()
  return (move_c_location);
}

void pointor_reload()
{
  if (move_toggle == 1)
  {
    move_toggle = 0;
    int d = 0;
    for (int j = 0; j < 19; j++)
    {
      for (int i = 0; i < 13; i++)
      {
        if (p_move_x == 0 && i == 0) continue;
        if (p_move_y == 0 && j == 0) continue;
        if (j >= 2 && i >= 2 && j <= 18 && i <= 12)
        {
          if (pointor[j - 1][i - 1] == 1)
          {
            //continue;
          }
        }
        if (p_move_y + j > 64) put_pixel_two(p_move_x + i - 1, p_move_y + j - 1, pgm_read_byte(&main2[0x46 + ((p_move_x + i - 1) * 2 + (p_move_y + j - 65) * 128 * 2) + 1]), pgm_read_byte(&main2[0x46 + ((p_move_x + i - 1) * 2 + (p_move_y + j - 65) * 128 * 2)]));
        else put_pixel_two(p_move_x + i - 1, p_move_y + j - 1, pgm_read_byte(&main1[0x46 + ((p_move_x + i - 1) * 2 + (p_move_y + j - 1) * 128 * 2) + 1]), pgm_read_byte(&main1[0x46 + ((p_move_x + i - 1) * 2 + (p_move_y + j - 1) * 128 * 2)]));
      }
    }
  }

  for (int j = 0; j < 17; j++)
  {
    for (int i = 0; i < 11; i++)
    {
      if (pointor[j][i] == 1) put_pixel(move_x + i, move_y + j, BLACK);
      //else put_pixel(move_x + i, move_y + j, WHITE);
    }
  }


  //  if (move_toggle == 1)
  //  {
  //    int d = 0;
  //    for (int j = 0; j < 17; j++)
  //    {
  //      for (int i = 0; i < 11; i++)
  //      {
  //        backup[j][d] = pgm_read_byte(&main1[0x46 + ((p_move_x + i) * 2 + (p_move_y + j) * 128 * 2) + 1]);
  //        backup[j][d + 1] = pgm_read_byte(&main1[0x46 + ((p_move_x + i) * 2 + (p_move_y + j) * 128 * 2)]);
  //        d += 2;
  //      }
  //    }
  //  }
  //  if (move_toggle == 1)
  //  {
  //    move_toggle = 0;
  //    int d = 0;
  //    for (int j = 0; j < 17; j++)
  //    {
  //      for (int i = 0; i < 11; i++)
  //      {
  //        if (pointor[j][i] == 0)
  //        {
  //          put_pixel_two(p_move_x + i, p_move_y + j, backup[j][d], backup[j][d + 1]);
  //          d += 2;
  //        }
  //      }
  //    }
  //  }
}

unsigned short adc_read(char channel)
{
  unsigned short adc_value = 0;

  ADMUX &= ~0x0f;
  ADMUX |= channel;

  ADCSRA |= 0x40;

  while (!(ADCSRA & 0x10));

  adc_value = ADC;
  return adc_value;
}
