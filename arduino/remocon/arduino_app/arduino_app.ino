#include "keymatrix.h"

char key_pin[8] = {2, 3, 4, 5, 6, 7, 8, 9};

void setup()
{
  Serial.begin(9600);
  keymatrix_init(key_pin);

}

unsigned long c_micros = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;
int in_num = 0;

void loop()
{
  c_millis = millis();
  char key_in = 0;
  keymatrix_input_millis(&key_in, c_millis);
  if(key_in != 0)
  {
    Serial.printf("key %d\n", key_in);
  }
}
