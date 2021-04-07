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

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
    /* Capture the current local time */
    timingdelay = LocalTime + nCount;  

    /* wait until the desired delay finish */  
    while(timingdelay > LocalTime)
    {     
    }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
    LocalTime += SYSTEMTICK_PERIOD_MS;
}

void System_Information()
{
    RCC_ClocksTypeDef  rcc_clocks;

    RCC_GetClocksFreq(&rcc_clocks);

    printf("SYSCLK_Frequency = %d\n",rcc_clocks.SYSCLK_Frequency );
    printf("HCLK_Frequency = %d\n",rcc_clocks.HCLK_Frequency );
    printf("PCLK1_Frequency = %d\n",rcc_clocks.PCLK1_Frequency );
    printf("PCLK2_Frequency = %d\n",rcc_clocks.PCLK2_Frequency );
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    uint8_t ch;

    /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */

    LED_Initialization();
    Key_Initialization();
    USART_Debug_Initialization();
    printf("USART_Debug_Initialization done.\n");
    printf("Mango-M32F2 Test - %s, %s\n", __DATE__, __TIME__);

    SysTick_Initialization();
    printf("SysTick_Initialization done.\n");

    while(1)
    {
        printf("\n---------------------\n");
        printf("Press menu key\n");
        printf("---------------------\n");
        printf("0> System Information\n");
        printf("---------------------\n");
        printf("1> Test LED\n");
#ifdef ETHERNET_TEST
        printf("2> Test HTTP Server\n");
#endif
#ifdef USB_TEST
        printf("2> Test USB Device VCP\n");
#endif
        printf("---------------------\n");
        printf("x> quit\n\n");

        ch = DebugUart_GetChar();
        printf(" is selected\n\n");

        switch((char)ch)
        {
        case '0':
            System_Information();
            break;

        case '1':
            Test_LED();
            break;

        case '2':
#ifdef ETHERNET_TEST
            Test_HttpServer();
#endif
#ifdef USB_TEST
            Test_USB_Device_VCP();
#endif
            break;
        }

        if('x' == (char)ch)
        {
            break;
        }
    }
}

