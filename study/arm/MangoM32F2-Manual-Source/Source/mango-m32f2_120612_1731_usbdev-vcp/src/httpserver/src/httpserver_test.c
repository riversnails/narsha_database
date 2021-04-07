/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
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
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
extern __IO uint32_t LocalTime; /* this variable is used to create a time reference incremented by 10ms */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  Test_HttpServer.
  * @param  None
  * @retval None
  */
void Test_HttpServer(void)
{
    printf("HTTP Server Test start ...\n");
  
    /* configure ethernet (GPIOs, clocks, MAC, DMA) */ 
    ETH_BSP_Config();
    printf("ETH_BSP_Config done ...\n");

    /* Initilaize the LwIP stack */
    LwIP_Init();
    printf("LwIP_Init done ...\n");

    /* Http webserver Init */
    httpd_init();
    printf("httpd_init done ...\n");

    /* Infinite loop */
    while (1)
    {  
        /* check if any packet received */
        if (ETH_CheckFrameReceived())
        { 
            printf("ETH_CheckFrameReceived ...\n");
            /* process received ethernet packet */
            LwIP_Pkt_Handle();
        }
        /* handle periodic timers for LwIP */
        LwIP_Periodic_Handle(LocalTime);
    }   
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
