/*
 * (C) COPYRIGHT 2012 CRZ
 *
 * File Name : hw_config.c
 * Author    : POOH
 * Version   : V1.0
 * Date      : 04/12/2012
 */

/* includes */

#include "hw_config.h"

/* defines */

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* global variables */

/** @defgroup STM322xG_EVAL_LOW_LEVEL_Private_Variables
  * @{
  */ 
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {KEY1_BUTTON_GPIO_PORT, KEY2_BUTTON_GPIO_PORT}; 
const uint16_t BUTTON_PIN[BUTTONn] = {KEY1_BUTTON_PIN, KEY2_BUTTON_PIN}; 
const uint32_t BUTTON_CLK[BUTTONn] = {KEY1_BUTTON_GPIO_CLK, KEY2_BUTTON_GPIO_CLK};

const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {KEY1_BUTTON_EXTI_LINE, KEY2_BUTTON_EXTI_LINE};
const uint8_t BUTTON_PORT_SOURCE[BUTTONn] = {KEY1_BUTTON_EXTI_PORT_SOURCE, KEY2_BUTTON_EXTI_PORT_SOURCE};
const uint8_t BUTTON_PIN_SOURCE[BUTTONn] = {KEY1_BUTTON_EXTI_PIN_SOURCE, KEY2_BUTTON_EXTI_PIN_SOURCE}; 
const uint8_t BUTTON_IRQn[BUTTONn] = {KEY1_BUTTON_EXTI_IRQn, KEY2_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {EVAL_COM0, EVAL_COM1};
GPIO_TypeDef* COM_TX_PORT[COMn] = {EVAL_COM0_TX_GPIO_PORT, EVAL_COM1_TX_GPIO_PORT};
GPIO_TypeDef* COM_RX_PORT[COMn] = {EVAL_COM0_RX_GPIO_PORT, EVAL_COM1_RX_GPIO_PORT};
const uint32_t COM_USART_CLK[COMn] = {EVAL_COM0_CLK, EVAL_COM1_CLK};
const uint32_t COM_TX_PORT_CLK[COMn] = {EVAL_COM0_TX_GPIO_CLK, EVAL_COM1_TX_GPIO_CLK};
const uint32_t COM_RX_PORT_CLK[COMn] = {EVAL_COM0_RX_GPIO_CLK, EVAL_COM1_RX_GPIO_CLK};
const uint16_t COM_TX_PIN[COMn] = {EVAL_COM0_TX_PIN, EVAL_COM1_TX_PIN};
const uint16_t COM_RX_PIN[COMn] = {EVAL_COM0_RX_PIN, EVAL_COM1_RX_PIN};
const uint8_t COM_TX_PIN_SOURCE[COMn] = {EVAL_COM0_TX_SOURCE, EVAL_COM1_TX_SOURCE};
const uint8_t COM_RX_PIN_SOURCE[COMn] = {EVAL_COM0_RX_SOURCE, EVAL_COM1_RX_SOURCE};
const uint8_t COM_TX_AF[COMn] = {EVAL_COM0_TX_AF, EVAL_COM1_TX_AF};
const uint8_t COM_RX_AF[COMn] = {EVAL_COM0_RX_AF, EVAL_COM1_RX_AF};

/* functions */

/**
  * @brief  Configures LED GPIO.
  * @param  Led: Specifies the Led to be configured. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  * @retval None
  */
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* Enable the GPIO_LED Clock */
    RCC_AHB1PeriphClockCmd(GPIO_CLK[Led], ENABLE);

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

/**
  * @brief  Turns selected LED On.
  * @param  Led: Specifies the Led to be set on. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  * @retval None
  */
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
    GPIO_PORT[Led]->BSRRL = GPIO_PIN[Led];
}

/**
  * @brief  Turns selected LED Off.
  * @param  Led: Specifies the Led to be set off. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  * @retval None
  */
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
    GPIO_PORT[Led]->BSRRH = GPIO_PIN[Led];  
}

/**
  * @brief  Toggles the selected LED.
  * @param  Led: Specifies the Led to be toggled. 
  *   This parameter can be one of following parameters:
  *     @arg LED1
  *     @arg LED2
  *     @arg LED3
  * @retval None
  */
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
    GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}

/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_KEY1: Key 1 Push Button 
  *     @arg BUTTON_KEY2: Key 2 Push Button 
  * @param  Button_Mode: Specifies Button mode.
  *   This parameter can be one of following parameters:   
  *     @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *     @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                     generation capability  
  * @retval None
  */
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the BUTTON Clock */
    RCC_AHB1PeriphClockCmd(BUTTON_CLK[Button], ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* Configure Button pin as input */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
    GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

    if (Button_Mode == BUTTON_MODE_EXTI)
    {
        /* Connect Button EXTI Line to Button GPIO Pin */
        SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

        /* Configure Button EXTI line */
        EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        /* Enable and set Button EXTI Interrupt to the lowest priority */
        NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

        NVIC_Init(&NVIC_InitStructure); 
    }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *   This parameter can be one of following parameters:    
  *     @arg BUTTON_KEY1: Key 1 Push Button 
  *     @arg BUTTON_KEY2: Key 2 Push Button 
  * @retval The Button GPIO pin value.
  */
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
  return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/**
  * @brief  Configures COM port.
  * @param  COM: Specifies the COM port to be configured.
  *   This parameter can be one of following parameters:    
  *     @arg COM1
  *     @arg COM2  
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *   contains the configuration information for the specified USART peripheral.
  * @retval None
  */
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

    if(COM == COM0)
    {
        /* Enable UART clock */
        RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
    }
    else if(COM == COM1)
    {
        /* Enable UART clock */
        RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
    }

    /* Connect PXx to USARTx_Tx*/
    GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

    /* Connect PXx to USARTx_Rx*/
    GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

    /* Configure USART Tx as alternate function  */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    /* Configure USART Rx as alternate function  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(COM_USART[COM], USART_InitStruct);

    /* Enable USART */
    USART_Cmd(COM_USART[COM], ENABLE);
}

/*
 * Name : USART_Debug_Initialization
 */
void USART_Debug_Initialization(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USARTx configured as follow:
          - BaudRate = 115200 baud  
          - Word Length = 8 Bits
          - One Stop Bit
          - No parity
          - Hardware flow control disabled (RTS and CTS signals)
          - Receive and transmit enabled
    */
    USART_InitStructure.USART_BaudRate   = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits   = USART_StopBits_1;
    USART_InitStructure.USART_Parity     = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
    
    STM_EVAL_COMInit(COM0, &USART_InitStructure);
}

/*
 * Name : LED_Initialization
 */
void LED_Initialization(void)
{
    STM_EVAL_LEDInit(LED1);
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);

    DevCtrl_LED_Off(LED1);
    DevCtrl_LED_Off(LED2);
    DevCtrl_LED_Off(LED3);
}

void DevCtrl_LED_On(Led_TypeDef Led) { STM_EVAL_LEDOff(Led); }
void DevCtrl_LED_Off(Led_TypeDef Led) { STM_EVAL_LEDOn(Led); }
void DevCtrl_LED_Toggle(Led_TypeDef Led) { STM_EVAL_LEDToggle(Led); }

/*
 * Name : Key_Initialization
 */
void Key_Initialization(void)
{
    STM_EVAL_PBInit(BUTTON_KEY1, BUTTON_MODE_EXTI);
    STM_EVAL_PBInit(BUTTON_KEY2, BUTTON_MODE_EXTI);
}

/*
 * Name : SysTick_Initialization
 */
void SysTick_Initialization(void)
{
    RCC_ClocksTypeDef RCC_Clocks;

    /* Configure Systick clock source as HCLK */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    
    /* SystTick configuration: an interrupt every 10ms */
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);  
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
    /* Write a character to the USART */  
    if( ch == '\n') {
        USART_SendData(EVAL_COM0, '\r');
        /* Loop until the end of transmission */
        while (USART_GetFlagStatus(EVAL_COM0, USART_FLAG_TC) == RESET){}
        USART_SendData(EVAL_COM0, '\n');
    }else {
        /* Place your implementation of fputc here */
        /* e.g. write a character to the USART */
        USART_SendData(EVAL_COM0, (uint8_t) ch);
    }

    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(EVAL_COM0, USART_FLAG_TC) == RESET){}

    return ch;
}

uint8_t DebugUart_GetChar(void)
{
    uint8_t data;

    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(EVAL_COM0, USART_FLAG_RXNE) == RESET);

    /* Write a character to the USART */
    data = USART_ReceiveData(EVAL_COM0);

    USART_SendData(EVAL_COM0, data);
    while(USART_GetFlagStatus(EVAL_COM0, USART_FLAG_TXE) == RESET);

    if( data == '\r' )  return (int)('\n');
    else                return(data);
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

  printf("ERROR !! %s - File:%s, Line:%d\n", __func__, file, line);

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

