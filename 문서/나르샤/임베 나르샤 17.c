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
#define digitalWrite_B(PIN, SWITCH) SWITCH == 0?PORTB &= ~PIN : PORTB |= PIN;
#define digitalWrite_C(PIN, SWITCH) SWITCH == 0?PORTC &= ~PIN : PORTC |= PIN;
#define digitalWrite_D(PIN, SWITCH) SWITCH == 0?PORTD &= ~PIN : PORTD |= PIN;
#define digitalRead_B(PIN) (PINB & PIN) != 0 ? 1 : 0;
#define digitalRead_C(PIN) (PINC & PIN) != 0 ? 1 : 0;
#define digitalRead_D(PIN) (PIND & PIN) != 0 ? 1 : 0;

int toggle_x = 0;
int toggle_y = 0;
int step_toggle_1 = 0;
int limit_switch_1 = 0;
unsigned step_count_1 = 0;

int step_toggle_2 = 0;
int limit_switch_2 = 0;
unsigned step_count_2 = 0;
int max_count_1 = 0;
int max_count_2 = 0;

int on_off_x = 0;
int move_on_1 = 0;
int move_on_2 = 0;
int on_off_y = 0;

void X_move(int max, int speed, char direction)
{
  if (on_off_x == 0) {
    on_off_x = 1;
    max_count_1 = max;
    OCR1A = speed;
    if (direction != DIR_STOP) move_on_1 = 1;
    else move_on_1 = 0;
    TIMSK1 = 0x02;
    if (direction == DIR_LEFT) {
      digitalWrite_C(X_DIR_PIN, HIGH);
    }
    else digitalWrite_C(X_DIR_PIN, LOW);
  }
}

void Y_move(int max, int speed, char direction)
{
  if (on_off_y == 0) {
    on_off_y = 1;
    max_count_2 = max;
    OCR3A = speed;
    if (direction != DIR_STOP) move_on_2 = 1;
    else move_on_2 = 0;
    TIMSK3 = 0x02;
    if (direction == DIR_UP) {
      digitalWrite_C(Y_DIR_PIN, HIGH);
    }
    else digitalWrite_C(Y_DIR_PIN, LOW);
  }
}

void setup()
{
  TCCR1A = 0x00;
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

void loop() // 프린터 직각 삼각형 만드는 코드
{
  X_move(3 * 800, 400, DIR_RIGHT);
  Y_move(6 * 800, 200, DIR_UP);
  while (TIMSK1 == 0x02 || TIMSK3 == 0x02);
  X_move(3 * 800, 400, DIR_STOP);
  Y_move(6 * 800, 200, DIR_DOWN);
  while (TIMSK1 == 0x02 || TIMSK3 == 0x02);
  X_move(3 * 800, 400, DIR_LEFT);
  Y_move(6 * 800, 200, DIR_STOP);
  while (TIMSK1 == 0x02 || TIMSK3 == 0x02);
}

ISR(TIMER1_COMPA_vect) {
  if (step_toggle_1 == 0) {
    if (move_on_1 == 1)digitalWrite_D(X_STEP_PIN, HIGH);
    step_toggle_1 = 1;
  }
  else {
    if (move_on_1 == 1)digitalWrite_D(X_STEP_PIN, LOW);
    step_toggle_1 = 0;
    step_count_1++;
    if (step_count_1 >= max_count_1) {
      step_count_1 = 0;
      TIMSK1 = 0x00;
      on_off_x = 0;
    }
    limit_switch_1 = digitalRead_C(X_STOP_PIN);
    if (limit_switch_1 == 1)
    {
      TIMSK1 = 0x00;
      TIMSK3 = 0x00;
    }
  }
}

ISR(TIMER3_COMPA_vect) {
  if (step_toggle_2 == 0) {
    if (move_on_2 == 1)digitalWrite_C(Y_STEP_PIN, HIGH);
    step_toggle_2 = 1;
  }
  else {
    if (move_on_2 == 1)digitalWrite_C(Y_STEP_PIN, LOW);
    step_toggle_2 = 0;
    step_count_2++;
    if (step_count_2 >= max_count_2) {
      step_count_2 = 0;
      TIMSK3 = 0x00;
      on_off_y = 0;
    }
    limit_switch_2 = digitalRead_C(Y_STOP_PIN);
    if (limit_switch_2 == 1)
    {
      TIMSK1 = 0x00;
      TIMSK3 = 0x00;
    }
  }
}