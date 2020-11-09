
#include "oled_1351.h"



uint8_t *num_ptr[3] = { 	
						num0_40_48_16bit,
					 	num0_40_48_16bit,
						num0_40_48_16bit,  
					};

//	uint8_t *num_ptr[3] = { 	
//							0,
//						 	0,
//							0
//						};



//uint8_t *bitmap_ptr = tiger_128_96_16bit_565;
uint8_t *bitmap_ptr = tiger_64_64_16bit;


char num_100_pre = 0;
char num_10_pre = 0;
char num_1_pre = 0;


void OLED_CS(uint8_t x)
{
  digitalWrite(oled_cs, x);
}

void OLED_RST(uint8_t x)
{
  digitalWrite(oled_rst, x);
}

void OLED_DC(uint8_t x)
{
  digitalWrite(oled_dc, x);
}

void OLED_SCK(uint8_t x) 
{
  digitalWrite(oled_sck, x);
}

void OLED_DIN(uint8_t x) 
{
  digitalWrite(oled_din, x);
}


void Write_Command(uint8_t cmd)  
{
  OLED_CS(LOW);

  OLED_DC(LOW);
  SPI.transfer(cmd);
  OLED_DC(HIGH);

  OLED_CS(HIGH);
}

void Write_Data(uint8_t dat) 
{
  OLED_CS(LOW);

  SPI.transfer(dat);

  OLED_CS(HIGH);
}



void Write_Data_bulk(uint8_t dat) 
{
  SPI.transfer(dat);
}



void oled_1351_init(void) {


  OLED_CS(LOW);
  OLED_RST(LOW);
  delay(500);
  OLED_RST(HIGH);
  delay(500);
    
  Write_Command(0xfd);  // command lock
  Write_Data(0x12);
  Write_Command(0xfd);  // command lock
  Write_Data(0xB1);

  Write_Command(0xae);  // display off
  
  Write_Command(0xB3);
  Write_Data(0xF1);

  Write_Command(0xa0);  //set re-map & data format
  Write_Data(0x74); //Horizontal address increment
  
  Write_Command(0xCA);  
  Write_Data(0x7F);

  Write_Command(0xa2);  //set display offset
  Write_Data(0x00);

  Write_Command(0xB5);  
  Write_Data(0x00);

  Write_Command(0xAB);
  Write_Data(0x01);

  Write_Command(0xB1);
  Write_Data(0x32);

  Write_Command(0xBE);
  Write_Data(0x05);

  Write_Command(0xA6);

  Write_Command(0xC1);
  Write_Data(0xC8);
  Write_Data(0x80);
  Write_Data(0xC8);  

  Write_Command(0xC7);
  Write_Data(0x0F);

  Write_Command(0xB4);
  Write_Data(0xA0);
  Write_Data(0xB5);
  Write_Data(0x55);  
  
  Write_Command(0xB6);
  Write_Data(0x01);

  Write_Command(0xAF);  
}

#if 0  
void oled_1351_init(void) 
{

  OLED_CS(LOW);
  OLED_RST(LOW);
  delay(500);
  OLED_RST(HIGH);
  delay(500);
    
  Write_Command(0xfd);  // command lock
  Write_Data(0x12);
  Write_Command(0xfd);  // command lock
  Write_Data(0xB1);

  Write_Command(0xae);  // display off
  Write_Command(0xa4);  // Normal Display mode

  Write_Command(0x15);  //set column address
  Write_Data(0x00); //column address start 00
  Write_Data(0x7f); //column address end 95
  Write_Command(0x75);  //set row address
  Write_Data(0x00); //row address start 00
  Write_Data(0x7f); //row address end 63  

  Write_Command(0xB3);
  Write_Data(0xF1);

  Write_Command(0xCA);  
  Write_Data(0x7F);

  Write_Command(0xa0);  //set re-map & data format
  Write_Data(0x74); //Horizontal address increment
            //74
  Write_Command(0xa1);  //set display start line
  Write_Data(0x00); //start 00 line

  Write_Command(0xa2);  //set display offset
  Write_Data(0x00);

  Write_Command(0xAB);  
  Write_Data(0x01);  

  Write_Command(0xB4);  
  Write_Data(0xA0);   
  Write_Data(0xB5);  
  Write_Data(0x55);    

  Write_Command(0xC1);  
  Write_Data(0xC8); 
  Write_Data(0x80);
  Write_Data(0xC0);

  Write_Command(0xC7);  
  Write_Data(0x0F);

  Write_Command(0xB1);  
  Write_Data(0x32);

  Write_Command(0xB2);  
  Write_Data(0xA4);
  Write_Data(0x00);
  Write_Data(0x00);

  Write_Command(0xBB);  
  Write_Data(0x17);

  Write_Command(0xB6);  
  Write_Data(0x01);

  Write_Command(0xBE);  
  Write_Data(0x05);

  Write_Command(0xA6);

  Clear_Screen();
  Write_Command(0xaf);   //display on


}
#endif

void RAM_Address(void)  
{
  
  Write_Command(0x15);
  Write_Data(0x00);
  Write_Data(0x7f);

  Write_Command(0x75);
  Write_Data(0x00);
  Write_Data(0x7f);
}

void Clear_Screen(void)  
{
  RAM_Address();
  Write_Command(0x5C);
  for(int i=0;i<128;i++)  {
    for(int j=0;j<128;j++)  {
      Write_Data(0x00);
      Write_Data(0x00); // one pixel == 2byte
    }
  }
}



void Set_Address(uint8_t column, uint8_t row)  
{
  
	Write_Command(SSD1351_CMD_SETCOLUMN);  
	Write_Data(column);  //X start 
	Write_Data(column); //X end 
	Write_Command(SSD1351_CMD_SETROW); 
	Write_Data(row);  //Y start 
	Write_Data(row+7);  //Y end 
	Write_Command(SSD1351_CMD_WRITERAM); 
}



void Put_Pixel(int16_t x, int16_t y, unsigned int color)
{
	uint8_t first_byte = (color & 0xff00) >> 8;
	uint8_t second_byte = color & 0xff;
	
	if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT)) return;
	if ((x < 0) || (y < 0)) return;

	Set_Address(x, y);

	Write_Data(first_byte);
	Write_Data(second_byte);
}



void Draw_Num(uint16_t x, uint16_t y, uint8_t num) 
{

	// set location
	Write_Command(SSD1351_CMD_SETCOLUMN);
	Write_Data(x);
	Write_Data(x+NUM_IMAGE_WIDTH-1);
	Write_Command(SSD1351_CMD_SETROW);
	Write_Data(y);
	Write_Data(y+NUM_IMAGE_HEIGHT-1);

	// fill!
	Write_Command(SSD1351_CMD_WRITERAM);  

	OLED_CS(LOW);	

	for (uint16_t i=0; i < NUM_IMAGE_WIDTH*NUM_IMAGE_HEIGHT; i++) {
		Write_Data_bulk(pgm_read_byte(&num_ptr[num][0x46 + i*2+1]));
		Write_Data_bulk(pgm_read_byte(&num_ptr[num][0x46 + i*2]));	
	}

	OLED_CS(HIGH);
}



void Draw_Bitmap(uint16_t x, uint16_t y) 
{
	
	// set location
	Write_Command(SSD1351_CMD_SETCOLUMN);
	Write_Data(x);
	Write_Data(x+BITMAP_IMAGE_WIDTH-1);
	Write_Command(SSD1351_CMD_SETROW);
	Write_Data(y);
	Write_Data(y+BITMAP_IMAGE_HEIGHT-1);
	
	// fill!
	Write_Command(SSD1351_CMD_WRITERAM);  
	
	OLED_CS(LOW);	
	
	for (uint16_t i=0; i < BITMAP_IMAGE_WIDTH*BITMAP_IMAGE_HEIGHT; i++) {
		Write_Data_bulk(pgm_read_byte(&bitmap_ptr[0x46 + i*2+1]));
		Write_Data_bulk(pgm_read_byte(&bitmap_ptr[0x46 + i*2]));	
	}
	
	OLED_CS(HIGH);
}
	
	
	

void Set_Init_Value()
{
	Draw_Num(0+NUM_POSITION_OFFSET,64,0);
	Draw_Num(40+NUM_POSITION_OFFSET,64,1);
	Draw_Num(80+NUM_POSITION_OFFSET,64,2);  
}



