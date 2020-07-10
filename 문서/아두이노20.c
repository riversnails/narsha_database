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
int school_tang_tang_play[7] = {250,250,250,250,250,250,500};
int school_tang_tang_rythem[7] = {50,50,50,50,50,50,100};

unsigned long c_micros = 0;
unsigned long p_micros = 0;

unsigned long c_millis = 0;
unsigned long p_millis = 0;

unsigned long p_millis1 = 0;

int toggle = 0;
int note_index = 0;
int play_on = 1;

void setup() 
{
	pinMode(3, OUTPUT);
}

void loop() 
{
	c_micros = micros();
	c_millis = millis();

	if(play_on == 1) // 우리가 가변적으로 키고 끄는것을 만듦
	{
		if(c_micros - p_micros > school_tang_tang[note_index]) // micros 구조의 것으로 소리만 내는것
		{
			p_micros = c_micros;

			if(toggle == 0)
			{
				toggle = 1;
				digitalWrite(3, HIGH);
			}
			else
			{
				toggle = 0;
				digitalWrite(3, LOW);
			}
		}
	}

	if(c_millis - p_millis > 325) // 끝날때 다시 시작 || 1
	{
		p_millis = c_millis;

		play_on = 1; // 위의 것을 실행시킴

		p_millis1 = c_millis; // 시작을 시킬때 부터 시간을 세기 위해 만듦

		note_index++;
		if(note_index == 7)
		{
			note_index = 0;
		}
	}
	
	if(play_on == 1) // 부저가 울리기 시작할때 같이 시작하기 위해 만듦 || 2
	{
		if(c_millis - p_millis1 > 250) // 250을 가변으로 넣을 수 있다
		{
			p_millis1 = c_millis;

			play_on = 0;
			digitalWrite(3, LOW);
			toggle = 0;
		}
	}

}

// 울리는것을 250 millis, 꺼져있는것을 75millis로 맞춘다는 가정하에 저 위의 1번은 처음 시작할떄의 초기화, 2번은 1번과 함께 시작하게 만든다