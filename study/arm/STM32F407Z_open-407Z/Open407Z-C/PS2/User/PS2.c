/*********************************************************************************************************
*
* File                : PS2.c
* Hardware Environment: 
* Build Environment   : ST Visual Develop 4.1.6
* Version             : V1.0
* By                  : Xiao xian hui
*
*                                  (c) Copyright 2005-2010, WaveShare
*                                       http://www.waveShare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

#include "PS2.h"
#include "PS2_code.h"

uint8_t rcvF = 0;
uint8_t keyVal;

/*PS2_DATA->PF10    PS2_CLK->PF11*/
void PS2_OUT_SDA(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/*PS2_DATA->PF10*/
    GPIO_InitStructure.GPIO_Pin = PS2_DATA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PS2_DATA_PORT, &GPIO_InitStructure);	

}

void PS2_IN_SDA(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/*PS2_DATA->PF10*/
	GPIO_InitStructure.GPIO_Pin = PS2_DATA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PS2_DATA_PORT, &GPIO_InitStructure);
}

void PS2_OUT_SCK(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/*PS2_CLK->PF11*/
    GPIO_InitStructure.GPIO_Pin = PS2_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PS2_CLK_PORT, &GPIO_InitStructure);
}

void PS2_IN_SCK(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	/*PS2_CLK->PF11*/
	GPIO_InitStructure.GPIO_Pin = PS2_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PS2_CLK_PORT, &GPIO_InitStructure);
}

void DELAY(void)
{
	u8 i=10;
	while(i--);
}

void PS2_Init(void)
{
	RCC_AHB1PeriphClockCmd(PS2_DATA_RCC | PS2_CLK_RCC,ENABLE);
	PS2_IN_SDA();
}

u8 keyHandle(u8 val) 
{
	u8 i;
	static u8 isUp = 0;
	static u8 shift = 0;
	rcvF = 0; 
	if(isUp == 0)
	{
		switch(val)
		{
			case 0xF0 :			// a relase action
				isUp = 1;
				break;
			case 0x12 :			// Left shift
				shift = 1;
				break;
			case 0x59 :			// Right shift
				shift = 1;
				break;
			default:
				if(shift == 0)		// If shift not pressed
				{ 

					//for(i=0; unshifted[i][0]!=val && unshifted[i][0];i++);


					for(i=0;unshifted[i][0]!=val && i<59; i++);
					if(unshifted[i][0] == val) 
						{
							//SHIFT_DATA_PORT = val;
							val = unshifted[i][1];
							return val;
						}
				} 
				else			// If shift pressed
				{			
					//for(i=0; unshifted[i][0]!=val && unshifted[i][0]; i++);

					for(i=0;shifted[i][0]!=val && i<59; i++);
					
						if(shifted[i][0] == val) 
						{
							//SHIFT_DATA_PORT = val;
							 val = shifted[i][1];
							return val;
						}
					
				}
		}
	}
	else 
	{
		isUp = 0;					
		switch(val)
		{
			case 0x12 :			// Left SHIFT
				shift = 0;
				break;
			case 0x59 :			// Right SHIFT
				shift = 0;
				break;
		}
	}
	return 0xff;
}

u8 PS2_Test(void)
{
	static u8 rcvBits = 0;
	
	PS2_OUT_SCK();
	DELAY();
	SET_CLK;
	DELAY();
	
	PS2_IN_SCK();
	DELAY();
	if(!GET_CLK)
	{
		if((rcvBits>0) && (rcvBits<9))
		{ 
			keyVal=keyVal>>1;
			//IN_SDA;
			//DELAY();
			if(GET_DATA) 
	 			keyVal=keyVal|0x80; 
		}
		rcvBits++;
		while(!GET_CLK);
	
		if(rcvBits>10)
		{
			rcvBits = 0;
			rcvF = 1;
		}
	}

	if(rcvF)
	{
		return keyHandle(keyVal);
	}
	else
		return 0xff;
}

