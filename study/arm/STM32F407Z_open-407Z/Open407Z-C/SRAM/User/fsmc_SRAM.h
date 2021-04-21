/*********************************************************************************************************
*
* File                : fsmc_nor.h
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

#ifndef __FSMC_NOR_H
#define __FSMC_NOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <string.h>
#include <stdio.h>

void SRAM_Init(void);
void SRAM_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
void SRAM_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);
void SRAM_TEST(void);
void Fill_Buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);

#endif /* __FSMC_NOR_H */

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/

