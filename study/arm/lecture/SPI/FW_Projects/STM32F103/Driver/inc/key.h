#ifndef __KEY_H
#define __KEY_H

#define RCC_KEY						RCC_APB2Periph_GPIOC
#define GPIO_KEY_PORT			GPIOC    
#define GPIO_KEY1						GPIO_Pin_1
#define GPIO_KEY2						GPIO_Pin_13

#define  NOKEY  0
#define  KEY1   1
#define  KEY2   2
#define  KEY3   3
#define  KEY4   4

u8 ReadKeyDown(void);
void GPIO_KEY_Config(void);
		 
#endif
