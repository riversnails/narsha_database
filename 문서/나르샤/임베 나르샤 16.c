void setup()
{
  DDRC = 0x20 | 0x80 | 0x40; // x_dir, y_dir, y_step
  DDRC &= ~0x04; // x_stop
  DDRC &= ~0x08; // y_stop
  
  DDRD = 0xC0; // x_step, xyeenable
}

unsigned long c_millis = 0;
unsigned long p_millis = 0;
int a = 0;
void loop() // 프린터
{
  c_millis = millis();
  
  
  if(c_millis - p_millis > 2000)
  {
    a++;
    //if(a == 2)
    //  a = 0;
    p_millis = c_millis;
  }

  if(a == 0){
    PORTD = 0x80;
    PORTC = 0x40;
    delayMicroseconds(200);
    PORTD = 0x00;
    PORTC = 0x00;
    delayMicroseconds(200);
  }
  else{
    PORTD = 0x00;
    PORTC = 0x00;
    delayMicroseconds(200);
    PORTD = 0x00;
    PORTC = 0x00;
    delayMicroseconds(200);
  }
     
  
}


// 대충적음
// 8 4 2 1 | 8 4 2 1  16진수
// 7 6 5 4 | 3 2 1 0  10진수
// 0 0 0 0 | 0 0 0 0   2진수