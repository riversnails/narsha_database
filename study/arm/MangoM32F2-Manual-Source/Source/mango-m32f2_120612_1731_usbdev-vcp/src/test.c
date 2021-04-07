/*
 * (C) COPYRIGHT 2012 CRZ
 *
 * File Name : test.c
 * Author    : POOH
 * Version   : V1.0
 * Date      : 04/13/2012
 */

/* includes */

#include "hw_config.h"

/* static functions */

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay_time_consume(__IO uint32_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

static void delay_1_second(void)
{
    delay_time_consume(0x4FFFFF);
}

static void led_on_all (void)
{
    DevCtrl_LED_On(LED1);
    DevCtrl_LED_On(LED2);
    DevCtrl_LED_On(LED3);
}

static void led_off_all (void)
{
    DevCtrl_LED_Off(LED1);
    DevCtrl_LED_Off(LED2);
    DevCtrl_LED_Off(LED3);
}

static void led_on_off_all_mult(uint32_t count)
{
    for(; count > 0; count --)
    {
        DevCtrl_LED_Off(LED1);
        DevCtrl_LED_On(LED2);
        DevCtrl_LED_On(LED3);
        delay_1_second();

        DevCtrl_LED_On(LED1);
        DevCtrl_LED_Off(LED2);
        DevCtrl_LED_On(LED3);
        delay_1_second();

        DevCtrl_LED_On(LED1);
        DevCtrl_LED_On(LED2);
        DevCtrl_LED_Off(LED3);
        delay_1_second();
    }
}

/* global functions */

void Test_LED(void)
{
    led_on_all();
    delay_1_second();
    led_off_all();
    delay_1_second();

    led_on_off_all_mult(2);
}

