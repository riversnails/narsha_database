#define ROW_0 12
#define COL_0 4

char value_0 = 0x23;
char value_1 = 0x37;
char num_2[8] = {0x00,0x38,0x44,0x04,0x08,0x10,0x20,0x7c};
char num_3[8] = {0x00,0x38,0x44,0x04,0x18,0x04,0x44,0x38};

char num[2][8] = {  //{0x00,0x10,0x44,0x04,0x08,0x10,0x20,0x7c}, 
					{0x00,0x38,0x44,0x04,0x08,0x10,0x20,0x7c}, 
					{0x00,0x38,0x44,0x04,0x18,0x04,0x44,0x38} };

int count = 0;
int dot_num = 0;

void setup()
{
	for(int i = 0; i < 16; i++)
	{
		pinMode(COL_0+i, OUTPUT);
	}
}

void loop() // 도트 메트릭스 사용
{
	for(int j = 0; j < 8; j ++) // 껏다 켯다 반복으로 키패드처럼 여러개 켜지는걸 방지한다.
	{
		// 1. all row off
		for(int i = 0; i < 8; i++)
		{
			digitalWrite(ROW_0+i, LOW);
		}

		// 2. 첫번째 줄 값
		for(int i = 0; i < 8; i++)
		{
			if(num[dot_num][j] & (0x80 >> i))
			{
				digitalWrite(COL_0+i, LOW);
			}
			else
			{
				digitalWrite(COL_0+i, HIGH);
			}
		}

		// 3. 첫 번째 줄 power on
		digitalWrite(ROW_0 + j, HIGH);

		delay(1);
	}

	count++;
	if(count == 125)
	{
		count = 0;
		dot_num++;
		if (dot_num == 9)
			dot_num = 0;
	}
}