#define ROW_0 12
#define COL_0 4

char value_0 = 0x23;
char value_1 = 0x37;
void setup()
{
	for(int i = 0; i < 16; i++)
	{
		pinMode(COL_0+i, OUTPUT);
	}
}

void loop() // 도트 메트릭스 사용
{
	// for(int i = 0; i < 8; i++)
	// {
	// 	digitalWrite(i+4, LOW);
	// 	digitalWrite(i+12, HIGH);
	// }

	// 1. all row off
	for(int i = 0; i < 8; i++)
	{
		digitalWrite(ROW_0+i, LOW);
	}

	// 2. 첫번째 줄 값
	for(int i = 0; i < 8; i++)
	{
		if(value_0 & (0x80 >> i))
		{
			digitalWrite(COL_0+i, LOW);
		}
		else
		{
			digitalWrite(COL_0+i, HIGH);
		}
	}

	// 3. 첫 번째 줄 power on
	digitalWrite(ROW_0, HIGH);

	delay(1);

	//2번째 코드
	// 1. all row off
	for(int i = 0; i < 8; i++)
	{
		digitalWrite(ROW_0+i, LOW);
	}

	// 2. 첫번째 줄 값
	for(int i = 0; i < 8; i++)
	{
		if(value_1 & (0x80 >> i))
		{
			digitalWrite(COL_0+i, LOW);
		}
		else
		{
			digitalWrite(COL_0+i, HIGH);
		}
	}

	// 3. 첫 번째 줄 power on
	digitalWrite(ROW_0 +1, HIGH);

	delay(1);
}