#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include "stm32f4xx.h"

/**
 * @brief Definition for connected to I2C1
 */
#define Open_I2C                        	I2C2
#define Open_I2C_CLK                    	RCC_APB1Periph_I2C2

#define Open_I2C_SDA_PIN                 	GPIO_Pin_11
#define Open_I2C_SDA_GPIO_PORT           	GPIOB
#define Open_I2C_SDA_GPIO_CLK            	RCC_AHB1Periph_GPIOB
#define Open_I2C_SDA_SOURCE              	GPIO_PinSource11
#define Open_I2C_SDA_AF                  	GPIO_AF_I2C2

#define Open_I2C_SCL_PIN                 	GPIO_Pin_10
#define Open_I2C_SCL_GPIO_PORT           	GPIOB
#define Open_I2C_SCL_GPIO_CLK            	RCC_AHB1Periph_GPIOB
#define Open_I2C_SCL_SOURCE              	GPIO_PinSource10
#define Open_I2C_SCL_AF                  	GPIO_AF_I2C2

#define I2C_SPEED               100000
#define I2C_SLAVE_ADDRESS7      0x30

#endif
