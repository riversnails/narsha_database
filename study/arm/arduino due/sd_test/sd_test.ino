#include<SPI.h>
#include<SD.h>

#if defined (ARDUINO_ARCH_SAM)
#define ARDUINO_DUE
#endif

#define CSPIN 4   //slecting DPin-4 does not require to set direction
File myFile;

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  SD.begin(CSPIN);        //SD Card is initialized
  //SD.remove("Test1.txt"); //remove any existing file with this name
  //myFile = SD.open("Test1.txt", FILE_WRITE);  //file created and opened for writing

  //  if (myFile)       //file has really been opened
  //  {
  //    myFile.println(4000);
  //    myFile.println(200);
  //    myFile.println(3000);
  //    myFile.close();
  //
  //  }
  myFile = SD.open("tiger.bmp", FILE_READ);  //file created and opened for writing

  if (myFile)       //file has really been opened
  {
    while (myFile.available())
    {
      Serial.print(myFile.read(), HEX);
    }
    myFile.close();
  }
  else
  {
    Serial.println("no");
  }
}

void loop()
{

}
