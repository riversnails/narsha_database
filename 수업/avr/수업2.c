
void setup()
{
	Serial.begin(9600);
	TCCR2A = 0x02; //CTC
	TCCR2B = 0x02; // 8분주
	//TCNT2 = 256 - 200; // 56
	TCNT2 = 0;
	OCR2A = 200; // 200에서 인터럽트 발생
	TIMSK2 = 0x02; // CTC : OCR2A
}

void loop() // timer ovf, ctc
{

}

int count = 0;
int count2 = 0;

SIGNAL(TIMER2_OVF_vect)
{
	count2++;
	if(count2 == 7812) 
	{
		Serial.println("hello2");
		count2 = 0;
	}
}

SIGNAL(TIMER2_COMPA_vect)
{
	//TCNT2 = 256 - 200; // 56
	count++;
	if(count == 10000) 
	{
		Serial.println("hello");
		count = 0;
	}
}

// 8분주로 2Mhz 
// 주기 = 1/2Mhz 즉 반마이크로 == 0.5micro
// 이때 256번 반복하니 128마이크로
// 1 초는 7812