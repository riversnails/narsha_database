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
	x_left, x_right, y_up, y_down
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

void x_move(int x_dis, int DIR)
{
	// dir
	if(DIR == x_left) PORTC = X_LEFT;
	if(DIR == x_right) PORTC = X_RIGHT;
	// distance
	x_distance = x_dis;
	// on
	TIMSK1 = 0x02;
}

void y_move(int y_dis, int DIR)
{
	// dir
	if(DIR == y_up) PORTC = Y_UP;
	if(DIR == y_down) PORTC = Y_DOWN;
	// distance
	y_distance = y_dis;
	// on
	TIMSK3 = 0x02;
}

void reset()
{
	OCR1A = 600; // 좀 살살박자고 넣어놓음
	OCR3A = 600;

	is_x_reset = 1;
	is_y_reset = 1;
	x_move(32000, x_left);
	y_move(32000, y_up);
	while(is_x_reset != 0 || is_y_reset != 0); // 끝날때까지 대기
	currunt_x = 0; // 초기화 했으니 좌표도 초기화
	currunt_y = 0;

	OCR1A = 400;
	OCR3A = 400;
}

void goXLocation(int x)
{
	goXLocation(x, 400);
}

void goXLocation(int x, int speed)
{
	if(currunt_x == x)
		return;

	int dir = x_right;
	if(currunt_x > x) dir = x_left;

	OCR1A = speed;

	x_move(abs(currunt_x - x) * ONE_MM, dir);
	currunt_x = x;
}

void goYLocation(int y)
{
	goYLocation(y, 400);
}

void goYLocation(int y, int speed)
{
	if(currunt_y == y)
		return;

	int dir = y_down;
	if(currunt_y > y) dir = y_up;

	OCR3A = speed;

	y_move(abs(currunt_y - y) * ONE_MM, dir);
	currunt_y = y;
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

	reset();

	goXLocation(50);
	goYLocation(50);
	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	delay(1000);
}

void loop() // 각도를 가진 직각 삼각형을 사인 코사인을 사용하여 프린트 하는 코드
{
  // PORTC |= X_DIR;
  // TIMSK1 = 0x02; // x켜주기
  // while(TIMSK1 != 0x00); // 밑의 TIMSK1 이 끝나는걸 기다림
  // PORTC &= ~(Y_DIR);
  // TIMSK3 = 0x02;
  // while(TIMSK3 != 0x00);
  // PORTC |= X_DIR;
  // TIMSK1 = 0x02;
  // while(TIMSK1 != 0x00);
  // PORTC |= Y_DIR;
  // TIMSK3 = 0x02;
  // while(TIMSK3 != 0x00);

	goXLocation(currunt_x + 50 * cos(ANGLE(30))); // 밑변 계산 || 빗변을 기준으로 하니  밑변/빗변 = cos 이 나온다 이때 우리는 cos(30)으로 할거니 식을 넣으면 된다.
	while(TIMSK1 != 0x00);
	goYLocation(currunt_y + 50 * sin(ANGLE(30))); // 높이 계산 || 위와 함께 설명 || currunt_y 를 +하는 이유 = 현재 위치에서 이동하기 위해이다, 50을 곱하는 이유 = 빗변이 50이니 1을 기준으로 구한 사인, 코사인값들을 곱해줌
	while(TIMSK3 != 0x00);
	goXLocation(50, 400 * sin(ANGLE(30))); // 두 친구의 길이를 구해서 반대로 넣어주면 속도의 비를 구하지 않아도 된다.	
	goYLocation(50, 400 * cos(ANGLE(30)));
	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	delay(1000);

	//goXLocation(50+43.30127018);
	// goYLocation(50);
	// while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	// goXLocation(50+43.30127018);
	// goYLocation(75);
	// while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	// OCR1A = 200;
	// OCR3A = 400;
	// goXLocation(50, 400/1.72);
	// goYLocation(50, 400);
	// while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	// delay(1000);
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
				x_move(ONE_CM*5, x_right);
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
				y_move(ONE_CM*5, y_down);
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