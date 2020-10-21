#define X_DIR 0x20 // c
#define X_STEP 0x80 // d
#define X_STOP 0x04 // c 2
#define Y_DIR 0x80 // c
#define Y_STEP 0x40 // c
#define Y_STOP 0x08 // c 3
#define XYEENABLE 0x40 // d

#define Z_DIR 0x04 // b
#define Z_STEP 0x08 // b
#define Z_STOP 0x10 // c
#define ZENABLE 0x20 // a

#define ONE_MM 80
#define ONE_CM 800
#define Z_ONE_MM 400
#define Z_ONE_CM 4000

#define X_LEFT PORTC | X_DIR
#define X_RIGHT PORTC & ~(X_DIR)
#define Y_UP PORTC | Y_DIR
#define Y_DOWN PORTC & ~(Y_DIR)
#define Z_UP PORTB | Z_DIR
#define Z_DOWN PORTB & ~(Z_DIR)

enum{ // 열거형  날자 표현할때 자주 사용 0,1,2,3이런식으로 나옴  define 대용
  x_left, x_right, y_up, y_down, z_up, z_down
};

volatile char is_x_reset = 0;
volatile int x_distance = 0;

volatile char is_y_reset = 0;
volatile int y_distance = 0;

volatile char is_z_reset = 0;
volatile int z_distance = 0;

volatile double currunt_x = 0; // 현재 좌표 && mm 단위로 봄
volatile double currunt_y = 0;

volatile int xz_changer = 0;

void x_move(int x_dis, int DIR, int speed)
{
  //dir
  if(DIR == x_left) PORTC = X_LEFT;
  if(DIR == x_right) PORTC = X_RIGHT;
  // distance
  x_distance = x_dis;
  // speed
  OCR1A = speed;
  // on
  xz_changer = 0;
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

void z_move(int z_dis, int DIR, int speed)
{
  // dir
  if(DIR == z_up) PORTB = Z_UP;
  if(DIR == z_down) PORTB = Z_DOWN;
  // distance
  z_distance = z_dis;
  // speed
  OCR1A = speed;
  // on
  xz_changer = 1;
  TIMSK1 = 0x02;
}

void ushift(double x, double y, int speed) // 대각선을 속도를 맞춰서 그리게하는 함수
{
	double dis_X = 0; // 변수들 초기화
	double dis_Y = 0; 
	int x_dir = x_right;
	int y_dir = y_down;

	dis_X = abs(currunt_x - x); // 가야할 거리를 음수가 아니게 만들어서 값을 저장해준다
	dis_Y = abs(currunt_y - y);

	if ((double)currunt_x > (double)x) x_dir = x_left; // 가야할 방향을 맞추게 만들었다
	if ((double)currunt_y > (double)y) y_dir = y_up;

	if(currunt_x - x == 0) // 하나의 축(x,y)이 움직이지 않으면 반대축만 움직이게 한다
	{
		y_move((dis_Y * ONE_MM), y_dir, speed);
		currunt_y = y;
		while (TIMSK3 != 0X00);
		if(is_y_reset == 1) is_y_reset = 0;
		return;
	}
	if(currunt_y - y == 0) 
	{
		x_move((dis_X * ONE_MM), x_dir, speed);
		currunt_x = x;
		while (TIMSK1 != 0X00);
		if(is_x_reset == 1) is_x_reset = 0;
		return;
	}

	double x_speed = 0; 
	double y_speed = 0;
	double angle = 0;

	angle = atan(dis_Y/dis_X); // 앵글 == 아크탄젠트(길이(y/x))

	x_speed = speed * (1/cos(angle)); // 원래 스피드를 구하는 식
	y_speed = speed * (1/sin(angle));

	x_move((dis_X * ONE_MM), x_dir, x_speed);
	y_move((dis_Y * ONE_MM), y_dir, y_speed);

	currunt_x = x; // 현재 좌표를 저장한다
	currunt_y = y;

	while (TIMSK1 != 0X00 || TIMSK3 != 0X00); // 끝나는걸 기다려준다
	if(is_x_reset == 1) is_x_reset = 0;
	if(is_y_reset == 1) is_y_reset = 0;
}

void reset()
{
  is_x_reset = 1;
  is_y_reset = 1;
  is_z_reset = 1;
  z_move(32000, z_down, 600);
  while(is_z_reset != 0);
  delay(100);
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

	DDRA |= ZENABLE;
	DDRB |= Z_DIR; 
	DDRB |= Z_STEP;
	DDRB &= ~(Z_STOP);
	DDRB |= 0x01 | 0x02;

	TCCR1A = 0x00;
	TCCR1B = 0x0a;
	TCCR1C = 0x00;
	OCR1A = 400;
	TIMSK1 = 0x00;

	TCCR2A = 0x02;
	TCCR2B = 0x06;
	TCNT2 = 0x00;
	OCR2A = 240;
	TIMSK2 = 0x00;

	TCCR3A = 0x00;
	TCCR3B = 0x0a;
	TCCR3C = 0x00;
	OCR3A = 400;
	TIMSK3 = 0x00;

	TIMSK2 = 0x02;
	//reset();
}

void loop() // 커스텀 프린터 태스트
{
	ushift(10,10,600);
	ushift(10,100,600);
	ushift(100,100,600);
	ushift(100,10,600);
}

volatile char x_step_toggle = 0;
volatile int x_step_count = 0;

volatile char y_step_toggle = 0;
volatile int y_step_count = 0;

volatile char z_step_toggle = 0;
volatile int z_step_count = 0;

volatile char toggle = 0;

ISR(TIMER1_COMPA_vect)
{
	if(xz_changer == 0)
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
	else
	{
		if(z_step_toggle == 0)
		{
			z_step_toggle = 1;
			PORTB |= Z_STEP;
		}
		else
		{
			z_step_toggle = 0;
			PORTB &= ~(Z_STEP);
			z_step_count++;
			char z_limit_switch = PINC & Z_STOP;

			if(z_step_count >= z_distance)
			{
				is_z_reset = 0;
				z_step_count = 0;
				TIMSK1 = 0x00;
			}
			if(z_limit_switch && is_z_reset != -1)
			{
				TIMSK1 = 0x00;
				z_step_count = 0;
				if(is_z_reset == 1)
				{
					is_z_reset = -1;
					z_move(Z_ONE_MM, z_up, 400);
				}
			}
		}
    }
}
// zsetp = 0.0025

ISR(TIMER2_COMPA_vect)
{
	if(toggle == 0)
	{
		toggle = 1;
		PORTB |= 0x02;
	}
	else
	{
		toggle = 0;
		PORTB &= ~0x02;
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