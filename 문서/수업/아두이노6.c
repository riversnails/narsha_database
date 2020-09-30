#define DB0 4
#define DB1 5
#define DB2 6
#define DB3 7
#define DB4 8
#define DB5 9
#define DB6 10
#define DB7 11

#define RS 14
#define RW 15
#define E 16

char string_hello[5] = {'h','e','l','l','o'};
void setup()
{
	Serial.begin(9600);
	char_LCD_init();
}

void loop()
{
	//lcd_data_set(0x31);
	//lcd_string_set(string_hello);
	lcd_string_set("hellow");
	delay(1000);
}

void char_LCD_init()
{
	for(int i=4;i<=11;i++)
	{
		pinMode(i, OUTPUT);
	}
	for(int i=14;i<=16;i++)
	{
		pinMode(i, OUTPUT);
	}

	digitalWrite(RW, LOW);
	digitalWrite(RS, HIGH);

	lcd_instruction_set(0x30);
	lcd_instruction_set(0x30);
	lcd_instruction_set(0x30);
	lcd_instruction_set(0x38);
	lcd_instruction_set(0x06);
	lcd_instruction_set(0x0C);
	lcd_instruction_set(0x80);
	lcd_instruction_set(0x01);
	delay(10);
}

void lcd_instruction_set(char inst)
{
	digitalWrite(RS, LOW);
	digitalWrite(RW, LOW);

	for(int i = 0; i < 7; i++)
	{
		if(inst & (0x01 << i))
		{
			digitalWrite(4+i, HIGH);
		}
		else
		{
			digitalWrite(4+i, LOW);
		}
	}

	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RS, HIGH);
	digitalWrite(RW, HIGH);
	
	delayMicroseconds(50);
}

void lcd_data_set(char inst)
{
	digitalWrite(RS, HIGH);
	digitalWrite(RW, LOW);

	for(int i = 0; i < 7; i++)
	{
		if(inst & (0x01 << i))
		{
			digitalWrite(4+i, HIGH);
		}
		else
		{
			digitalWrite(4+i, LOW);
		}
	}

	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RS, HIGH);
	digitalWrite(RW, HIGH);

	delayMicroseconds(50);
}

void lcd_string_set(char *string)
{
	// for(int i = 0; i < 5; i++)
	// 	lcd_data_set(string[i]);

	int i = 0;

	while(1)
	{
		if(string[i] == 0)
			break;
		else
			lcd_data_set(string[i++]);
	}
}