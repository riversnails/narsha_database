/*

                         \\\|///
                       \\  - -  //
                        (  @ @  )
+---------------------oOOo-(_)-oOOo-------------------------+
|                 智林STM32开发板试验程序                   |
|                   1.8‘ TFT LCD Driver                     |
|                 刘笑然 by Xiaoran Liu                     |
|                        2008.4.16                          |
|                                                           |
|           智林测控技术研究所 ZERO research group          |
|                      www.the0.net                         |
|                             Oooo                          |
+-----------------------oooO--(   )-------------------------+
                       (   )   ) /
                        \ (   (_/
                         \_)     

*/
/*----------------------------------------------------------*\
 |  引入相关芯片的头文件                                    |
\*----------------------------------------------------------*/
#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "MyType.h"
#include "TFT018.h"
/*----------------------------------------------------------*\
 | HARDWARE DEFINITIONS                                     |
\*----------------------------------------------------------*/
/* PINS: 
   - DB0 = PC0
   - DB1 = PC1
   - DB2 = PC2
   - DB3 = PC3
   - DB4 = PC4
   - DB5 = PC5
   - DB6 = PC6
   - DB7 = PC7
   - RD  = PC8
   - RW  = PC9
   - RS  = PC10
   - RST = PC11
   - CS  = PC12                                             */

#define PIN_CS               (   1 << 12)
#define PIN_RST              (   1 << 11)
#define PIN_RS               (   1 << 10)
#define PIN_RW               (   1 <<  9)
#define PIN_RD               (   1 <<  8)
#define PINS_CTRL            (0x1F <<  8)
#define PINS_DATA            (0xFF <<  0)
#define PINS_ALL             (PINS_CTRL | PINS_DATA)

/* Enable Clock for peripheral driving LCD pins                               */
#define LCD_CLOCK_EN         (RCC->APB2ENR |= (1 << 4)); // enable clock for GPIOC

/* pin CS  setting to 0 or 1                                                  */
#define LCD_CS(x)             GPIOC->ODR = (GPIOC->ODR & ~PIN_CS) | (x ? PIN_CS : 0);
/* pin RST setting to 0 or 1                                                   */
#define LCD_RST(x)            GPIOC->ODR = (GPIOC->ODR & ~PIN_RST) | (x ? PIN_RST : 0);
/* pin RS setting to 0 or 1                                                   */
#define LCD_RS(x)             GPIOC->ODR = (GPIOC->ODR & ~PIN_RS) | (x ? PIN_RS : 0);
/* pin RW setting to 0 or 1                                                   */
#define LCD_RW(x)             GPIOC->ODR = (GPIOC->ODR & ~PIN_RW) | (x ? PIN_RW : 0);
/* pin RD setting to 0 or 1                                                   */
#define LCD_RD(x)             GPIOC->ODR = (GPIOC->ODR & ~PIN_RD) | (x ? PIN_RD : 0);

/* Reading DATA pins                                                          */
#define LCD_DATA_IN           (((GPIOC->IDR & PINS_DATA) >> 0) & 0xFF)
/* Writing value to DATA pins                                                 */
#define LCD_DATA_OUT(x)       GPIOC->ODR = (GPIOC->ODR & ~PINS_DATA) | (x << 0);

/* Setting all pins to output mode                                            */
#define LCD_ALL_DIR_OUT       GPIOC->CRL = (GPIOC->CRL & 0x00000000) | 0x33333333; \
                              GPIOC->CRH = (GPIOC->CRH & 0xFFF00000) | 0x00033333;
 
/* Setting DATA pins to input mode                                            */
#define LCD_DATA_DIR_IN       GPIOC->CRL = (GPIOC->CRL & 0x00000000) | 0x44444444;

/* Setting DATA pins to output mode                                           */
#define LCD_DATA_DIR_OUT      GPIOC->CRL = (GPIOC->CRL & 0x00000000) | 0x33333333;

/*----------------------------------------------------------*\
 | SOFTWARE DEFINITIONS                                     |
\*----------------------------------------------------------*/
/*------------------------- Speed dependant settings -------------------------*/

/* If processor works on high frequency delay has to be increased, it can be 
   increased by factor 2^N by this constant                                   */
#define DELAY_2N     0

#define  write_command	 	LCD_Inst
#define  write_parameter	LCD_Data


volatile U8 Font=0;

/*----------------------------------------------------------*\
 | Delay in while loop cycles                               |
 | cnt:    number of while cycles to delay                  |
\*----------------------------------------------------------*/
static void delay (unsigned int cnt) {
  cnt <<= DELAY_2N;

  while (cnt--);
  }
/*----------------------------------------------------------*\
 | LOW LEVEL OPERATE : BUS simulate                         |
\*----------------------------------------------------------*/
void LCD_Bus_Initial( void ) {
  LCD_CLOCK_EN                          /* Enable clock for peripheral        */

  /* Set all pins for LCD as outputs                                          */
  LCD_ALL_DIR_OUT

  LCD_RST(0);
  delay( 25000 );
  delay( 25000 );
  LCD_CS(1);
  LCD_RW(1);
  LCD_RD(1);
  LCD_RS(1);
  LCD_RST(1);
  }
void LCD_Bus_Write_Byte( unsigned char Data ) {
  LCD_DATA_OUT( Data );
  LCD_RW(0);
  delay( 1 );
  LCD_RW(1);
  }
void LCD_Bus_Write_Data( unsigned short Data ) {
  LCD_Bus_Write_Byte( Data>>8 );
  LCD_Bus_Write_Byte( Data );
  }
unsigned int LCD_Bus_Read_Byte( void ) {
  unsigned int BusData;

  LCD_DATA_DIR_IN;
  LCD_RD(0);
  BusData = LCD_DATA_IN;
  delay( 1 );
  LCD_RD(1);

  return BusData>>8;
  }
void LCD_Inst(unsigned int Cmd) {
  LCD_CS(0);
  LCD_RD(1);
  LCD_RS(0);
  LCD_Bus_Write_Byte( Cmd );
  LCD_CS(1);
  }
void LCD_Data(unsigned int Data) {
  LCD_CS(0);
  LCD_RD(1);
  LCD_RS(1);
  LCD_Bus_Write_Byte( Data );
  LCD_CS(1);
  }
void LCD_Reg_Set(unsigned int Cmd,unsigned int Data) {
  LCD_CS(0);
  LCD_RD(1);
  LCD_RS(0);
  LCD_Bus_Write_Byte( Cmd );
  LCD_RS(1);
  LCD_Bus_Write_Byte( Data );
  LCD_CS(1);
  }
/*----------------------------------------------------------*\
 | HIGHLEVEL OPERATION                                      |
\*----------------------------------------------------------*/
/**
void LCD_Init( void ) {
	LCD_Bus_Initial();

	LCD_Inst(0x11);	// sleep out/power on(SLPOUT)
	delay( 1500 );
	LCD_Inst(0x20);	// display inversion off
	LCD_Inst(0x38);	// ID mode off (IDMOFF)
	LCD_Inst(0x13);	// Normal display mode on (Partial mode off)
	// color mode Interface pixel format  (COLMOD)
	LCD_Reg_Set(0x3A,0x05);	// 16-bit/pixel , 1-times data transfer
	LCD_Reg_Set(0xC0,0x03);	// power control 1      (PWCTR1) , GVDD voltage set 4.65
	LCD_Reg_Set(0xC1,0x05);	// VCOMH voltage set 4.10V
	delay( 1500 );
	LCD_Reg_Set(0xC5,0xBA);	// VCOMAC voltage set 4.35V
	LCD_Reg_Set(0xC6,0x10);	// VCOMAC voltage set 4.35V
	delay( 1500 );
	// Gamma voltage adjustalbe level
	//Gamma + Polarity correction characteristics set
	LCD_Inst(0xE0);
	LCD_Data(0x01);
	LCD_Data(0x0A);
	LCD_Data(0x11);
	LCD_Data(0x23);
	LCD_Data(0x20);
	LCD_Data(0x01);
	LCD_Data(0x1E);
	LCD_Data(0x02);
	LCD_Data(0x06);
	LCD_Data(0x05);
	LCD_Data(0x0E);
	LCD_Data(0x0D);
	LCD_Data(0x01);
	LCD_Data(0x05);
	LCD_Data(0x03);
	LCD_Data(0x06);
	//Gamma + Polarity correction characteristics set
	LCD_Inst(0xE1);
	LCD_Data(0x01);
	LCD_Data(0x20);
	LCD_Data(0x23);
	LCD_Data(0x11);
	LCD_Data(0x0A);
	LCD_Data(0x01);
	LCD_Data(0x02);
	LCD_Data(0x1E);
	LCD_Data(0x06);
	LCD_Data(0x03);
	LCD_Data(0x05);
	LCD_Data(0x01);
	LCD_Data(0x0D);
	LCD_Data(0x0E);
	LCD_Data(0x05);
	LCD_Data(0x06);
	// memory data access control  (MADCTR)
	LCD_Reg_Set(0x36,0xA0);	// MY=1; MX=0; MV=0; ML=0; RGB=0
	LCD_Inst(0x37);
	LCD_Data(0x00);
	LCD_Data(0x00);
	// display on
	LCD_Inst(0x29);
	}
 **/



/*----------------------------------------------------------*\
 | ST7735 HIGHLEVEL OPERATION                                      |
\*----------------------------------------------------------*/
void LCD_Init( void ) {
  LCD_Bus_Initial();


	delay(25000);
	write_command(0x11);	//Sleep Out
	delay(25000);

	write_command(0x3A);
	write_parameter(0x05);

	write_command(0x36);

//  ORG 
//	write_parameter(0xC8);	//  1100 1000

	write_parameter(0xA8);	//  1100 1000

/*
	write_command(0x0B);	//Y Address Setup
	write_parameter(0x00);
	write_parameter(0xE0); 	// MY=0; MX=1; MV=1; ML=0; RGB=0 ; MH, D0, D1
*/



//	write_command(0x21);

	write_command(0xB1);	//Setup Frame Rate Control (In Normal Mode/ Full Colors)
	write_parameter(0x06);
	write_parameter(0x18);
	write_parameter(0x14);

	write_command(0xB2);	//Setup Frame Rate Control (In Idle Mode/ 8-Colors)
	write_parameter(0x06);
	write_parameter(0x18);
	write_parameter(0x14);

	write_command(0xB3);	//Setup Frame Rate Control (In Partial Mode/ Full Colors)
	write_parameter(0x06);
	write_parameter(0x18);
	write_parameter(0x14);
	write_parameter(0x06);
	write_parameter(0x18);
	write_parameter(0x14);

	write_command(0xB4);	//Display Inversion Control
	write_parameter(0x03);

//	write_command(0xB6);	//Display Function Set 5
//	write_parameter(0x15);
//	write_parameter(0x00);

	write_command(0xC0);	//Power Control-1
	write_parameter(0x02);
	write_parameter(0x70);

	write_command(0xC1);	//Power Control-2
	write_parameter(0x07);

	write_command(0xC2);	//Power Control-3 (In Normal Mode/ Full Colors)
	write_parameter(0x01);
	write_parameter(0x01);

	write_command(0xC3);	//Power Control-4 (In Idle Mode/ 8-Colors)
	write_parameter(0x02);
	write_parameter(0x07);

	write_command(0xC4);	//Power Control-5 (In Partial Mode/ Full-Colors)
	write_parameter(0x02);
	write_parameter(0x04);

	write_command(0xC5);	//VCOM Control-2
	write_parameter(0x3e);//39  
	write_parameter(0x42);//42	


//	write_command(0xC7);	//Power Control-2
//	write_parameter(0xEF);


	write_command(0xFF);	//VCOM Control-1
	write_parameter(0x40);
	write_parameter(0x03);
	write_parameter(0x1A);

	write_command(0xFC);	//Power Control-6
	write_parameter(0x11);
	write_parameter(0x17);



	write_command(0xE0);	//Setup Gamma (‘+’polarity) Correction Characteristics Setting
	write_parameter(0x09);
	write_parameter(0x16);
	write_parameter(0x09);
	write_parameter(0x27);
	write_parameter(0x2E);
	write_parameter(0x25);
	write_parameter(0x1C);
	write_parameter(0x20);
	write_parameter(0x1E);
	write_parameter(0x1A);
	write_parameter(0x24);
	write_parameter(0x2D);
	write_parameter(0x04);
	write_parameter(0x05);
	write_parameter(0x02);
	write_parameter(0x0E);

	write_command(0xE1);	//Setup Gamma (‘-’polarity) Correction Characteristics Setting
	write_parameter(0x0B);
	write_parameter(0x14);
	write_parameter(0x09);
	write_parameter(0x26);
	write_parameter(0x27);
	write_parameter(0x22);
	write_parameter(0x1C);
	write_parameter(0x20);
	write_parameter(0x1D);
	write_parameter(0x1A);
	write_parameter(0x25);
	write_parameter(0x2D);
	write_parameter(0x06);
	write_parameter(0x06);
	write_parameter(0x02);
	write_parameter(0x0f);

	write_command(0x2A);	//X Address Setup
	write_parameter(0x00);
	write_parameter(0x02);
	write_parameter(0x00);
	write_parameter(0x81);

	write_command(0x2B);	//Y Address Setup
	write_parameter(0x00);
	write_parameter(0x01);
	write_parameter(0x00);
	write_parameter(0xA0);

	write_command(0x29);	//Display-On Mode

	delay(25000);

	write_command(0x2C);	//Write Display Data

 
}

void LCD_SetArea( U8 x0, U8 y0, U8 x1, U8 y1 ) {

	LCD_Inst(0x2A);
	LCD_Data(0x00);
	LCD_Data(x0+1);
	LCD_Data(0x00);
	LCD_Data(x1+1);

	LCD_Inst(0x2B);
	LCD_Data(0x00);
	LCD_Data(y0+1);
	LCD_Data(0x00);
	LCD_Data(y1+1);
	}
void LCD_Rectangle( unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned int Data ) {
	int x,y;

	LCD_SetArea(x0,y0,x1,y1);

	LCD_Inst(0x2C);

	LCD_CS(0);
	LCD_RD(1);

	LCD_RS(1);
	for(x=0;x<=x1-x0;x++)
		for(y=0;y<=y1-y0;y++) {
			LCD_Bus_Write_Byte( Data>>8 );
			LCD_Bus_Write_Byte( Data );
			}

	LCD_CS(1);
	}
void LCD_Clear_Screen( unsigned int Data ) {
	LCD_Rectangle(0,0,159,128, Data );
	}
void LCD_SetPixel(unsigned short x, unsigned short y, unsigned short color) {
	LCD_SetArea(x,y,x,y);
	LCD_Inst(0x2C);

	LCD_CS(0);
	LCD_RD(1);
	LCD_RS(1);
	LCD_Bus_Write_Byte( color>>8 );
	LCD_Bus_Write_Byte( color );
	LCD_CS(1);
	}
#include "8X16.h"

void LCD_PutChar8x16(unsigned short x, unsigned short y, char c, unsigned int f, unsigned int b) {
	register unsigned int i,j;

	LCD_SetArea(x,y,x+8-1,y+16-1);
	LCD_Inst(0x2C);

	LCD_CS(0);
	LCD_RD(1);
	LCD_RS(1);

	for(i=0; i<16;i++) {
		unsigned char m=Font8x16[c*16+i];
		for(j=0;j<8;j++) {
			if((m&0x80)==0x80) {
				LCD_Bus_Write_Byte(f>>8);
				LCD_Bus_Write_Byte(f);
				}
			else {
				LCD_Bus_Write_Byte(b>>8);
				LCD_Bus_Write_Byte(b);
				}
			m<<=1;
			}
		}

	LCD_CS(1);
	}
#include "8X8h.h"

void LCD_PutChar8x8(unsigned short x, unsigned short y, char c, unsigned int f, unsigned int b) {
	register unsigned int i,j;

	LCD_SetArea(x,y,x+8-1,y+8-1);
	LCD_Inst(0x2C);

	LCD_CS(0);
	LCD_RD(1);
	LCD_RS(1);

	for(i=0; i<8;i++) {
		unsigned char m=Font8x8[c][i];
		for(j=0;j<8;j++) {
			if((m&0x80)==0x80) {
				LCD_Bus_Write_Byte(f>>8);
				LCD_Bus_Write_Byte(f);
				}
			else {
				LCD_Bus_Write_Byte(b>>8);
				LCD_Bus_Write_Byte(b);
				}
			m<<=1;
			}
		}

	LCD_CS(1);
	}

#include "GB1616.h"	//16*16汉字字模
void PutGB1616(unsigned short x, unsigned short  y, unsigned char c[2], unsigned int f,unsigned int b){
	unsigned int i,j,k;

	LCD_SetArea(x, y, x+16-1, y+16-1);
	LCD_Inst(0x2C);

	LCD_CS(0);
	LCD_RD(1);
	LCD_RS(1);

	for (k=0;k<22;k++) {
	  if ((codeGB_16[k].Index[0]==c[0])&&(codeGB_16[k].Index[1]==c[1])){ 
    	for(i=0;i<32;i++) {
		  unsigned short m=codeGB_16[k].Msk[i];
		  for(j=0;j<8;j++) {
			if((m&0x80)==0x80) {
				LCD_Bus_Write_Byte(f>>8);
				LCD_Bus_Write_Byte(f);
				}
			else {
				LCD_Bus_Write_Byte(b>>8);
				LCD_Bus_Write_Byte(b);
				}
			m<<=1;
			} 
		  }
		}  
	  }	

	LCD_CS(1);
	}

void LCD_PutChar(unsigned short x, unsigned short y, char c, unsigned int f, unsigned int b) {
	if( Font )
		LCD_PutChar8x8( x, y, c, f, b );
	else
		LCD_PutChar8x16( x, y, c, f, b );
	}

void LCD_PutString(unsigned short x, unsigned short y, char *s, unsigned int f, unsigned int b) {
	register unsigned char l=0;
	while(*s) {
		if( *s < 0x80 ) {
			LCD_PutChar(x+l*8,y,*s,f,b);
			s++;l++;
			}
		else {
			PutGB1616(x+l*8,y,(unsigned char *)s,f,b);
			s+=2;l+=2;
			}
		}
	}

void LCD_BMP( U8 x0, U8 y0, U8 x1, U8 y1 , U8 * Bmp ) {
	S16 i,j;

	LCD_SetArea(x0,y0,x1,y1);
	LCD_Inst(0x2C);

	LCD_CS(0);
	LCD_RD(1);
	LCD_RS(1);

	for( i = y1 - y0; i >= 0; i-- )
		for( j = 0; j <= x1 - x0; j++ ) {
			U16* PixP =  (U16*)(Bmp+0x46+i*(x1-x0+1)*2);
			U16 Pix = *(PixP+j);
			LCD_Bus_Write_Byte(Pix>>8);
			LCD_Bus_Write_Byte(Pix);
			}

	LCD_CS(1);
	}

/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
