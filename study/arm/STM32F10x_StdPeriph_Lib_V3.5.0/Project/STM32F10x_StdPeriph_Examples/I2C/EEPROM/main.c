/**
  ******************************************************************************
  * @file    I2C/EEPROM/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32_eval_i2c_ee.h"


#include <stdio.h>  // by joshua


#ifdef USE_STM3210E_EVAL
 #include "stm3210e_eval_lcd.h"
#elif defined(USE_STM3210B_EVAL)
 #include "stm3210b_eval_lcd.h"
#elif defined(USE_STM3210C_EVAL)
 #include "stm3210c_eval_lcd.h"
#elif defined(USE_STM32100B_EVAL)
 #include "stm32100b_eval_lcd.h"
#elif defined(USE_STM32100E_EVAL)
 #include "stm32100e_eval_lcd.h"
#endif /* USE_STM3210E_EVAL */  

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2C_EEPROM
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
/* Uncomment the following line to enable using LCD screen for messages display */
#define ENABLE_LCD_MSG_DISPLAY

#define sEE_WRITE_ADDRESS1        0x50
#define sEE_READ_ADDRESS1         0x50
#define BUFFER_SIZE1             (countof(Tx1_Buffer)-1)
#define BUFFER_SIZE2             (countof(Tx2_Buffer)-1)
#define sEE_WRITE_ADDRESS2       (sEE_WRITE_ADDRESS1 + BUFFER_SIZE1)
#define sEE_READ_ADDRESS2        (sEE_READ_ADDRESS1 + BUFFER_SIZE1)

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/

#if 0
uint8_t Tx1_Buffer[] = "/* STM32F10xx I2C Firmware Library EEPROM driver example: \
                        buffer 1 transfer into address sEE_WRITE_ADDRESS1 */ \
                        Example Description \
                        This firmware provides a basic example of how to use the I2C firmware library and\
                        an associate I2C EEPROM driver to communicate with an I2C EEPROM device (here the\
                        example is interfacing with M24C64 EEPROM)\
                          \
                        I2C peripheral is configured in Master transmitter during write operation and in\
                        Master receiver during read operation from I2C EEPROM. \
                          \
                        The peripheral used is I2C1 but can be configured by modifying the defines values\
                        in stm32xxxx_eval.h file. The speed is set to 200kHz and can be configured by \
                        modifying the relative define in stm32_eval_i2c_ee.h file.\
                         \
                        For M24C64 devices all the memory is accessible through the two-bytes \
                        addressing mode and need to define block addresses. In this case, only the physical \
                        address has to be defined (according to the address pins (E0,E1 and E2) connection).\
                        This address is defined in i2c_ee.h (default is 0xA0: E0, E1 and E2 tied to ground).\
                        The EEPROM addresses where the program start the write and the read operations \
                        is defined in the main.c file. \
                         \
                        First, the content of Tx1_Buffer is written to the EEPROM_WriteAddress1 and the\
                        written data are read. The written and the read buffers data are then compared.\
                        Following the read operation, the program waits that the EEPROM reverts to its \
                        Standby state. A second write operation is, then, performed and this time, Tx2_Buffer\
                        is written to EEPROM_WriteAddress2, which represents the address just after the last \
                        written one in the first write. After completion of the second write operation, the \
                        written data are read. The contents of the written and the read buffers are compared.\
                         \
                        All transfers are managed in DMA mode (except when 1-byte read/write operation is\
                        required). Once sEE_ReadBuffer() or sEE_WriteBuffer() function is called, the \
                        use application may perform other tasks in parallel while Read/Write operation is\
                        managed by DMA.\
                          \
                        This example provides the possibility to use the STM32XXXX-EVAL LCD screen for\
                        messages display (transfer status: Ongoing, PASSED, FAILED).\
                        To enable this option uncomment the define ENABLE_LCD_MSG_DISPLAY in the main.c\
                        file.                                                                              ";
#endif

uint8_t Tx1_Buffer[] = "abcd1234";


uint8_t Tx2_Buffer[] = "/* STM32F10xx I2C Firmware Library EEPROM driver example: \
                        buffer 2 transfer into address sEE_WRITE_ADDRESS2 */";
uint8_t Rx1_Buffer[BUFFER_SIZE1], Rx2_Buffer[BUFFER_SIZE2];
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
volatile uint16_t NumDataRead = 0;

/* Private functions ---------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);


USART_InitTypeDef USART_InitStructure;

/* Private function prototypes -----------------------------------------------*/

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


void RCC_Configuration(void)
{
ErrorStatus HSEStartUpStatus;
  
 /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
//    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
   
  /* TIM2 clock enable */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
//  											|RCC_APB2Periph_AFIO  | RCC_APB2Periph_USART1, ENABLE);



}



#if 1

#define ClockSpeed   100000
//#define ClockSpeed   400000
#define SLAVE_ADDRESS   0x70

/**
  * @brief  Configures I2C1
  * @param  None
  * @retval : None
  */
void Master_Configuration(void)
{
    I2C_InitTypeDef  I2C_InitStructure;

    I2C_DeInit(I2C1);
    /* I2C1 Init */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x30;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_ITConfig(I2C1, I2C_IT_ERR , ENABLE);
}

/**
  * @brief  Configures I2C2.
  * @param  None
  * @retval : None
  */
void Slave_Configuration(void)
{
    I2C_InitTypeDef  I2C_InitStructure;

    I2C_DeInit(I2C2);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = SLAVE_ADDRESS;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
    /* I2C2 Init */
    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_ITConfig(I2C2, I2C_IT_ERR, ENABLE);
    I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
}

#endif


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */

#if 1
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     
     
  GPIO_InitTypeDef GPIO_InitStructure;


//   RCC_Configuration();


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);

  /* Configure USARTy Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  // GPIO_Pin_10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USARTy Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

#if 1

  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

//  STM_EVAL_COMInit(COM1, &USART_InitStructure);


  /* Configure USART1 */
  USART_Init(USART1, &USART_InitStructure);

//  USART1->BRR = (uint16_t)0x271;  // by joshua

  
  /* Enable the USART1 */
  USART_Cmd(USART1, ENABLE);
		    USART_SendData(USART1, 0x31);

#if 0
while(1){
    USART_SendData(USART1, 0x32);
    /* Loop until USARTy DR register is empty */ 
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
}

  /* Output a message on Hyperterminal using printf function */
  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
#endif

#endif

#if 1

Master_Configuration();
//Slave_Configuration();

//while(1);

#endif

  /* Initialize the I2C EEPROM driver ----------------------------------------*/
  sEE_Init();  

//while(1);

  /* First write in the memory followed by a read of the written data --------*/
  /* Write on I2C EEPROM from sEE_WRITE_ADDRESS1 */
  sEE_WriteBuffer(Tx1_Buffer, sEE_WRITE_ADDRESS1, BUFFER_SIZE1);
 // sEE_WriteBuffer("0x31", sEE_WRITE_ADDRESS1, 1);

//while(1);

  /* Set the Number of data to be read */
  NumDataRead = BUFFER_SIZE1;
//NumDataRead=1;
  
  /* Read from I2C EEPROM from sEE_READ_ADDRESS1 */
  sEE_ReadBuffer(Rx1_Buffer, sEE_READ_ADDRESS1,(uint16_t *)(&NumDataRead)); 

//while(1);

  /* Wait till DMA transfer is complete (Transfer complete interrupt handler 
    resets the variable holding the number of data to be read) */
  //while (NumDataRead > 0)
  {    
    /* Starting from this point, if the requested number of data is higher than 1, 
       then only the DMA is managing the data transfer. Meanwhile, CPU is free to 
       perform other tasks:
       
      // Add your code here: 
      //...
      //...
  
       For simplicity reasons, this example is just waiting till the end of the 
       transfer. */    
  }
  
  /* Check if the data written to the memory is read correctly */
  TransferStatus1 = Buffercmp(Tx1_Buffer, Rx1_Buffer, BUFFER_SIZE1);
  /* TransferStatus1 = PASSED, if the transmitted and received data 
     to/from the EEPROM are the same */
  /* TransferStatus1 = FAILED, if the transmitted and received data 
     to/from the EEPROM are different */

	if(TransferStatus1 == PASSED){
		int i;
		for(i=0;i<10;i++)
		{
		    USART_SendData(USART1, 0x31);
		    /* Loop until USARTy DR register is empty */ 
		    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
		}
	}
	else{
		int i;
		for(i=0;i<10;i++)
		{
		    USART_SendData(USART1, 0x30);
		    /* Loop until USARTy DR register is empty */ 
		    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
		}
	}





#if 0

  
  /* Second write in the memory followed by a read of the written data -------*/
  /* Write on I2C EEPROM from sEE_WRITE_ADDRESS2 */
  sEE_WriteBuffer(Tx2_Buffer, sEE_WRITE_ADDRESS2, BUFFER_SIZE2);

  /* Set the Number of data to be read */
  NumDataRead = BUFFER_SIZE2;  
  
  /* Read from I2C EEPROM from sEE_READ_ADDRESS2 */
  sEE_ReadBuffer(Rx2_Buffer, sEE_READ_ADDRESS2, (uint16_t *)(&NumDataRead));

  
  /* Wait till DMA transfer is complete (Transfer complete interrupt handler 
    resets the variable holding the number of data to be read) */
  while (NumDataRead > 0)
  {
    /* Starting from this point, if the requested number of data is higher than 1, 
       then only the DMA is managing the data transfer. Meanwhile, CPU is free to 
       perform other tasks:
       
      // Add your code here: 
      //...
      //...
  
       For simplicity reasons, this example is just waiting till the end of the 
       transfer. */    
  }
  
  /* Check if the data written to the memory is read correctly */
  TransferStatus2 = Buffercmp(Tx2_Buffer, Rx2_Buffer, BUFFER_SIZE2);
  /* TransferStatus2 = PASSED, if the transmitted and received data 
     to/from the EEPROM are the same */
  /* TransferStatus2 = FAILED, if the transmitted and received data 
     to/from the EEPROM are different */


	if(TransferStatus1 == PASSED){
		int i;
		for(i=0;i<10;i++)
		{
		    USART_SendData(USART1, 0x33);
		    /* Loop until USARTy DR register is empty */ 
		    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
		}
	}
	else{
		int i;
		for(i=0;i<10;i++)
		{
		    USART_SendData(USART1, 0x34);
		    /* Loop until USARTy DR register is empty */ 
		    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
		}
	}

#endif
  
  /* Free all used resources */
  sEE_DeInit();


  while (1)
  {
  }
}



#else

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     

#ifdef ENABLE_LCD_MSG_DISPLAY
  /* Initialize the LCD screen for information display */
 #ifdef USE_STM3210E_EVAL
  STM3210E_LCD_Init();
 #elif defined(USE_STM3210B_EVAL)
  STM3210B_LCD_Init();
 #elif defined(USE_STM3210C_EVAL)
  STM3210C_LCD_Init();
 #elif defined(USE_STM32100B_EVAL)
  STM32100B_LCD_Init();
 #elif defined(USE_STM32100E_EVAL)
  STM32100E_LCD_Init();
 #endif /* USE_STM3210E_EVAL */  
  
  /* Display application information */
/*  LCD_Clear(LCD_COLOR_BLUE);  
  LCD_SetBackColor(LCD_COLOR_BLUE);
  LCD_SetTextColor(LCD_COLOR_WHITE);
  LCD_DisplayStringLine(LCD_LINE_0, "SMT32F1xx FW Library");
  LCD_DisplayStringLine(LCD_LINE_1, "   EEPROM Example   ");*/
#endif /* ENABLE_LCD_MSG_DISPLAY */    
  
  /* Initialize the I2C EEPROM driver ----------------------------------------*/
  sEE_Init();  

  /* First write in the memory followed by a read of the written data --------*/
  /* Write on I2C EEPROM from sEE_WRITE_ADDRESS1 */
  sEE_WriteBuffer(Tx1_Buffer, sEE_WRITE_ADDRESS1, BUFFER_SIZE1);

  /* Set the Number of data to be read */
  NumDataRead = BUFFER_SIZE1;
  
  /* Read from I2C EEPROM from sEE_READ_ADDRESS1 */
  sEE_ReadBuffer(Rx1_Buffer, sEE_READ_ADDRESS1, (uint16_t *)(&NumDataRead)); 

#ifdef ENABLE_LCD_MSG_DISPLAY  
/  LCD_DisplayStringLine(LCD_LINE_3, " Transfer 1 Ongoing ");
#endif /* ENABLE_LCD_MSG_DISPLAY */   

  /* Wait till DMA transfer is complete (Transfer complete interrupt handler 
    resets the variable holding the number of data to be read) */
  //while (NumDataRead > 0)
  {    
    /* Starting from this point, if the requested number of data is higher than 1, 
       then only the DMA is managing the data transfer. Meanwhile, CPU is free to 
       perform other tasks:
       
      // Add your code here: 
      //...
      //...
  
       For simplicity reasons, this example is just waiting till the end of the 
       transfer. */    
  }
  
  /* Check if the data written to the memory is read correctly */
  TransferStatus1 = Buffercmp(Tx1_Buffer, Rx1_Buffer, BUFFER_SIZE1);
  /* TransferStatus1 = PASSED, if the transmitted and received data 
     to/from the EEPROM are the same */
  /* TransferStatus1 = FAILED, if the transmitted and received data 
     to/from the EEPROM are different */
#ifdef ENABLE_LCD_MSG_DISPLAY  
 /* if (TransferStatus1 == PASSED)
  {
    LCD_DisplayStringLine(LCD_LINE_3, " Transfer 1 PASSED  ");
  }
  else
  {
    LCD_DisplayStringLine(LCD_LINE_3, " Transfer 1 FAILED  ");
  }  */
#endif /* ENABLE_LCD_MSG_DISPLAY */

/*----------------------------------
  
                                    ------------------------------------------*/
  
  /* Second write in the memory followed by a read of the written data -------*/
  /* Write on I2C EEPROM from sEE_WRITE_ADDRESS2 */
  sEE_WriteBuffer(Tx2_Buffer, sEE_WRITE_ADDRESS2, BUFFER_SIZE2);

  /* Set the Number of data to be read */
  NumDataRead = BUFFER_SIZE2;  
  
  /* Read from I2C EEPROM from sEE_READ_ADDRESS2 */
  sEE_ReadBuffer(Rx2_Buffer, sEE_READ_ADDRESS2, (uint16_t *)(&NumDataRead));

#ifdef ENABLE_LCD_MSG_DISPLAY   
  LCD_DisplayStringLine(LCD_LINE_5, " Transfer 2 Ongoing ");
#endif /* ENABLE_LCD_MSG_DISPLAY */  
  
  /* Wait till DMA transfer is complete (Transfer complete interrupt handler 
    resets the variable holding the number of data to be read) */
  while (NumDataRead > 0)
  {
    /* Starting from this point, if the requested number of data is higher than 1, 
       then only the DMA is managing the data transfer. Meanwhile, CPU is free to 
       perform other tasks:
       
      // Add your code here: 
      //...
      //...
  
       For simplicity reasons, this example is just waiting till the end of the 
       transfer. */    
  }
  
  /* Check if the data written to the memory is read correctly */
  TransferStatus2 = Buffercmp(Tx2_Buffer, Rx2_Buffer, BUFFER_SIZE2);
  /* TransferStatus2 = PASSED, if the transmitted and received data 
     to/from the EEPROM are the same */
  /* TransferStatus2 = FAILED, if the transmitted and received data 
     to/from the EEPROM are different */
#ifdef ENABLE_LCD_MSG_DISPLAY   
  if (TransferStatus1 == PASSED)
  {
    LCD_DisplayStringLine(LCD_LINE_5, " Transfer 2 PASSED  ");
  }
  else
  {
    LCD_DisplayStringLine(LCD_LINE_5, " Transfer 2 FAILED  ");
  }  
#endif /* ENABLE_LCD_MSG_DISPLAY */
  
  /* Free all used resources */
  sEE_DeInit();

#ifdef ENABLE_LCD_MSG_DISPLAY
  /* Display end of example information */
  LCD_DisplayStringLine(LCD_LINE_7, "---End Of Example---");
#endif /* ENABLE_LCD_MSG_DISPLAY */  

  while (1)
  {
  }
}


#endif


#ifndef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Example of timeout situation management.
  * @param  None.
  * @retval None.
  */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* Use application may try to recover the communication by resetting I2C
    peripheral (calling the function I2C_SoftwareResetCmd()) then re-start
    the transmission/reception from a previously stored recover point.
    For simplicity reasons, this example only shows a basic way for errors 
    managements which consists of stopping all the process and requiring system
    reset. */

#if 0  
#ifdef ENABLE_LCD_MSG_DISPLAY   
  /* Display error message on screen */
  LCD_Clear(LCD_COLOR_RED);  
  LCD_DisplayStringLine(LCD_LINE_4, "Communication ERROR!");
  LCD_DisplayStringLine(LCD_LINE_5, "Try again after res-");
  LCD_DisplayStringLine(LCD_LINE_6, "  etting the Board  ");
#endif /* ENABLE_LCD_MSG_DISPLAY */
#endif
  
  /* Block communication and all processes */

  while (1)
  {   
  }  
}

#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
