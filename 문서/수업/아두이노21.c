#define DATA 4
#define CLK 5
#define LATCH 6

#define ROW_0 12

char dot_data = 0x23;

void setup()
{
	for(int i = 0 ; i < 3; i++)
	{
		pinMode(4+i, OUTPUT);
	}

	for(int i = 0 ; i < 8; i++)
	{
		pinMode(ROW_0+i, OUTPUT);
	}

	for(int i = 0; i < 8; i++)
	{
		digitalWrite(ROW_0+i, LOW);
	}

	set_column(dot_data);
	digitalWrite(ROW_0, HIGH); 
}

void set_column(char dot_data){
	for (int i = 0; i < 8; i++) 
	{
		if (dot_data & (0x80 >> i)) 
		{
			digitalWrite(DATA, LOW);
		} 
		else
		{
			digitalWrite(DATA, HIGH);
		}

		digitalWrite(CLK, HIGH);
		digitalWrite(CLK, LOW);
	}

	digitalWrite(LATCH, HIGH);
	digitalWrite(LATCH, LOW);
}

void loop() //모듈을 이용한 8비트 
{
	set_column(0x01);
	delay(1000);
	set_column(0x00);
	delay(1000);
}