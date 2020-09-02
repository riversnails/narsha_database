// 1125 mm / min

// 1125 / 60 = 18.75 mm / sec

// 3200 step : 20 개 = 40 mm

// 1 step : 0.0125

// 18.75 / 0.0125 = 1500 step

// 1s : 1500 step = x sec : 1step
// xsec = 1s / 1500 = 666.666 us
// => 반스텝 = 666.666 / 2 = 333 us
// //-----------------------------------------

// (1125 / 60 / 0.0125) ==> 초당 step 수
// 1000000 / (1125 / 60 / 0.0125) / 2 = 반스텝 (반마이크로)
// -> 1000000 / (x/60/0.0125)/2 = 반스텝
// -> (1000000/x*60*0.0125)/2
// -> (1000000*60*0.0125/2/x)
// -> (375000 /x) = 반스

// // -> 1000000 / (x/0.75)/2 => 500000(x/0.75)
// // => 1000000 * 2400 / x = 

// //-----------------------------------------

// ==> 1 step = 666 us 
// ==> 1 sec -> 1500 step
// ==> 1 min -> 1500 * 60 = 90,000 step
// ==> 길이 : 90,000 step * 0.0125 = 1125 mm

void setup()
{
	// Extruder
	DDRD |= 0x40; // XYEenable

	DDRB |= 0x03; //EDIR & ESTEP
	PORTB &= ~0x01;

	DDRD |= 0x10;
	PORTD |= 0x10;
	// Thermister
	DDRD |= 0x20;
	PORTD |= 0x20;

	DDRB |= 0x10; // fan

	TCCR2A = 0x02;
	TCCR2B = 0x05; // 64분주
	TCNT2 = 0x00;
	OCR2A = 100;
	//TIMSK2 = 0x02;
}

void loop() // extruder 태스트
{
	int bed_analog_value = analogRead(A6);
	int end_analog_value = analogRead(A7);

	if(bed_analog_value <= 920) PORTD &= ~0x10;
	else PORTD |= 0x10;

	if(end_analog_value < 95) 
	{
		PORTD &= ~0x20;
		PORTB |= 0x10; // fan
		TIMSK2 = 0x02;
	}
	else if(end_analog_value > 100)
	{
		PORTD |= 0x20;
		PORTB &= ~0x10; // fan
		//TIMSK2 = 0x00;
	}
}

volatile char toggle = 0;

ISR(TIMER2_COMPA_vect){
	if(toggle == 0)
	{
		toggle = 1;
		PORTB |= 0x02;
	}
	else
	{
		toggle = 0;
		PORTB &= ~0x02;
	}
}