#define SCL 19
#define SDA  18

#define oled_i2c_ADDR 0x3C

#define SIZE_INIT_VALUE 25
#define SIZE_COMMAND_VALUE 6
#define SIZE_BUFFER 128*8

#define WHITE 1
#define BLACK 0

//char oled_init_value[26] = {0xAE,0xD5,0x80,0xA8,63,
//                            0xD3,0x00,0x40,0x8D,0x10,
//                            0x20,0x00,0xA1,0xC8,
//                            0xDA,0x12,0x81,0x9F,
//                            0xD9,
//                            0x22,
//                            0xDB,0x40,0xA4,0xA6,0x2E,0xAF
//                          };

char oled_init_value[SIZE_INIT_VALUE] = { 0xAE,0xD5,0x80,0xA8,0x3F,
                                          0xD3,0x00,0x40,0x8D,0x14,
                                          0xA1,0xC0,
                                          0xDA,0x12,0x81,0xCF,
                                          0xD9,
                                          0xF1,
                                          0xDB,0x40,0xA4,0xA6,0x20,0x00,0xAF
                                        };
                          

char oled_area_command_value[SIZE_COMMAND_VALUE] = {0x22,0x00,63,0x21,0x00,127};

char oled_buffer[SIZE_BUFFER];

char mouse_pointer_arry[8] = {0xc0, 0xA0, 0x90, 0x88, 0x84, 0xBE, 0xC0, 0x80};

void setup()
{
  Serial.begin(115200);

  for(int i=0;i<SIZE_BUFFER;i++)
  {
    oled_buffer[i] = 0x0f;
  }
  
  i2c_init();
  oled_128_64_init(oled_init_value);

  oled_clear();
//  for(int j=0;j<128;j++)
//  {
//    for(int i=0;i<8;i++)
//    {
//      oled_128_64_i2c_data(0x55);     
//    }
//  }

}

char page_num = 0;
char pos_x = 0, pos_y = 0;
char ch = 0;
unsigned long c_millis = 0;
unsigned long p_millis = 0;

void loop()
{
  c_millis = millis();

  if(c_millis - p_millis > 10)
  {
    p_millis = c_millis;
    int joystick_x_value = analogRead(A0);
    int joystick_y_value = analogRead(A1);

    if(0 <= joystick_x_value && joystick_x_value <= 400)
    {
      mouse_pointer(pos_x, pos_y, BLACK);
      pos_x--;
      if(pos_x < 0) pos_x = 0;
    }
    else if(600 <= joystick_x_value && joystick_x_value <= 1024)
    {
      mouse_pointer(pos_x, pos_y, BLACK);
      pos_x++;
      if(pos_x > 120) pos_x = 120;
    }
    
    if(0 <= joystick_y_value && joystick_y_value <= 400)
    {
      mouse_pointer(pos_x, pos_y, BLACK);
      pos_y--;
      if(pos_y < 0) pos_y = 0;
    }
    else if(600 <= joystick_y_value && joystick_y_value <= 1024)
    {
      mouse_pointer(pos_x, pos_y, BLACK);
      pos_y++;
      if(pos_x > 56) pos_x = 56;
    }
  }
  
  ch = 0;
  if(Serial.available())
  {
    ch = Serial.read();
    if(ch == 'a')
    {
      mouse_pointer(pos_x, pos_y, BLACK);
      pos_x--;
      if(pos_x < 0) pos_x = 0;
    }
    else if(ch == 'd')
    {
      mouse_pointer(pos_x, pos_y, BLACK);
      pos_x++;
      if(pos_x > 120) pos_x = 120;
    }
    else if(ch == 'w')
    {
      mouse_pointer(pos_x, pos_y, BLACK);
      pos_y--;
      if(pos_y < 0) pos_y = 0;
    }
    else if(ch == 's')
    {
      mouse_pointer(pos_x, pos_y, BLACK);
      pos_y++;
      if(pos_x > 56) pos_x = 56;
    }
  }
  
  mouse_pointer(pos_x, pos_y, WHITE);
}

void mouse_pointer(char x, char y, char color)
{
  for(int j = 0; j < 8; j++)
  {
    for(int i = 0; i < 8; i++)
    {
      if(mouse_pointer_arry[j] & (0x80 >> i))
      {
        put_pixel(x + i, y + j, color);
      }
    }
  }

//  int pos = 0;
//  char value = 0;
//
//
//  for(int i = 0; i < 8; i++)
//  {
//    page_num = 7 - (y/8);
//    pos = page_num*128 + x;
//  
//    if(color == WHITE)  
//      oled_buffer[pos] |= (0x80 >> (y%8)); 
//    else if(color == BLACK) 
//      oled_buffer[pos] &= ~(0x80 >> (y%8));
//  }
  
  oled_display();
}

void put_pixel(char x, char y, int color)
{
  int pos = 0;
  char value = 0;
  
  page_num = 7 - (y/8);
  pos = page_num*128 + x;

  if(color == WHITE)  
    oled_buffer[pos] |= (0x80 >> (y%8)); // 색 결정
  else if(color == BLACK) 
    oled_buffer[pos] &= ~(0x80 >> (y%8));
  
  //oled_buffer[pos] = oled_buffer[pos] | (0x01 << (y%8)); // 현재의 페이지의 8bit 를 비교하여 계산 후 넣어준다
  
  //oled_display();
}

void oled_clear()
{
  for(int i=0;i<SIZE_BUFFER;i++)
  {
    oled_buffer[i] = 0x00;
  }
  oled_display();
}

void oled_display()
{
  oled_128_64_i2c_command_multi(oled_area_command_value, SIZE_COMMAND_VALUE);
  oled_128_64_i2c_data_multi(oled_buffer); 
}

void oled_128_64_init(char *init_value)
{
  oled_128_64_i2c_command_multi(init_value, SIZE_INIT_VALUE);
}

void oled_128_64_i2c_command(char command)
{
  i2c_start();
  i2c_8bit(oled_i2c_ADDR << 1);
  i2c_ack();
  //i2c_8bit(0x00);
  i2c_8bit(0x80);  
  i2c_ack();
  i2c_8bit(command);
  i2c_ack();  
  i2c_stop();
}

void oled_128_64_i2c_command_multi(char *command, int array_count)
{
  i2c_start();
  i2c_8bit(oled_i2c_ADDR << 1);
  i2c_ack();
  i2c_8bit(0x00);
  i2c_ack();

  for(int i=0;i<array_count;i++)
  {
    i2c_8bit(*(command+i));
    i2c_ack();      
  }
 
  i2c_stop();
}

void oled_128_64_i2c_data(char data)
{
  i2c_start();
  i2c_8bit(oled_i2c_ADDR << 1);
  i2c_ack();
  //i2c_8bit(0x40);
  i2c_8bit(0xC0);  
  i2c_ack();
  i2c_8bit(data);
  i2c_ack();  
  i2c_stop();
}


void oled_128_64_i2c_data_multi(char *data)
{
  i2c_start();
  i2c_8bit(oled_i2c_ADDR << 1);
  i2c_ack();
  i2c_8bit(0x40);
  i2c_ack();

  for(int i=0;i<SIZE_BUFFER;i++)
  {
    i2c_8bit(*(data+i));
    i2c_ack();      
  }  
 
  i2c_stop();
}
//=====================================

void i2c_init()
{
  pinMode(SCL, OUTPUT);
  pinMode(SDA, OUTPUT); 

  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, HIGH);
}

void i2c_start()
{
  digitalWrite(SCL, HIGH);  
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW); 
}

void i2c_stop()
{
  digitalWrite(SDA, LOW); 
  digitalWrite(SCL, HIGH);  
  digitalWrite(SDA, HIGH);
}


void i2c_8bit(char data)
{
  for(int i=0;i<8;i++)
  {
    if(data & (0x80 >> i))
    {
      digitalWrite(SDA, HIGH);
    }
    else
    {
      digitalWrite(SDA, LOW);
    }
    digitalWrite(SCL, HIGH);  
    digitalWrite(SCL, LOW);   
  }


}

void i2c_ack()
{
  pinMode(SDA, INPUT);   
  digitalWrite(SCL, HIGH); 
  //Serial.printf("%d",digitalRead(SDA)); 
  pinMode(SDA, OUTPUT);     
  digitalWrite(SCL, LOW); 

 
}
