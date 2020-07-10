#define NOTE_C4 1911 // 계이름들의 반주기
#define NOTE_D4 1702
#define NOTE_E4 1516
#define NOTE_F4 1431
#define NOTE_G4 1275
#define NOTE_A4 1136
#define NOTE_B4 1012

#define C 1911 // 계이름들의 반주기
#define D 1702
#define E 1516
#define F 1431
#define G 1275
#define A 1136
#define B 1012

int note_arr[8] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4};
//솔 솔 라 라 솔 솔 미

int school_tang_tang[7] = {G,G,A,A,G,G,E};
int school_tang_tang_rythem[7] = {1000000,1000000,1000000,1000000,1000000,1000000};

void setup() 
{
	pinMode(3, OUTPUT);
}

void loop() 
{
	for(int j = 0; j < 7; j++)
	{
		for(int i = 0; i < school_tang_tang_rythem[j]/(school_tang_tang[j]*2) ; i++)
		{
			digitalWrite(3, HIGH);
			delayMicroseconds(school_tang_tang[j]);
			digitalWrite(3, LOW);
			delayMicroseconds(school_tang_tang[j]);
		}
		delay(50);
	}
}