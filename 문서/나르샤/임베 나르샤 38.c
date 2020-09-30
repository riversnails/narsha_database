void setup()
{
	DDRD |= 0x10;
	PORTD |= 0x10;

	DDRD |= 0x20;
	PORTD |= 0x20;

	DDRB |= 0x10;

	PORTB |= 0x10;
	Serial.begin(9600);
}

void loop() // 배드, 노즐 온도 태스트
{
	int bed_analog_value = analogRead(A6);
	int end_analog_value = analogRead(A7);

	Serial.print("bed : ");
	Serial.print(bed_analog_value);
	Serial.print(" end : ");
	Serial.println(end_analog_value);

	if(bed_analog_value <= 920) PORTD &= ~0x10;
	else PORTD |= 0x10;

	if(end_analog_value <= 100) 
	{
		PORTD &= ~0x20;
	}
	else 
	{
		PORTD |= 0x20;
	}

	delay(300);
}