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
	pinMode(X_MIN_PIN, OUTPUT);
	digitalWrite(X_MIN_PIN, HIGH);
	
	Serial.begin(9600);
	TCCR2A = 0x02;
	TCCR2B = 0x03;
	OCR2A = 100;
	TIMSK2 = 0x02;

	digitalWrite(X_DIR_PIN, HIGH);
	digitalWrite(X_ENABLE_PIN, LOW);
	digitalWrite(X_STEP_PIN, LOW);

}


void loop() // 3d 프린터 개조보드를 이용한 조작
{
	

}

unsigned int step_count = 0;
int step_toggle = 0;
int limit_switch;

int step_toggle1 = 0;

SIGNAL(TIMER2_COMPA_vect) {



	if (step_toggle == 0) {
		step_toggle = 1;
		digitalWrite(X_STEP_PIN, HIGH);
			
	}
	else {
		step_toggle = 0;
		digitalWrite(X_STEP_PIN,LOW);
	
		step_count++;
		if (step_count >= 5000) {
			if(step_toggle1 == 0){
				digitalWrite(X_DIR_PIN,LOW);
				step_toggle1 = 1;
			}
			else{
				digitalWrite(X_DIR_PIN,HIGH);
				step_toggle1 = 0;
			}
			step_count = 0;
			// TIMSK2 = 0x00;
		}
		limit_switch = digitalRead(X_MIN_PIN);
		if (limit_switch == 1) {
			step_count = 0;
			TIMSK2 = 0x00;
		}
	}

}