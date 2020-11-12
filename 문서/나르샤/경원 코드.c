#define X_STEP 0x80 //PD7
#define X_DIR 0x20 //PC5
#define X_STOP 0x04 //PC2

#define Y_STEP 0x40 //PC6
#define Y_DIR 0x80 //PC7
#define Y_STOP 0x08 //PC3

#define Z_STEP 0x08 //PB3
#define Z_DIR 0x04 //PB2
#define Z_STOP 0x10 //PC4
#define ZENABLE 0x20 //PA5
#define XYEENABLE 0x40 //PD6

#define LCD_SCK  0x02 //PA1
#define LCD_CS   0x08 // PA3
#define LCD_MOSI 0x02 // PC1

#define BLTOUCH 0x10 //PA4
#define ONE_mm 80
#define ONE_cm 800
#define z_cm 4000

#define Encoder_A 0x04
#define Encoder_B 0x08
#define Encoder_Button 0x01

bool x_step_toggle = true;
bool x_dir_toggle = false;
int x_count = 0;
int x_distance = 0;
double current_x = 0;

bool y_step_toggle = true;
bool y_dir_toggle = false;
int y_count = 0;
int y_distance = 0;

bool z_step_toggle = true;
bool z_dir_toggle = false;
int z_count = 0;
int z_distance = 4000;

/* x_reset */
bool is_x_stop = false;
bool is_x_reset = false;

/* y_reset */
bool is_y_stop = false;
bool is_y_reset = false;

/* z_reset */
bool is_z_stop = false;
bool is_z_finish = false;
bool is_z_reset = false;

bool y_flag = true;
bool is_not_reset = true;

unsigned long c_micros;
unsigned long c_millis;
unsigned long p_millis = 0;
unsigned long rst_micros = 0;

bool is_z_up = true;
bool toggle = false;
int delays = 1475;

bool isOne = true;

typedef enum _DIR {
  X_RIGHT,
  X_LEFT,
  Y_UP,
  Y_DOWN,
  Z_UP,
  Z_DOWN
} DIR;

void setup() {
  /* x setting */
  DDRD |= X_STEP;
  DDRC |= X_DIR;
  DDRC &= ~X_STOP;
  DDRD |= XYEENABLE;

  /*y setting */
  DDRC |= Y_STEP;
  DDRC |= Y_DIR;
  DDRC &= ~Y_STOP;

  /*z setting */
  DDRB |= Z_STEP;
  DDRB |= Z_DIR;
  DDRC &= ~Z_STOP;
  DDRA |= ZENABLE;

  /* x timer setting */
  TCCR1A = 0x00;
  TCCR1B = 0x0A;
  TCCR1C = 0x00;
  OCR1A = 400;
  TIMSK1 = 0x00;

  /* y timer setting */
  TCCR3A = 0x00;
  TCCR3B = 0x0A;
  TCCR3C = 0x00;
  OCR3A = 400;
  TIMSK3 = 0x00;

  DDRA |= BLTOUCH;

  PORTA |= BLTOUCH;

  DDRD & ~(Encoder_A | Encoder_B);
  DDRC & ~(Encoder_Button);

  PORTD |= Encoder_A | Encoder_B;
  PORTC |= Encoder_Button;

  Serial.begin(9600);
}

void loop() {
  c_millis = millis();

  rotate_button(c_millis);
  processMenu(c_millis);
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
  if(button_c_millis - button_p_millis > 10)
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

void printMenu(char men) {
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
  else if (menu == '2') {
    if (moveMenu == '0') {
      Serial.println("back - 1");
      Serial.println("10mm move - 2");
      Serial.println("1mm move - 3");
      Serial.println("0.1mm move - 4");
    } 
    else if (moveMenu != '1') {
      Serial.println("back - 1");
      Serial.println(current_x);
    }
  }
}

void processMenu(unsigned long processMenu_c_millis)
{
  if (processMenu_c_millis - processMenu_p_millis > 1000) 
  {
    processMenu_p_millis = processMenu_c_millis;
    printMenu(menu);
  }

  if (menu == '0') {
    if (Serial.available()) {
      menu = Serial.read();
    }
  }
  else if (menu == '1') {
    if (is_not_reset) {
      is_x_stop = false;
      is_x_reset = false;
      is_y_stop = false;
      is_y_reset = false;
      is_z_stop = false;
      is_z_finish = false;
      is_z_reset = false;
      is_not_reset = false;
    }
    reset(micros());
    if (is_z_reset && is_y_reset && is_x_reset) 
    {
      menu = '0';
      is_not_reset = true;
    }
  } 
  else if (menu == '2') {
    if (moveMenu == '0') {
      if (Serial.available()) {
        moveMenu = Serial.read();
        if (moveMenu == '1') {
          menu = '0';
          moveMenu = '0';
        }
      }
  }

  if (moveMenu == '2') {
    if (Serial.available()) {
      char dir = Serial.read();

      if (dir == '1') {
        moveMenu = '0';
      }
      if (dir == 'a') {
        current_x -= 10;
        x_move(10 * ONE_mm, X_LEFT, 400);
        if (current_x < 0) {
          current_x = 0;
        }
      } 
      else if (dir == 'd') {
        current_x += 10;
        x_move(10 * ONE_mm, X_RIGHT, 400);
      }
    }
  }
  if (moveMenu == '3') {
    if (Serial.available()) {
      char dir = Serial.read();

      if (dir == '1') {
        moveMenu = '2';
      }
      if (dir == 'a') {
        current_x -= 1;
        x_move(1 * ONE_mm, X_LEFT, 400);
        if (current_x < 0) {
          current_x = 0;
        }
      } 
      else if (dir == 'd') {
        current_x += 1;
        x_move(1 * ONE_mm, X_RIGHT, 400);
      }
    }
  }
  if (moveMenu == '4') {
    if (Serial.available()) {
      char dir = Serial.read();

      if (dir == '1') {
        moveMenu = '2';
      }
      if (dir == 'a') {
        current_x -= 0.1;
        x_move(0.1 * ONE_mm, X_LEFT, 400);
        if (current_x < 0) {
          current_x = 0;
        }
      } 
      else if (dir == 'd') {
        current_x += 0.1;
        x_move(0.1 * ONE_mm, X_RIGHT, 400);
      }
    }
  }

}

/* x location ISR */
ISR(TIMER1_COMPA_vect) {
  if (x_step_toggle) {
    x_step_toggle = false;
    PORTD |= X_STEP;
  }
  else {
    x_step_toggle = true;
    PORTD &= ~X_STEP;
    if ((x_count++) > x_distance) {
      x_count = 0;
      TIMSK1 = 0x00;

      if (is_x_stop) {
        is_x_reset = true;
      }
    }

    if (PINC & X_STOP) {
      TIMSK1 = 0x00;
      x_count = 0;
      is_x_stop = true;
    }
  }
}

ISR(TIMER3_COMPA_vect) {
  if (y_flag) {
    if (y_step_toggle) {
      y_step_toggle = false;
      PORTC |= Y_STEP;
    }
    else {
      y_step_toggle = true;
      PORTC &= ~Y_STEP;

      if ((y_count++) > y_distance) {
        y_count = 0;
        TIMSK3 = 0x00;
        if (is_y_stop) {
          is_y_reset = true;
        }
      }

      if (PINC & Y_STOP) {
        TIMSK3 = 0x00;
        y_count = 0;
        is_y_stop = true;
      }
    }
  } else {
    if (z_step_toggle) {
      z_step_toggle = false;
      PORTB |= Z_STEP;
    }
    else {
      z_step_toggle = true;
      PORTB &= ~Z_STEP;

      if ((z_count++) > z_distance) {
        z_count = 0;
        TIMSK3 = 0x00;
        if (is_z_stop) {
          is_z_reset = true;
        }
      }

      if (!is_z_up) {
        if (PINC & Z_STOP) {
          TIMSK3 = 0x00;
          z_count = 0;
          is_z_stop = true;
        }
      }
    }
  }
}

void x_move(double distance, DIR dir, int speed_x) {
  TIMSK1 = 0x02;
  OCR1A = speed_x;
  x_distance = distance;
  if (dir == X_RIGHT) {
    PORTC &= ~X_DIR;
  }
  else if (dir == X_LEFT) {
    PORTC |= X_DIR;
  }
}

void y_move(double distance, DIR dir, int speed_y) {
  y_flag = true;
  TIMSK3 = 0x02;

  y_distance = distance;
  OCR3A = speed_y;

  if (dir == Y_UP) {
    PORTC |= Y_DIR;
  }
  else if (dir == Y_DOWN) {
    PORTC &= ~Y_DIR;
  }
}

void z_move(double distance, DIR dir, int speed_z) {
  TIMSK3 = 0x02;
  y_flag = false;

  z_distance = distance;
  OCR3A = speed_z;

  if (dir == Z_UP) {
    PORTB |= Z_DIR;
    is_z_up = true;
  }
  else if (dir == Z_DOWN) {
    PORTB &= ~Z_DIR;
    is_z_up = false;
  }
}



double radian(double anagle) {
  return anagle * (PI / 180);
}

void reset(unsigned long micros) {
  if (micros - rst_micros > delays) {
    rst_micros = micros;

    if (toggle) {
      PORTA |= BLTOUCH;
    } else {
      PORTA &= ~BLTOUCH;
    }

    toggle = !toggle;
  }

  if (!is_x_reset) {
    if (TIMSK1 == 0x00 && !is_x_stop) {
      x_move(400.0 * ONE_cm, X_LEFT, 400);
    } else if (TIMSK1 == 0x00) {
      x_move(15.7 * ONE_cm, X_RIGHT, 400);

      current_x = 157;
    }

    delay(1);
  } else if (!is_z_reset) {
    if (TIMSK3 == 0x00 && is_z_stop) {
      delays = 1475;
      z_move(5 * z_cm / 10, Z_UP, 400);
    } else if (TIMSK3 == 0x00) {
      delays = 650;
      z_move(400 * z_cm, Z_DOWN, 400);
    }
  }

  if (!is_y_reset) {
    y_flag = true;
    if (TIMSK3 == 0x00 && !is_y_stop) {
      y_move(400.0 * ONE_cm, Y_UP, 400);
    } else if (TIMSK3 == 0x00) {
      y_move(12.3 * ONE_cm, Y_DOWN, 400);
    }

    delay(1);
  }
}