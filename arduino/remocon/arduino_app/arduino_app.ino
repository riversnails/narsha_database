#include"gpio.h"
#include "fnd.h"

//fnd : a,b,c,d,e,f,g,dp
char fnd_pin_1[8] = {4, 5, 6, 7, 8, 9, 10, 11};
//fnd : a,b,c,d,e,f,g,dp,common1,common2,common3,common4
char fnd_pin_2[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
char fnd_pin[12] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

void setup()
{
  Serial.begin(9600);
  //fnd_init_1(fnd_pin_1, C_CATHODE);
  //fnd_init_2(fnd_pin_2, C_ANODE);
  fnd_init(4, fnd_pin, C_ANODE);
}


int fndNumber = 0;
int c_millis = 0;
int p_millis = 0;

void loop()
{
  c_millis = millis();
  //  fnd_num_1(2);
  //  delay(1000);
  //  fnd_num_1(3);
  //  delay(1000);

  if (c_millis - p_millis > 50)
  {
    p_millis = c_millis;
    if (fndNumber == 10000) fndNumber= 0;
    fndNumber++;
  }
  fnd_num_millis(c_millis, fndNumber);
}
