/**
  ******************************************************************************
  * @file    USB_Device/HID_Standalone/Src/main.c
  * @author  MCD Application Team
  * @brief   USB device HID demo main file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "pn532.h"
#include<stdio.h>




/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USBD_HandleTypeDef USBD_Device;
SPI_HandleTypeDef hspi1;
PN532 pn532;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);

static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

/* Private functions ---------------------------------------------------------*/

#define USB_HID_MODIFIER_RIGHT_CTRL  0x10
#define USB_HID_MODIFIER_RIGHT_SHIFT 0x20
#define USB_HID_MODIFIER_RIGHT_ALT   0x40
#define USB_HID_MODIFIER_RIGHT_GUI   0x80

#define USB_HID_KEY_A     0x04
#define USB_HID_KEY_B     0x05
#define USB_HID_KEY_C     0x06
#define USB_HID_KEY_D     0x07
#define USB_HID_KEY_E     0x08
#define USB_HID_KEY_F     0x09
#define USB_HID_KEY_G     0x0A
#define USB_HID_KEY_H     0x0B
#define USB_HID_KEY_I     0x0C
#define USB_HID_KEY_J     0x0D
#define USB_HID_KEY_K     0x0E
#define USB_HID_KEY_L     0x0F
#define USB_HID_KEY_M     0x10
#define USB_HID_KEY_N     0x11
#define USB_HID_KEY_O     0x12
#define USB_HID_KEY_P     0x13
#define USB_HID_KEY_Q     0x14
#define USB_HID_KEY_R     0x15
#define USB_HID_KEY_S     0x16
#define USB_HID_KEY_T     0x17
#define USB_HID_KEY_U     0x18
#define USB_HID_KEY_V     0x19
#define USB_HID_KEY_W     0x1A
#define USB_HID_KEY_X     0x1B
#define USB_HID_KEY_Y     0x1C
#define USB_HID_KEY_Z     0x1D

int key_value1 = 0;
extern int key_interrupt;

#define KEY_A     0x04
#define KEY_F     0x09
#define KEY_I     0x0C
#define KEY_H     0x0B
#define KEY_E     0x08
#define KEY_L     0x0F
#define KEY_L     0x0F
#define KEY_O     0x12

char hello_array[5] = {KEY_H, KEY_E, KEY_L, KEY_L, KEY_O};

char fail[5] = {KEY_F, KEY_A, KEY_I, KEY_L, KEY_L};


UART_HandleTypeDef huart2;

#define __HAL_RCC_USART2_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);\
                                        /* Delay after an RCC peripheral clock enabling */\
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);\
                                        UNUSED(tmpreg); \
                                      } while(0U)
                                      
static void MX_USART2_UART_Init(void)
{
#if 1
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_USART2_CLK_ENABLE();
	
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
	
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    //Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}


UART_HandleTypeDef huart1;               
static void MX_USART1_UART_Init(void)
{
#if 1
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_USART1_CLK_ENABLE();
	
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
	
  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    //Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}


//	int fputc(int ch, FILE *f)
//	{
//	while(!(USART1->SR & (0x01 << 7) ) );
//	USART1->DR = (unsigned char)ch;
//	  	return ch;
//	}


int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  
  return ch;
}


/**
  * @brief  Main program
  * @param  Nonehelo
  * @retval None
  */

int count = 0;

int main(void)
{
  /* STM32F0xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user
             can eventually implement his proper time base source (a general purpose
             timer for example or other time source), keeping in mind that Time base
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
             handled in milliseconds basis.
       - Low Level Initialization
     */

  uint8_t buff[255];
  uint8_t uid[MIFARE_UID_MAX_LENGTH];
  uint8_t key_a[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  uint32_t pn532_error = PN532_ERROR_NONE;
  int32_t uid_len = 0;
  
  HAL_Init();

  /* Configure the system clock to get correspondent USB clock source */
  SystemClock_Config();

  /* Configure User button for remote wakeup */
//  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_EXTI);


  //-------------------------------------------------------

  MX_GPIO_Init();

  //-------------------------------------------------------
  //MX_USART2_UART_Init();
  MX_USART1_UART_Init();  
//	  int ch = 0x31;
//	  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  printf("haha1\r\n");
 //-------------------------------------------------------

//	   
//	
//	  
//	  //MX_SPI1_Init();
//	  
//	
//	  PN532 pn532;
//	  PN532_Init(&pn532);
//	  //PN532_GetFirmwareVersion(&pn532, buff);
//	  if (PN532_GetFirmwareVersion(&pn532, buff) == PN532_STATUS_OK) {
//	    printf("Found PN532 with firmware version: %d.%d\r\n", buff[1], buff[2]);
//	
//	//		for(int i=0;i<5;i++) HIDbuffer[2+i] = hello_array[i];	
//	//		 USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//	//		 HAL_Delay(30);
//	//		 for(int i=0;i<6;i++) HIDbuffer[2 + i] = 0;
//	//		USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//	//		 HAL_Delay(30);	
//		
//	  }
//	  else
//	  {
//	  	printf("Error : Finding PN532 \r\n");
//		
//		
//	//		for(int i=0;i<5;i++) HIDbuffer[2+i] = fail[i];	
//	//		 USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//	//		 HAL_Delay(30);
//	//		 for(int i=0;i<6;i++) HIDbuffer[2 + i] = 0;
//	//		USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//	//		 HAL_Delay(30);	
//	  }
//	  PN532_SamConfiguration(&pn532);
//	  printf("Waiting for RFID/NFC card...\r\n");
//	
//	 
//	  /* Infinite loop */
//	  /* USER CODE BEGIN WHILE */
//	  while (1)
//	  {
//	    /* USER CODE END WHILE */
//	
//	    /* USER CODE BEGIN 3 */
//	    // Check if a card is available to read
//	    uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 1000);
//	    if (uid_len == PN532_STATUS_ERROR) {
//	      printf(".");
//	    } else {
//	      printf("Found card with UID: ");
//	      for (uint8_t i = 0; i < uid_len; i++) {
//	        printf("%02x ", uid[i]);
//	      }
//	      printf("\r\n");
//	      break;
//	    }
//	  }
//	  printf("Reading blocks...\r\n");
//	  for (uint8_t block_number = 0; block_number < 64; block_number++) {
//	    pn532_error = PN532_MifareClassicAuthenticateBlock(&pn532, uid, uid_len,
//	      block_number, MIFARE_CMD_AUTH_A, key_a);
//	    if (pn532_error != PN532_ERROR_NONE) {
//	      break;
//	    }
//	    pn532_error = PN532_MifareClassicReadBlock(&pn532, buff, block_number);
//	    if (pn532_error != PN532_ERROR_NONE) {
//	      break;
//	    }
//	    printf("%d: ", block_number);
//	    for (uint8_t i = 0; i < 16; i++) {
//	      printf("%02x ", buff[i]);
//	    }
//	    printf("\r\n");
//	  }
//	  if (pn532_error) {
//	    printf("Error: 0x%02x\r\n", pn532_error);
//	  }
//	
//	
//	while(1);


  //-------------------------------------------------------
  #if 1
  
  /* Init Device Library */
  USBD_Init(&USBD_Device, &HID_Desc, 0);

  /* Register the HID class */
  USBD_RegisterClass(&USBD_Device, &USBD_HID);

  /* Start Device Process */
  USBD_Start(&USBD_Device);


//-------------------------------------------------------

uint8_t HIDbuffer[8] = {0}; 
// modifiers in 0, zero in 1, char code in 2-7
//uint8_t *buf;
//uint32_t USBDelay;
//buf = HIDbuffer;
//USBDelay = USBD_HID_GetPollingInterval(&USBD_Device);



//	  keyboardHID.modifiers = USB_HID_MODIFIER_LEFT_SHIFT;
//	
//	        keyboardHID.key1 = USB_HID_KEY_H;
//	
//	        USBD_HID_SendReport(&hUsbDeviceFS, &keyboardHID, sizeof(struct keyboardHID_t));
//	
//	        HAL_Delay(30);

 	HAL_Delay(1000);

//	 HIDbuffer[2] = 0x38;		
//	 USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//	 HAL_Delay(30);
//	 HIDbuffer[2] = 0;
//	USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);

	char haha[5] = {KEY_H, KEY_A, KEY_H, KEY_A, KEY_O};
	for(int i=0;i<5;i++) HIDbuffer[2+i] = haha[i];	
	 USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
	 HAL_Delay(30);
	 for(int i=0;i<6;i++) HIDbuffer[2 + i] = 0;
	USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
	 HAL_Delay(30);	
	 
	
#endif
  //==============================

  

  
while(1)
{
	if(key_interrupt == 1)
	{
		key_interrupt = 0;

		printf("count = %d \r\n",count++);
		
		
//			 HIDbuffer[2] = 0x04 + key_value1++;		
//			 USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//			 HAL_Delay(30);
//			 HIDbuffer[2] = 0;
//			USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//			HAL_Delay(30);
	 //HAL_Delay(1000);
	}
}

//==============================


  while (1)
  {
  }
}


#if 0
void uart_init()
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : SS_Pin */
  GPIO_InitStruct.Pin = SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SS_GPIO_Port, &GPIO_InitStruct);

  
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

// UART
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  

  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
}
#endif


//	#define SS_Pin GPIO_PIN_7
//	#define SS_GPIO_Port GPIOB

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  //HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_RESET);
  //HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : SS_Pin */
  GPIO_InitStruct.Pin = SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SS_GPIO_Port, &GPIO_InitStruct);

	GPIOB->ODR |= (0x01 << 7);

//////////////////////////////////////////
// for GPIO SPI
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**SPI1 GPIO Configuration    
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//////////////////////////////////////////


}

static void MX_SPI1_Init(void)
{
//		
//	  hspi1.Instance               = SPIx;
//		
//	  //hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
//	  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
//	  hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
//	  hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;
//	  hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;
//	  hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
//	  hspi1.Init.CRCPolynomial     = 7;
//	  hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;
//	  //hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
//	  hspi1.Init.FirstBit 		= SPI_FIRSTBIT_LSB;
//	  hspi1.Init.NSS               = SPI_NSS_SOFT;
//	  hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
//	  hspi1.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
//	  hspi1.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
//	  hspi1.Init.Mode = SPI_MODE_MASTER;
//		
//	  if(HAL_SPI_Init(&hspi1) != HAL_OK)
//	  {
//	    /* Initialization Error */
//	    Error_Handler();
//	  }


	
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  //hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  //hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;

  hspi1.Init.FirstBit = SPI_FIRSTBIT_LSB;
  //hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    //Error_Handler();
  }

  
}

//		
//	static void MX_SPI1_Init_back(void)
//	{
//	
//	  /* USER CODE BEGIN SPI1_Init 0 */
//	  
//	    GPIO_InitTypeDef GPIO_InitStruct = {0};
//	  
//	  
//	 /* USER CODE END SPI1_MspInit 0 */
//	    /* Peripheral clock enable */
//	    __HAL_RCC_SPI1_CLK_ENABLE();
//	  
//	    __HAL_RCC_GPIOA_CLK_ENABLE();
//	    /**SPI1 GPIO Configuration    
//	    PA5     ------> SPI1_SCK
//	    PA6     ------> SPI1_MISO
//	    PA7     ------> SPI1_MOSI 
//	    */
//	    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_7;
//	    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	
//	    GPIO_InitStruct.Pin = GPIO_PIN_6;
//	    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	    GPIO_InitStruct.Pull = GPIO_NOPULL;
//	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	
//		
//	
//	  /* USER CODE END SPI1_Init 0 */
//	
//	  /* USER CODE BEGIN SPI1_Init 1 */
//	
//	  /* USER CODE END SPI1_Init 1 */
//	  /* SPI1 parameter configuration*/
//	  hspi1.Instance = SPI1;
//	  hspi1.Init.Mode = SPI_MODE_MASTER;
//	  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
//	  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
//	  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
//	  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
//	  hspi1.Init.NSS = SPI_NSS_SOFT;
//	  //hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
//	  //hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
//	  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
//	  
//	  hspi1.Init.FirstBit = SPI_FIRSTBIT_LSB;
//	  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
//	  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//	  hspi1.Init.CRCPolynomial = 10;
//	  if (HAL_SPI_Init(&hspi1) != HAL_OK)
//	  {
//	    //Error_Handler();
//	  }
//	}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow:
  *
  *            HSI48 used as USB clock source (USE_USB_CLKSOURCE_CRSHSI48 defined in main.h)
  *              - System Clock source            = HSI48
  *              - SYSCLK(Hz)                     = 48000000
  *              - HCLK(Hz)                       = 48000000
  *              - AHB Prescaler                  = 1
  *              - APB1 Prescaler                 = 1
  *              - Flash Latency(WS)              = 1
  *
  *              - PLL(HSE) used as USB clock source (USE_USB_CLKSOURCE_PLL defined in main.h)
  *              - System Clock source            = PLL (HSE)
  *              - SYSCLK(Hz)                     = 48000000
  *              - HCLK(Hz)                       = 48000000
  *              - AHB Prescaler                  = 1
  *              - APB1 Prescaler                 = 1
  *              - HSE Frequency(Hz)              = 8000000
  *              - PREDIV                         = 1
  *              - PLLMUL                         = 6
  *              - Flash Latency(WS)              = 1
  *
  * @param  None
  * @retval None
  */

#if 1

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};

#if defined (USE_USB_CLKSOURCE_CRSHSI48)
  static RCC_CRSInitTypeDef RCC_CRSInitStruct = {0};
#endif

#if defined (USE_USB_CLKSOURCE_CRSHSI48)

  /* Enable HSI48 Oscillator to be used as system clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;

//=============================================
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
//	  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
//	  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
//=============================================
  
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select HSI48 as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Select HSI48 as system clock source and configure the HCLK and PCLK1
  clock dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
//=============================================
  //RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//=============================================	
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  /*Configure the clock recovery system (CRS)**********************************/

  /*Enable CRS Clock*/
  __HAL_RCC_CRS_CLK_ENABLE();

  /* Default Synchro Signal division factor (not divided) */
  RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;

  /* Set the SYNCSRC[1:0] bits according to CRS_Source value */
  RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;

  /* HSI48 is synchronized with USB SOF at 1KHz rate */
  RCC_CRSInitStruct.ReloadValue =  __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
  RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;

  /* Set the TRIM[5:0] to the default value*/
  RCC_CRSInitStruct.HSI48CalibrationValue = 0x20;

  /* Start automatic synchronization */
  HAL_RCCEx_CRSConfig (&RCC_CRSInitStruct);

#elif defined (USE_USB_CLKSOURCE_PLL)

  /* Enable HSE Oscillator and activate PLL with HSE as source
  PLLCLK = (8 * 6) / 1) = 48 MHz */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /*Select PLL 48 MHz output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* Select PLL as system clock source and configure the HCLK and PCLK1
  clock dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

#endif /*USE_USB_CLKSOURCE_CRSHSI48*/

  /* Enable Power Controller clock */
  __HAL_RCC_PWR_CLK_ENABLE();
}

#endif


#if 0

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Select HSI48 Oscillator as PLL source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    Error_Handler();
  }
}

#endif


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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
