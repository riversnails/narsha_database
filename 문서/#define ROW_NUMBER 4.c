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

//int in_key[COLUM_NUMBER] = {0,0,0,0};
//int key_toggle[ROW_NUMBER] = {0,0,0,0};
int key_toggle = 0;
int key_toggle2 = 0;
int in_key = 0;
int in_key2 = 0;


void loop()
{
	digitalWrite(5, HIGH);
	

	in_key = digitalRead(9);
	if(in_key == 1)
	{
	  if(key_toggle == 0)
	  {
		Serial.println("sw1 down");
		key_toggle = 1;
	 }
		
	}
	else
	{
	if(key_toggle == 1)
	{
      Serial.println("sw1 up");
		key_toggle = 0;
	 }
    }
	//------------------------------------


	in_key2 = digitalRead(10);
	if(in_key2 == 1)
	{
	  if(key_toggle2 == 0)
	  {
		Serial.println("sw2 down");
		key_toggle2 = 1;
	 }
		
	}
	else
	{
	if(key_toggle2 == 1)
	{
      Serial.println("sw2 up");
		key_toggle2 = 0;
	 }
    }



	delay(10);
}