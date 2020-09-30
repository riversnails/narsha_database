#define ECHO 3
#define TRIGGER 4
#define BUZZER_PIN 5
#define BUZZER_DELAY 1275


void setup()
{
   pinMode(ECHO, INPUT);  //  echo 
   pinMode(TRIGGER, OUTPUT); // trigger
   pinMode(BUZZER_PIN, OUTPUT);

   Serial.begin(9600);

   attachInterrupt(1, myISR, CHANGE);
}


int delays = 1000;
int toggle1 = 0;
int toggle2 = 0;
int play_on = 1;
int checker = 0;

unsigned long c_micros = 0;
unsigned long c_millis = 0;
unsigned long p_micros = 0;
unsigned long p_micros1 = 0;
unsigned long p_millis = 0;
unsigned long p_millis1 = 0;
unsigned long p_millis2 = 0;

void loop()
{
   c_micros = micros();
   c_millis = millis();

   if(delays == -1) play_on = 0;

   if(c_millis - p_millis2 >= 1000)
   {
      p_millis2 = c_millis;
      checker = 0;
   }

   if(c_micros - p_micros1 >= 10 && checker == 0)
   {
      p_micros1 = c_micros;
      if(toggle2 == 0)
      {
         toggle2 = 1;
         digitalWrite(TRIGGER, HIGH);
      }
      else
      {
         checker = 1;
         toggle2 = 0;
         digitalWrite(TRIGGER, LOW);
      }
   }

   if(play_on == 1)
   {
      if(c_micros - p_micros > BUZZER_DELAY)
      {
         p_micros = c_micros;

         if(toggle1 == 0)
         {
            toggle1 = 1;
            digitalWrite(BUZZER_PIN, HIGH);
         }
         else
         {
            toggle1 = 0;
            digitalWrite(BUZZER_PIN, LOW);
         }
      }
   }

   if(c_millis - p_millis > delays * 2)
   {
      p_millis = c_millis;

      play_on = 1;

      p_millis1 = c_millis;
   }
   
   if(play_on == 1) 
   {
      if(c_millis - p_millis1 > delays) 
      {
         play_on = 0;
         digitalWrite(BUZZER_PIN, LOW);
         toggle1 = 0;
      }
   }
   //---------------------------------------------------------------------
}   

double rising_time = 0;
double falling_time = 0;
double distance = 0;
int diff_time = 0;

void myISR()
{
   int in_data = digitalRead(ECHO);

   if(in_data == 1)
   {
      rising_time = micros();    
   }
   else if(in_data == 0)
   {
      falling_time = micros();
      diff_time = falling_time - rising_time;    

      distance = 0.017 * (double)diff_time;
        
      if(distance > 50) delays = -1;
      else if(distance <= 50 && distance > 30) delays = 1000;
      else if(distance <= 30 && distance > 20) delays = 500;
      else if(distance <= 20 && distance > 10) delays = 100;
      else if(distance <= 10) delays = 0;

      Serial.print("distance : ");
      Serial.println(distance);
      Serial.print("delay : ");
      Serial.println(delays);
   }
}