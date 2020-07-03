void setup()
{
  DDRC = 0x20; // 5
  DDRC &= ~0x04; // 2

  DDRD = 0xC0; // 7
}

unsigned long c_millis = 0;
unsigned long p_millis = 0;
int a = 0;
void loop() // 프린터 처음부터 만드는거 
{
  c_millis = millis();
  
  PORTD = 0x80;
  if(c_millis - p_millis > 1000)
  {
    a++;
    if(a == 2)
      a = 0;
    p_millis = c_millis;
  }

  if(a == 0){
    PORTC = 0x00;
  }
  else{
    PORTC = 0x20;
  }
     
  delayMicroseconds(200);
  PORTD = 0x00;
  delayMicroseconds(200);
}


// 대충적음
// 8 4 2 1 | 8 4 2 1  16진수
// 7 6 5 4 | 3 2 1 0  10진수
// 0 0 0 0 | 0 0 0 0   2진수