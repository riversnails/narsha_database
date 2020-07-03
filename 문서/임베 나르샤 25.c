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

volatile float currunt_x = 0; // 현재 좌표 && mm 단위로 봄
volatile float currunt_y = 0;

volatile const float xy_pos[200][2] = {
66.268 , 82.377,
66.527 , 82.217,
66.796 , 82.073,
67.072 , 81.946,
67.356 , 81.836,
67.647 , 81.743,
67.942 , 81.669,
68.241 , 81.613,
68.544 , 81.576,
68.848 , 81.557,
131.152, 81.557,
131.456, 81.576,
131.759, 81.613,
132.058, 81.669,
132.353, 81.743,
132.644, 81.836,
132.928, 81.946,
133.204, 82.073,
133.473, 82.217,
133.732, 82.377,
133.980, 82.553,
134.218, 82.744,
134.443, 82.950,
134.655, 83.168,
134.853, 83.400,
135.036, 83.643,
135.205, 83.897,
135.357, 84.160,
135.493, 84.433,
135.611, 84.714,
135.713, 85.001,
135.796, 85.294,
135.861, 85.591,
135.908, 85.892,
135.936, 86.196,
135.945, 86.500,
135.936, 86.804,
135.908, 87.108,
135.861, 87.409,
135.796, 87.706,
135.713, 87.999,
135.611, 88.286,
135.493, 88.567,
135.357, 88.840,
135.205, 89.103,
135.036, 89.357,
134.853, 89.600,
134.664, 89.821,
134.225, 90.251,
103.227, 117.248,
102.980, 117.446,
102.732, 117.622,
102.473, 117.783,
102.204, 117.927,
101.928, 118.054,
101.644, 118.164,
101.353, 118.257,
101.058, 118.331,
100.759, 118.387,
100.456, 118.424,
100.152, 118.443,
99.848 , 118.443,
99.544 , 118.424,
99.241 , 118.387,
98.942 , 118.331,
98.647 , 118.257,
98.356 , 118.164,
98.072 , 118.054,
97.796 , 117.927,
97.527 , 117.783,
97.268 , 117.622,
97.021 , 117.447,
96.737 , 117.217,
65.775 , 90.251,
65.336 , 89.821,
65.147 , 89.600,
64.964 , 89.357,
64.795 , 89.103,
64.643 , 88.840,
64.507 , 88.567,
64.389 , 88.286,
64.287 , 87.999,
64.204 , 87.706,
64.139 , 87.409,
64.092 , 87.108,
64.064 , 86.804,
64.055 , 86.500,
64.064 , 86.196,
64.092 , 85.892,
64.139 , 85.591,
64.204 , 85.294,
64.287 , 85.001,
64.389 , 84.714,
64.507 , 84.433,
64.643 , 84.160,
64.795 , 83.897,
64.964 , 83.643,
65.147 , 83.400,
65.345 , 83.168,
65.557 , 82.950,
65.782 , 82.745,
66.020 , 82.553,
65.586 , 81.979,
65.870 , 81.777,
66.167 , 81.593,
66.475 , 81.428,
66.792 , 81.282,
67.117 , 81.156,
67.449 , 81.051,
67.788 , 80.965,
68.131 , 80.901,
68.477 , 80.858,
68.826 , 80.837,
131.174, 80.837,
131.523, 80.858,
131.869, 80.901,
132.212, 80.965,
132.551, 81.051,
132.883, 81.156,
133.208, 81.282,
133.525, 81.428,
133.833, 81.593,
134.130, 81.777,
134.414, 81.979,
134.686, 82.197,
134.944, 82.433,
135.187, 82.683,
135.414, 82.948,
135.624, 83.227,
135.817, 83.517,
135.992, 83.820,
136.147, 84.132,
136.283, 84.453,
136.399, 84.782,
136.495, 85.118,
136.569, 85.459,
136.623, 85.804,
136.655, 86.151,
136.666, 86.500,
136.655, 86.849,
136.623, 87.196,
136.569, 87.541,
136.495, 87.882,
136.399, 88.218,
136.283, 88.547,
136.147, 88.868,
135.992, 89.180,
135.817, 89.483,
135.624, 89.773,
135.414, 90.052,
135.190, 90.313,
134.714, 90.780,
103.689, 117.800,
103.414, 118.021,
103.130, 118.223,
102.833, 118.407,
102.525, 118.572,
102.208, 118.718,
101.883, 118.844,
101.551, 118.949,
101.212, 119.035,
100.869, 119.099,
100.523, 119.142,
100.174, 119.163,
99.826 , 119.163,
99.477 , 119.142,
99.131 , 119.099,
98.788 , 119.035,
98.449 , 118.949,
98.117 , 118.844,
97.792 , 118.718,
97.475 , 118.572,
97.167 , 118.407,
96.870 , 118.223,
96.586 , 118.022,
96.274 , 117.769,
65.286 , 90.780,
64.810 , 90.313,
64.586 , 90.052,
64.376 , 89.773,
64.183 , 89.483,
64.008 , 89.180,
63.853 , 88.868,
63.717 , 88.547,
63.601 , 88.218,
63.505 , 87.882,
63.431 , 87.541,
63.377 , 87.196,
63.345 , 86.849,
63.334 , 86.500,
63.345 , 86.151,
63.377 , 85.804,
63.431 , 85.459,
63.505 , 85.118,
63.601 , 84.782,
63.717 , 84.453,
63.853 , 84.132,
64.008 , 83.820,
64.183 , 83.517,
64.376 , 83.227
}; 

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

void goXLocation(float x, int speed)
{
	int dir = x_right;
	if(currunt_x > x) dir = x_left;

	goXLocation(x, speed, dir);
}

void goXLocation(float x, int speed, int dir)
{
	if(currunt_x == x)
		return;

	int x_len;
	x_len = (int)(abs(currunt_x - x) + 0.5);
	x_move(x_len * ONE_MM, speed, dir);
	currunt_x = x;
}

void goYLocation(float y, int speed)
{
	int dir = y_down;
	if(currunt_y > y) dir = y_up;

	goYLocation(y, speed, dir);
}

void goYLocation(float y, int speed, int dir)
{
	if(currunt_y == y)
		return;

	int y_len;
	y_len = (int)(abs(currunt_y - y) + 0.5);
	y_move(y_len * ONE_MM, speed,  dir);
	currunt_y = y;
}

void goXYLocation(float x, float y)
{
	if((currunt_x - x) == 0 || (currunt_y - y) == 0)
		goXYLocation_line(x,y);

	int xs = (int)((abs(currunt_x - x)/10) + 0.5);
	int ys = (int)((abs(currunt_y - y)/10) + 0.5);
	goXLocation(x, 150 * ys);
	goYLocation(y, 150 * xs);

	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
}

void goXYLocation_line(int x, int y)
{
	goXYLocation_line(x, y, 400);
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

	x_move(x_len, x_speed, x_dir);
	y_move(y_len, y_speed, y_dir);

	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
}

void goXYLen(float x, float y)
{
	int x_speed, x_len;
	int y_speed, y_len;

	x_speed = (int)(400 * (abs(currunt_y - y) + 0.5));
	y_speed = (int)(400 * (abs(currunt_x - x) + 0.5));

	goXLocation(x, x_speed);
	goYLocation(y, y_speed);
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
	// for(int i = 0; i < 200; i++)
	// {
	// 	goXYLen(xy_pos[i][0] , xy_pos[i][1]);
	// 	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	// 	delay(10);
	// }
	// delay(1000);
}

void loop() // g코드를 사용하여 프린팅
{

	// delay(1000);
	// goLenHypo(50, x_left, y_down, 30);
	// y_move(50 * ONE_MM, y_down);
	// while(TIMSK3 != 0x00);
	// goLenHypo(50, x_right, y_down, 30);
	// goLenHypo(50, x_right, y_up, 30);
	// y_move(50 * ONE_MM, y_up);
	// while(TIMSK3 != 0x00);
	// goLenHypo(50, x_left, y_up, 30);

	goXYLocation(10, 50);
	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	goXYLocation(40, 30);
	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	goXYLocation(10, 10);
	while(TIMSK1 != 0x00 || TIMSK3 != 0x00);
	delay(1000);

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