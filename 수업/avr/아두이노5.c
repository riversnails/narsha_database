#define DB0 11
#define DB1 10
#define DB2 9
#define DB3 8
#define DB4 7
#define DB5 6
#define DB6 5
#define DB7 4

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

	//------------------------------------

	// instruction 0x30
	digitalWrite(RS, LOW);
	digitalWrite(RW, LOW);

	// 0x30 : 0 0 1 1 / 0 0 0 0

	digitalWrite(DB0, LOW);
	digitalWrite(DB1, LOW);
	digitalWrite(DB2, HIGH);
	digitalWrite(DB3, HIGH);
	digitalWrite(DB4, LOW);
	digitalWrite(DB5, LOW);
	digitalWrite(DB6, LOW);
	digitalWrite(DB7, LOW);
	
	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RW, HIGH);
	digitalWrite(RS, HIGH);

	delayMicroseconds(50);

	//------------------------------------

	// instruction 0x30
	digitalWrite(RS, LOW);
	digitalWrite(RW, LOW);

	// 0x30 : 0 0 1 1 / 0 0 0 0

	digitalWrite(DB0, LOW);
	digitalWrite(DB1, LOW);
	digitalWrite(DB2, HIGH);
	digitalWrite(DB3, HIGH);
	digitalWrite(DB4, LOW);
	digitalWrite(DB5, LOW);
	digitalWrite(DB6, LOW);
	digitalWrite(DB7, LOW);
	
	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RW, HIGH);
	digitalWrite(RS, HIGH);

	delayMicroseconds(50);

	//------------------------------------

	// instruction 0x30
	digitalWrite(RS, LOW);
	digitalWrite(RW, LOW);

	// 0x30 : 0 0 1 1 / 0 0 0 0

	digitalWrite(DB0, LOW);
	digitalWrite(DB1, LOW);
	digitalWrite(DB2, HIGH);
	digitalWrite(DB3, HIGH);
	digitalWrite(DB4, LOW);
	digitalWrite(DB5, LOW);
	digitalWrite(DB6, LOW);
	digitalWrite(DB7, LOW);
	
	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RW, HIGH);
	digitalWrite(RS, HIGH);
	
	delayMicroseconds(50);

	//------------------------------------

	// instruction 0x38
	digitalWrite(RS, LOW);
	digitalWrite(RW, LOW);

	// 0x38 : 0 0 1 1 / 1 0 0 0

	digitalWrite(DB0, LOW);
	digitalWrite(DB1, LOW);
	digitalWrite(DB2, HIGH);
	digitalWrite(DB3, HIGH);
	digitalWrite(DB4, HIGH);
	digitalWrite(DB5, LOW);
	digitalWrite(DB6, LOW);
	digitalWrite(DB7, LOW);
	
	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RW, HIGH);
	digitalWrite(RS, HIGH);
	
	delayMicroseconds(50);

	//------------------------------------

	// instruction 0x06
	digitalWrite(RS, LOW);
	digitalWrite(RW, LOW);

	// 0x06 : 0 0 0 0 / 0 1 1 0

	digitalWrite(DB0, LOW);
	digitalWrite(DB1, LOW);
	digitalWrite(DB2, LOW);
	digitalWrite(DB3, LOW);
	digitalWrite(DB4, LOW);
	digitalWrite(DB5, HIGH);
	digitalWrite(DB6, HIGH);
	digitalWrite(DB7, LOW);
	
	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RW, HIGH);
	digitalWrite(RS, HIGH);
	
	delayMicroseconds(50);

	//------------------------------------

	// instruction 0x0C
	digitalWrite(RS, LOW);
	digitalWrite(RW, LOW);

	// 0x0C : 0 0 0 0 / 1 1 0 0

	digitalWrite(DB0, LOW);
	digitalWrite(DB1, LOW);
	digitalWrite(DB2, LOW);
	digitalWrite(DB3, LOW);
	digitalWrite(DB4, HIGH);
	digitalWrite(DB5, HIGH);
	digitalWrite(DB6, LOW);
	digitalWrite(DB7, LOW);
	
	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RW, HIGH);
	digitalWrite(RS, HIGH);
	
	delayMicroseconds(50);

	//------------------------------------

	// instruction 0x80
	digitalWrite(RS, LOW);
	digitalWrite(RW, LOW);

	// 0x80 : 0 1 0 0 / 0 0 0 0

	digitalWrite(DB0, LOW);
	digitalWrite(DB1, HIGH);
	digitalWrite(DB2, LOW);
	digitalWrite(DB3, LOW);
	digitalWrite(DB4, LOW);
	digitalWrite(DB5, LOW);
	digitalWrite(DB6, LOW);
	digitalWrite(DB7, LOW);
	
	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RW, HIGH);
	digitalWrite(RS, HIGH);
	
	delayMicroseconds(50);

	//------------------------------------

	// instruction 0x01
	digitalWrite(RS, LOW);
	digitalWrite(RW, LOW);

	// 0x01 : 0 0 0 0 / 0 0 0 1

	digitalWrite(DB0, LOW);
	digitalWrite(DB1, LOW);
	digitalWrite(DB2, LOW);
	digitalWrite(DB3, LOW);
	digitalWrite(DB4, LOW);
	digitalWrite(DB5, LOW);
	digitalWrite(DB6, LOW);
	digitalWrite(DB7, HIGH);
	
	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RW, HIGH);
	digitalWrite(RS, HIGH);
	
	delayMicroseconds(50);
	
	delay(10);

	//------------------------------------
}

void loop() // lcd 출력 코드 노가다라 안씀
{
	//------------------------------------

	// instruction 0x31
	digitalWrite(RS, HIGH);
	digitalWrite(RW, LOW);

	// 0x31 : 0 0 1 1 / 0 0 0 1

	digitalWrite(DB0, LOW);
	digitalWrite(DB1, LOW);
	digitalWrite(DB2, HIGH);
	digitalWrite(DB3, HIGH);
	digitalWrite(DB4, LOW);
	digitalWrite(DB5, LOW);
	digitalWrite(DB6, LOW);
	digitalWrite(DB7, HIGH);
	
	// enable
	digitalWrite(E, HIGH);
	digitalWrite(E, LOW);

	digitalWrite(RW, HIGH);
	digitalWrite(RS, HIGH);
	
	delayMicroseconds(50);
	
	delay(1000);

	//------------------------------------
}