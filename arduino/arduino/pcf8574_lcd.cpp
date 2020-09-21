#include "pcf8574_lcd.h"
#include "i2c.h"

#define SCL 19
#define SDA 18

unsigned char pcf8574_lcd = 0;

void pcf8574_lcd_init(){
  i2c_init(SCL, SDA);
  
  //digitalWrite(RS, HIGH);
  pcf8574_lcd |= 0x01;
  i2c_8bit_data_write(0x27, pcf8574_lcd);// data write에서 데이터의 끝을 1로 설정 0000 0001
  //digitalWrite(RW, LOW);
  pcf8574_lcd &= ~0x02; // LOW 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  //이친구는 4bit로 이뤄져있음.

  pcf8574_lcd_inst_8bit(0x30);
  pcf8574_lcd_inst_8bit(0x30);
  pcf8574_lcd_inst_8bit(0x30);
  pcf8574_lcd_inst_8bit(0x28);
  
  pcf8574_lcd_inst_4bit(0x28);
  pcf8574_lcd_inst_4bit(0x06);
  pcf8574_lcd_inst_4bit(0x0c);
  pcf8574_lcd_inst_4bit(0x80);
  pcf8574_lcd_inst_4bit(0x01);
  delay(10);
  
}
//============================================================== INST

void pcf8574_lcd_inst_8bit(unsigned char inst){
  //digitalWrite(RS, LOW);
  pcf8574_lcd &= ~0x01; // RS LOW 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  pcf8574_lcd &= ~0xf0;
  pcf8574_lcd |= inst & 0xf0; // 8bit 포함.
  i2c_8bit_data_write(0x27, pcf8574_lcd);

  pcf8574_lcd |= 0x04; // E HIGH 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  pcf8574_lcd &= ~0x04; // E LOW 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  //digitalWrite(RS, HIGH);
  pcf8574_lcd |= 0x01; // RS HIGH
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  delayMicroseconds(50); // delay 50us
}


void pcf8574_lcd_inst_4bit(unsigned char inst) {
  pcf8574_lcd_inst_4bit_high(inst);
  pcf8574_lcd_inst_4bit_low(inst);
}

void pcf8574_lcd_inst_4bit_high(unsigned char inst){ // High bit
  //digitalWrite(RS, LOW);
  pcf8574_lcd &= ~0x01; // RS LOW 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  pcf8574_lcd &= ~0xf0;
  pcf8574_lcd |= (inst & 0xf0); // 상위 4bit 포함.
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  
  pcf8574_lcd |= 0x04; // E HIGH 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  pcf8574_lcd &= ~0x04; // E LOW 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  //digitalWrite(RS, HIGH);
  pcf8574_lcd |= 0x01; // RS HIGH
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  delayMicroseconds(50); // delay 50us
}

void pcf8574_lcd_inst_4bit_low(unsigned char inst){ // High bit
  //digitalWrite(RS, LOW);
  pcf8574_lcd &= ~0x01; // RS LOW 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  pcf8574_lcd &= ~0xf0;
  pcf8574_lcd |= ((inst & 0x0f) << 4); // 하위 4bit 포함.
  i2c_8bit_data_write(0x27, pcf8574_lcd);

  pcf8574_lcd |= 0x04; // E HIGH 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  pcf8574_lcd &= ~0x04; // E LOW 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  //digitalWrite(RS, HIGH);
  pcf8574_lcd |= 0x01; // RS HIGH
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  delayMicroseconds(50); // delay 50us
}

//======================================================= DATA
void pcf8574_lcd_data_4bit(unsigned char data) {
  pcf8574_lcd_data_4bit_high(data);
  pcf8574_lcd_data_4bit_low(data);
}

void pcf8574_lcd_data_4bit_high(unsigned char data){ // High bit

  pcf8574_lcd &= ~0xf0;
  pcf8574_lcd |= (data & 0xf0); // 상위 4bit 포함.
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  
  pcf8574_lcd |= 0x04; // E HIGH 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  pcf8574_lcd &= ~0x04; // E LOW 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  delayMicroseconds(50); // delay 50us
}

void pcf8574_lcd_data_4bit_low(unsigned char data){ // High bit

  pcf8574_lcd &= ~0xf0;
  pcf8574_lcd |= ((data & 0x0f) << 4); // 하위 4bit 포함.
  i2c_8bit_data_write(0x27, pcf8574_lcd);

  pcf8574_lcd |= 0x04; // E HIGH 
  i2c_8bit_data_write(0x27, pcf8574_lcd);
  
  pcf8574_lcd &= ~0x04; // E LOW 
  i2c_8bit_data_write(0x27, pcf8574_lcd);

  delayMicroseconds(50); // delay 50us
}

//==============================================================

void pcf8574_lcd_backlight_on_off(unsigned char on_off) {
  // 1 On 0 OFF
  if(on_off == 1) {
    pcf8574_lcd |= 0x08;
    i2c_8bit_data_write(0x27, pcf8574_lcd); // 3번 bit
  }else if(on_off == 0){
    pcf8574_lcd &= ~0x08;
    i2c_8bit_data_write(0x27, pcf8574_lcd); // 3번 bit
  }
}

//==============================================================

unsigned long p_millis = 0;

unsigned long p_micros = 0;

unsigned char pcf8574_lcd_pwm_count = 0;
unsigned char pcf8574_lcd_pwm_duty = 10;

void pcf8574_lcd_backlight_pwm(unsigned long c_millis, unsigned long c_micros, unsigned int pwm_cycle){ // infinity loop
  //c_micros - 작동을 위함임.
  //c_millis - duty의 가변을 위함임.
  if(c_micros - p_micros > 100){
    p_micros = c_micros;
    if(pcf8574_lcd_pwm_count == 100) {
      // HIGH 로 올림.
      pcf8574_lcd_pwm_count = 0;
      pcf8574_lcd_backlight_on_off(1);
    }else if( pcf8574_lcd_pwm_count == pcf8574_lcd_pwm_duty) {
      pcf8574_lcd_backlight_on_off(0);
    }
    pcf8574_lcd_pwm_count++;
  }
  
  if(c_millis - p_millis > pwm_cycle) {
    p_millis = c_millis;
    pcf8574_lcd_pwm_duty++;
    if(pcf8574_lcd_pwm_duty == 99){
      pcf8574_lcd_pwm_duty = 1;
    }
  }
  
}
