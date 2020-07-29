#include <SPI.h>
#include <SD.h>
#include<string.h>
#define MAXNUM 100
double data_queue[100][3];
int head = 0;
int tail = 0;
File myFile;

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

  while (myFile.available()) {
    if ((tail + 1) % MAXNUM == head) {
      printf("Queue end!");
      continue;
    }

    myFile = SD.open("test.txt");
    fseek(myFile, totalSize, SEEK_SET);
    fgets(buffer, sizeof(buffer), myFile);
    printf("size = %d\n",strlen(buffer));    // hello.txt에서 문자열을 읽음
    totalSize += strlen(buffer) + 1;
    printf("totalSize = %d\n", totalSize);
    printf("문자열 = %s\n", buffer);    // Hello, world!: 파일의 내용 출력

    start_idx = strchr(buffer, 'X');
    if (start_idx != NULL) {
      start_idx = start_idx + 1;
      end_idx = strchr(start_idx, ' ');
      if (end_idx != NULL) {
        strncpy(float_buffer, start_idx, end_idx - start_idx);
        float_buffer[end_idx - start_idx] = '\0';
        sscanf(float_buffer, "%lf", &data_queue[count][0]);
        printf("string x = %s\n", float_buffer);
        printf("double x = %lf\n", data_queue[count][0]);
      }
    }
    start_idx = strchr(buffer, 'Y');
    if (start_idx != NULL) {
      start_idx = start_idx + 1;
      end_idx = strchr(start_idx, ' ');
      if (end_idx != NULL) {
        strncpy(float_buffer, start_idx, end_idx - start_idx);
        float_buffer[end_idx - start_idx] = '\0';
        sscanf(float_buffer, "%lf", &data_queue[count][1]);
        printf("string y = %s\n", float_buffer);
        printf("double y = %lf\n", data_queue[count][1]);
      }
    }

    fclose(fp);
    count++;
    Sleep(3000);
  }

}