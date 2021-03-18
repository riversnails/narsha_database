void setup() {
  Serial.begin(112500);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
}

char toggle = 0;
char current_state = 0, previous_state = 0;
int lcount = 0;
unsigned long current_millis = 0;
unsigned long previous_millis = 0;

void loop() {
  current_millis = millis();

  if (current_millis - previous_millis > 10)
  {
    previous_millis = current_millis;
    int a = digitalRead(8); // 값 읽어오기
    int b = digitalRead(9);
    //int button = PINC & encoder_button;
    if (a != 0) a = 1; // 이상한 값이 나올 수 있으니 0이 아니면 1로 변환
    if (b != 0) b = 1;

    if (a == 0 && b == 1) current_state = 1;
    else if (a == 0 && b == 0) current_state = 2;
    else if (a == 1 && b == 0) current_state = 3;
    else if (a == 1 && b == 1) current_state = 4;


    if (current_state == 3 && previous_state == 2)
    {
      lcount++;
    }
    if (current_state == 1 && previous_state == 2)
    {
      lcount--;
    }
    Serial.write(13);
    Serial.println(lcount);
    previous_state = current_state;
  }
}
