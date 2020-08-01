#define X_DIR 0x20 // c
#define X_STEP 0x80 // d
#define X_STOP 0x04 // c
#define Y_DIR 0x80 // c
#define Y_STEP 0x40 // c
#define Y_STOP 0x08 // c
#define XYEENABLE 0x40 // d

#define ONE_MM 80
#define ONE_CM 800

#define X_LEFT PORTC | X_DIR
#define X_RIGHT PORTC & ~(X_DIR)
#define Y_UP PORTC | Y_DIR
#define Y_DOWN PORTC & ~(Y_DIR)

enum{ // 열거형  날자 표현할때 자주 사용 0,1,2,3이런식으로 나옴  define 대용
	stop, x_left, x_right, y_up, y_down
};

volatile char is_x_reset = 0;
volatile int x_distance = 0;

volatile char is_y_reset = 0;
volatile int y_distance = 0;

volatile int currunt_x = 0; // 현재 좌표 && mm 단위로 봄
volatile int currunt_y = 0;

double ANGLE(int x){ // 이 내장 사인 코사인들이 라디안을 사용하여 구하기 때문에 라디안으로 리턴함
  return PI * (x / 180.0);
}

void x_move(int x_dis, int DIR, int speed)
{
	// dir
	if(DIR == x_left) PORTC = X_LEFT;
	if(DIR == x_right) PORTC = X_RIGHT;
	// distance
	x_distance = x_dis;
	// speed
	OCR1A = speed;
	// on
	TIMSK1 = 0x02;
}

void y_move(int y_dis, int DIR, int speed)
{
	// dir
	if(DIR == y_up) PORTC = Y_UP;
	if(DIR == y_down) PORTC = Y_DOWN;
	// distance
	y_distance = y_dis;
	// speed
	OCR3A = speed;
	// on
	TIMSK3 = 0x02;
}

void shift(double len, int x_dir, int y_dir, int speed, int angle) 
{
	x_move(len*cos(ANGLE(angle)) * ONE_MM, x_dir, speed * (1 / cos(ANGLE(angle))));
	y_move(len*sin(ANGLE(angle)) * ONE_MM, y_dir, speed * (1 / sin(ANGLE(angle))));

	while (TIMSK1 != 0X00 || TIMSK3 != 0X00);
}

void reset()
{
	is_x_reset = 1;
	is_y_reset = 1;
	x_move(32000, x_left, 600);
	y_move(32000, y_up, 600);
	while(is_x_reset != 0 || is_y_reset != 0); // 끝날때까지 대기
	currunt_x = 0; // 초기화 했으니 좌표도 초기화
	currunt_y = 0;
}

void setup()
{
	DDRC |= X_DIR; // 오른쪽
	DDRC |= Y_DIR;
	DDRC |= Y_STEP;
	DDRD |= X_STEP | XYEENABLE;
	DDRC &= ~(X_STOP);
	DDRC &= ~(Y_STOP);

	TCCR1A = 0x00;
	TCCR1B = 0x0a;
	TCCR1C = 0x00;
	OCR1A = 400;
	TIMSK1 = 0x00;

	TCCR3A = 0x00;
	TCCR3B = 0x0a;
	TCCR3C = 0x00;
	OCR3A = 400;
	TIMSK3 = 0x00;

	//reset();
	//delay(1000);
}

void loop() // 빗변의 속도를 같이했으니 함수화 시키고 응용하는 코드
{
	// 정삼각형
	// x_move(50 * ONE_MM, x_right, 400);
	// while (TIMSK1 != 0X00 || TIMSK3 != 0X00);
	// shift(50, x_left, y_down, 400, 60);
	// shift(50, x_left, y_up, 400, 60);

	// 30도짜리 직각삼각형
	// x_move(50*cos(ANGLE(30)) * ONE_MM, x_right, 400);
	// while (TIMSK1 != 0X00 || TIMSK3 != 0X00);
	// y_move(50*sin(ANGLE(30)) * ONE_MM, y_down, 400);
	// while (TIMSK1 != 0X00 || TIMSK3 != 0X00);
	// shift(50, x_left, y_up, 400, 30);
}

volatile char x_step_toggle = 0;
volatile int x_step_count = 0;

volatile char y_step_toggle = 0;
volatile int y_step_count = 0;

ISR(TIMER1_COMPA_vect)
{

	if(x_step_toggle == 0)
	{
		x_step_toggle = 1;
		PORTD |= X_STEP;
	}
	else
	{
		x_step_toggle = 0;
		PORTD &= ~(X_STEP);
		x_step_count++;
		char x_limit_switch = PINC & X_STOP;

		if(x_step_count >= x_distance)
		{
			is_x_reset = 0;
			x_step_count = 0;
			TIMSK1 = 0x00;
		}
		if(x_limit_switch && is_x_reset != -1)
		{
			TIMSK1 = 0x00;
			x_step_count = 0;
			if(is_x_reset == 1)
			{
				is_x_reset = -1;
				x_move(ONE_CM*5, x_right, 400);
			}
		}
	}
		
}

ISR(TIMER3_COMPA_vect)
{

	if(y_step_toggle == 0)
	{
		y_step_toggle = 1;
		PORTC |= Y_STEP;
	}
	else
	{
		y_step_toggle = 0;
		PORTC &= ~(Y_STEP);
		y_step_count++;
		char y_limit_switch = PINC & Y_STOP;

		if(y_step_count >= y_distance)
		{
			is_y_reset = 0;
			y_step_count = 0;
			TIMSK3 = 0x00;
		}
		if(y_limit_switch && is_y_reset != -1)
		{
			TIMSK3 = 0x00;
			y_step_count = 0;
			if(is_y_reset == 1)
			{
				is_y_reset = -1;
				y_move(ONE_CM*5, y_down, 400);
			}
		}
	}

}

//1 step = 0.0125mm
//80 step = 1mm

// 대충적음
// 8 4 2 1 | 8 4 2 1  16진수
// 7 6 5 4 | 3 2 1 0  10진수
// 0 0 0 0 | 0 0 0 0   2진수