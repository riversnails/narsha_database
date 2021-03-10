#include "i2clcd.h"
#include "oled.h"
#include "servo.h"
#include "pwm.h"
#include "move.h"
#include "ultrasonic.h"
#include "remocon.h"
#include "keypad.h"

char block_location[6][2] =
{
  {40, 25},
  {70, 25},
  {100, 80},
  {70, 95},
  {40, 100},
  {10, 75}
};
char c_location = 0;
char p_location = 0;
char i = 1, j = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;
unsigned long p_millis_ultra = 0;
char select_toggle = 0;
char button_toggle = 0;
char d = 'a';
char p_d;
char r_d = 30;
char r_b_d;
char r_p_d;
int r_toggle = 0;
float ultrasonic_dist = 0;
char buff[10];

void setup()
{
  Serial.begin(115200);
  pinMode(53, OUTPUT); // mega
  pinMode(OLED_CS, OUTPUT);
  pinMode(OLED_RST, OUTPUT);
  pinMode(OLED_DC, OUTPUT);
  pinMode(OLED_SCK, OUTPUT);
  pinMode(OLED_DATA, OUTPUT);
  digitalWrite(OLED_SCK, LOW);

  pinMode(7, INPUT);
  digitalWrite(7, HIGH);
  spi_init();
  oled_1351_init();
  Clear_Screen(WHITE);
  Draw_Bitmap(0);
  delay(1000);
  Clear_Screen(WHITE);
  Draw_Bitmap(1);
  i2c_init();
  servo_init();
  pwm_init();
  ultrasonic_init();
  remocon_init();
  keypad_init();

  ADMUX |= 0x40;
  ADCSRA = 0x87;
  ADCSRB = 0x00;

}

char ck;
char keypad_ck;
char bt_ck;
char keypad_output;
int i2c_cnt = 0;

void loop()
{
  //  Serial.print(c_location, DEC);
  //  Serial.print(" ");
  //  Serial.println(p_location, DEC);
  c_millis = millis();
  if (Serial.available()) d = Serial.read();
  bt_ck = digitalRead(7);
  r_b_d = r_d;
  r_d = c_remocon();

  keypad_output = keypad_print(c_location);
  //Serial.println(keypad_output,DEC);
  if (keypad_output == -2) bt_ck = 0;
  else keypad_ck = keypad_output;

  ck = move_print(c_location, p_location, block_location);
  p_location = c_location;
  if (c_location != ck)
  {
    c_location = ck;
    select_toggle = 0;
  }
  else if (keypad_ck != ck)
  {
    c_location = keypad_ck;
    select_toggle = 0;
  }
  if (r_d == 'p' && r_toggle == 0) {
    c_location--;
    if (c_location == -1) c_location = 5;
    r_toggle = 1;
    r_d = r_b_d;
    set_remocon(0x30);
  }
  else if (r_d == 'n' && r_toggle == 0) {
    c_location++;
    if (c_location == 6) c_location = 0;
    r_toggle = 1;
    r_d = r_b_d;
    set_remocon(0x30);
  }
  else if (r_d == 'l' && r_toggle == 0) {
    bt_ck = 0;
    r_toggle = 1;
    r_d = r_b_d;
    set_remocon(0x30);
  }
  else r_toggle = 0;


  if (bt_ck == 0 && button_toggle == 0)
  {
    button_toggle = 1;
    if (c_location == 0)
    {
      if (select_toggle == 0) {
        select_toggle = 1;
        servo_start();
      }
      else {
        select_toggle = 0;
        servo_stop();
      }
    }
    else if (c_location == 1)
    {
      if (select_toggle == 0) {
        select_toggle = 1;
        pwm_start();
      }
      else {
        select_toggle = 0;
        pwm_stop();
      }
    }
    else if (c_location == 2)
    {
      if (select_toggle == 0) {
        select_toggle = 1;
      }
      else {
        select_toggle = 0;
        stop_all();
      }
    }
    else if (c_location == 3)
    {
      if (select_toggle == 0) {
        select_toggle = 1;
      }
      else {
        select_toggle = 0;
        stop_all();
      }
    }
    else if (c_location == 4)
    {
      if (select_toggle == 0) {
        select_toggle = 1;
      }
      else {
        select_toggle = 0;
        stop_all();
      }
    }
    else if (c_location == 5)
    {
      if (select_toggle == 0) {
        select_toggle = 1;
      }
      else {
        select_toggle = 0;
        stop_all();
      }
    }
  }
  else if (digitalRead(7) == 1 && button_toggle == 1) button_toggle = 0;

  if (c_millis - p_millis > 100)
  {
    p_millis = c_millis;

    if (c_location == 0)
    {
      servo_change(j++);
    }
    else if (c_location == 1)
    {
      pwm_change((i++) * 5);
    }
    else if (c_location == 2)
    {
      if (select_toggle == 1)
      {
        if (c_millis - p_millis_ultra > 1000)
        {
          p_millis_ultra = c_millis;
          ultrasonic_trigger();
        }
        if (ultrasonic_callback(&ultrasonic_dist))
        {
          string_write(40, 70, WHITE, buff);
          ultrasonic_callback_flag_clear();
          String s = (String)ultrasonic_dist;
          strcpy(buff, s.c_str());
          string_write(40, 70, BLACK, buff);
        }
      }
    }
    else if (c_location == 3)
    {
      if (select_toggle == 1)
      {
        if (p_d != d) font_write(95, 105, WHITE, p_d);
        font_write(95, 105, BLACK, d);
        p_d = d;
      }
      else if (select_toggle == 0) font_write(95, 105, WHITE, d);
    }
    else if (c_location == 4)
    {
      if (select_toggle == 1)
      {
        lcd_data_set(0x08);
        i2c_cnt++;
        if(i2c_cnt == 10)
        {
          i2c_cnt = 0;
          lcd_data_set(0x00);
        }
      }
      else if (select_toggle == 0) lcd_data_set(0x00);

    }
    else if (c_location == 5)
    {
      if (select_toggle == 1)
      {
        if (r_p_d != r_d) font_write(50, 70, WHITE, r_p_d);
        font_write(50, 70, BLACK, r_d);
        r_p_d = r_d;
      }
      else if (select_toggle == 0) font_write(50, 70, WHITE, r_d);
    }
    if (j == 20) j = 3;
    if (i == 20) i = 1;

  }
}

void stop_all()
{
  servo_stop();
  pwm_stop();
  string_write(40, 70, WHITE, buff);
  font_write(95, 105, WHITE, p_d);
  font_write(50, 70, WHITE, r_p_d);
  lcd_data_set(0x00);
}
