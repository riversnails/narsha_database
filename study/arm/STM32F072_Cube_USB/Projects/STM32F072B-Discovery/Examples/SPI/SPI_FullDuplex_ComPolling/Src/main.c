/**
  ******************************************************************************
  * @file    SPI/SPI_FullDuplex_ComPolling/Src/main.c
  * @author  MCD Application Team
  * @brief   This sample code shows how to use STM32F0xx SPI HAL API to transmit
  *          and receive a data buffer with a communication process based on
  *          Polling transfer.
  *          The communication is done using 2 Boards.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "pn532.h"
#include<stdio.h>

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @addtogroup SPI_FullDuplex_ComPolling
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Uncomment this line to use the board as master, if not it is used as slave */
//#define MASTER_BOARD

/* Private variables ---------------------------------------------------------*/
/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;
PN532 pn532;


/* Buffer used for transmission */
uint8_t aTxBuffer[] = "****SPI - Two Boards communication based on Polling **** SPI Message ******** SPI Message ******** SPI Message ****";

/* Buffer used for reception */
uint8_t aRxBuffer[BUFFERSIZE];

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Timeout_Error_Handler(void);
static uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/


//	#define SS_Pin GPIO_PIN_3
//	#define SS_GPIO_Port GPIOA

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
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
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


int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  
  return ch;
}



/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F0xx HAL library initialization:
       - Configure the Flash prefetch
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

  /* Configure LED3, LED4 and LED5 */
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  BSP_LED_Init(LED5);
   BSP_LED_Init(LED6); 



   
  /* Configure the system clock to 48 MHz */
  SystemClock_Config();

   MX_GPIO_Init();
   
  //-------------------------------------------------------
  //MX_USART2_UART_Init();
  MX_USART1_UART_Init();  
//	  int ch = 0x31;
//	  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  printf("haha\r\n");

 //-------------------------------------------------------



//	
//	  /*##-1- Configure the SPI peripheral #######################################*/
//	  /* Set the SPI parameters */
//	  SpiHandle.Instance               = SPIx;
//	
//	  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
//	  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
//	  SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
//	  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
//	  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
//	  SpiHandle.Init.CRCPolynomial     = 7;
//	  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
//	  //SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
//	  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_LSB;
//	  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
//	  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
//	  SpiHandle.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
//	  SpiHandle.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
//	
//	
//	
//	  SpiHandle.Init.Mode = SPI_MODE_MASTER;
//	
//	  
//	//	
//	//	#ifdef MASTER_BOARD
//	//	  SpiHandle.Init.Mode = SPI_MODE_MASTER;
//	//	BSP_LED_On(LED3);
//	//	BSP_LED_On(LED4);
//	//	BSP_LED_On(LED5);
//	//	BSP_LED_On(LED6);
//	//	#else
//	//	  SpiHandle.Init.Mode = SPI_MODE_SLAVE;
//	//	
//	//	BSP_LED_On(LED5);
//	//	BSP_LED_On(LED6);
//	//	
//	//	#endif /* MASTER_BOARD */
//	
//	
//	  if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
//	  {
//	    /* Initialization Error */
//	    Error_Handler();
//	  }
//	

  PN532 pn532;
  PN532_Init(&pn532);
  BSP_LED_On(LED4);
  //PN532_GetFirmwareVersion(&pn532, buff);
  if (PN532_GetFirmwareVersion(&pn532, buff) == PN532_STATUS_OK) {

	BSP_LED_On(LED5);	
	
    printf("Found PN532 with firmware version: %d.%d\r\n", buff[1], buff[2]);

//		for(int i=0;i<5;i++) HIDbuffer[2+i] = hello_array[i];	
//		 USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//		 HAL_Delay(30);
//		 for(int i=0;i<6;i++) HIDbuffer[2 + i] = 0;
//		USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//		 HAL_Delay(30);	


	
  }
  else
  {

	    while (1)
	  {
	    BSP_LED_Toggle(LED3);
	    HAL_Delay(100);
	  }

	
  	//printf("Error : Finding PN532 \r\n");


	
	
//		for(int i=0;i<5;i++) HIDbuffer[2+i] = fail[i];	
//		 USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//		 HAL_Delay(30);
//		 for(int i=0;i<6;i++) HIDbuffer[2 + i] = 0;
//		USBD_HID_SendReport(&USBD_Device, HIDbuffer, 8);
//		 HAL_Delay(30);	
  }
  PN532_SamConfiguration(&pn532);
  printf("Waiting for RFID/NFC card...\r\n");
  
  
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // Check if a card is available to read
    uid_len = PN532_ReadPassiveTarget(&pn532, uid, PN532_MIFARE_ISO14443A, 1000);
    if (uid_len == PN532_STATUS_ERROR) {
      printf(".");
    } else {
      printf("Found card with UID: ");
      for (uint8_t i = 0; i < uid_len; i++) {
        printf("%02x ", uid[i]);
      }
      printf("\r\n");
      break;
    }
  }

  
//////////////////////////////////////////////////////////////
// for Mifare

  #if 0
  printf("Reading blocks...\r\n");
  for (uint8_t block_number = 0; block_number < 64; block_number++) {
    pn532_error = PN532_MifareClassicAuthenticateBlock(&pn532, uid, uid_len,
      block_number, MIFARE_CMD_AUTH_A, key_a);
    if (pn532_error != PN532_ERROR_NONE) {
      break;
    }
    pn532_error = PN532_MifareClassicReadBlock(&pn532, buff, block_number);
    if (pn532_error != PN532_ERROR_NONE) {
      break;
    }
    printf("%d: ", block_number);
    for (uint8_t i = 0; i < 16; i++) {
      printf("%02x ", buff[i]);
    }
    printf("\r\n");
  }
  if (pn532_error) {
    printf("Error: 0x%02x\r\n", pn532_error);
  }
#endif

//////////////////////////////////////////////////////////////
// for Ntag2xx

  #if 1
  printf("Reading blocks...\r\n");
  for (uint8_t block_number = 0; block_number < 135; block_number++) {
    pn532_error = PN532_Ntag2xxReadBlock(&pn532, buff, block_number);
    if (pn532_error != PN532_ERROR_NONE) {
      break;
    }
    printf("%d: ", block_number);
    for (uint8_t i = 0; i < 4; i++) {
      printf("%02x ", buff[i]);
    }
    printf("\r\n");
  }
  if (pn532_error) {
    printf("Error: 0x%02x\r\n", pn532_error);
  }
#endif

//////////////////////////////////////////////////////////////




while(1);


#ifdef MASTER_BOARD
  /* Configure push button */
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);
  /* Wait for Button press before starting the Communication */
  while (BSP_PB_GetState(BUTTON_USER) != GPIO_PIN_SET)
  {
    BSP_LED_Toggle(LED3);
    HAL_Delay(100);
  }
  BSP_LED_Off(LED3);
#endif /* MASTER_BOARD */

  /*##-2- Start the Full Duplex Communication process ########################*/  
  /* While the SPI in TransmitReceive process, user can transmit data through 
     "aTxBuffer" buffer & receive data through "aRxBuffer" */
  /* Timeout is set to 5S */
  
  switch(HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)aTxBuffer, (uint8_t *)aRxBuffer, BUFFERSIZE, 5000))
  {
    case HAL_OK:
      /* Communication is completed ___________________________________________ */
      /* Compare the sent and received buffers */
      if (Buffercmp((uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, BUFFERSIZE))
      {
        /* Transfer error in transmission process */
        Error_Handler();
      }
      /* Turn LED3 on: Transfer in transmission process is correct */
      BSP_LED_On(LED3);
      /* Turn LED4 on: Transfer in reception process is correct */
      BSP_LED_On(LED4);
      break;

    case HAL_TIMEOUT:
      /* A Timeout Occur ______________________________________________________*/
      /* Call Timeout Handler */
      Timeout_Error_Handler();
      break;
      /* An Error Occur ______________________________________________________ */
    case HAL_ERROR:
      /* Call Timeout Handler */
      Error_Handler();
      break;
    default:
      break;
  }

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED5 on */
  BSP_LED_On(LED5);
  while(1)
  {
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Timeout_Error_Handler(void)
{
  /* Toggle LED5 on */
  while(1)
  {
    BSP_LED_On(LED5);
    HAL_Delay(500);
    BSP_LED_Off(LED5);
    HAL_Delay(500);
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSI48)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 48000000
  *            PREDIV                         = 2
  *            PLLMUL                         = 2
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */

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



#if 1

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};

//#if defined (USE_USB_CLKSOURCE_CRSHSI48)
#if 1
	static RCC_CRSInitTypeDef RCC_CRSInitStruct = {0};
#endif

//#if defined (USE_USB_CLKSOURCE_CRSHSI48)
#if 1

  /* Enable HSI48 Oscillator to be used as system clock source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;

//=============================================
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
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



/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
