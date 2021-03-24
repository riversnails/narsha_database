#include <SoftwareSerial.h>
#define RXD 8
#define TXD 9
SoftwareSerial serial(RXD, TXD);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  serial.begin(9600);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(serial.available())
  {
    char read_data[100] = "\0";
    serial.readBytes(read_data, 100);
    Serial.print(read_data);
    if(strcmp(read_data, "on") == 0)
    {
      Serial.print("asdf");
      digitalWrite(12, HIGH);
    }
    else if(strcmp(read_data, "off") == 0)
    {
      Serial.print("asdf");
      digitalWrite(12, LOW);
    }
  }
}
