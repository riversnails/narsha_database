
void setup()
{
	// Serial.begin(9600);
	TCCR2A = 0x03; // fastPWM
	TCCR2B = 0x05; // 128분주
	TCNT2 = 6; // 250개 = 2millis
	OCR2A = 25; // 10%
	TIMSK2 = 0x03;

	//pinMode(13, OUTPUT);
	DDRB |= 0x20;
}

unsigned long c_micros = 0;
unsigned long p_micros = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;
int count = 0;
int duty = 1;

int duty_value = 1;

void loop() // 2ms 주기, 가변 duty
{
	//c_micros = micros();
	c_millis = millis();

	if(c_millis - p_millis > 5)
	{
		p_millis = c_millis;

		duty_value++;
		if(duty_value > 249) duty_value = 1;
	}
}

int count_ovf = 0;
int count_cmp = 0;

SIGNAL(TIMER2_OVF_vect)
{
	OCR2A = duty_value;
	PORTB |= 0x20;
}

SIGNAL(TIMER2_COMPA_vect)
{
	PORTB &= ~0x20;
}

// 8분주로 2Mhz 
// 주기 = 1/2Mhz 즉 반마이크로 == 0.5micro
// 이때 256번 반복하니 128마이크로
// 1 초는 7812