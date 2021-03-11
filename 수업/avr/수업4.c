
void setup()
{
	// Serial.begin(9600);
	TCCR2A = 0x00; // ovf
	TCCR2B = 0x02; // 8분주
	TCNT2 = 256 - 40; // 216
	TIMSK2 = 0x01;

	//pinMode(13, OUTPUT);
	DDRB |= 0x20;
}

unsigned long c_micros = 0;
unsigned long p_micros = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;
int count = 0;
int duty = 1;

void loop() // 2ms 주기, 10% duty
{
	//c_micros = micros();
	c_millis = millis();

	// if(c_micros - p_micros > 20)
	// {
	// 	p_micros = c_micros;

	// 	if(count == 100)
	// 	{
	// 		digitalWrite(13, HIGH);
	// 		count = 0;
	// 	}
	// 	else if(count == duty)
	// 	{
	// 		digitalWrite(13, LOW);
	// 	}

	// 	count++;
	// }

	if(c_millis - p_millis > 10)
	{
		p_millis = c_millis;

		duty++;
		if(duty == 99) duty = 1;
	}
}

int count_ovf = 0;
SIGNAL(TIMER2_OVF_vect)
{
	TCNT2 = 256 - 40; // 216
	count_ovf++;
	if(count_ovf == 100)
		{
			//digitalWrite(13, HIGH);
			PORTB |= 0x20;
			count_ovf = 0;
		}
		else if(count_ovf == duty)
		{
			//digitalWrite(13, LOW);
			PORTB &= ~0x20;
		}
}

// 8분주로 2Mhz 
// 주기 = 1/2Mhz 즉 반마이크로 == 0.5micro
// 이때 256번 반복하니 128마이크로
// 1 초는 7812