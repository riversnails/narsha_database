#include <SoftwareSerial.h>

int tx = 2;
int rx = 3;
SoftwareSerial mySerial(tx, rx);

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

unsigned long c_millis = 0;
unsigned long p_millis = 0;
unsigned long p_millis1 = 0;
int buzzer_toggle = 1;
int back_data = 0;
void loop()
{
  c_millis = millis();
  
  if (mySerial.available()) {
    char read_data[100] = "\n", cut_data1[100] = "\n", cut_data2[100] = "\n";
    float cut_float_data;

    mySerial.readBytes(read_data, 100);

    for (int i = 0; i < 4; i++) cut_data1[i] = read_data[i];
    for (int i = 0; i < 5; i++) cut_data2[i] = read_data[i + 5];


    if (strcmp(cut_data1, "temp") == 0) {
      cut_float_data = atof(cut_data2);
      if (cut_float_data >= 20) {
        digitalWrite(13, HIGH);
      }
      else digitalWrite(13, LOW);
    }
    else if (strcmp(cut_data1, "humi") == 0) {
      cut_float_data = atof(cut_data2);
      if (cut_float_data >= 40) {
        digitalWrite(12, HIGH);
      }
      else digitalWrite(12, LOW);
    }

    //Serial.write(read_data);
  }


  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

  if(buzzer_toggle == 1 && c_millis - p_millis >= 200) {
    p_millis = c_millis;
    
    int analog_data = analogRead(A0)/200;
    if(back_data != analog_data)
    {
      back_data = analog_data;
      char a1[100], a2[100] = "BUZZ_";
      itoa(analog_data, a1, 10);
      strcat(a2, a1);
      Serial.println(a2);
      
      if(c_millis - p_millis1 >= 1000) {
        p_millis1 = c_millis;
        mySerial.write(a2);
      }
    }
  }
  
}
