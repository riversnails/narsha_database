int receive_index = 0;
int repeat_flag = 0;
int max_flag = 0;
int count_data = 0;
int data_received = 0;
int remocon_data[32];
unsigned long falling_time[40];
unsigned int diff_time[33];

unsigned char remocon_num[10][8] = {
  {0,1,1,0,1,0,0,0},
  {0,0,1,1,0,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,1,1,1,1,0,1,0},
  {0,0,0,1,0,0,0,0},
  {0,0,1,1,1,0,0,0},
  {0,1,0,1,1,0,1,0},
  {0,1,0,0,0,0,1,0},
  {0,1,0,0,1,0,1,0},
  {0,1,0,1,0,0,1,0}
};

unsigned char data[10] = {0x68,0x30,0x18,0x7a,0x10,0x38,0x5a,0x42,0x4a,0x52};

char rem_data = '0';
char data_hex = 0x00;

void setup()
{
	Serial.begin(9600);
	attachInterrupt(1, remocon_ISR, FALLING); // 리드코드 13.5 지난뒤 2~2.25 == 1, 1~1.5 == 0
}


void loop() // 계산이나 출력은 루프에서
{

	if(max_flag == 1) // 끝이 난걸 받음
	{
		max_flag = 0; // 다시 입력받기 대기

		for(int i = 1; i < 33; i++) // remocon_data를 32개로 만들었으니 32번 돌림? | 1로 시작하는 이유은 첫번째는 리드코드이기 때문
		{
			if(diff_time[i]> 1000 && diff_time[i] < 1500) // 0을 받음
			{
				remocon_data[i-1] = 0; 
			}
			else if(diff_time[i] > 2000 && diff_time[i] < 2500) // 1을 받음
			{
				remocon_data[i-1] = 1;
			}
		}
		// 32비트의 리모컨 값을 넘겨주는데 그 값들은 8비트씩 쪼개저 있다. 제일 앞의 커스텀 데이터는 기기마다의 고유값? 이런느낌 (8 8 8 8)

		// count_data = 0;
		// for(int i = 16; i < 24; i++) // 일단 3번째에 있는 데이터 값만 받아서 검사함
		// {
		// 	if(remocon_data[i] == remocon_num[0][i-16])
		// 	{
		// 		count_data++;
		// 	}
		// }

		// if(count_data == 8) // 데이터가 같다면
		// {
		// 	Serial.println("0");
		// 	rem_data = '0';
		// 	data_received = 1;
		// }

		data_hex = 0x00;
		for(int i = 16; i < 24; i++) // 이진수를 16진수로 변환 | 16 == msb
		{
			if(remocon_data[i] == 1)
			{
				data_hex |= 0x01 << (23-i); // ?????? 아무튼 바뀜
			}
		}

		for(int i = 0; i < 10; i++) //10번해서 숫자 같은거 있는지 찾기
		{
			if(data_hex == data[i])
			{
				char temp_data = i+0x30; // 0x30이 아스키 0
				Serial.println(temp_data);
				rem_data = temp_data;
				data_received = 1;
			}
		}
	}

	if(repeat_flag == 1 && data_received == 1)
	{
		repeat_flag = 0;
		Serial.println(rem_data);
	}

}


void remocon_ISR() // 여기서는 최소한으로 해야함 & 그래서 나온게 콜백펑션
{
	falling_time[receive_index] = micros();

	if(receive_index > 0)
	{
		if(falling_time[receive_index] - falling_time[receive_index-1] > 13000 && falling_time[receive_index] - falling_time[receive_index-1] < 14000) // 초기값 설정 == 리드코드 재밍 방지
		{
			receive_index = 1; // 넘어가야하니 1
			data_received = 0; // 반복되고 있으면 리피트코드만 나오니 다른값이 들어올때 초기화
		} 
		if(falling_time[receive_index] - falling_time[receive_index-1] > 11000 && falling_time[receive_index] - falling_time[receive_index-1] < 12000) // 꾹 누르면 리피트코드가 나오는데 값이 11.5
		{
			receive_index = 0; // 리피트코드니까 처음부터 다시
			repeat_flag = 1; // 반복되고 있는것을 알려줌
		}

		diff_time[receive_index-1] = falling_time[receive_index] - falling_time[receive_index-1]; // 중간의 값들을 저장 & 1부터 저장해야하니 이 if문안에 박음 & 루프로 던져줌

	}

	receive_index++;
	if(receive_index == 34) // 34개 다 받았을때
	{
		receive_index = 0;

		max_flag = 1; // 루프로 끝났다고 보내줌
	}

}