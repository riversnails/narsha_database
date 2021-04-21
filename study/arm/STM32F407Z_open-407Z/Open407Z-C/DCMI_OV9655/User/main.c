/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
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
#include "stm32f4xx.h"
#include "usart.h"
#include "LCD/LCD.h"
#include "camera/dcmi_OV9655.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup IOToggle
  * @{
  */ 
void LCD_CS_ResetBits(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
static __IO uint32_t TimingDelay;

uint32_t	fps=0;
uint32_t	disfps=0;
uint32_t	fpsbuff[5];
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
	OV9655_IDTypeDef OV9655ID;
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f2xx.c file
     */

	USART_Configuration();
	//USART_NVIC_Config();

	LCD_Initializtion();
	LCD_Clear(Red);
	GUI_Text(50,70,"Camera Init..",White,Red);

	printf("\r\n\r\nWelcome to WaveShare STM32F2 series MCU Board Open207I\r\n");
	printf("OV9655 Init..\r\n");
	if(DCMI_OV9655Config()==0){
		printf("Camera Have Init..\r\n");
		GUI_Text(50,90,"Camera Have Init..",White,Red);
		}
	else {
		printf("OV9655 Init fails!!\r\n");
		GUI_Text(50,90,"OV9655 Init fails!!",White,Red);
		}

	DCMI_OV9655_ReadID(&OV9655ID);
	printf("OV9655 ID:0x%x 0x%x 0x%x 0x%x\r\n",OV9655ID.Manufacturer_ID1, OV9655ID.Manufacturer_ID2, OV9655ID.PID, OV9655ID.Version);
	
			
	LCD_WriteReg(0x0011,0x6068);
	LCD_SetCursor(0,0);
	Prepare_Write_RAM();

	/* Start Image capture and Display on the LCD *****************************/
    /* Enable DMA transfer */
    DMA_Cmd(DMA2_Stream1, ENABLE);

    /* Enable DCMI interface */
    DCMI_Cmd(ENABLE); 

    /* Start Image capture */ 
    DCMI_CaptureCmd(ENABLE);
	   
	//DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8d, 0x10); //color test	on
	//DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0c, 0x80); //color test	on
	//uint32_t i=0x1fffff;
//	while(i--);
	//DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8d, 0x00); //color test off
	//DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0c, 0x00); //color test off					
	while (1)
	{
	}
}

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0)
  {}
}

void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void LCD_CS_ResetBits(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	//LCD_CS PD7

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	


	GPIO_ResetBits(GPIOD , GPIO_Pin_7);		 //CS=0;
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
