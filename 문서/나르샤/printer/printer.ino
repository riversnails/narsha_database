#include <avr/pgmspace.h>
#include "parsing.h"
// #define X_DIR 0x20 // c
// #define X_STEP 0x80 // d
// #define X_STOP 0x04 // c 2

// #define Y_DIR 0x80 // c
// #define Y_STEP 0x40 // c
// #define Y_STOP 0x08 // c 3

// #define E_DIR 0x01 // b
// #define E_STEP 0x02 // b

// #define XYEENABLE 0x40 // d

// #define Z_DIR 0x04 // b
// #define Z_STEP 0x08 // b
// #define Z_STOP 0x10 // c
// #define ZENABLE 0x20 // a

//-------------------------------------
#define X_STEP 0x01 // f
#define X_DIR 0x02 // f
#define X_ENABLE 0x80 // d  
//#define X_STOP

#define Y_STEP 0x40 // f
#define Y_DIR 0x80 // f
#define Y_ENABLE 0x04 // f

#define Z_STEP 0x08 // l
#define Z_DIR 0x01 // l
#define Z_ENABLE 0x01 // k

#define E_STEP 0x10 // a
#define E_DIR 0x40 // a
#define E_ENABLE 0x04 // a

#define E1_STEP 0x02 // c
#define E1_DIR 0x08 // c
#define E1_ENABLE 0x80 // c
//-------------------------------------

#define X_MIN 0x20 // e
#define Y_MIN 0x02 // j
#define Z_MIN 0x08 // d

#define ONE_MM 80
#define ONE_CM 800
#define Z_ONE_MM 400
#define Z_ONE_CM 4000

#define X_LEFT PORTF & ~(X_DIR)
#define X_RIGHT PORTF | X_DIR
#define Y_UP PORTF & ~(Y_DIR)
#define Y_DOWN PORTF | Y_DIR
#define Z_UP PORTL & ~(Z_DIR)
#define Z_DOWN PORTL | Z_DIR

enum { 
  x_left, x_right, y_up, y_down, z_up, z_down
};

// move value
volatile char x_reset = 0;
volatile int x_distance = 0;
volatile char y_reset = 0;
volatile int y_distance = 0;
volatile char z_reset = 0;
volatile int z_distance = 0;
volatile double current_x = 0; // 현재 좌표 && mm 단위로 봄
volatile double current_y = 0;
volatile double current_z = 0;

// temp value
volatile int end_analog_value = 0;
volatile int bed_analog_value = 0;
volatile int bed_set_temp = 865;
volatile int end_set_temp = 95;

const PROGMEM int speeds[911] = {
  667, 667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 156,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  156, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  156, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  156, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 667, 667,
  667, 156, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 1000,
  1000, 1000, 156,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  156, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 156, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625, 625, 625,
  625,
};

void x_move(double x_dis, int DIR, int speed)
{
  //dir
  if (DIR == x_left) PORTF = X_LEFT;
  if (DIR == x_right) PORTF = X_RIGHT;
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
  if (DIR == y_up) PORTF = Y_UP;
  if (DIR == y_down) PORTF = Y_DOWN;
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
  if (DIR == z_up) PORTL = Z_UP;
  if (DIR == z_down) PORTL = Z_DOWN;
  // distance
  z_distance = z_dis;
  // speed
  OCR4A = speed;
  // on
  TIMSK4 = 0x02;
}

void ushift(float x, float y, int speed) // 대각선을 속도를 맞춰서 그리게하는 함수
{
  float dis_X = 0; // 변수들 초기화
  float dis_Y = 0;
  int x_dir = x_right;
  int y_dir = y_up;

  dis_X = abs(current_x - x); // 가야할 거리를 음수가 아니게 만들어서 값을 저장해준다
  dis_Y = abs(current_y - y);

  if ((float)current_x > (float)x) x_dir = x_left; // 가야할 방향을 맞추게 만들었다
  if ((float)current_y > (float)y) y_dir = y_down;

  if (current_x - x == 0) // 하나의 축(x,y)이 움직이지 않으면 반대축만 움직이게 한다
  {
    y_move((dis_Y * ONE_MM), y_dir, speed);
    current_y = y;
    while (TIMSK3 != 0X00);
    if (y_reset == 1) y_reset = 0;
    return;
  }
  if (current_y - y == 0)
  {
    x_move((dis_X * ONE_MM), x_dir, speed);
    current_x = x;
    while (TIMSK1 != 0X00);
    if (x_reset == 1) x_reset = 0;
    return;
  }

  float x_speed = 0;
  float y_speed = 0;
  float angle = 0;

  angle = atan(dis_Y / dis_X); // 앵글 == 아크탄젠트(길이(y/x))

  x_speed = speed * (1 / cos(angle)); // 원래 스피드를 구하는 식
  y_speed = speed * (1 / sin(angle));

  x_move((dis_X * ONE_MM), x_dir, x_speed);
  y_move((dis_Y * ONE_MM), y_dir, y_speed);

  current_x = x; // 현재 좌표를 저장한다
  current_y = y;

  // Serial.print(x); // 디버깅용 코드
  // Serial.print(" , y=");
  // Serial.print(y);
  // Serial.print(" , x_dir=");
  // Serial.print(x_dir);
  // Serial.print(" , y_dir=");
  // Serial.print(y_dir);
  // Serial.println();
  //while (TIMSK1 != 0X00 || TIMSK3 != 0X00); // 끝나는걸 기다려준다
  if (x_reset == 1) x_reset = 0;
  if (y_reset == 1) y_reset = 0;
}

void reset()
{
  x_reset = 1;
  y_reset = 1;
  z_reset = 1;
  z_move(32000, z_down, 600);
  while (z_reset != 0);
  delay(100);
  x_move(32000, x_left, 600);
  y_move(32000, y_down, 600);
  while (x_reset != 0 || y_reset != 0); // 끝날때까지 대기
  current_x = 0; // 초기화 했으니 좌표도 초기화
  current_y = 0;
}

unsigned long heat_p_millis = 0;

void heat_control(unsigned long heat_c_millis)
{
  end_analog_value = analogRead(A13);
  bed_analog_value = analogRead(A14);
  
  if(bed_analog_value < bed_set_temp)
  {
    digitalWrite(A5, LOW);
  }
  else if(bed_set_temp+5 < bed_analog_value)
  {
    digitalWrite(A5, HIGH);
  }

  if(end_analog_value < end_set_temp)
  {
    digitalWrite(10, LOW);
  }
  else if(end_set_temp+5 < end_analog_value)
  {
    digitalWrite(10, HIGH);
  }
    
  if(heat_c_millis - heat_p_millis > 1000)
  {
    heat_p_millis = heat_c_millis;
    Serial.print(" b:");
    Serial.print(bed_analog_value);
    Serial.print(" e:");
    Serial.print(end_analog_value);
    Serial.println(" ");
  }
}

void init_pin() // 핀 설정
{
  DDRF |= X_STEP | X_DIR;
  DDRD |= X_ENABLE;
  DDRF |= Y_STEP | Y_DIR | Y_ENABLE;
  DDRL |= Z_STEP | Z_DIR;
  DDRK |= Z_ENABLE;
  DDRA |= E_STEP | E_DIR | E_ENABLE;
  DDRC |= E1_STEP | E1_DIR | E1_ENABLE;

  DDRE &= ~(X_MIN);
  DDRJ &= ~(Y_MIN);
  DDRD &= ~(Z_MIN);

  PORTE |= X_MIN;
  PORTJ |= Y_MIN;
  PORTD |= Z_MIN;
  PORTA |= E_DIR;  

  pinMode(8, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(A13, INPUT); // ther 0
  pinMode(A14, INPUT); // ther 1
  pinMode(A5, OUTPUT); // relay pin
}

void setup()
{
  init_pin();
  //---------------------------
  // DDRD |= 0x40;
  // DDRB |= 0x03;
  // PORTB &= ~0x01;
  // DDRD |= 0x10;
  // PORTD &= ~0x10; // bed
  // DDRD |= 0x20;
  // PORTD &= ~0x20; // end
  // DDRB |= 0x10; // fan
  //---------------------------

  // x
  TCCR1A = 0x00;
  TCCR1B = 0x0a;
  TCCR1C = 0x00;
  OCR1A = 400;
  TIMSK1 = 0x00;

  // e
  TCCR2A = 0x02;
  TCCR2B = 0x06;
  TCNT2 = 0x00;
  OCR2A = 240;
  TIMSK2 = 0x00;

  // y
  TCCR3A = 0x00;
  TCCR3B = 0x0a;
  TCCR3C = 0x00;
  OCR3A = 400;
  TIMSK3 = 0x00;

  // z
  TCCR4A = 0x00;
  TCCR4B = 0x0a;
  TCCR4C = 0x00;
  OCR4A = 400;
  TIMSK4 = 0x00;

 // z_distance = 5000;
  //TIMSK4 = 0x02;
  current_y = 0;
  current_x = 0;
  //reset();

  reset();

  Serial.begin(115200);
  int i = 0;
  while(1)
  {
    ushift(pgm_read_float_near(&xy_pos1[i][0]), pgm_read_float_near(&xy_pos1[i][1]), 600);
    i++;
  }

//  while(1)
//  {
//    for(int i = 0; i < 911; i++)
//    {
//      Serial.print(" x:");
//      Serial.print(pgm_read_float_near(&xy_pos[i][0]));
//      Serial.print(" y:");
//      Serial.print(pgm_read_float_near(&xy_pos[i][0]));
//      Serial.print(" speed:");
//      Serial.println(pgm_read_word_near(&speeds[i]));
//      delay(100);
//    }
//  }
  while (1)
  {
    heat_control(millis());
    if(end_analog_value <= end_set_temp+3 && bed_analog_value <= bed_set_temp+3)
    {
      break;
    }
  }

  Serial.println("setup end");
}

volatile unsigned long c_millis = 0;
volatile unsigned long p_millis = 0;
volatile unsigned long c_micros = 0;
volatile unsigned long p_micros = 0;
volatile int i = 0;
volatile int count = 0;
int atoggle = 0;

void loop() // 커스텀 프린터 태스트
{
  c_millis = millis();
  heat_control(c_millis);

  

  if(TIMSK1 == 0X00 && TIMSK3 == 0X00)
  {
    atoggle = 1;
    
  }

  if(atoggle == 1)
  {
    atoggle = 0;
    //Serial.printf(" %lf, %lf, %lf",xy_pos[i][0], xy_pos[i][1], speeds[i]);
    //ushift(pgm_read_float_near(&xy_pos[i][0]), pgm_read_float_near(&xy_pos[i][1]), pgm_read_word_near(&speeds[i]));
    if(i == 911) i = 0;
  }
}

volatile char x_step_toggle = 0;
volatile int x_step_count = 0;

volatile char y_step_toggle = 0;
volatile int y_step_count = 0;

volatile char z_step_toggle = 0;
volatile int z_step_count = 0;

volatile char e_step_toggle = 0;

ISR(TIMER1_COMPA_vect)
{
  if (x_step_toggle == 0)
  {
    x_step_toggle = 1;
    PORTF |= X_STEP;
  }
  else
  {
    x_step_toggle = 0;
    PORTF &= ~(X_STEP);
    x_step_count++;
    char x_limit_switch = PINE & X_MIN;

    if (x_step_count >= x_distance)
    {
      x_step_count = 0;
      TIMSK1 = 0x00;
    }
    if(x_limit_switch != 0 && x_reset == 1)
    {
      TIMSK1 = 0x00;
      x_step_count = 0;
      x_reset = 0;
    }
  }
}

ISR(TIMER2_COMPA_vect) {
  if (e_step_toggle == 0)
  {
    e_step_toggle = 1;
    PORTA |= E_STEP;
  }
  else
  {
    e_step_toggle = 0;
    PORTA &= ~(E_STEP);
  }
}

ISR(TIMER3_COMPA_vect)
{
  if (y_step_toggle == 0)
  {
    y_step_toggle = 1;
    PORTF |= Y_STEP;
  }
  else
  {
    y_step_toggle = 0;
    PORTF &= ~(Y_STEP);
    y_step_count++;
    char y_limit_switch = PINJ & Y_MIN;

    if (y_step_count >= y_distance)
    {
      y_step_count = 0;
      TIMSK3 = 0x00;
    }
    if(y_limit_switch != 0 && y_reset == 1)
    {
      TIMSK3 = 0x00;
      y_step_count = 0;
      y_reset = 0;
    }
  }
}

ISR(TIMER4_COMPA_vect)
{
  if (z_step_toggle == 0)
  {
    z_step_toggle = 1;
    PORTL |= 0x08;
  }
  else
  {
    z_step_toggle = 0;
    PORTL &= ~(0x08);
    z_step_count++;
    char z_limit_switch = PIND & Z_MIN;

    if (z_step_count >= z_distance)
    {
      z_step_count = 0;
      TIMSK4 = 0x00;
    }
    if(z_limit_switch != 0 && z_reset == 1)
    {
      TIMSK4 = 0x00;
      z_step_count = 0;
      z_reset = 0;
    }
  }
}
//1 step = 0.0125mm
//80 step = 1mm
//1 zsetp = 0.0025mm
