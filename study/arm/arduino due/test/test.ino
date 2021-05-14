#include<DueTimer.h>

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  while(!Serial);
  
  pinMode(13, OUTPUT);
  
  Timer.getAvailable().attachInterrupt(led_isr).start(100);
}

// the loop function runs over and over again forever
void loop() {
  
}

char led_flag = 0;
int led_count = 0;

void led_isr()
{
  if(led_count == 100)
  {
    Serial.println("asdf");
    digitalWrite(13, HIGH);
    led_count = 0;
  }
  if(led_count == 10)
  {
    digitalWrite(13, LOW);
  }

  led_count++;
  
//  if(led_flag == 0)
//  {
//    led_flag = 1;
//    digitalWrite(13, HIGH);
//  }
//  else
//  {
//    led_flag = 0;
//    digitalWrite(13, LOW);
//  }
}
