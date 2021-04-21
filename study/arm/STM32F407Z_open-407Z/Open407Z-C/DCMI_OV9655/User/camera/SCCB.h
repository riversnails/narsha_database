/*********************************************************************************************************
*
* File                : 24c02.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SCCB_H
#define __SCCB_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "config.h"
					  
/* Private define ------------------------------------------------------------*/
#define I2C_PAGESIZE	4

/**
 * @DCMI SCCB connected to I2C2
 */
 /* Configure I2C2 pins: PB10->SIOC and PB11->SIOD */ 
#define Open207I_SCCB                        	I2C2
#define Open207I_SCCB_CLK                    	RCC_APB1Periph_I2C2

#define Open207I_SCCB_SDA_PIN                 	GPIO_Pin_11
#define Open207I_SCCB_SDA_GPIO_PORT           	GPIOB
#define Open207I_SCCB_SDA_GPIO_CLK            	RCC_AHB1Periph_GPIOB
#define Open207I_SCCB_SDA_SOURCE              	GPIO_PinSource11
#define Open207I_SCCB_SDA_AF                  	GPIO_AF_I2C2

#define Open207I_SCCB_SCL_PIN                 	GPIO_Pin_10
#define Open207I_SCCB_SCL_GPIO_PORT           	GPIOB
#define Open207I_SCCB_SCL_GPIO_CLK            	RCC_AHB1Periph_GPIOB
#define Open207I_SCCB_SCL_SOURCE              	GPIO_PinSource10
#define Open207I_SCCB_SCL_AF                  	GPIO_AF_I2C2


/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SCCB_Open207I_FLAG_TIMEOUT         10000
#define OV9655_DEVICE_WRITE_ADDRESS    0x60
#define OV9655_DEVICE_READ_ADDRESS     0x61


#define SCCB_SPEED               100000
#define SCCB_SLAVE_ADDRESS7      0xFE

/* Private function prototypes -----------------------------------------------*/
void SCCB_GPIO_Config(void);
uint8_t DCMI_SingleRandomWrite(uint8_t Device, uint16_t Addr, uint8_t Data);
uint8_t DCMI_SingleRandomRead(uint8_t Device, uint16_t Addr,uint8_t *Data);

#endif 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
