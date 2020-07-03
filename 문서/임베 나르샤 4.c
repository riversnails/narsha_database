unsigned long c_millis = 0;
unsigned long p_millis = 0;
int duty = 1;

void setup()
{
  DDRB |= 0x20;
  
  TCCR2A = 0x00;
  TCCR2B = 0x02;
  TIMSK2 = 0x01; 
  TCNT2 = 56;
}

void loop() // 타이머로 깜빡이고 루프에서 밝기조절하는 코드
{
  c_millis = millis();
  if(c_millis - p_millis > 10)
  {
    p_millis = c_millis;

    duty++;
    if(duty == 99) duty = 1;
    
  }
}

int count = 0;
int toggle = 0;

SIGNAL(TIMER2_OVF_vect)
{
  count++;
  if(count == 100)
  {
    count = 0;
    PORTB = 0x20;
  }
  else if(count == duty)
  {
    PORTB = 0x00;
  }
}