
// void setup()
// {
// 	Serial.begin(9600);
// 	attachInterrupt(1,myISR,FALLING);

// }

// void loop()
// {

// }

// int count = 0;
// unsigned long time_value[50];
// unsigned int diff_time[33];

// void myISR()
// {
// 	time_value[count] = micros();
// 	count++;
// 	if(count == 34)
// 	{
// 		for(int i = 0; i < 33; i++)
// 		{
// 			diff_time[i] = time_value[i+1] - time_value[i];
// 			//Serial.println(time_value[i]);
// 		}
// 		for(int i = 0; i < 33; i++)
// 		{
// 			if(diff_time[i+1] > 1000 && diff_time[i+1] > 1500)
// 				num_array[i] = 0;
// 			if(diff_time[i+1] > 2000 && diff_time[i+1] > 2500)
// 				num_array[i] = 1;
// 		}
// 		Serial.println();
// 	}
// }



// void setup()
// {
//   Serial.begin(9600);
//  attachInterrupt(1,myISR,FALLING);

// }

// void loop()
// {

// }

// int count = 0;
// unsigned long time_value[50];
// unsigned int diff_time[33];

// void myISR()
// {
//  time_value[count] = micros();
//  count++;
//  if(count == 34)
//  {
//    for(int i = 0; i < 33; i++)
//    {
//      diff_time[i] = time_value[i+1] - time_value[i];
//      //Serial.println(time_value[i]);
//    }
//    for(int i = 0; i < 33; i++)
//    {
//      if(diff_time[i+1] > 1000 && diff_time[i+1] > 1500)
//        num_array[i] = 0;
//      if(diff_time[i+1] > 2000 && diff_time[i+1] > 2500)
//        num_array[i] = 1;
//    }
//    Serial.println();
//  }
// }


int count = 0;
unsigned long time_value[50];
unsigned long num_array[50];
unsigned int diff_time[33];
char remocon_numnum[10][8] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
}

void setup() {
  Serial.begin(9600);
  attachInterrupt(1, myISR, FALLING);
  
}

void loop() {
  delay(1000);

}

void myISR() {
  	time_value[count] = micros();

  	if(count > 0)
  	{
  		diff_time[count] = time_value[count + 1] = time_value[count];
  		if(diff_time[count] > 13000 && diff_time[count] < 14000)
  		{
  			count = 1;
  			Serial.println("readCode");
  		}
  	}


   count++;
  // if(count == 34) {
  //   count = 0;

  // for(int i = 0; i < 33; i++)
  //     {
  //       diff_time[i] = time_value[i+1] - time_value[i];
  //     }


  //   for(int i = 0; i < 33; i++)
  //   {
  //     if(diff_time[i+1] > 1000 && diff_time[i+1] < 1500){
  //       num_array[i] = 0;}
  //     else if(diff_time[i+1] > 2000 && diff_time[i+1] < 2500){
  //       num_array[i] = 1;}
  //   }


  //   for(int i = 0; i < 33; i++)
  //   {
  //     Serial.print(diff_time[i]);
  //     Serial.print(", ");
  //   }

  // 	Serial.println();
  // 	for(int i = 0; i < 33; i++)
  //   {
  //     Serial.print(num_array[i]);
  //     Serial.print(", ");
  //   }
    

  //   Serial.println();
  //   count = 0;
  // }
  
}

