#include "remocon.h"
#include "ultrasonic.h"
#include "i2c.h"

#define REMOCON_PIN 2
#define ULTRASONIC_ECHO_PIN 3
#define ULTRASONIC_TRIGGER_PIN 4

float ultrasonic_dist = 0;

char remocon_number = 0;
unsigned long c_millis = 0;
unsigned long p_millis_ultra = 0;

void setup()
{
  Serial.begin(9600);
  remocon_init(REMOCON_PIN); // arduino pin 2
  ultrasonic_init(ULTRASONIC_ECHO_PIN, ULTRASONIC_TRIGGER_PIN); // arduino pin 3

  i2c_init();
//  while(1)
//  {
//    i2c_byte_write(0x27, 0x08);
//    delay(1000);
//    i2c_byte_write(0x27, 0x00);
//    delay(1000);
//  }
}

void loop()
{
  c_millis = millis();
  if (c_millis - p_millis_ultra > 1000)
  {
    p_millis_ultra = c_millis;
    ultrasonic_trigger();
  }

  // ultrasonic_callback func
  if (ultrasonic_callback(&ultrasonic_dist))
  {
    ultrasonic_callback_flag_clear();
    Serial.println(ultrasonic_dist);
  }
  
  // remocon_callback func
  if (remocon_callback(&remocon_number))
  {
    int_callback_flag_clear();
    Serial.println(remocon_number);
  }

}
