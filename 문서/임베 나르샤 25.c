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

void x_move(int x_dis, int DIR)
{
	x_move(x_dis, 400, DIR);
}

void x_move(int x_dis, int speed, int DIR)
{
	// dir
	if(DIR == x_left) PORTC = X_LEFT;
	if(DIR == x_right) PORTC = X_RIGHT;
	// distance
	x_distance = (int)x_dis;
	// speed
	OCR1A = speed;
	// on
	TIMSK1 = 0x02;
}

void y_move(int y_dis, int DIR)
{
	y_move(y_dis, 400, DIR);
}

void y_move(int y_dis, int speed, int DIR)
{
	// dir
	if(DIR == y_up) PORTC = Y_UP;
	if(DIR == y_down) PORTC = Y_DOWN;
	// distance
	y_distance = (int)y_dis;
	// speed
	OCR3A = speed;
	// on
	TIMSK3 = 0x02;
}

void reset()
{
	is_x_reset = 1;
	is_y_reset = 1;
	x_move(32000, 600, x_left);
	y_move(32000, 600, y_up);
	while(is_x_reset != 0 || is_y_reset != 0); // 끝날때까지 대기
	currunt_x = 0; // 초기화 했으니 좌표도 초기화
	currunt_y = 0;
}

void goXLocation(int x, int speed)
{
	int dir = x_right;
	if(currunt_x > x) dir = x_left;

	goXLocation(x, speed, dir);
}

void goXLocation(int x, int speed, int dir)
{
	if(currunt_x == x)
		return;

	x_move(abs(currunt_x - x) * ONE_MM, speed, dir);
	currunt_x = x;
}

void goYLocation(int y, int speed)
{
	int dir = y_down;
	if(currunt_y > y) dir = y_up;

	goYLocation(y, speed, dir);
}

void goYLocation(int y, int speed, int dir)
{
	if(currunt_y == y)
		return;

	y_move(abs(currunt_y - y) * ONE_MM, speed,  dir);
	currunt_y = y;
}

void goXYLocation(int x, int y)
{
	if((currunt_x - x) == 0 || (currunt_y - y) == 0)
		goXYLocation_line(x,y);

	int xs = (int)((abs(currunt_x - x)/10) + 0.5);
	int ys = (int)((abs(currunt_y - y)/10) + 0.5);

	Serial.print(currunt_x);
	Serial.println(currunt_y);
	Serial.print(x);
	Serial.println(y);

	goXLocation(x, 100 * ys);
	goYLocation(y, 100 * xs);

	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
}

void goXYLocation_line(int x, int y)
{
	goXYLocation_line(x, y, 400);

	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
}

void goXYLocation_line(int x, int y, int speed)
{
	goXLocation(x, speed);
	goYLocation(y, speed);

	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
}

void goLenHypo(int len, int x_dir, int y_dir, int degree) // 길이와 뱡항을 넣어 각도를 구해 가는 코드
{
	int x_speed, x_len;
	int y_speed, y_len;
	len *= ONE_MM;

	x_len = (int)(len * cos(ANGLE(degree)) + 0.5);
	y_len = (int)(len * sin(ANGLE(degree)) + 0.5);
	x_speed = (int)(400 * sin(ANGLE(degree)) + 0.5);
	y_speed = (int)(400 * cos(ANGLE(degree)) + 0.5);

	if(x_dir == x_right)
		currunt_x += x_len/100;
	else
		currunt_x -= x_len/100;

	if(y_dir == y_down)
		currunt_y += y_len/100;
	else
		currunt_y -= y_len/100;

	//Serial.print(x_len/100);
	//Serial.println(y_len/100);

	x_move(x_len, x_speed, x_dir);
	y_move(y_len, y_speed, y_dir);

	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
}

void godgree(int len, int x, int y, int degree) // 길이와 뱡항을 넣어 각도를 구해 가는 코드
{
	int x_speed, x_len;
	int y_speed, y_len;
	len *= ONE_MM;

	x_len = (int)80 * (cos(ANGLE(degree) + 0.5));
	y_len = (int)80 * (sin(ANGLE(degree) + 0.5));
	x_speed = (int)(400 * sin(ANGLE(degree)) + 0.5);
	y_speed = (int)(400 * cos(ANGLE(degree)) + 0.5);

	Serial.print(currunt_x);
	Serial.println(currunt_y);

	goXLocation(x+x_len, x_speed);
	goYLocation(y+y_len, y_speed);

	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
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

	Serial.begin(9600);
	reset();

	Serial.print(currunt_x);
	Serial.println(currunt_y);
	goXYLocation(50,50);


	Serial.print(currunt_x);
	Serial.println(currunt_y);
	//delay(1000);
}

void loop() // 원뿔을 하려 했던것
{
	//육각형

	Serial.print(currunt_x);
	Serial.println(currunt_y);
	delay(1000);
	goXYLocation(80, 50);

	for(int i = 0; i < 90; i += 5)
	{
		godgree(50, 50, 50, i);
		delay(100);
	}

	goXYLocation(50,50);

	// goXYLocation(10, 50);
	// while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	// goXYLocation(40, 30);
	// while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	// goXYLocation(10, 10);
	// while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	// delay(1000);

	// goXYLocation(currunt_x + 50 * cos(ANGLE(30)), currunt_y);
	// while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	// goXYLocation(currunt_x, currunt_y + 50 * sin(ANGLE(30)));
	// while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	// goXYLocation(10, 10, 400 * sin(ANGLE(30)) , 400 * cos(ANGLE(30)));
	// while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	// delay(1000);

	// goXLocation(currunt_x + 50 * cos(ANGLE(30))); // 밑변 계산 || 빗변을 기준으로 하니  밑변/빗변 = cos 이 나온다 이때 우리는 cos(30)으로 할거니 식을 넣으면 된다.
	// while(TIMSK1 != 0x00);
	// goYLocation(currunt_y + 50 * sin(ANGLE(30))); // 높이 계산 || 위와 함께 설명 || currunt_y 를 +하는 이유 = 현재 위치에서 이동하기 위해이다, 50을 곱하는 이유 = 빗변이 50이니 1을 기준으로 구한 사인, 코사인값들을 곱해줌
	// while(TIMSK3 != 0x00);
	// goXLocation(50, 400 * sin(ANGLE(30))); // 두 친구의 길이를 구해서 반대로 넣어주면 속도의 비를 구하지 않아도 된다.	
	// goYLocation(50, 400 * cos(ANGLE(30)));
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