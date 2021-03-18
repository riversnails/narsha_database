
#include <SPI.h>

#include "oled_1351.h"


void gpio_init()
{
	//pinMode(53, OUTPUT);  // for mega
	pinMode(10, OUTPUT);  // for uno

	pinMode(oled_cs, OUTPUT);
	pinMode(oled_rst, OUTPUT);
	pinMode(oled_dc, OUTPUT);

	pinMode(oled_sck, OUTPUT);
	pinMode(oled_din, OUTPUT);

	pinMode(2, INPUT);

}


void spi_init()
{
	//Init SPI
	SPI.setDataMode(SPI_MODE0);
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	SPI.begin();

}
	

void setup()  {

	Serial.begin(115200);

	gpio_init();
	spi_init();
	
	oled_1351_init();
	Clear_Screen();

	/*
	for(int j = 0; j < 128; j++)
	{
		for(int i = 0; i < 128; i++)
		{
			if(j < 42) Put_Pixel(i, j, RED);
			else if(j < 84) Put_Pixel(i, j, GREEN);
			else if(j < 128) Put_Pixel(i, j, BLUE);
		}
	}

	delay(1000);

	Draw_Bitmap(uint16_t x, uint16_t y)
	 */

	Draw_Bitmap(0,0);

	
	while(1);
	
	Set_Init_Value();
	delay(3000);

	Draw_Bitmap(0,128-96);
	delay(3000);
}



int count = 0;
int delay_count = 0;
char button_on_flag = 0;
char button_on_toggle = 0;


void loop() {
	for(int j=0;j<128-96;j++)
	{
		for(int i=0;i<128;i++)
		{
			Put_Pixel(i,j,RED);
			delay(10);
		}
			
	}

}

