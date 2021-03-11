#define RS 14
#define RW 15
#define E  16

#define DB0 4
#define DB1 5
#define DB2 6
#define DB3 7
#define DB4 8
#define DB5 9
#define DB6 10
#define DB7 11

int receive_index = 0;
unsigned long falling_time[40];
unsigned int diff_time[34];
int repeat_flag = 0;
int max_flag = 0;
char remocon_data[32];
int count_data = 0;
char rem_data = '0';
int data_received = 0;
char remocon_num[10][8] = {
  {0,1,1,0,1,0,0,0}, // 0
  {0,0,1,1,0,0,0,0}, // 1
  {0,0,0,1,1,0,0,0}, // 2
  {0,1,1,1,1,0,1,0}, // 3
  {0,0,0,1,0,0,0,0}, // 4
  {0,0,1,1,1,0,0,0}, // 5
  {0,1,0,1,1,0,1,0}, // 6
  {0,1,0,0,0,0,1,0}, // 7
  {0,1,0,0,1,0,1,0}, // 8
  {0,1,0,1,0,0,1,0}  // 9
};
unsigned char data[10] = {0x68, 0x30, 0x18, 0x7A, 0x10, 0x38, 0x5A, 0x42, 0x4A, 0x52};
unsigned char data_hex = 0x00;

void setup()
{
  Serial.begin(9600);
  for(int i=0;i<4;i++)
  {
    pinMode(8+i, OUTPUT);
    if(i < 3)pinMode(14+i, OUTPUT);
  }
  digitalWrite(RS, HIGH);
  digitalWrite(RW, LOW);

  charLCD_init();
  attachInterrupt(1, remocon_ISR, FALLING);
}

unsigned long c_millis = 0;
unsigned long p_millis = 0;
int char_count = 0;

void loop()
{
  c_millis = millis();
  if(c_millis - p_millis > 1000)
  {
    p_millis = c_millis;
    char_count++;
    if(char_count > 9)char_count = 0;
    charLCD_set_inst_4bit(0x80);
    charLCD_set_data_4bit(char_count + 0x30);
  }

  if(max_flag == 1)
  {
    max_flag = 0;
    for(int i = 1; i < 33; i++)
    {
      if(diff_time[i] > 1000 && diff_time[i] < 1500)
      {
        remocon_data[i - 1] = 0;
      }
      if(diff_time[i] > 2000 && diff_time[i] < 2500)
      {
        remocon_data[i - 1] = 1;
      }
    }
    for(int j = 0; j < 10; j++)
    {
      data_hex = 0x00;
      for(int i = 16; i < 24; i++)
      {
        if(remocon_data[i] == 1)
        {
          data_hex |= 0x01 << (23 - i);
        }
      }
    }
    for(int i=0;i<10;i++)
      {
        if(data_hex == data[i])
        {
          char tmp_data = i + 0x30;
          Serial.print("puch start : ");
          charLCD_set_inst_4bit(0xC0);
          charLCD_set_data_4bit(tmp_data);
          Serial.println(tmp_data);
          rem_data = tmp_data;
          data_received = 1;
        }
      }
  }
  if(repeat_flag == 1 && data_received == 1)
  {
	    repeat_flag = 0;
	    Serial.print("pushing : ");
		Serial.println(rem_data);
	  charLCD_set_inst_4bit(0xC0);
	  charLCD_set_data_4bit(rem_data);
    //charLCD_set_data_4bit(rem_data);
  }
}

void charLCD_set_inst_8bit(int inst)
{
  digitalWrite(RS, LOW);

  for(int i=0;i<8;i++)
  {
    if((inst & (0x01 << i)) != 0) digitalWrite(DB0 + i, HIGH);
    else digitalWrite(DB0 + i, LOW);
  }

  digitalWrite(E, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(RS, HIGH);

  delayMicroseconds(50);
}

void charLCD_set_inst_4bit_tmp(int inst)
{
  digitalWrite(RS, LOW);

  for(int i=0;i<4;i++)
  {
    if((inst & (0x01 << i)) != 0) digitalWrite(DB0 + i + 4, HIGH);
    else digitalWrite(DB0 + i + 4, LOW);
  }

  digitalWrite(E, HIGH);
  digitalWrite(E, LOW);
  digitalWrite(RS, HIGH);

  delayMicroseconds(50);
}
void charLCD_set_inst_4bit(int inst)
{
  charLCD_set_inst_4bit_tmp((inst >> 4) & 0x0F);
  charLCD_set_inst_4bit_tmp(inst & 0x0F);
}

void charLCD_set_data_4bit_tmp(int data)
{

  for(int i=0;i<4;i++)
  {
    if((data & (0x01 << i)) != 0) digitalWrite(DB0 + i + 4, HIGH);
    else digitalWrite(DB0 + i + 4, LOW);
  }

  digitalWrite(E, HIGH);
  digitalWrite(E, LOW);

  delayMicroseconds(50);
}
void charLCD_set_data_4bit(int data)
{
  charLCD_set_data_4bit_tmp((data >> 4) & 0x0F);
  charLCD_set_data_4bit_tmp(data & 0x0F);
}

void charLCD_init()
{
  // 8 bit mode instruction
  charLCD_set_inst_8bit(0x30);
  charLCD_set_inst_8bit(0x30);
  charLCD_set_inst_8bit(0x30);
  charLCD_set_inst_8bit(0x28);

  // 4 bit mode instruction
  charLCD_set_inst_4bit(0x28);
  charLCD_set_inst_4bit(0x06);
  charLCD_set_inst_4bit(0x0C);
  charLCD_set_inst_4bit(0x80);
  charLCD_set_inst_4bit(0x01);
  delay(10);
}



void remocon_ISR()
{
  falling_time[receive_index] = micros();

  if(receive_index > 0)
  {
    if(falling_time[receive_index] - falling_time[receive_index - 1] > 13000 && falling_time[receive_index] - falling_time[receive_index - 1] < 14000){
      receive_index = 1;
    }
    else if(falling_time[receive_index] - falling_time[receive_index - 1] > 11000 && falling_time[receive_index] - falling_time[receive_index - 1] < 12000){
      receive_index = 0;
      repeat_flag = 1;
    }
    diff_time[receive_index - 1] = falling_time[receive_index] - falling_time[receive_index - 1];
  }
  
  receive_index++;
  
  if(receive_index == 34)
  {
    receive_index = 0;

    max_flag = 1;
  }
}