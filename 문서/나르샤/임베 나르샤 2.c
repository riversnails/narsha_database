/*
void setup() // 1번 문제
{
	//8 4 2 1   8 4 2 1
	//7 6 5 4 | 3 2 1 0
	//0 0 0 0 | 0 0 0 0
	DDRB = 0x20 | 0x10;// 따로 따로 넣으면 덮어쓰기로 되는 샘이니 or로 처리해준다
	//DDRB = 0x10;
	PORTB = 0x20;
}
int count = 0;
void loop()
{
	PORTB = 0x20 | 0x10;
	delay(1000);
	PORTB = PORTB & (~0x10);
	delay(1000);
}
*/


void setup()// 내가 만든 버러지코드
{
	//8 4 2 1   8 4 2 1
	//7 6 5 4 | 3 2 1 0
	//0 0 0 0 | 0 0 0 0
	DDRB = 0x20 | 0x10;// 따로 따로 넣으면 덮어쓰기로 되는 샘이니 or로 처리해준다
	//DDRB = 0x10;
    Serial.begin(9600);
}
int pin13 = 0, pin12 = 0;
void loop()
{
	if(pin13 == 0)
	{
		PORTB = PORTB | 0x20;
	}
	else if(pin13 == 2)
	{
		PORTB = PORTB & (~0x20);
	}
	else if(pin13 == 3)
	{
		pin13 = -1;
	}

	if(pin12 == 0)
	{
		PORTB = PORTB | 0x10;
		pin12++;
		pin13++;
	}
	else if(pin12 == 1)
	{
		PORTB = PORTB & (~0x10);
		pin12 = 0;
		pin13++;
	}
	delay(500);
  	Serial.println(pin13);
    Serial.println(pin12);
    Serial.println("--------------------");
}



/*
void setup()// 내가 만든 버러지코드
{
	//8 4 2 1   8 4 2 1
	//7 6 5 4 | 3 2 1 0
	//0 0 0 0 | 0 0 0 0
	DDRB = 0x20 | 0x10;// 따로 따로 넣으면 덮어쓰기로 되는 샘이니 or로 처리해준다
	//DDRB = 0x10;
}
void loop()
{
	for(int i = 0; i < 4; i++)
	{
		if(i == 0)
			PORTB = 0x20 | 0x10;
		else if(i == 1)
			PORTB = PORTB & (~(0x10));
		else if(i == 2)
		{
			PORTB = PORTB & (~(0x20));
			PORTB = 0x10;
		}
		else if(i == 3)
			PORTB = PORTB & (~(0x10));

		delay(500);
	}

}
*/


/*  캐이스로 포문 왜씀 형태
void setup()
{
	//8 4 2 1   8 4 2 1
	//7 6 5 4 | 3 2 1 0
	//0 0 0 0 | 0 0 0 0
	DDRB = 0x20 | 0x10;// 따로 따로 넣으면 덮어쓰기로 되는 샘이니 or로 처리해준다
	//DDRB = 0x10;
}
void loop()
{
	for(int i = 0; i < 1; i++)
	{
		PORTB = 0x20 | 0x10;
		delay(500);
		PORTB = PORTB & (~0x10);
		delay(500);
		PORTB = PORTB & (~0x20);
		PORTB = 0x10;
		delay(500);
		PORTB = PORTB & (~(0x20 | 0x10));
		delay(500);
	}
}
*/