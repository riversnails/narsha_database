#include "KEY&LED.h"

#define USER_KEY_Port					GPIOF
#define USER_KEY_Pin					GPIO_Pin_10
#define USER_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOF
/****************************************************************/
#define WAKEUP_KEY_Port					GPIOA
#define WAKEUP_KEY_Pin					GPIO_Pin_0
#define WAKEUP_KEY_RCC_AHBPeriph		RCC_AHB1Periph_GPIOA
/****************************************************************/
#define JOY_A_KEY_Port					GPIOC
#define JOY_A_KEY_Pin					GPIO_Pin_13
#define JOY_A_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOC

#define JOY_B_KEY_Port					GPIOE 
#define JOY_B_KEY_Pin					GPIO_Pin_6
#define JOY_B_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOE

#define JOY_C_KEY_Port					GPIOE
#define JOY_C_KEY_Pin					GPIO_Pin_4
#define JOY_C_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOE

#define JOY_D_KEY_Port					GPIOE
#define JOY_D_KEY_Pin					GPIO_Pin_2
#define JOY_D_KEY_RCC_AHBPeriph			RCC_AHB1Periph_GPIOE

#define JOY_CTR_KEY_Port				GPIOA
#define JOY_CTR_KEY_Pin					GPIO_Pin_5
#define JOY_CTR_KEY_RCC_AHBPeriph		RCC_AHB1Periph_GPIOA 
/****************************************************************/
#define LED1_Port						GPIOF
#define LED1_Pin						GPIO_Pin_6
#define LED1_RCC_AHBPeriph				RCC_AHB1Periph_GPIOF

#define LED2_Port						GPIOF
#define LED2_Pin						GPIO_Pin_7
#define LED2_RCC_AHBPeriph				RCC_AHB1Periph_GPIOF

#define LED3_Port						GPIOF
#define LED3_Pin						GPIO_Pin_8
#define LED3_RCC_AHBPeriph				RCC_AHB1Periph_GPIOF

#define LED4_Port						GPIOF
#define LED4_Pin						GPIO_Pin_9
#define LED4_RCC_AHBPeriph				RCC_AHB1Periph_GPIOF


static void Delay(__IO uint32_t nCount);

void JOYState_LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOF Periph clock enable */
	RCC_AHB1PeriphClockCmd(LED1_RCC_AHBPeriph | LED2_RCC_AHBPeriph | LED3_RCC_AHBPeriph | LED4_RCC_AHBPeriph, ENABLE);
	
	/* Configure PF6 PF7 PF8 PF9 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED1_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(LED1_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED2_Pin;
	GPIO_Init(LED2_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED3_Pin;
	GPIO_Init(LED3_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LED4_Pin;
	GPIO_Init(LED4_Port, &GPIO_InitStructure);
	
	/*--------------------------------------------------------------------------------
	
	Key GPIO Config
	
	--------------------------------------------------------------------------------*/
	/* Periph clock enable */
	RCC_AHB1PeriphClockCmd(USER_KEY_RCC_AHBPeriph | WAKEUP_KEY_RCC_AHBPeriph | JOY_A_KEY_RCC_AHBPeriph |
							 JOY_B_KEY_RCC_AHBPeriph |  JOY_C_KEY_RCC_AHBPeriph |  JOY_D_KEY_RCC_AHBPeriph |
							  JOY_CTR_KEY_RCC_AHBPeriph, ENABLE);
	
	/* Configure Input pushpull mode */
	GPIO_InitStructure.GPIO_Pin = USER_KEY_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(USER_KEY_Port, &GPIO_InitStructure);
	

	GPIO_InitStructure.GPIO_Pin = WAKEUP_KEY_Pin;
	GPIO_Init(WAKEUP_KEY_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = JOY_A_KEY_Pin;
	GPIO_Init(JOY_A_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_B_KEY_Pin;
	GPIO_Init(JOY_B_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_C_KEY_Pin;
	GPIO_Init(JOY_C_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_D_KEY_Pin;
	GPIO_Init(JOY_D_KEY_Port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = JOY_CTR_KEY_Pin;
	GPIO_Init(JOY_CTR_KEY_Port, &GPIO_InitStructure);
	
}

uint8_t Read_JOYState(void)
{
	if(!GPIO_ReadInputDataBit(USER_KEY_Port,USER_KEY_Pin))
		return 0;
	else if(!GPIO_ReadInputDataBit(WAKEUP_KEY_Port,WAKEUP_KEY_Pin))
		return 1;
	else if(!GPIO_ReadInputDataBit(JOY_A_KEY_Port,JOY_A_KEY_Pin))
		return 2;
	else if(!GPIO_ReadInputDataBit(JOY_B_KEY_Port,JOY_B_KEY_Pin))
		return 3;
	else if(!GPIO_ReadInputDataBit(JOY_C_KEY_Port,JOY_C_KEY_Pin))
		return 4;
	else if(!GPIO_ReadInputDataBit(JOY_D_KEY_Port,JOY_D_KEY_Pin))
		return 5;
	else if(!GPIO_ReadInputDataBit(JOY_CTR_KEY_Port,JOY_CTR_KEY_Pin))
		return 6;
	return 7;
}

void Led_Toggle(uint8_t keyvulua)
{

		

	switch(keyvulua)
		{
			case 0:
				GPIO_SetBits(LED4_Port , LED4_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED4_Port , LED4_Pin);
				Delay(0x1fffff);
				break;
			case 1:
				GPIO_SetBits(LED1_Port , LED1_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED1_Port , LED1_Pin);
				Delay(0x1fffff);
				break;
			case 2:
			  	GPIO_SetBits(LED2_Port , LED2_Pin);
			  	Delay(0x1fffff);
			  	GPIO_ResetBits(LED2_Port , LED2_Pin);
			  	Delay(0x1fffff);
				break;
			case 3:
				GPIO_SetBits(LED3_Port , LED3_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED3_Port , LED3_Pin);
				Delay(0x1fffff);
				break;
			case 4:
				GPIO_SetBits(LED1_Port , LED1_Pin);
			  	GPIO_SetBits(LED2_Port , LED2_Pin);
			  	GPIO_SetBits(LED3_Port , LED3_Pin);
			  	GPIO_SetBits(LED4_Port , LED4_Pin);
			  	Delay(0x1fffff);
			  	GPIO_ResetBits(LED1_Port , LED1_Pin);
			  	GPIO_ResetBits(LED2_Port , LED2_Pin);
			  	GPIO_ResetBits(LED3_Port , LED3_Pin);
			  	GPIO_ResetBits(LED4_Port , LED4_Pin);
			  	Delay(0x1fffff);
				break;
			case 5:
				GPIO_SetBits(LED1_Port , LED1_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED1_Port , LED1_Pin);
				
				GPIO_SetBits(LED2_Port , LED2_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED2_Port , LED2_Pin);
				
				GPIO_SetBits(LED3_Port , LED3_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED3_Port , LED3_Pin);
				
				GPIO_SetBits(LED4_Port , LED4_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED4_Port , LED4_Pin);
				break;
			case 6:
				GPIO_SetBits(LED4_Port , LED4_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED4_Port , LED4_Pin);
				
				GPIO_SetBits(LED3_Port , LED3_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED3_Port , LED3_Pin);
				
				GPIO_SetBits(LED2_Port , LED2_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED2_Port , LED2_Pin);
				
				GPIO_SetBits(LED1_Port , LED1_Pin);
				Delay(0x1fffff);
				GPIO_ResetBits(LED1_Port , LED1_Pin);
				break;
			case 7:
				break;
		}
		
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
static void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
