#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>     // fopen, fgets, fclose 함수가 선언된 헤더 파일
#include<string.h>
#include<windows.h>
#define MAXNUM 100
double data_queue[100][3];
int head = 0;
int tail = 0;

int main() // 그냥c에서 파일 파싱
{
	char *start_idx, *end_idx;
	char float_buffer[100];
	char buffer[10000];    // 파일을 읽을 때 사용할 임시 공간
	int totalSize = 0;
	int count = 0;

	FILE *fp = fopen("C:\\test.txt", "r");    // hello.txt 파일을 읽기 모드로 열기.  
										   // 파일 포인터를 반환


	if (fp == NULL) {
		printf("오류");
		return;
	}
	while (feof(fp) == 0) {
		if ((tail + 1) % MAXNUM == head) {
			printf("Queue end!");
			Sleep(1000);
			continue;
		}
		fp = fopen("C:\\test.txt", "r");
		fseek(fp, totalSize, SEEK_SET);
		fgets(buffer, sizeof(buffer), fp);
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

	return 0;
}