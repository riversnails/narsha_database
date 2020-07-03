unsigned long c_millis = 0;
unsigned long p_millis = 0;
int duty = 1024 - 500;

void setup()
{
  DDRB |= 0x20;
  
  TCCR1A = 0x03;
  TCCR1B = 0x0B;
  TIMSK1 = 0x03;
  OCR1A = 1024 - 500 + 50;
  TCNT1 = 1024 - 500;
  //TCNT2 = 256 - 64;
  
}

void loop() // 오버플로우 타이머인데 비트가 올라간거 == timsk1
{
  c_millis = millis();
  if(c_millis - p_millis > 10)
  {
    p_millis = c_millis;

    duty+=5;
    if(duty > 1023) duty = 1024 - 500;
  }
}

int count = 0;

 ISR(TIMER1_OVF_vect)
 {
   TCNT1 = 1024 - 500;

   PORTB = 0x20;

   OCR1A = duty;
 }

ISR(TIMER1_COMPA_vect)
{ 
   PORTB = 0;
}