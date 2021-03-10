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
#define E_ENABLE 0x02 // a

#define E1_STEP 0x02 // c
#define E1_DIR 0x08 // c
#define E1_ENABLE 0x80 // c
//-------------------------------------

#define X_MIN 0x20 // e
#define Y_MIN 0x02 // j
#define Z_MIN 0x08 // d

#define BLTOUCH 0x10 //PA4
#define ONE_mm 80
#define ONE_cm 800
#define z_cm 4000

#define X_LEFT PORTF & ~(X_DIR)
#define X_RIGHT PORTF | X_DIR
#define Y_UP PORTF & ~(Y_DIR)
#define Y_DOWN PORTF | Y_DIR
#define Z_UP PORTL & ~(Z_DIR)
#define Z_DOWN PORTL | Z_DIR

#define LCD_SCK  0x02 //PA1
#define LCD_CS   0x08 // PA3
#define LCD_MOSI 0x02 // PC1

#define Encoder_A 0x04
#define Encoder_B 0x08
#define Encoder_Button 0x01

enum {
  x_left, x_right, y_up, y_down, z_up, z_down
};

volatile char x_reset = 0;
volatile int x_distance = 0;
volatile char y_reset = 0;
volatile int y_distance = 0;
volatile char z_reset = 0;
volatile int z_distance = 0;
volatile double current_x = 0; // 현재 좌표 && mm 단위로 봄
volatile double current_y = 0;
volatile double current_z = 0;

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

double radian(double anagle) {
  return anagle * (PI / 180);
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

char menu = '0';
char moveMenu = '0';

int c_state = 0;
int p_state = 0;
int rotary_cnt = 0;
unsigned long button_p_millis = 0;
unsigned long processMenu_p_millis = 0;

void rotate_button(unsigned long button_c_millis)
{
  if (button_c_millis - button_p_millis > 10)
  {
    int state_A = PIND & Encoder_A; // read pin
    int state_B = PIND & Encoder_B;

    if (state_A != 0) state_A = 1; // change 0 or 1
    if (state_B != 0) state_B = 1;

    if (state_A == 0 && state_B == 1) c_state = 1; // change now state
    else if (state_A == 0 && state_B == 0) c_state = 2;
    else if (state_A == 1 && state_B == 0) c_state = 3;
    else if (state_A == 1 && state_B == 1) c_state = 4;

    if (c_state == 3 && p_state == 2)
    {
      rotary_cnt++;
      Serial.print("right : ");
      Serial.println(rotary_cnt);
    }

    if (c_state == 1 && p_state == 2)
    {
      rotary_cnt--;
      Serial.print("left : ");
      Serial.println(rotary_cnt);
    }

    p_state = c_state;
  }
}

void move_func(char axis, float move_length, int DIR) // 축, 거리모드, 방향
{
//    Serial.print(moveMenu);
//    Serial.print(" ");
//    Serial.print(menu);
//    Serial.print(" ");
//    Serial.print(axis);
//    Serial.print(" ");
//    Serial.print(move_length);
//    Serial.print(" ");
//    Serial.print(DIR);
//    Serial.print(" ");
//    Serial.print(x_right);
  printMenu(axis);
  if (axis == 'x')
  {
    if (DIR == x_right) current_x += move_length;
    else if (DIR == x_left)
    {
      if (current_x - move_length < 0) return;
      current_x -= move_length;
    }
    x_move(move_length * ONE_mm, DIR, 400);
  }
  else if (axis == 'y')
  {
    if (DIR == y_up) current_y += move_length;
    else if (DIR == y_down)
    {
      if (current_y - move_length < 0) return;
      current_y -= move_length;
    }
    y_move(move_length * ONE_mm, DIR, 400);
  }
  else if (axis == 'z')
  {
    // if (current_z < 0) return; // z축은 레벨링때문에 막기 꺼둠
    if (DIR == z_up) current_z += move_length;
    else if (DIR == z_down)
    {
      //if (current_z - move_length < 0) return;
      current_z -= move_length;
    }
    z_move(move_length * ONE_mm, DIR, 400);
  }

}

void printMenu(char move_axis) {
  Serial.write(12); // ffeed == clear table
  if (menu == '0') {
    Serial.println("auto home - 1");
    Serial.println("x move - 2");
    Serial.println("y move - 3");
    Serial.println("z move - 4");
  }
  else if (menu == '1') {
    Serial.println("starting auto home");
  }
  else if (menu == '2' || menu == '3' || menu == '4') {
    if (moveMenu == '0') {
      Serial.println("back - 1");
      Serial.println("10mm move - 2");
      Serial.println("1mm move - 3");
      Serial.println("0.1mm move - 4");
    }
    else if (moveMenu != '1') {
      Serial.println("back - 1");
      Serial.print(move_axis);
      Serial.print(" : ");
      if (menu == '2') Serial.println(current_x);
      else if (menu == '3') Serial.println(current_y);
      else if (menu == '4') Serial.println(current_z);
    }
  }
}

void processMenu(unsigned long processMenu_c_millis)
{
  if (processMenu_c_millis - processMenu_p_millis > 500)
  {
    processMenu_p_millis = processMenu_c_millis;
    printMenu(menu + 70);
  }

  if (menu == '0') { // input menu
    if (Serial.available()) {
      menu = Serial.read();
    }
  }
  else if (menu == '1') {
    if (z_reset != 0 && y_reset != 0 && x_reset != 0)
    {
      Serial.write(12);
      Serial.println("do auto home... waiting");
      reset();
    }
    if (z_reset && y_reset && x_reset) {
      menu = '0';
    }
  }
  else if (menu == '2' || menu == '3' || menu == '4') { // loop moveMenu
    if (moveMenu == '0') { // out check
      if (Serial.available()) {
        moveMenu = Serial.read();

        if (moveMenu == '1') {
          menu = '0';
          moveMenu = '0';
        }
      }
    }
    else if (moveMenu == '2') {
      if (Serial.available()) {
        char dir = Serial.read();
        
        if (dir == '1') {
          moveMenu = '0';
        }
        else if (dir == 'a') {
          if (menu == '2') move_func('x', 10, x_left);
          else if (menu == '3') move_func('y', 10, y_up);
          else if (menu == '4') move_func('z', 10, z_down);
        }
        else if (dir == 'd') {
          if (menu == '2') move_func('x', 10, x_right);
          else if (menu == '3') move_func('y', 10, y_down);
          else if (menu == '4') move_func('z', 10, z_up);
        }
      }
    }
    else if (moveMenu == '3') {
      if (Serial.available()) {
        char dir = Serial.read();
        
        if (dir == '1') {
          moveMenu = '2';
        }
        else if (dir == 'a') {
          if (menu == '2') move_func('x', 1, x_left);
          else if (menu == '3') move_func('y', 1, y_up);
          else if (menu == '4') move_func('z', 10, z_down);
        }
        else if (dir == 'd') {
          if (menu == '2') move_func('x', 1, x_right);
          else if (menu == '3') move_func('y', 1, y_down);
          else if (menu == '4') move_func('z', 1, z_up);
        }
      }
    }
    else if (moveMenu == '4') {
      if (Serial.available()) {
        char dir = Serial.read();
        
        if (dir == '1') {
          moveMenu = '2';
        }
        else if (dir == 'a') {
          if (menu == '2') move_func('x', 0.1, x_left);
          else if (menu == '3') move_func('y', 0.1, y_up);
          else if (menu == '4') move_func('z', 0.1, z_down);
        }
        else if (dir == 'd') {
          if (menu == '2') move_func('x', 0.1, x_right);
          else if (menu == '3') move_func('y', 0.1, y_down);
          else if (menu == '4') move_func('z', 0.1, z_up);
        }
      }
    }
  }
}

void setup() {
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

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(A13, INPUT); // ther 0
  pinMode(A14, INPUT); // ther 1
  pinMode(A5, OUTPUT); // relay pin

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

  DDRA |= BLTOUCH;

  PORTA |= BLTOUCH;

  DDRD & ~(Encoder_A | Encoder_B);
  DDRC & ~(Encoder_Button);

  PORTD |= Encoder_A | Encoder_B;
  PORTC |= Encoder_Button;

  Serial.begin(9600);
}

unsigned long c_millis = 0;

void loop() {
  c_millis = millis();

  processMenu(c_millis);
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
    if (x_limit_switch != 0 && x_reset == 1)
    {
      TIMSK1 = 0x00;
      x_step_count = 0;
      x_reset = 0;
    }
  }
}

ISR(TIMER2_COMPA_vect) {
  if (toggle == 0)
  {
    toggle = 1;
    PORTA |= E_STEP;
  }
  else
  {
    toggle = 0;
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
    if (y_limit_switch != 0 && y_reset == 1)
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
    if (z_limit_switch != 0 && z_reset == 1)
    {
      TIMSK4 = 0x00;
      z_step_count = 0;
      z_reset = 0;
    }
  }
}