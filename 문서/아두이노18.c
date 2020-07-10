#include<stdio.h>
#include<Windows.h>

int main(void)
{

	unsigned char data = 0x23;
	unsigned char data1 = 0x37;

	unsigned char num_2[8] = {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c};
	unsigned char num_3[8] = {0x00, 0x38, 0x44, 0x04, 0x18, 0x04, 0x44, 0x38};

	while (1)
	{

		for(int j = 0; j < 8; j++)
		{
			for (int i = 0; i < 8; i++)
			{
				if (num_2[j] & (0x80 >> i)) // msb  즉 상위 비트부터 검사하기 위해 이렇게 함
				{
					printf("*");
				}
				else
				{
					printf("-");
				}
			}

			printf(" ");

			for (int i = 0; i < 8; i++) // 3 출력 | 바로 옆에 찍기 위해서 \n을 뒤로 뺌
			{
				if (num_3[j] & (0x80 >> i)) // msb  즉 상위 비트부터 검사하기 위해 이렇게 함
				{
					printf("*");
				}
				else
				{
					printf("-");
				}
			}
			printf("\n");
		}

		printf(" ");

		Sleep(300);
		system("cls"); // 창 비우기

		for(int i = 0; i < 8; i++)
		{
			num_3[i] = num_3[i] >> 1; // 먼저 한칸 밀고

			if (num_2[i] & 0x01) // 옆으로 넘기기
			{
				num_3[i] = num_3[i] | 0x80; // 그냥 char 면 -가 되면서 오류가 나니 위의 char를 unsigned로 한다
			}

			num_2[i] = num_2[i] >> 1; // 앞에것도 넘기기
		}

		

	}

}