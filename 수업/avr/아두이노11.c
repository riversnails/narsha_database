#define RS 14
#define RW 15
#define E  16

#define DB0 4
#define DB1 5
#define DB2 6
#define DB3 7
#define DB4 8
#define DB5 9
#define DB6 10
#define DB7 11

void setup()
{
	for(int i=0;i<4;i++)
	{
		pinMode(8+i, OUTPUT);
		if(i < 3) pinMode(14+i, OUTPUT);
	}
	digitalWrite(RS, HIGH);
	digitalWrite(RW, LOW);

	charLCD_init();
}
void loop()
{
	charLCD_set_data_4bit(0x30);
	delay(1000);
}

void charLCD_init()
{
	// 8 bit mode instruction
	charLCD_set_inst_8bit(0x30);
	charLCD_set_inst_8bit(0x30);
	charLCD_set_inst_8bit(0x30);
	charLCD_set_inst_8bit(0x28);

	// 4 bit mode instruction
	charLCD_set_inst_4bit(0x28);
	charLCD_set_inst_4bit(0x06);
	charLCD_set_inst_4bit(0x0C);
	charLCD_set_inst_4bit(0x80);
	charLCD_set_inst_4bit(0x01);
	delay(10);
}

void charLCD_set_inst_8bit(int inst)
{
	digitalWrite(RS, LOW);

	for(int i=0;i<8;i++)
	{
		if((inst & (0x01 << i)) != 0) digitalWrite(DB0 + i, HIGH);
		else digitalWrite(DB0 + i, LOW);
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);
	digitalWrite(RS, HIGH);

	delayMicroseconds(50);
}

void charLCD_set_inst_4bit_tmp(int inst)
{
	digitalWrite(RS, LOW);

	for(int i=0;i<4;i++)
	{
		if((inst & (0x01 << i)) != 0) digitalWrite(DB0 + i + 4, HIGH);
		else digitalWrite(DB0 + i + 4, LOW);
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);
	digitalWrite(RS, HIGH);

	delayMicroseconds(50);
}
void charLCD_set_inst_4bit(int inst)
{
	charLCD_set_inst_4bit_tmp((inst >> 4) & 0x0F);
	charLCD_set_inst_4bit_tmp(inst & 0x0F);
}

void charLCD_set_data_4bit_tmp(int data)
{

	for(int i=0;i<4;i++)
	{
		if((data & (0x01 << i)) != 0) digitalWrite(DB0 + i + 4, HIGH);
		else digitalWrite(DB0 + i + 4, LOW);
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	delayMicroseconds(50);
}
void charLCD_set_data_4bit(int data)
{
	charLCD_set_data_4bit_tmp((data >> 4) & 0x0F);
	charLCD_set_data_4bit_tmp(data & 0x0F);
}
