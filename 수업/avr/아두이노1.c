//파워 all off, 숫자, power on, delay
#define INPUT_ROW 2

int colum_pin_num[4] = {11,12,10,9};
int row_pin_num[2] = {2,3}

void setup()
{
	Serial.begin(9600);

	pinMode(INPUT_ROW, INPUT);
	for(int i = 0; i < 4; i++)
		pinMode(colum_pin_num[i], OUTPUT);

}

int current_column_num = 0;
int toggle_down[4] = {0,0,0,0};
int in_value = 0;
//int in_value[2] = {0,0};

void loop()
{
	//all column off
	for(int i = 0; i < 4; i++)
		digitalWrite(colum_pin_num[i], LOW);

	//one column on
	digitalWrite(colum_pin_num[current_column_num], HIGH);
	//----------------------------------------------------------------------
	//row read
	in_value = digitalRead(INPUT_ROW);

	if(in_value == 1)
	{
		if(toggle_down[current_column_num] == 0)
		{
			toggle_down[current_column_num] = 1;
			Serial.print("down");//컨트롤 / 주석
			Serial.println(current_column_num+1);
		}
	}
	else
	{
		if(toggle_down[current_column_num] == 1)
		{
			toggle_down[current_column_num] = 0;
			Serial.print("up");
			Serial.println(current_column_num+1);
		}
	}

	//check colum_num
	current_column_num++;
	if(current_column_num == 4) 
		current_column_num = 0;


	/*
	in_value = digitalRead(INPUT_ROW);
  	
	if(in_value == 1 && toggle1 == 0)
	{
		Serial.println("down1");
      	toggle1 = 1;
	}
	else if(in_value == 0 && toggle1 == 1)
	{
		Serial.println("up1");
		toggle1 = 0;
	}


	digitalWrite(OUTPUT_COlUMN_1, LOW);
    digitalWrite(OUTPUT_COlUMN_2, HIGH);
 	digitalWrite(OUTPUT_COlUMN_3, LOW);
    digitalWrite(OUTPUT_COlUMN_4, LOW);


    in_value = digitalRead(INPUT_ROW);
  	
	if(in_value == 1 && toggle2 == 0)
	{
		Serial.println("down2");
      	toggle2 = 1;
	}
	else if(in_value == 0 && toggle2 == 1)
	{
		Serial.println("up2");
		toggle2 = 0;
	}
   
  
 	digitalWrite(OUTPUT_COlUMN_1, LOW);
    digitalWrite(OUTPUT_COlUMN_2, LOW);
 	digitalWrite(OUTPUT_COlUMN_3, HIGH);
    digitalWrite(OUTPUT_COlUMN_4, LOW);

  
 	in_value = digitalRead(INPUT_ROW);
  	
	if(in_value == 1 && toggle3 == 0)
	{
		Serial.println("down3");
      	toggle3 = 1;
	}
	else if(in_value == 0 && toggle3 == 1)
	{
		Serial.println("up3");
		toggle3 = 0;
	}
  
  
  	digitalWrite(OUTPUT_COlUMN_1, LOW);
    digitalWrite(OUTPUT_COlUMN_2, LOW);
 	digitalWrite(OUTPUT_COlUMN_3, LOW);
    digitalWrite(OUTPUT_COlUMN_4, HIGH);

  
 	in_value = digitalRead(INPUT_ROW);
  	
	if(in_value == 1 && toggle4 == 0)
	{
		Serial.println("down4");
      	toggle4 = 1;
	}
	else if(in_value == 0 && toggle4 == 1)
	{
		Serial.println("up4");
		toggle4 = 0;
	}
	delay(10);*/
}