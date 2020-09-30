unsigned long c_millis = 0;
unsigned long p_millis = 0;
int duty = 1;

void setup()
{
  DDRB |= 0x20;
  
  TCCR2A = 0x02;
  TCCR2B = 0x02;
  TIMSK2 = 0x02;
  OCR2A = 200;
  TCNT2 = 56;
}

void loop() // 이것도 4번과 비슷함
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
SIGNAL(TIMER2_COMPA_vect)
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