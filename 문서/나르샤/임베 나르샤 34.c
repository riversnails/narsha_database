#define encoder_a 0x04
#define encoder_b 0x08
#define encoder_button 0x01

void setup()
{
	DDRC &= ~encoder_button;
	DDRD &= ~(encoder_a | encoder_b); //핀 초기화
	PORTC |= encoder_button;
	PORTD |= encoder_a | encoder_b; // 풀업
	Serial.begin(9600);
}

int previous_state = 0;
int current_state = 0;
int rcount = 0;
int lcount = 0;
int toggle = 0;
unsigned int current_millis = 0;
unsigned int previous_millis = 0;

void loop() // lcd 버튼 회전및 클릭
{
	current_millis = millis();

	if(current_millis - previous_millis > 10)
	{
		previous_millis = current_millis;
		int a = PIND & encoder_a; // 값 읽어오기
		int b = PIND & encoder_b;
		int button = PINC & encoder_button;
		
		if(a != 0) a = 1; // 이상한 값이 나올 수 있으니 0이 아니면 1로 변환
		if(b != 0) b = 1;
		if(button != 0) button = 1;

		if(a == 0 && b == 1) current_state = 1;
		else if(a == 0 && b == 0) current_state = 2;
		else if(a == 1 && b == 0) current_state = 3;
		else if(a == 1 && b == 1) current_state = 4;


		if(current_state == 3 && previous_state == 2)
		{
			Serial.print("right ");
			Serial.println(rcount++);
		}
		if(current_state == 1 && previous_state == 2)
		{
			Serial.print("left ");
			Serial.println(lcount++);
		}
		if(button == 0 && toggle == 0)
		{
			toggle = 1;
			Serial.println("button down");
		}
		else if(button == 1 && toggle == 1)
		{
			toggle = 0;
			Serial.println("button up");
		}


		previous_state = current_state;
		// Serial.print(a);
		// Serial.println(b);
	}
}