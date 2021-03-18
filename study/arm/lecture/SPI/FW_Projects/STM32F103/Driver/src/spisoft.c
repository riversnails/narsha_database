#include "stm32f10x.h"
#include <STM32F10X_GPIO.h>
#include <STM32F10X_RCC.h>
#include "spisoft.h"
#include "delay.h"

#define SW_SPI_WAIT_TIME  100

u32 SW_SPI_CPHA_val = 0;
u32 SW_SPI_CPOL_val = 0;

void SPISoft_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable SPI1 and GPIOA clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_SetBits(GPIOA, GPIO_Pin_2); 
}

u16 SPISoft_Flash_ReadID(void)
{
	u16 Temp = 0;
	SPI_SOFT_CS_L;
	SPISoft_ReadWriteByte(0x90);
	SPISoft_ReadWriteByte(0x00);
	SPISoft_ReadWriteByte(0x00);
	SPISoft_ReadWriteByte(0x00);
	Temp|=SPISoft_ReadWriteByte(0xFF)<<8;
	Temp|=SPISoft_ReadWriteByte(0xFF);
	SPI_SOFT_CS_H;
	return Temp;
}

u8 SPISoft_ReadWriteByte(u8 TxData)
{
	u8  rx_data = 0;
    u32 i;
    u8 bitCheckVal = 0x80;

		SW_SPI_CLK_L;//CLR_SPI_SCK;
 
		Delay(200);

		for(i = 0; i < 8; i++)
		{
				if(TxData & bitCheckVal)SW_SPI_MOSI_H;
				else SW_SPI_MOSI_L;
				bitCheckVal >>= 1;

				SW_SPI_CLK_L;//CLR_SPI_SCK;
				Delay(200);

        rx_data <<= 1;
        if(SW_SPI_ReadVal_MISO == Bit_SET)
        {
            rx_data |= 0x1;
        }
				
				SW_SPI_CLK_H;//SET_SPI_SCK;
				Delay(200);
		}

		SW_SPI_CLK_L;//CLR_SPI_SCK;
		SW_SPI_MOSI_H;//SET_SPI_MOSI;

		Delay(200);
		return rx_data;

 /*   u8  rx_data = 0;

    u32 i;
    u8 bitCheckVal = 0x80;

    if(1 == SW_SPI_CPHA_val)
    {
        SW_SPI_CLK_Toggle();    
    }

	for (i=0; i<8; i++) 
	{
		SW_SPI_MOSI_OUT(TxData & bitCheckVal);
		bitCheckVal >>= 1;

        SW_SPI_CLK_Toggle();

        rx_data <<= 1;
        if(SW_SPI_ReadVal_MISO == Bit_SET)
        {
            rx_data |= 0x1;
        }

        if((7 == i) && (1 == SW_SPI_CPHA_val))
        {    // In last loop, if CPHA is 1, should skip CLK Toggle ...
           break;
        }
        SW_SPI_CLK_Toggle();
	}

    Delay(SW_SPI_WAIT_TIME);//TIMER_Wait_us(SW_SPI_WAIT_TIME);
    return rx_data;*/
}

void SW_SPI_MOSI_OUT(u8 out)
{
    if (out)
    {
        SW_SPI_MOSI_H;
    }
    else
    {
        SW_SPI_MOSI_L;
    }
    Delay(SW_SPI_WAIT_TIME);//TIMER_Wait_us(SW_SPI_WAIT_TIME);
}

void SW_SPI_CLK_Init(void)
{
    if(0 == SW_SPI_CPOL_val)
    {
        SW_SPI_CLK_L;
    }
    else
    {
        SW_SPI_CLK_H;
    }
    Delay(SW_SPI_WAIT_TIME);//TIMER_Wait_us(SW_SPI_WAIT_TIME);
}

void SW_SPI_CLK_Toggle(void)
{
    GPIO_WriteBit(GPIOA, GPIO_Pin_5,
        (BitAction)(1-GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5)));
    Delay(SW_SPI_WAIT_TIME);//TIMER_Wait_us(SW_SPI_WAIT_TIME);
}

u8 SPISoft_Flash_ReadSR(void)
{
	u8 byte=0;
	SPI_SOFT_CS_L;
	SPISoft_ReadWriteByte(W25X_ReadStatusReg);
	byte=SPISoft_ReadWriteByte(0Xff);
	SPI_SOFT_CS_H;
	return byte;
}

void SPISoft_FLASH_Write_SR(u8 sr)
{
	SPI_SOFT_CS_L;
	SPISoft_ReadWriteByte(W25X_WriteStatusReg);
	SPISoft_ReadWriteByte(sr);
	SPI_SOFT_CS_H;
}   

void SPISoft_FLASH_Write_Enable(void)
{
	SPI_SOFT_CS_L;
    SPISoft_ReadWriteByte(W25X_WriteEnable);
	SPI_SOFT_CS_H;
}

void SPISoft_FLASH_Write_Disable(void)
{
	SPI_SOFT_CS_L;
    SPISoft_ReadWriteByte(W25X_WriteDisable);
	SPI_SOFT_CS_H;
}

void SPISoft_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)
{
 	u16 i;
	SPI_SOFT_CS_L;
    SPISoft_ReadWriteByte(W25X_ReadData);
    SPISoft_ReadWriteByte((u8)((ReadAddr)>>16));
    SPISoft_ReadWriteByte((u8)((ReadAddr)>>8));
    SPISoft_ReadWriteByte((u8)ReadAddr);
    for(i=0;i<NumByteToRead;i++)
	{
        pBuffer[i]=SPISoft_ReadWriteByte(0XFF);
    }
	SPI_SOFT_CS_H;  
}
//======================
void SPISoft_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;
    SPISoft_FLASH_Write_Enable();
	SPI_SOFT_CS_L;
    SPISoft_ReadWriteByte(W25X_PageProgram);
    SPISoft_ReadWriteByte((u8)((WriteAddr)>>16));
    SPISoft_ReadWriteByte((u8)((WriteAddr)>>8));
    SPISoft_ReadWriteByte((u8)WriteAddr);
    for(i=0;i<NumByteToWrite;i++)SPISoft_ReadWriteByte(pBuffer[i]);
	SPI_SOFT_CS_H;
	SPISoft_Flash_Wait_Busy();
}

void SPISoft_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u16 pageremain;
	pageremain=256-WriteAddr%256;
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;
	while(1)
	{
		SPISoft_Flash_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;
	 	else
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;

			NumByteToWrite-=pageremain;
			if(NumByteToWrite>256)pageremain=256;
			else pageremain=NumByteToWrite;
		}
	}
}
 
u8 SPISoft_FLASH_BUF[4096];
void SPISoft_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;
 	u16 i;
	secpos=WriteAddr/4096;
	secoff=WriteAddr%4096;
	secremain=4096-secoff;
	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;
	while(1)
	{
		SPISoft_Flash_Read(SPISoft_FLASH_BUF,secpos*4096,4096);
		for(i=0;i<secremain;i++)
		{
			if(SPISoft_FLASH_BUF[secoff+i]!=0XFF)break;
		}
		if(i<secremain)
		{
			SPISoft_Flash_Erase_Sector(secpos);
			for(i=0;i<secremain;i++)
			{
				SPISoft_FLASH_BUF[i+secoff]=pBuffer[i];
			}
			SPISoft_Flash_Write_NoCheck(SPISoft_FLASH_BUF,secpos*4096,4096);

		}else SPISoft_Flash_Write_NoCheck(pBuffer,WriteAddr,secremain);
		if(NumByteToWrite==secremain)break;
		else
		{
			secpos++;
			secoff=0;

		   	pBuffer+=secremain;
			WriteAddr+=secremain;
		   	NumByteToWrite-=secremain;
			if(NumByteToWrite>4096)secremain=4096;
			else secremain=NumByteToWrite;
		}
	}
}

void SPISoft_Flash_Erase_Chip(void)
{
    SPISoft_FLASH_Write_Enable();
    SPISoft_Flash_Wait_Busy();
  	SPI_SOFT_CS_L;
    SPISoft_ReadWriteByte(W25X_ChipErase);
	SPI_SOFT_CS_H;
	SPISoft_Flash_Wait_Busy();
}

void SPISoft_Flash_Erase_Sector(u32 Dst_Addr)
{
	Dst_Addr*=4096;
    SPISoft_FLASH_Write_Enable();
    SPISoft_Flash_Wait_Busy();
  	SPI_SOFT_CS_L;
    SPISoft_ReadWriteByte(W25X_SectorErase);
    SPISoft_ReadWriteByte((u8)((Dst_Addr)>>16));
    SPISoft_ReadWriteByte((u8)((Dst_Addr)>>8));
    SPISoft_ReadWriteByte((u8)Dst_Addr);
	SPI_SOFT_CS_H;
    SPISoft_Flash_Wait_Busy();
}

void SPISoft_Flash_Wait_Busy(void)
{
	while ((SPISoft_Flash_ReadSR()&0x01)==0x01);
}

void SPISoft_Flash_PowerDown(void)
{
  	SPI_SOFT_CS_L;
    SPISoft_ReadWriteByte(W25X_PowerDown);
	SPI_SOFT_CS_H;
    delay_us(3);
}

void SPISoft_Flash_WAKEUP(void)
{
  	SPI_SOFT_CS_L;
    SPISoft_ReadWriteByte(W25X_ReleasePowerDown);
	SPI_SOFT_CS_H;
    delay_us(3);
}



