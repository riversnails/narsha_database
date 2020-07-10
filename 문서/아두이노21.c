#define CLK 4
#define DATA 5
#define LATCH 6

#define ROW_0 12

char dot_data = 0x20;

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

void set_column(char dot_data)
{
	for(int i = 0 ; i < 8; i++)
	{
		if(dot_data & (0x80 >> i))
		{
			digitalWrite(DATA, LOW);
		}
		else
		{
			digitalWrite(DATA, HIGH);
		}
	}

	digitalWrite(CLK, HIGH);
	digitalWrite(CLK, LOW);
}

void loop()
{
	set_column(0x01);
}