#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN          13   // ANALOG NUMBERING
#define TEMP_1_PIN          14   // ANALOG NUMBERING

void setup()
{
	pinMode(X_STEP_PIN, OUTPUT);
	pinMode(X_DIR_PIN, OUTPUT);
	pinMode(X_ENABLE_PIN, OUTPUT);
	pinMode(X_MIN_PIN, INPUT);
	digitalWrite(X_MIN_PIN, HIGH);

	pinMode(Y_STEP_PIN, OUTPUT);
	pinMode(Y_DIR_PIN, OUTPUT);
	pinMode(Y_ENABLE_PIN, OUTPUT);
	pinMode(Y_MIN_PIN, INPUT);
	digitalWrite(Y_MIN_PIN, HIGH);

	Serial.begin(9600);

	//timer1
	TCCR1A = 0x00;
	TCCR1B = 0x0A;
	OCR1A = 400;
	TIMSK1 = 0x00;
	//timer3
	TCCR3A = 0x00;
	TCCR3B = 0x0A;
	OCR3A = 400;
	TIMSK3 = 0x00;
	digitalWrite(X_DIR_PIN, HIGH);
	digitalWrite(X_ENABLE_PIN, LOW);
	digitalWrite(X_STEP_PIN, LOW);

	digitalWrite(Y_DIR_PIN, HIGH);
	digitalWrite(Y_ENABLE_PIN, LOW);
	digitalWrite(Y_STEP_PIN, LOW);

}

unsigned int step_count = 0;
unsigned int step_count1 = 0;
int count_X = 0;
int count_Y = 0;
int toggle_X = 0;
int toggle_Y = 0;
int toggle_1 = 0;
int len_X = 5000;
int len_Y = 5000;

void loop() // 3d 프린터 개조보드를 이용한 조작
{

	if(count_X == -1 || count_Y == -1){
		TIMSK1 = 0x00;
		TIMSK3 = 0x00;
	}
	if(toggle_1 == 0){
		if(count_X == 0 && count_Y != 1 && count_X != -1){
			TIMER1_ON(0);
			toggle_1 = 1;
		}
		else if(count_X == 1 && count_Y != 0 && count_X != -1){
			TIMER1_ON(1);
			toggle_1 = 1;
		}
	}
	if(toggle_1 == 1){
		if(count_Y == 0 && count_X != 0 && count_Y != -1){
			TIMER3_ON(0);
			toggle_1 = 0;
		}
		else if(count_Y == 1 && count_X != 1 && count_Y != -1){
			TIMER3_ON(1);
			toggle_1 = 0;
		}
	}

}

void TIMER1_ON(int TIMER1_toggle){
	if(TIMER1_toggle == 0)
	{
		digitalWrite(X_DIR_PIN, HIGH);
		TIMSK1 = 0x02;
	}
	else if(TIMER1_toggle == 1)
	{
		digitalWrite(X_DIR_PIN, LOW);
		TIMSK1 = 0x02;
	}
}
void TIMER3_ON(int TIMER3_toggle){
	if(TIMER3_toggle == 0)
	{
		digitalWrite(Y_DIR_PIN, HIGH);
		TIMSK3 = 0x02;
	}
	else if(TIMER3_toggle == 1)
	{
		digitalWrite(Y_DIR_PIN, LOW);
		TIMSK3 = 0x02;
	}
}

int step_toggle = 0;
int limit_switch = 0;

int step_toggle1 = 0;
int limit_switch1 = 0;

SIGNAL(TIMER1_COMPA_vect)
{
	if(step_toggle == 0)
	{
		step_toggle = 1;

		digitalWrite(X_STEP_PIN, HIGH);
	}
	else
	{
		step_toggle = 0;

		digitalWrite(X_STEP_PIN, LOW);

		step_count++;
		if(step_count >= len_X)
		{
			step_count = 0;
			TIMSK1 = 0x00;

			count_X += 1;
			if(count_X == 2){
				count_X = 0;
			}
		}

		limit_switch = digitalRead(X_MIN_PIN);
		if(limit_switch == 1)
		{
			step_count = 0;
			TIMSK1 = 0x00;
			count_X = -1;
		}
	}
}

SIGNAL(TIMER3_COMPA_vect)
{
	if(step_toggle1 == 0)
	{
		step_toggle1 = 1;

		digitalWrite(Y_STEP_PIN, HIGH);
	}
	else
	{
		step_toggle1 = 0;

		digitalWrite(Y_STEP_PIN, LOW);

		step_count1++;
		if(step_count1 >= len_Y)
		{
			step_count1 = 0;
			TIMSK3 = 0x00;

			count_Y += 1;
			if(count_Y == 2){
				count_Y = 0;
			}
		}

		limit_switch1 = digitalRead(Y_MIN_PIN);
		if(limit_switch1 == 1)
		{
			step_count1 = 0;
			TIMSK3 = 0x00;
			count_Y = -1;
		}
	}
}