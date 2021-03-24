#include <SoftwareSerial.h>
#define RXD 8
#define TXD 9
SoftwareSerial serial(RXD, TXD);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    serial.write(Serial.read());
  }
}
