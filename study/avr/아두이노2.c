// //파워 all off, 숫자, power on, delay
// #define INPUT_ROW 2

// int colum_pin_num[4] = {11,12,10,9};
// int row_pin_num[2] = {2,3};

// void setup()
// {
// 	Serial.begin(9600);

// 	for(int i = 0; i < 2; i++)
// 		pinMode(row_pin_num[i], INPUT);

// 	for(int i = 0; i < 4; i++)
// 		pinMode(colum_pin_num[i], OUTPUT);

// }

// int current_column_num = 0;
// int toggle_down[4] = {0,0,0,0};
// int toggle_down1[4][2] = {0,0,0,0}, {0,0,0,0};
// int in_value[2] = {0,0};
// int num;
// int i = 0;

// void loop()
// {
// 	//all column off
// 	for(int i = 0; i < 4; i++)
// 		digitalWrite(colum_pin_num[i], LOW);

// 	//one column on
// 	digitalWrite(colum_pin_num[current_column_num], HIGH);
// 	//----------------------------------------------------------------------
// 	//row read
// 	for(int i = 0; i < 2; i++)
// 		in_value[i] = digitalRead(row_pin_num[i]);

// 	if(in_value[0] == 1 || in_value[1] == 1)
// 	{
// 		if(toggle_down[current_column_num] == 0)
// 		{
// 			toggle_down[current_column_num] = 1;
// 			if(in_value[1] == 1)
// 			{
// 				num = current_column_num+5;
// 				Serial.print("down");
// 				Serial.println(num);
// 			}
// 			else
// 			{
// 				num = current_column_num+1;
// 				Serial.print("down");//컨트롤 / 주석
// 				Serial.println(num);
// 			}
			
// 		}
// 	}
// 	else
// 	{
// 		if(toggle_down[current_column_num] == 1)
// 		{
// 			toggle_down[current_column_num] = 0;
// 			Serial.print("up");
// 			Serial.println(num);
// 		}
// 	}

// 	//check colum_num
// 	current_column_num++;
// 	if(current_column_num == 4) 
// 		current_column_num = 0;
// }

//파워 all off, 숫자, power on, delay
#define INPUT_ROW 2

int colum_pin_num[4] = {11,12,10,9};
int row_pin_num[2] = {2,3};

void setup()
{
	Serial.begin(9600);

	for(int i = 0; i < 2; i++)
		pinMode(row_pin_num[i], INPUT);

	for(int i = 0; i < 4; i++)
		pinMode(colum_pin_num[i], OUTPUT);

}

int toggle = 0;
int current_column_num = 0;
int toggle_down[4] = {0,0,0,0};
int toggle_down1[4][2] = {0,0,0,0}, {0,0,0,0};
int in_value[2] = {0,0};
int num;

void loop()
{
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				digitalWrite(colum_pin_num[k], LOW);
			}

			digitalWrite(colum_pin_num[toggle], HIGH);
			in_value[i] = digitalRead(row_pin_num[j]);

			if(in_value[j] == 1)
			{
				if(toggle_down1[j][toggle] == 1)
				{
					Serial.print("up");
					Serial.println(toggle + 1 + (4 * j))
				}
			}
		}
	}
}