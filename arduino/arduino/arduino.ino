#include"char_lcd.h"
#include"i2c.h"
#include"pcf8574_lcd.h"

#define SCL 19
#define SDA 18

char char_lcd_pin_8bit[11] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}; //rs, rw ,e ,db0 ~ db7
char char_lcd_pin_4bit[7] = {3, 4, 5, 10, 11, 12, 13}; //rs, rw ,e ,db0 ~ db7

void setup()
{
  Serial.begin(9600);
  //  char_lcd_init_4bit(char_lcd_pin_4bit);
  //  i2c_init(SCL, SDA);
  pcf8574_lcd_init();
}

unsigned long current_millis = 0;
unsigned long current_micros = 0;
unsigned long previous_millis = 0;

void loop()
{
  //i2c_8bit_data_write(0x27, 0x08);
    current_millis = millis();
    current_micros = micros();
    pcf8574_lcd_backlight_on_off(1);
    pcf8574_lcd_backlight_pwm(current_millis, current_micros, 20);
  
    if(current_millis - previous_millis > 1000)
    {
      previous_millis = current_millis;
  
      pcf8574_lcd_data_4bit(0x31);
    }
}
