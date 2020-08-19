#define encoder_a 0x04
#define encoder_b 0x08
#define encoder_button 0x01

void setup()
{
	DDRA |= 0x0A;
	DDRC |= 0x02;

	PORTA = 0x00;
	PORTC = 0x00;

	DDRC &= ~encoder_button;
	DDRD &= ~(encoder_a | encoder_b); //핀 초기화
	PORTC |= encoder_button;
	PORTD |= encoder_a | encoder_b; // 풀업
//---------------------

	lcd_set_inst(0x30);
	lcd_set_inst(0x30);
	lcd_set_inst(0x30);

	lcd_set_inst(0x30); // clear
	lcd_set_inst(0x06);	
	lcd_set_inst(0x0c);

	lcd_set_inst(0x01);
	delay(10);
	lcd_cls();
	delay(1000);
}

int previous_state = 0;
int current_state = 0;
int rcount = 0;
int lcount = 0;
int toggle = 0;
unsigned int current_millis = 0;
unsigned int previous_millis = 0;
unsigned int previous_millis2 = 0;

short num_2[8] = {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c};

unsigned char num[10][8] = {
  {0x00, 0x38, 0x44, 0x4C, 0x54, 0x64, 0x44, 0x38},
  {0x00, 0x10, 0x30, 0x50, 0x10, 0x10, 0x10, 0x7c},
  {0x00, 0x38, 0x44, 0x04, 0x08, 0x10, 0x20, 0x7c},
  {0x00, 0x38, 0x44, 0x04, 0x18, 0x04, 0x44, 0x38},
  {0x00, 0x08, 0x18, 0x28, 0x48, 0x7C, 0x08, 0x08},
  {0x00, 0x7C, 0x40, 0x78, 0x04, 0x04, 0x44, 0x38},
  {0x00, 0x38, 0x40, 0x40, 0x78, 0x44, 0x44, 0x38},
  {0x00, 0x7C, 0x04, 0x08, 0x10, 0x20, 0x20, 0x20},
  {0x00, 0x38, 0x44, 0x44, 0x38, 0x44, 0x44, 0x38},
  {0x00, 0x38, 0x44, 0x44, 0x3C, 0x04, 0x44, 0x38},
};

char block_l[8][8] = {  {0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0},
						{0,0,1,1,0,0,0,0},
						{0,0,1,1,0,0,0,0},
						{0,0,1,1,1,1,1,1},
						{0,0,1,1,1,1,1,1},
						{0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0}
					};

void loop() // lcd 버튼과 lcd 연동
{
	current_millis = millis();

	for(int i = 0; i < 8; i++)
	{
		set_data(0,i+7, num[lcount / 10][i]);
		set_data(1,i+7, num[lcount % 10][i]);
	}


	if(current_millis - previous_millis > 10)
	{
		previous_millis = current_millis;
		int a = PIND & encoder_a; // 값 읽어오기
		int b = PIND & encoder_b;
		int button = PINC & encoder_button;
		if(a != 0) a = 1; // 이상한 값이 나올 수 있으니 0이 아니면 1로 변환
		if(b != 0) b = 1;
		if(button != 0) button = 1;

		if(a == 0 && b == 1) current_state = 1;
		else if(a == 0 && b == 0) current_state = 2;
		else if(a == 1 && b == 0) current_state = 3;
		else if(a == 1 && b == 1) current_state = 4;


		if(current_state == 3 && previous_state == 2)
		{
			lcount++;
		}
		if(current_state == 1 && previous_state == 2)
		{
			lcount--;
		}
		if(button == 0 && toggle == 0)
		{
			toggle = 1;
		}
		else if(button == 1 && toggle == 1)
		{
			toggle = 0;
		}


		previous_state = current_state;
		// Serial.print(a);
		// Serial.println(b);
	}
	
}

void lcd_cls()
{
	for(int j = 0; j < 32; j++)
	{
		for(int i = 0; i < 16; i++)
		{
			set_data(i,j,0x0000);
		}
	}
}

void set_data(int x, int y, short data)
{
	char addr_x = 0x80 | (x & 0x0f);
	char addr_y = 0x80 | (y & 0x7f);
	char data_high = (data & 0xff00) >> 8;
	char data_low = data & 0x00ff;

	lcd_set_inst(0x36);
	lcd_set_inst(addr_y);
	lcd_set_inst(addr_x);

	lcd_set_inst(0x30);
	lcd_set_data(data_high);
	lcd_set_data(data_low);

}

void lcd_set_inst(char inst)
{
	PORTA |= 0x08; // cs high

	shift_out(0xf8);
	shift_out(inst & 0xf0);
	shift_out((inst & 0x0f) << 4);

	PORTA &= ~0x08; // cs low
	delayMicroseconds(50);
}

void lcd_set_data(char data)
{
	PORTA |= 0x08; // cs high

	shift_out(0xfa);
	shift_out(data & 0xf0);
	shift_out((data & 0x0f) << 4);

	PORTA &= ~0x08; // cs low
	delayMicroseconds(50);
}

void shift_out(char data)
{
	for(int i = 0; i < 8; i++)
	{
		if(data & (0x80 >> i))
		{
			PORTC |= 0x02; // data high
		}
		else
		{
			PORTC &= ~0x02; // data low
		}

		PORTA |= 0x02;
		PORTA &= ~0x02;
	}
}