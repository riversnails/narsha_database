#define ROW_NUMBER 4
#define COLUM_NUMBER 4

void setup()
{
	Serial.begin(9600);

	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);

	pinMode(9, INPUT);
	pinMode(10, INPUT);
	pinMode(11, INPUT);
	pinMode(12, INPUT);
}

int key_toggle[4][4] = {0};
int in_key = 0;
int num[4][4] = {{'1','4','7','*'},
				 {'2','5','8','0'},
				 {'3','6','9','#'},
				 {'A','B','C','D'}};

void loop()
{
	
	for(int i = 0; i < 4; i++)
	{
		digitalWrite(i+5, HIGH);
		for(int k = 0; k < 4; k++)
		{

			in_key = digitalRead(k+9);
			if(in_key == 1)
			{
				if(key_toggle[k][i] == 0)
				{	
					Serial.println((char)num[i][k]);

					Serial.print("sw");
					Serial.print(k+(i*4)+1);
					Serial.println("down");

					key_toggle[k][i] = 1;
				}
				

			}

			else
			{

				if(key_toggle[k][i] == 1)
				{
					Serial.print("sw");
					Serial.print(k+(i*4)+1);
					Serial.println("up");

					key_toggle[k][i] = 0;
				}
			}
		}
		digitalWrite(i+5, LOW);
	}
	


	//------------------------------------


	// in_key2 = digitalRead(10);
	// if(in_key2 == 1)
	// {
	// 	if(key_toggle2 == 0)
	// 	{
	// 		Serial.println("sw2 down");
	// 		key_toggle2 = 1;
	// 	}
		
	// }
	// else
	// {
	// 	if(key_toggle2 == 1)
	// 	{
	// 		Serial.println("sw2 up");
	// 		key_toggle2 = 0;
	// 	}
	// }




	delay(10);
}