/*
  SD card basic file test

  This test shows how to create and destroy an SD card file
  The circuit. Pin numbers reflect the default
  SPI pins for Uno and Nano models:
   SD card attached to SPI bus as follows:
 ** SDO - pin 11
 ** SDI - pin 12
 ** CLK - pin 13
 ** CS - depends on your SD card shield or module.
        Pin 10 used here for consistency with other Arduino tests
    (for MKRZero SD: SDCARD_SS_PIN)

  created   Nov 2010
  by David A. Mellis
  modified 24 July 2020
  by Tom Igoe

  This test code is in the public domain.
*/
#include <SD.h>

const int chipSelect = 10;
File myFile;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // wait for Serial Monitor to connect. Needed for native USB port boards only:
  while (!Serial);

  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  if (SD.exists("test.txt")) {
    Serial.println("test.txt exists.");
  } else {
    Serial.println("test.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  Serial.println("Opening test.txt on write...");
  myFile = SD.open("test.txt", FILE_WRITE);
  myFile.write("hello world!");
  //Serial.println(myFile.read(0, 2));
  myFile.close();

  // Check to see if the file exists:
  if (SD.exists("test.txt")) {
    Serial.println("test.txt exists.");
  } else {
    Serial.println("test.txt doesn't exist.");
  }

  Serial.println("Opening test.txt on read...");
  myFile = SD.open("test.txt", FILE_READ);
  String text = my
  //Serial.println(myFile.read(0, 2));
  myFile.close();
  
  // Check to see if the file exists:
  if (SD.exists("test.txt")) {
    Serial.println("test.txt exists.");
  } else {
    Serial.println("test.txt doesn't exist.");
  }
  
  // delete the file:
//  Serial.println("Removing test.txt...");
//  SD.remove("test.txt");
}

void loop() {
  // nothing happens after setup finishes.
}