
void setup()
{
	pinMode(1, OUTPUT);

	digitalWrite(1, HIGH);
}

void loop() // tx, rx   bordrate 9600
{
	// start 1bit
	digitalWrite(1, LOW);
	delayMicroseconds(102);
	// data 8bit : 0x31
	digitalWrite(1, HIGH);
	delayMicroseconds(102);
	digitalWrite(1, LOW);
	delayMicroseconds(102);
	digitalWrite(1, LOW);
	delayMicroseconds(102);
	digitalWrite(1, LOW);
	delayMicroseconds(102);
	digitalWrite(1, HIGH);
	delayMicroseconds(102);
	digitalWrite(1, HIGH);
	delayMicroseconds(102);
	digitalWrite(1, LOW);
	delayMicroseconds(102);
	digitalWrite(1, LOW);
	delayMicroseconds(102);
	//stop 1bit
	digitalWrite(1, HIGH);
	delayMicroseconds(102);

	delay(1);
}

//------------------------------------

void setup()
{
	pinMode(0, INPUT);
	pinMode(1, OUTPUT);

	UCSR0A = 0x00;
	UCSR0B = 0x18;
	UCSR0C = 0x06;
	UBRR0 = 103;


}

unsigned long c_millis = 0;
unsigned long p_millis = 0;

void loop()
{
	c_millis = millis();
	if(c_millis - p_millis > 1000)
	{
		p_millis = c_millis;

		for(int i = 0; i < 10; i++)
		{
			while(!(UCSR0A & 0x20));
			UDR0 = 0x30 + i;
		}
	}
	
	if(UCSR0A & 0x80)
	{
		char in_data = UDR0;
		
		while(!(UCSR0A & 0x20));
		UDR0 = in_data;
	}
}