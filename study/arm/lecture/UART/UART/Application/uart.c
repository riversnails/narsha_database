
#include <stdio.h>

#include <stm32f10x_lib.h>    // STM32F10x Library Definitions
#include "STM32_Init.h"       // STM32 Initialization
/*----------------------------------------------------------*\
 | HARDWARE DEFINE                                          |
\*----------------------------------------------------------*/
#define LED             ( 1 << 5 )              // PB5: LED D2
/*----------------------------------------------------------*\
 | SOFTWARE DATA                                            |
\*----------------------------------------------------------*/
#define I2C_Speed    100000
#define I2C_OwnAddr  0xA0

unsigned short  analog [3];

I2C_InitTypeDef i = {
	I2C_Mode_I2C,
	I2C_DutyCycle_2,
	I2C_OwnAddr,
	I2C_Ack_Enable,
	I2C_AcknowledgedAddress_7bit,
	I2C_Speed
	};
/*----------------------------------------------------------*\
 | I2C Initialisation                                       |
\*----------------------------------------------------------*/
void I2C_Initialisation( void ) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//  RCC->APB2ENR |= (1<<0);                      /* enable periperal clock for AFIO      */
  /* Enable I2C1 clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  I2C_DeInit(I2C1);
  I2C_Init(I2C1, &i);

  I2C_Cmd (I2C1, ENABLE);
  }
u32 waitForEvent(u32 event) {
  int i;
  for(i = 2000000; i > 0; i--) {
	    u32 s = I2C1->SR1;
	//if (s & (I2C_FLAG_TIMEOUT | I2C_FLAG_PECERR | I2C_FLAG_OVR | I2C_FLAG_AF | I2C_FLAG_ARLO | I2C_FLAG_BERR | I2C_FLAG_STOPF));
    if (s & event){

/*
	if(event == I2C_FLAG_SB) printf("waiting for I2C_FLAG_SB(0x20000001), and return flag is %08x..\n\r",s);
	else if(event == I2C_FLAG_ADDR) printf("waiting for I2C_FLAG_ADDR(0xA0000002), and return flag is %08x..\n\r",s);
	else if(event == I2C_FLAG_BTF) printf("waiting for I2C_FLAG_BTF(0x60000004), and return flag is %08x..\n\r",s);
	else if(event == I2C_FLAG_TXE) printf("waiting for I2C_FLAG_TXE(0x00000080), and return flag is %08x..\n\r",s);
	else if(event == I2C_FLAG_RXNE) printf("waiting for I2C_FLAG_RXNE(0x00000040), and return flag is %08x..\n\r",s);
*/
	return s;
    }
  	}
  return 0;
  }
void sendStart(void) {
	I2C_GenerateSTART(I2C1, ENABLE);							// send start when bus becomes available
	waitForEvent(I2C_FLAG_SB);
	I2C1->DR = I2C_OwnAddr;										// send slave address for transmission
	waitForEvent(I2C_FLAG_ADDR);
	I2C1->SR2;												// clear event
  }
void sendData(u8 data){
  //waitForEvent(I2C_FLAG_TXE);
//  waitForEvent(I2C_FLAG_BTF);
  I2C1->DR = data;											// send byte
   waitForEvent(I2C_FLAG_BTF);
 }
void sendStop(void) {
  waitForEvent(I2C_FLAG_TXE);
  I2C_GenerateSTOP(I2C1, ENABLE);							// send stop after current byte
  }
u8 receiveByte(void) {
  I2C_GenerateSTART(I2C1, ENABLE);							// send start when bus becomes available
  waitForEvent(I2C_FLAG_SB);
  I2C_AcknowledgeConfig(I2C1, DISABLE);					// only one byte will be read
  I2C1->DR = I2C_OwnAddr | 0x01;								// send slave address for reception
  waitForEvent(I2C_FLAG_ADDR);
  I2C1->SR2;												// clear event

  waitForEvent(I2C_FLAG_RXNE);
  I2C_GenerateSTOP(I2C1, ENABLE);							// send stop after current byte
  return I2C1->DR;											// receive byte
  }
/*----------------------------------------------------------*\
 | I2C Write Byte                                           |
\*----------------------------------------------------------*/
void WriteByte(u16 addr, u8 data) {
  /* Enable I2C1 acknowledgement if it is already disabled by other function */
  //I2C_AcknowledgeConfig(I2C1, ENABLE);
  //I2C_AcknowledgeConfig(I2C1, DISABLE);					// only one byte will be read

  sendStart();
  sendData( addr & 0xFF );
  //I2C_AcknowledgeConfig(I2C1, DISABLE);					// only one byte will be read
  sendData( data );
  waitForEvent(I2C_FLAG_BTF);
  sendStop();
  }
/*----------------------------------------------------------*\
 | I2C Read Byte                                            |
\*----------------------------------------------------------*/
u8 ReadByte(u16 addr) {
  /* Enable I2C1 acknowledgement if it is already disabled by other function */
  //I2C_AcknowledgeConfig(I2C1, ENABLE);

  sendStart();
  sendData( addr & 0xFF );
  //sendStart();
  //sendStop();
  return receiveByte();
  }

/*----------------------------------------------------------*\
 | SendChar                                                 |
 | Write character to Serial Port.                          |
\*----------------------------------------------------------*/
int SendChar (int ch)  {

  while (!(USART1->SR & USART_FLAG_TXE));
  USART1->DR = (ch & 0x1FF);

  return (ch);
}
/*----------------------------------------------------------*\
 | GetKey                                                   |
 | Read character to Serial Port.                           |
\*----------------------------------------------------------*/
int GetKey (void)  {

  while (!(USART1->SR & USART_FLAG_RXNE));

  return ((int)(USART1->DR & 0x1FF));
}


void USART1_IRQHandler (void) {
  volatile unsigned int IIR;
  char receive_char;

    IIR = USART1->SR;
    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
      USART1->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
	receive_char =  (USART1->DR & 0x1FF);
//	printf("received data is %c..\n\r",receive_char);
	printf("uart 1 : %c",receive_char);

    }
 /*
    if (IIR & USART_FLAG_TXE) {
      USART1->SR &= ~USART_FLAG_TXE;	          // clear interrupt
	//USART1->DR = '5';
	printf("sent data is %c..\n\r",USART1->DR);
    }
 */
}


void USART2_IRQHandler (void) {
  volatile unsigned int IIR;
  char receive_char;

    IIR = USART2->SR;
    if (IIR & USART_FLAG_RXNE) {                  // read interrupt
      USART2->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
	receive_char =  (USART2->DR & 0x1FF);
//	printf("received data is %c..\n\r",receive_char);
	printf("uart2 : %c",receive_char);

    }
}


void EXTI0_IRQHandler(void)
{
  if (EXTI->PR & (1<<0)) {                        
	printf("in exti0 irq handler...\r\n");
    EXTI->PR |= (1<<0);                           // clear pending interrupt
  }
}

void EXTI1_IRQHandler(void)
{
  if (EXTI->PR & (1<<1)) {                        
	printf("in exti0 irq handler...\r\n");
	PWR_EnterSTANDBYMode();

    EXTI->PR |= (1<<1);                           // clear pending interrupt
  }
}


void adc_Init (void) {

//  GPIOA->CRL &= ~0x0000000F;                   /* set PIN1 analog input (see stm32_Init.c) */

  RCC->AHBENR |= (1<<0);                       /* enable periperal clock for DMA       */

//  DMA1_Channel1->CMAR  = (unsigned long)&analog; /* set chn1 memory address     */
  DMA1_Channel1->CMAR  = (unsigned long)analog; /* set chn1 memory address     */
  DMA1_Channel1->CPAR  = (unsigned long)&(ADC1->DR);     /* set chn1 peripheral address */
  DMA1_Channel1->CNDTR = 3;                     /* transmit 3 words                     */
//  DMA_Channel1->CCR   = 0x00002520;            /* configure DMA channel 1              */
  DMA1_Channel1->CCR   = 0x000025A0;            /* configure DMA channel 1              */
                                               /* circular mode, memory increment mode */
                                               /* memory & peripheral size 16bit       */
                                               /* channel priotity high                */
  DMA1_Channel1->CCR  |= (1 << 0);              /* enable DMA Channe1                   */

  RCC->APB2ENR |= (1<<9);                      /* enable periperal clock for ADC1      */

  ADC1->SQR1  = 0x00200000;                    /* three conversions                    */
  ADC1->SQR3  = (3<<10) | (2<<5) | (1<<0);     /* set order to chn1 - chn2 - chn3      */
  ADC1->SMPR2 = (5<< 9) | (5<<6) | (5<<3);     /* set sample time (55,5 cycles)        */ 

  ADC1->CR1   =  0x00000100;                   /* use independant mode, SCAN mode      */
  ADC1->CR2   =  0x000E0103;                   /* data align right, cont. conversion   */
                                               /* EXTSEL = SWSTART                     */ 
                                               /* enable ADC, DMA mode                 */
  ADC1->CR2  |=  0x00500000;                   /* start SW conversion                  */
  }


void Delay(vu32 nCount) {
  for(; nCount != 0; nCount--);
  }

void abc(char *a)
{
	char (*ab)[3];
	
	ab = (char (*)[3])a;
	
	ab[1][1] = 5;
	
	//printf("a[1][1] = %d\n",a[1][1]);
}


	
/*----------------------------------------------------------*\
 | MIAN ENTRY                                               |
\*----------------------------------------------------------*/
int main (void) {
  	char s[20];
	int i;
	
	int a;
	int *b;
	char *aaa;
	
	char *d = "abcd";
	
	int c[3] = {1,2,3};
	
	char ab[3][3] ={ 	{ 1,2,3 },
										{ 4,3,6 },
										{ 7,8,9 }
								};
	
	
	aaa = (char *)(0x20002000);

	
	
	stm32_Init ();                                // STM32 setup
	//*aaa = 10;
	
								
								
	while(1);
								
								
	//aaa = "hello";
	
	//printf("addr of hello = %p\n",aaa);
								
	printf("ab[1][1] = %d\n",ab[1][1]);							
	abc((char *)ab);
	printf("ab[1][1] = %d\n",ab[1][1]);	
								
								
while(1);
	
	a=5;
	b=&a;
	*b=0x12345678;
	
//	printf("%p\n",&a);
//	printf("%p\n",b);	
	
//	printf("%d\n",*(c+1));
//	printf("%p\n",c+1);
//printf("%p\n",&c[1]);

		printf("%p\n",d);
		printf("%c\n",*(d+1));

	
	//printf("hello");
	
	
	
	while(1){
		printf("%s\n",d);
		
		Delay(100000);
	}
	
  //adc_Init();
	
#if 0
	I2C_Initialisation();

 	for(;;) {
		printf("i2c write start\r\n");
		
		for(i=0;i<10;i++)
			WriteByte(i, i);

		printf("i2c write end\r\n");

		printf("i2c read start\r\n");

		for(i=0;i<10;i++)
			printf("[%d]read byte is %d...\n\r",i,ReadByte(i));

		printf("i2c read end\r\n");

		//	printf("before write : Read:0x%02X\n\r", ReadByte(0x00) ); 
		//	WriteByte(0x00, 0x55);
		//	printf("after write : Read:0x%02X\n\r", ReadByte(0x00) ); 


	}
 #else

	/*
	for(;;) {
		printf("haha=%d\r",i++);
//		printf("haha\r\n");
		//printf("% 4.2fV \r\n", (float)(analog[0] * 3.3 / 0xFFF));
		//printf("% 4.2fV , p=%p\r\n", (float)(analog[0] * 3.3 / 0xFFF), &analog[0]);
	}
*/
	
		for(;;) {
			
			
			for(i=0;i<10;i++) printf("                                                          \r\n");
			
				printf("--------\r\n");
				printf("-******-\r\n");
				printf("------*-\r\n");
				printf("------*-\r\n");
				printf("-******-\r\n");
				printf("------*-\r\n");
				printf("------*-\r\n");
				printf("-******-\r\n");
				printf("--------\r\n");
			
			Delay(500000);

		}
	
 #endif
  }
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
/*----------------------------------------------------------*\
 | END OF FILE                                              |
\*----------------------------------------------------------*/
