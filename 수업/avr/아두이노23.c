#define SCL 19
#define SDA 18
#define OLED_I2C_ADDR 0x3C

char oled_init_value[] = { 0xAE, 0xD5, 0x80, 0xA8, 63,
						   0xD3, 0x00, 0x40, 0x8D, 0x10,
						   0x20, 0x00, 0xA1, 0xC8, 
						   0xDA, 0x12, 0x81, 0x9F, 
						   0xD9, 
						   0x22, 
						   0xDB, 0x40, 0xA4, 0xA6, 0x2E, 0xAF
						};

char oled_area_command_value[] = {0x22,0x00,0xff,0x21,0x00,127};

char oled_buffer[128 * 8];
void setup() 
{
	Serial.begin(115200);

	i2c_init();
	oled_128_64_init(oled_init_value);
	while (1)
	{
		for (int i = 0; i < 128 * 8; i++)
		{
	  		oled_buffer[i] = 0x0f;
		}

		oled_128_64_i2c_command_multi(oled_area_command_value, 6);
		oled_128_64_i2c_data_multi(oled_buffer, 128 * 8);
		delay(1000);

		for (int i = 0; i < 128 * 8; i++)
		{
	  		oled_buffer[i] = 0x00;
		}

		oled_128_64_i2c_command_multi(oled_area_command_value, 6);
		oled_128_64_i2c_data_multi(oled_buffer, 128 * 8);
		delay(1000);
	}
}

void loop() 
{

}

void oled_128_64_init(char *init_value)
{
	oled_128_64_i2c_command_multi(init_value, 26);
}

void oled_128_64_i2c_command(char command)
{
	i2c_start();
	i2c_8bit(OLED_I2C_ADDR << 1);
	i2c_ack();
	i2c_8bit(0x00);
	i2c_ack();
	i2c_8bit(command);
	i2c_ack();
	i2c_stop();
}

void oled_128_64_i2c_command_multi(char *command, int len)
{
	i2c_start();
	i2c_8bit(OLED_I2C_ADDR << 1);
	i2c_ack();
	i2c_8bit(0x00);
	i2c_ack();
	for(int i = 0; i < len; i++)
	{
		i2c_8bit(*(command+i));
		i2c_ack();
	}
	i2c_stop();
}

void oled_128_64_i2c_data(char data)
{
	i2c_start();
	i2c_8bit(OLED_I2C_ADDR << 1);
	i2c_ack();
	i2c_8bit(0x40);
	i2c_ack();
	i2c_8bit(data);
	i2c_ack();
	i2c_stop();
}

void oled_128_64_i2c_data_multi(char *data, int len)
{
	i2c_start();
	i2c_8bit(OLED_I2C_ADDR << 1);
	i2c_ack();
	i2c_8bit(0x40);
	i2c_ack();
	for(int i = 0; i < len; i++)
	{
		i2c_8bit(*(data+i));
		i2c_ack();
	}
	i2c_stop();
}

//==================================================

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
		digitalWrite(SCL, HIGH);
		digitalWrite(SCL, LOW);
	}
}

void i2c_ack()
{
	pinMode(SDA, INPUT);
	digitalWrite(SCL, HIGH);
	Serial.printf("%d", digitalRead(SDA));
	pinMode(SDA, OUTPUT);
	digitalWrite(SCL, LOW);
}