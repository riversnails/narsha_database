#define ROW_0 12
#define COL_0 4

unsigned char parking[7][8] = {
	{0x00, 0x38, 0x44, 0x4C, 0x54, 0x64, 0x44, 0x38},
	{0x00, 0x10, 0x30, 0x50, 0x10, 0x10, 0x10, 0x7c},
	{0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c},
	{0x00, 0x38, 0x44, 0x04, 0x18, 0x04, 0x44, 0x38},
	{0x00, 0x08, 0x18, 0x28, 0x48, 0x7C, 0x08, 0x08},
	{0x00, 0x7C, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38},
	{0x00, 0x38, 0x40, 0x40, 0x78, 0x44, 0x44, 0x38}
};

unsigned char print_buffer[8] = {0x00};

int count = 0;
int dot_num = 0;
int shift_count = 0;

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
			if(parking[0][j] & (0x80 >> i))
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
	if(count == 30)
	{
		count = 0;

		for(int i = 0; i < 8; i++)
		{
			for(int k = 0; k < 5; k++) // 한 배열에서 계속 옆으로 미는 코드
			{
				parking[k][i] = parking[k][i] >> 1;
				if(parking[k+1][i] & 0x01) // 마지막 놈이 끝에 있으면
				{
					parking[k][i] |= 0x80; // 왼쪽 끝으로 보냄
				}
				//parking[k+1][i] = parking[k+1][i] >> 1;
			}
		}
		

		shift_count++;
		if(shift_count == 8)
		{
			shift_count = 0;
			dot_num++;
			if (dot_num == 10) dot_num = 0;
		}
	}
}