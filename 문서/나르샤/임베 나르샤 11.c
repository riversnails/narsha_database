#define X_STEP_PIN         54
#define X_DIR_PIN          55
#define X_ENABLE_PIN       38
#define X_MIN_PIN           3
#define X_MAX_PIN           2

#define Y_STEP_PIN         60
#define Y_DIR_PIN          61
#define Y_ENABLE_PIN       56
#define Y_MIN_PIN          14
#define Y_MAX_PIN          15

#define Z_STEP_PIN         46
#define Z_DIR_PIN          48
#define Z_ENABLE_PIN       62
#define Z_MIN_PIN          18
#define Z_MAX_PIN          19

#define E_STEP_PIN         26
#define E_DIR_PIN          28
#define E_ENABLE_PIN       24

#define SDPOWER            -1
#define SDSS               53
#define LED_PIN            13

#define FAN_PIN            9

#define PS_ON_PIN          12
#define KILL_PIN           -1

#define HEATER_0_PIN       10
#define HEATER_1_PIN       8
#define TEMP_0_PIN          13   // ANALOG NUMBERING
#define TEMP_1_PIN          14   // ANALOG NUMBERING


void setup() {
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(X_MIN_PIN, INPUT);
  pinMode(X_MAX_PIN, INPUT);
  
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(Y_MIN_PIN, INPUT);
  pinMode(Y_MAX_PIN, INPUT);

  digitalWrite(X_DIR_PIN, LOW);
  digitalWrite(X_ENABLE_PIN, LOW);

  digitalWrite(Y_DIR_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  digitalWrite(Y_MIN_PIN, HIGH);
  
  Serial.begin(9600);

  TCCR2A = 0x02;
  TCCR2B = 0x02;
  
//  for(int i = 0; i < 5000; i++)
//  {
//    digitalWrite(X_STEP_PIN, HIGH);
//    Serial.println(
//    delayMicroseconds(1000);
//    digitalWrite(X_STEP_PIN, LOW);
//    delayMicroseconds(1000);
//  }

//  while(1)
//  {
//    int input_test = digitalRead(Y_MIN_PIN);
//    Serial.println(input_test);
//    delay(100);
//  }
  
  while(1)
  {
    int input = digitalRead(Y_MIN_PIN);
    if(input == 1)
    {
      break;
    }

    digitalWrite(Y_STEP_PIN, HIGH);
    delayMicroseconds(200);
    digitalWrite(Y_STEP_PIN, LOW);
    delayMicroseconds(200);
  }
}

void loop() // 3d 프린터 개조보드를 이용한 조작
{ 
  
}