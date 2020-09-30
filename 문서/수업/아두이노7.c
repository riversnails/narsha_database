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


void setup()
{
	Serial.begin(9600);

	for(int i=4;i<=11;i++)
	{
		pinMode(i, OUTPUT);
	}
	for(int i=14;i<=16;i++)
	{
		pinMode(i, OUTPUT);
	}
	pinMode(3, INPUT);

	digitalWrite(RW,LOW);

	delayMicroseconds(50);

	charLCD_init();
}

int toggle = 0;
int remote = 0;

void loop()
{

	charLCD_set_data_4bit(0x03);
	charLCD_set_data_4bit(0x00);
	delay(1000);
	charLCD_set_data_4bit(0x03);
	charLCD_set_data_4bit(0x01);
	delay(1000);
	charLCD_set_data_4bit(0x03);
	charLCD_set_data_4bit(0x02);
	delay(1000);
	charLCD_set_data_4bit(0x03);
	charLCD_set_data_4bit(0x03);
	delay(1000);
	charLCD_set_data_4bit(0x03);
	charLCD_set_data_4bit(0x04);
	delay(1000);
	charLCD_set_data_4bit(0x03);
	charLCD_set_data_4bit(0x05);
	delay(1000);
	charLCD_set_data_4bit(0x03);
	charLCD_set_data_4bit(0x06);
	delay(1000);
	charLCD_set_data_4bit(0x03);
	charLCD_set_data_4bit(0x07);
	delay(1000);
	charLCD_set_data_4bit(0x03);
	charLCD_set_data_4bit(0x08);
	delay(1000);
	charLCD_set_data_4bit(0x03);
	charLCD_set_data_4bit(0x09);
	delay(1000);

	
	remote = digitalRead(3);
	if(remote != 0)
	{
		Serial.println("1");
	}
}

void charLCD_init()
{
	charLCD_set_inst_8bit(0x30);
	charLCD_set_inst_8bit(0x30);
	charLCD_set_inst_8bit(0x30);
	charLCD_set_inst_8bit(0x28);

	//----------------------------

	charLCD_set_inst_4bit(0x02);
	charLCD_set_inst_4bit(0x08);

	charLCD_set_inst_4bit(0x00);
	charLCD_set_inst_4bit(0x06);

	charLCD_set_inst_4bit(0x00);
	charLCD_set_inst_4bit(0x0C);

	charLCD_set_inst_4bit(0x08);
	charLCD_set_inst_4bit(0x00);

	charLCD_set_inst_4bit(0x00);
	charLCD_set_inst_4bit(0x01);
	delay(10);
}

void charLCD_set_inst_4bit(char inst)
{
	digitalWrite(RS, LOW);
	digitalWrite(RW, LOW);

	for(int i = 0; i < 4; i++)
	{
		if(inst & (0x01<<i))
		{
			digitalWrite(DB4+i,HIGH);
		}
		else
		{
			digitalWrite(DB4+i,LOW);
		}
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RS, HIGH);
	digitalWrite(RW, HIGH);
	delayMicroseconds(50);
}

void charLCD_set_data_4bit(char data)
{
	digitalWrite(RS, HIGH);
	digitalWrite(RW, LOW);

	for(int i = 0; i < 4; i++)
	{
		if(data & (0x01<<i))
		{
			digitalWrite(DB4+i,HIGH);
		}
		else
		{
			digitalWrite(DB4+i,LOW);
		}
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RS, HIGH);
	digitalWrite(RW, HIGH);
	delayMicroseconds(50);
}

void charLCD_set_inst_8bit(char inst)
{
	digitalWrite(RS,LOW);

	for(int i = 0; i < 8; i++)
	{
		if(inst & (0x01<<i))
		{
			digitalWrite(DB0+i,HIGH);
		}
		else
		{
			digitalWrite(DB0+i,LOW);
		}
	}

	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);
	
	digitalWrite(RS, HIGH);
	digitalWrite(RW, HIGH);
	delayMicroseconds(50);
}