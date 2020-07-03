void setup()
{
    Serial.begin(9600);
    attachInterrupt(1, myISR, FALLING);//신호받을떄때 출력
}

void loop()
{

}

int count = 0;
unsigned long time_value[50]; // 한 주기가 걸린 시간?
unsigned int diff_time[34]; // 그 시간들의 사이를 구해준것
unsigned char num_array[32]; // 시간들을 비교하여 0과 1로 변환하여 저장

char remocon_num[2][8] = {
           { 0,1,1,0,1,0,0,0 }, //0번 리모컨
           { 0,0,1,1,0,0,0,0 }  //1번 리모켠
};

int rem_number = 0;
int count_same_num = 0;

void myISR()
{
    time_value[count] = micros();

    if (count > 0) // 자세히는 기억나지 않지만 대충 연속으로 눌렀을때 나오는 값을 계산하여서 계속누르면 기억된 값을 출력하도록 한 코드
    {
        diff_time[count] = time_value[count] - time_value[count - 1];

        if (diff_time[count] > 13000 && diff_time[count] < 14000)
        {
            count = 1;
            //Serial.println("Lead Code");
        }

        if (diff_time[count] > 11000 && diff_time[count] < 12000)
        {
            Serial.println(rem_number);
            count = 0;
        }
    }

    count++;
    if (count == 34)
    {
        count = 0;

        for (int i = 0; i < 33; i++) // 34개로 이뤄저있는데 그 사이니 1개를 빼서 33번 왕복한다
        {
            diff_time[i] = time_value[i + 1] - time_value[i]; // 두 거리 사이의 시간을 계산함
        }

        for (int i = 0; i < 33; i++) // 34개로 이뤄저있는데 그 사이니 1개를 빼서 33번 왕복한다
        {
            if (diff_time[i + 1] > 1000 && diff_time[i + 1] < 1500) // 0로 변환
            {
                num_array[i] = 0;
            }
            else if (diff_time[i + 1] > 2000 && diff_time[i + 1] < 2500) // 1로 변환
            {
                num_array[i] = 1;
            }


            for (int j = 0; j < 2; j++) // 두번 반복해서 1번인지 2번인지 구분
            {
                count_same_num = 0;
                for (int i = 16; i <= 23; i++) // 8개로 나눠서 3번째부터 4번째가 번호구별인것으로 기억
                {
                    if (num_array[i] == remocon_num[j][i - 16]) // 카운트변수를 활용하여 현재상태가 같은 캐이스가 있는지 확인함
                    {
                        count_same_num++; // 이하같음
                    }

                    if (count_same_num == 8) // 모두 같아서 0이나 1이 되면 rem_number라는 변수에 값을 담아준다
                    {
                        rem_number = j;
                        Serial.println(j);
                    }
                }
            }

        }
    }

}