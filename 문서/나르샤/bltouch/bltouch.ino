
void setup() {
  //Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(10, OUTPUT);
  
  digitalWrite(2, HIGH);
}

unsigned long c_micros = 0;
unsigned long p_micros = 0;
unsigned long p_micros1 = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;
int duty = 20;
int i = 0;
int j = 0; 

void loop() {
  c_micros = micros();
  //c_millis = millis();
  
  if(c_micros - p_micros >= 32)
  {
    p_micros = c_micros;
    
    if(i == 625)
    {
      digitalWrite(10, HIGH);
      i = 0;
      //j++;
    }
    else if(i == duty) // 20, 46 
    {
      digitalWrite(10, LOW);
    }
    
    i++;
  }

  //Serial.println(digitalRead(2));
  if((PINE & 0x10) != 0)
  {
    duty = 46;
  }


//  if(j == 50)
//  {
//    j = 0;
//    if(duty == 46) duty = 20;
//    else duty = 46;
//  }


//  for(int i = 0; i< 50; i++)
//  {
//    digitalWrite(10, HIGH);
//    delayMicroseconds(640);
//    digitalWrite(10, LOW);
//    delay(17);
//    delayMicroseconds(3000-640);
//  }
//  for(int i = 0; i< 50; i++)
//  {
//    digitalWrite(10, HIGH);
//    delayMicroseconds(1472);
//    digitalWrite(10, LOW);
//    delay(17);
//    delayMicroseconds(3000-1472);
//  }
}
