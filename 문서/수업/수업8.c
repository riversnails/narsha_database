#define SCL 19
#define SDA 18

void shift_out(unsigned char data)
{
  for(int i = 0; i < 8; i++)
  {
    if(data & (0x80 >> i))
    {
      digitalWrite(SDA, HIGH);
    }
    else
    {
      digitalWrite(SDA, LOW);
    }
  }
}

void setup()
{
  pinMode(SCL, OUTPUT); 
  pinMode(SDA, OUTPUT);

  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, HIGH);

  //start
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW); // clock LOW

  //adress
  shift_out(0x4E);


  //ack
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);

  //data
  shift_out(0x08);

  //ack
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);

  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, HIGH);
}

void loop() // lcd
{
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, HIGH);

  //start
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW); // clock LOW

  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //1
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //1
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //1
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //1
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);


  //ack
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);


  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //1
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);

  //ack
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);

  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, HIGH);
  delay(1000);
digitalWrite(SCL, HIGH);
  digitalWrite(SDA, HIGH);

  //start
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW); // clock LOW

  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //1
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //1
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //1
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //1
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);


  //ack
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);


  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);
  //0
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);

  //ack
  digitalWrite(SCL, HIGH);
  digitalWrite(SCL, LOW);

  digitalWrite(SDA, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(SDA, HIGH);
  delay(1000);

  
}
// SCL : PC5, arduino 19
// SDA : PC4, arduino 18