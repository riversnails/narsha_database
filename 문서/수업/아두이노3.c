#define INPUT_ROW_1 2
#define INPUT_ROW_2 3
#define OUTPUT_COLUME_1 9
#define OUTPUT_COLUME_2 10
#define OUTPUT_COLUME_3 11
#define OUTPUT_COLUME_4 12
#define BUTTUN_NUMBER 4
#define BUTTUN_COLUME 2

int toggle = 0;
int toggle_down[BUTTUN_COLUME][BUTTUN_NUMBER] = {
							{0,0,0,0},
							{0,0,0,0}
						};
int toggle_arr[BUTTUN_NUMBER] = {OUTPUT_COLUME_1,OUTPUT_COLUME_2,OUTPUT_COLUME_3,OUTPUT_COLUME_4};
int in_value[BUTTUN_COLUME] = { 0 };
int input_row[BUTTUN_COLUME] = {INPUT_ROW_1,INPUT_ROW_2};

void setup()
{
	Serial.begin(9600);
	for(int i = 0; i < BUTTUN_COLUME; i++)
	{
		pinMode(input_row[i], INPUT);
	}
	for(int i=0;i<BUTTUN_NUMBER;i++)
	{
		pinMode(toggle_arr[i], OUTPUT);
	}
}


void loop()
{
	for(int j = 0;j < BUTTUN_COLUME;j++)
	{
		for(int k = 0; k < BUTTUN_NUMBER; k++)
		{

			for(int i=0;i<BUTTUN_NUMBER;i++)
			{
				digitalWrite(toggle_arr[i], LOW);
			}

			digitalWrite(toggle_arr[toggle], HIGH);

			in_value[j] = digitalRead(input_row[j]);

			if(in_value[j] == 1)
			{
				if(toggle_down[j][toggle] == 0)
				{
					Serial.print(toggle + 1 + (BUTTUN_NUMBER * j));
					Serial.println("down");
					toggle_down[j][toggle] = 1;
				}
			}
			else
			{
				if(toggle_down[j][toggle] == 1)
				{
					Serial.print(toggle + 1 + (BUTTUN_NUMBER * j));
					Serial.println("up");
					toggle_down[j][toggle] = 0;
				}
			}

			toggle++;
			if(toggle >= BUTTUN_NUMBER)toggle = 0;

			delay(2);
		}
	}
}