
void setup()
{
	// 0 : arduino 2, 1: arduino 3

	//attachInterrupt(0, myISR, FALLING);
	DDRD &= ~0x04;
	//EICRA = 0x02; // falling edge
	EICRA = 0x01; // falling/rising edge
	EIMSK = 0x01;
	Serial.begin(9600);
}

void loop()
{

}

// void myISR()
// {
// 	Serial.println("button down");
// }

int toggle = 0;

SIGNAL(INT0_vect)
{
	// int in_value = 0;
	// unsigned char in_data = 0;

	// in_value = digitalRead(2);
	// if(in_value == 0) Serial.println("button down");
	// else Serial.println("button up");

	unsigned char in_data = 0;

	in_data = PIND;

	if(in_data & 0x04 && toggle == 0)
	{
		toggle = 1;
		Serial.println("button up");
	} 
	else if(toggle == 1) 
	{
		toggle = 0;
		Serial.println("button down");
	}

}

// 7 6 5 4 | 3 2 1 0
// 1 1 1 1 | 1 1 1 1
// 8 4 2 1 | 8 4 2 1
// DDR, PORT, PIN

