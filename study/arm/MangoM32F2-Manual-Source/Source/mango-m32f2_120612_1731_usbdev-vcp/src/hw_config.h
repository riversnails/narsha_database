/*
 * (C) COPYRIGHT 2012 CRZ
 *
 * File Name : hw_config.h
 * Author    : POOH
 * Version   : V1.0
 * Date      : 04/12/2012
 */

#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* includes */

#include "stm32f2xx.h"
#include "stm32f2xx_conf.h"
#include <stdio.h>
#include "test.h"

#ifdef ETHERNET_TEST
#include "stm32f2x7_eth.h"
#include "main.h"
#include "netconf.h"
#include "httpd.h"
#endif

#ifdef USB_TEST
#include "usb_core.h"
#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#endif

/* defines */

typedef enum 
{
    LED1 = 0,
    LED2 = 1,
    LED3 = 2
} Led_TypeDef;

typedef enum 
{  
    BUTTON_KEY1 = 0,
    BUTTON_KEY2 = 1
} Button_TypeDef;

typedef enum 
{  
    BUTTON_MODE_GPIO = 0,
    BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum 
{
    COM0 = 0, /* UART1 */
    COM1 = 1  /* UART3 */
} COM_TypeDef;   

/** @addtogroup STM322xG_EVAL_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             3

#define LED1_PIN                         GPIO_Pin_13
#define LED1_GPIO_PORT                   GPIOE
#define LED1_GPIO_CLK                    RCC_AHB1Periph_GPIOE
  
#define LED2_PIN                         GPIO_Pin_14
#define LED2_GPIO_PORT                   GPIOE
#define LED2_GPIO_CLK                    RCC_AHB1Periph_GPIOE
  
#define LED3_PIN                         GPIO_Pin_15
#define LED3_GPIO_PORT                   GPIOE
#define LED3_GPIO_CLK                    RCC_AHB1Periph_GPIOE

/** @addtogroup STM322xG_EVAL_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          2

/**
 * @brief KEY1 push-button
 */
#define KEY1_BUTTON_PIN                GPIO_Pin_11
#define KEY1_BUTTON_GPIO_PORT          GPIOE
#define KEY1_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOE
#define KEY1_BUTTON_EXTI_LINE          EXTI_Line11
#define KEY1_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOE
#define KEY1_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource11
#define KEY1_BUTTON_EXTI_IRQn          EXTI15_10_IRQn 

/**
 * @brief KEY2 push-button
 */
#define KEY2_BUTTON_PIN                GPIO_Pin_12
#define KEY2_BUTTON_GPIO_PORT          GPIOE
#define KEY2_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOE
#define KEY2_BUTTON_EXTI_LINE          EXTI_Line12
#define KEY2_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOE
#define KEY2_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource12
#define KEY2_BUTTON_EXTI_IRQn          EXTI15_10_IRQn 

/** @addtogroup STM322xG_EVAL_LOW_LEVEL_COM
  * @{
  */
#define COMn                             2

/**
 * @brief Definition for COM port0, connected to USART1
 */ 
#define EVAL_COM0                        USART1
#define EVAL_COM0_CLK                    RCC_APB2Periph_USART1
#define EVAL_COM0_TX_PIN                 GPIO_Pin_6
#define EVAL_COM0_TX_GPIO_PORT           GPIOB
#define EVAL_COM0_TX_GPIO_CLK            RCC_AHB1Periph_GPIOB
#define EVAL_COM0_TX_SOURCE              GPIO_PinSource6
#define EVAL_COM0_TX_AF                  GPIO_AF_USART1
#define EVAL_COM0_RX_PIN                 GPIO_Pin_7
#define EVAL_COM0_RX_GPIO_PORT           GPIOB
#define EVAL_COM0_RX_GPIO_CLK            RCC_AHB1Periph_GPIOB
#define EVAL_COM0_RX_SOURCE              GPIO_PinSource7
#define EVAL_COM0_RX_AF                  GPIO_AF_USART1
#define EVAL_COM0_IRQn                   USART1_IRQn

/**
 * @brief Definition for COM port1, connected to USART3
 */ 
#define EVAL_COM1                        USART3
#define EVAL_COM1_CLK                    RCC_APB1Periph_USART3
#define EVAL_COM1_TX_PIN                 GPIO_Pin_10
#define EVAL_COM1_TX_GPIO_PORT           GPIOC
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_TX_AF                  GPIO_AF_USART3
#define EVAL_COM1_RX_PIN                 GPIO_Pin_11
#define EVAL_COM1_RX_GPIO_PORT           GPIOC
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource11
#define EVAL_COM1_RX_AF                  GPIO_AF_USART3
#define EVAL_COM1_IRQn                   USART3_IRQn

/* functions */

void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);

void USART_Debug_Initialization(void);
void LED_Initialization(void);
void Key_Initialization(void);
void SysTick_Initialization(void);

uint8_t DebugUart_GetChar(void);

void DevCtrl_LED_On(Led_TypeDef Led);
void DevCtrl_LED_Off(Led_TypeDef Led);
void DevCtrl_LED_Toggle(Led_TypeDef Led);

void Time_Update(void);
void Delay(uint32_t nCount);

#endif  /*__HW_CONFIG_H */

