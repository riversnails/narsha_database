
void setup()
{
	Serial.begin(9600);
	TCCR2A = 0x03; //fast PWM = CTC + ovf
	TCCR2B = 0x02; // 8분주
	TCNT2 = 56; // 100 us 마다
	OCR2A = 156; // 50 us 마다
	TIMSK2 = 0x03; // CTC : OCR2A
}

void loop() // fast PWM
{

}

int count = 0;
int count2 = 0;

SIGNAL(TIMER2_OVF_vect)
{
	TCNT2 = 56; // 56
	count2++;
	if(count2 == 10000) 
	{
		Serial.println("hello(1sec)");
		count2 = 0;
	}
}

SIGNAL(TIMER2_COMPA_vect)
{
	count++;
	if(count == 5000) 
	{
		Serial.println("hello(0.5s)");
		count = 0;
	}
}

// 8분주로 2Mhz 
// 주기 = 1/2Mhz 즉 반마이크로 == 0.5micro
// 이때 256번 반복하니 128마이크로
// 1 초는 7812