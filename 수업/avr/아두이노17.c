#include<stdio.h>
#include<Windows.h>

int main(void)
{

	unsigned char data = 0x23;
	unsigned char data1 = 0x37;

	unsigned char num_2[8] = {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c};

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
			printf("\n");
		}

		printf(" ");

		// for (int i = 0; i < 8; i++)
		// {
		// 	if (data1 & (0x80 >> i)) // msb  즉 상위 비트부터 검사하기 위해 이렇게 함
		// 	{
		// 		printf("*");
		// 	}
		// 	else
		// 	{
		// 		printf("-");
		// 	}
		// }

		Sleep(1000);
		system("cls"); // 창 비우기

		// data1 = data1 >> 1;
		// if (data & 0x01)
		// {
		// 	data1 = data1 | 0x80; // 그냥 char 면 -가 되면서 오류가 나니 위의 char를 unsigned로 한다
		// }

		// data = data >> 1; // 데이터를 쉬프트 시킴
	}

}
