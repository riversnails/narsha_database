#define X_STEP_PIN         0x80 //PD7 0100 0000
#define X_DIR_PIN          0x20 //PC5 0010 0000
#define X_STOP_PIN         0x04 //PC2 0000 0100
#define XYEENABLE_PIN      0x40 //PD6 0100 0000
#define Y_STEP_PIN         0x40 //PC6 0100 0000
#define Y_DIR_PIN          0x80 //PC7 1000 0000
#define Y_STOP_PIN         0x08 //PC3 0000 1000
#define Z_STEP_PIN         0x08 //PB3
#define Z_DIR_PIN          0x04 //PB2
#define Z_STOP_PIN         0x10 //PC4
#define DIR_STOP  0
#define DIR_UP    1
#define DIR_LEFT  2
#define DIR_DOWN  3
#define DIR_RIGHT 4

volatile int toggle_x = 0;
volatile int step_toggle_x = 0;
volatile int limit_switch_x = 0;
unsigned step_count_x = 0;

volatile int toggle_y = 0;
volatile int step_toggle_y = 0;
volatile int limit_switch_y = 0;
unsigned step_count_y = 0;

volatile int max_len_x = 0;
volatile int on_off_x = 0;
volatile int move_on_x = 0;

volatile int max_len_y = 0;
volatile int on_off_y = 0;
volatile int move_on_y = 0;

volatile int no_check_limit = 0;

void X_move(int len, int speed, char direction)
{
  if (on_off_x == 0) {
    on_off_x = 1; // 토글로 여러번 방지
    max_len_x = len;
    OCR1A = speed;

    if (direction != DIR_STOP) move_on_x = 1; // 방향의 유효성 검사
    else move_on_x = 0;

    TIMSK1 = 0x02; // 작동시작
    if (direction == DIR_LEFT) // 방향조절 핀
      digitalWrite(X_DIR_PIN, HIGH, 'C');
    else 
      digitalWrite(X_DIR_PIN, LOW, 'C');

  }
}

void Y_move(int len, int speed, char direction)
{
  if (on_off_y == 0) {
    on_off_y = 1;
    max_len_y = len;
    OCR3A = speed;

    if (direction != DIR_STOP) move_on_y = 1;
    else move_on_y = 0;

    TIMSK3 = 0x02;
    if (direction == DIR_UP)
      digitalWrite(Y_DIR_PIN, HIGH, 'C');
    else 
      digitalWrite(Y_DIR_PIN, LOW, 'C');
  }
}

void reset()
{
  int reset_toggle = 0;

  while(1)
  {
    if(reset_toggle == 0 && on_off_y == 0)
    {
      X_move(100000, 800, DIR_LEFT);
        reset_toggle = 1;
    }
    if(reset_toggle == 1 && on_off_x == 0)
    {
      no_check_limit = 1;
      step_count_x = 0;
      X_move(1000, 200, DIR_RIGHT);
      reset_toggle = 2;
    }
    if(reset_toggle == 2 && on_off_x == 0)
    {
      no_check_limit = 0;
      Y_move(100000, 800, DIR_UP);
      reset_toggle = 3;
    }
    if(reset_toggle == 3 && on_off_y == 0)
    {
      no_check_limit = 1;
      step_count_y = 0;
      Y_move(1000, 200, DIR_DOWN);
      reset_toggle = 4;
      break;
    }

    no_check_limit = 0;
  }
}

void digitalWrite(int PIN, int TYPE, char PORT)
{
  if(PORT == 'A')
  {
    if(TYPE == 0) PORTA &= ~PIN; 
    else PORTA |= PIN;
  }
  else if(PORT == 'B')
  {
    if(TYPE == 0) PORTB &= ~PIN;
    else PORTB |= PIN;
  }
  else if(PORT == 'C')
  {
    if(TYPE == 0) PORTC &= ~PIN;
    else PORTC |= PIN;
  }
  else if(PORT == 'D')
  {
    if(TYPE == 0) PORTD &= ~PIN;
    else PORTD |= PIN;
  }
}

int digitalRead(int PIN, char PORT)
{
  if(PORT == 'A')
  {
    if((PINC & PIN) != 0) return 1; 
    else return 0;
  }
  else if(PORT == 'B')
  {
    if((PINB & PIN) != 0) return 1;
    else return 0;
  }
  else if(PORT == 'C')
  {
    if((PINC & PIN) != 0) return 1;
    else return 0;
  }
  else if(PORT == 'D')
  {
    if((PINC & PIN) != 0) return 1;
    else return 0;
  }
}

void setup()
{
  TCCR1A = 0x00; // 타이머 초기화
  TCCR1B = 0x0A;
  TCCR1C = 0x0A;
  TIMSK1 = 0x00;
  OCR1A = 400;
  TCCR3A = 0x00;
  TCCR3B = 0x0A;
  TCCR3C = 0x0A;
  TIMSK3 = 0x00;
  OCR3A = 400;

  DDRB = Z_STEP_PIN | Z_DIR_PIN;
  DDRC = X_DIR_PIN | Y_STEP_PIN | Y_DIR_PIN;
  DDRD = X_STEP_PIN | XYEENABLE_PIN;
  DDRC &= ~(X_STOP_PIN | Y_STOP_PIN | Z_STOP_PIN);

  PORTB = 0x00;
  PORTC = 0x00;
  PORTD = 0x00;
}

int toggle_1 = 0;

void loop() // 리셋인데 더럽게 짠 코드
{
  reset();
  delay(10000);

}

ISR(TIMER1_COMPA_vect) {
  if (step_toggle_x == 0) {

    if (move_on_x == 1)
      digitalWrite(X_STEP_PIN, HIGH, 'D');

    step_toggle_x = 1;
  }
  else {

    if (move_on_x == 1)
      digitalWrite(X_STEP_PIN, LOW, 'D');

    step_toggle_x = 0;
    step_count_x++;

    if (step_count_x >= max_len_x) {
      step_count_x = 0;
      TIMSK1 = 0x00;
      on_off_x = 0;

    }

    limit_switch_x = digitalRead(X_STOP_PIN, 'C'); // 엔드스탑 검사

    if (limit_switch_x == 1 && no_check_limit == 0)
    {
      on_off_x = 0;
      TIMSK1 = 0x00;
      TIMSK3 = 0x00;
    }

  }
}

ISR(TIMER3_COMPA_vect) {
  if (step_toggle_y == 0) {

    if (move_on_y == 1)
      digitalWrite(Y_STEP_PIN, HIGH, 'C');

    step_toggle_y = 1;
  }
  else {

    if (move_on_y == 1)
      digitalWrite(Y_STEP_PIN, LOW, 'C');

    step_toggle_y = 0;
    step_count_y++;

    if (step_count_y >= max_len_y) {
      step_count_y = 0;
      TIMSK3 = 0x00;
      on_off_y = 0;
    }

    limit_switch_y = digitalRead(Y_STOP_PIN, 'C'); // 엔드스탑 검사

    if (limit_switch_y == 1 && no_check_limit == 0)
    {
      on_off_y = 0;
      TIMSK1 = 0x00;
      TIMSK3 = 0x00;
    }
  }
}