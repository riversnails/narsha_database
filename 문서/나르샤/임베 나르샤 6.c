unsigned long c_millis = 0;
unsigned long p_millis = 0;
int duty = 7;

void setup()
{
  DDRB |= 0x20;
  
  TCCR2A = 0x03;
  TCCR2B = 0x05;
  OCR2A = 25;
  TIMSK2 = 0x03;
}

void loop() // 마찬가지
{
  c_millis = millis();
  if(c_millis - p_millis > 10)
  {
    p_millis = c_millis;

    duty++;
    if(duty == 254) duty = 7;
  }
}

int count = 0;

ISR(TIMER2_OVF_vect)
{
  TCNT2 = 6;
  PORTB = 0x20;
  OCR2A = duty;
}

ISR(TIMER2_COMPA_vect)
{ 
  PORTB = 0x00;
}