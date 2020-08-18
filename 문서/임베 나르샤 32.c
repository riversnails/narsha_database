#include <SPI.h>
#include <SD.h>
#include<string.h>
#define MAXNUM 100
double data_queue[100][3];
int head = 0;
int tail = 0;
File myFile;

char *start_idx, *end_idx;
char float_buffer[100];
char buffer[10000];    // 파일을 읽을 때 사용할 임시 공간
int totalSize = 0;
int count = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  //delay(1000);
  if (!SD.begin(14)) {
    Serial.println("initialization failed!");
    while(1);
  }
  Serial.println("initialization done.");
  // open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}
void loop() { // 파일 파싱 하는 코드들
  // nothing happens after setup
  myFile = SD.open("test.txt");

  while (myFile.available()) {
    if ((tail + 1) % MAXNUM == head) {
      printf("Queue end!");


      +
      continue;
    }

    
    myFile.seek(totalSize);
    totalSize += strlen(buffer) + 1;
    count++;
    delay(1000);
  }

  Serial.println(count);

    myFile.close();
}