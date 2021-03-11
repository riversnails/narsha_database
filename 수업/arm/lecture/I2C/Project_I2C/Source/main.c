#include "main.h"
#include "stm32f10x_it.h"
#include <stdio.h>

#define ACK_ENABLE  ENABLE
#define ACK_DISABLE DISABLE

void I2C_configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;

    I2C_Init(I2C1, &I2C_InitStruct);
    I2C_AcknowledgeConfig(I2C1, ENABLE);

    //I2C_ITConfig(I2C1, I2C_IT_EVT, ENABLE);

    I2C_Cmd(I2C1, ENABLE);
    
}

void UART_configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    //USART1 RCC enable
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);

    //USART1 TX pin setting
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //USART1 RX pin setting
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    //GPIO_InitStruct.GPIO_Speed = 0;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    //USART1 base setting
    USART_InitStruct.USART_BaudRate = 115200;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStruct);
    
    USART_ClearFlag(USART1, USART_FLAG_RXNE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);
}

void delay(uint32_t cnt)
{
    while(cnt--);
}


void SendString_while(USART_TypeDef * USARTx, char *str)
{
    char c;
    do
    {
        switch(c= *str)
        {
            case 0x0a:
            //case 0x0d:
                USART_SendData(USARTx, 0x0d);
                USART_SendData(USARTx, 0x0a);
                break;
            default:
                USART_SendData(USARTx, (uint16_t)c);
        }
        //wait transmission complete & clear status register
        while(!USART_GetFlagStatus(USARTx, USART_FLAG_TC));
        USART_ClearFlag(USARTx, USART_FLAG_TC);
    }while(*str++);
}


/**
  *     @arg I2C_EVENT_MASTER_MODE_SELECT                          : EV5
  *     @arg I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED            : EV6     
  *     @arg I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED               : EV6
  *     @arg I2C_EVENT_MASTER_BYTE_RECEIVED                        : EV7
  *     @arg I2C_EVENT_MASTER_BYTE_TRANSMITTING                    : EV8
  *     @arg I2C_EVENT_MASTER_BYTE_TRANSMITTED                     : EV8_2
  */

uint8_t I2C_Start(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t I2C_Direction, FunctionalState ACK_state)
{
    /* * *
    uint16_t delay_cnt=1000;
    if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
    {
        delay_cnt--;
        if(delay_cnt == 0)
        {
            return 2;
        }
    }
    * * */

    //Generate start pulse 
    I2C_GenerateSTART(I2Cx, ENABLE);
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_AcknowledgeConfig(I2Cx, ACK_state);
    
    if(I2C_Direction == I2C_Direction_Transmitter)
    {
        I2C_Send7bitAddress(I2Cx, Addr, I2C_Direction_Transmitter);
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    }
    else if(I2C_Direction == I2C_Direction_Receiver)
    {
        I2C_Send7bitAddress(I2Cx, Addr, I2C_Direction_Receiver);
        while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }

    I2C_ClearFlag(I2Cx, I2C_FLAG_ADDR);
    
    return 0;
}

uint8_t I2C_Stop(I2C_TypeDef * I2Cx)
{
    uint16_t delay_cnt=1000;
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE|I2C_FLAG_BTF))
    {
        delay_cnt--;
        if(delay_cnt == 0)
        {
            return 1;
        }
    }
    
    I2C_GenerateSTOP(I2Cx, ENABLE);

    return 0;
}

void I2C_WriteData(I2C_TypeDef * I2Cx, uint8_t data)
{
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE));
    I2C_SendData(I2Cx, data);
}

uint8_t I2C_ReadData_Ack(I2C_TypeDef * I2Cx)
{
    uint8_t data;

    I2C_AcknowledgeConfig(I2Cx, ENABLE);

    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

    data = I2C_ReceiveData(I2Cx);

    return data;
}

uint8_t I2C_ReadData_Nack(I2C_TypeDef * I2Cx)
{
    uint8_t data;

    I2C_AcknowledgeConfig(I2Cx, DISABLE);

    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

    data = I2C_ReceiveData(I2Cx);

    return data;
}

void I2C_Write(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t Reg, uint8_t data)
{
    I2C_Start(I2Cx, Addr, I2C_Direction_Transmitter, ACK_DISABLE);
    I2C_WriteData(I2Cx, Reg);
    I2C_WriteData(I2Cx, data);
    I2C_Stop(I2Cx);
}

void I2C_MultiWrite(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t Reg, uint8_t* data, uint16_t cnt)
{
    uint16_t i;
    I2C_Start(I2Cx, Addr, I2C_Direction_Transmitter, ACK_DISABLE);
    I2C_WriteData(I2Cx, Reg);
    for(i=0 ; i<cnt ; i++)
    {
        I2C_WriteData(I2Cx, data[i]);
    }
    I2C_Stop(I2Cx);
}

void I2C_Write_NoReg(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t data)
{
    I2C_Start(I2Cx, Addr, I2C_Direction_Transmitter, ACK_DISABLE);
    I2C_WriteData(I2Cx, data);
    I2C_Stop(I2Cx);
}

void I2C_MultiWrite_NoReg(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t* data, uint16_t cnt)
{
    uint16_t i;
    I2C_Start(I2Cx, Addr, I2C_Direction_Transmitter, ACK_DISABLE);
    for(i=0 ; i<cnt ; i++)
    {
        I2C_WriteData(I2Cx, data[i]);
    }
    I2C_Stop(I2Cx);
}

uint8_t I2C_Read(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t Reg)
{
    uint8_t received_data;
    I2C_Start(I2Cx, Addr, I2C_Direction_Transmitter, ACK_DISABLE);
    I2C_WriteData(I2Cx, Reg);

    received_data = I2C_Start(I2Cx, Addr, I2C_Direction_Receiver, ACK_DISABLE);
   // received_data = I2C_ReadData_Ack(I2Cx);
    received_data = I2C_ReadData_Nack(I2Cx);

    I2C_Stop(I2Cx);
	
    return received_data;
}

void I2C_MultiRead(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t Reg, uint8_t* data, uint16_t cnt)
{
    uint16_t i;
    I2C_Start(I2Cx, Addr, I2C_Direction_Transmitter, ACK_ENABLE);
    I2C_WriteData(I2Cx, Reg);
    //I2C_Stop(I2Cx);
    I2C_Start(I2Cx, Addr, I2C_Direction_Receiver, ACK_ENABLE);
    for(i=0 ; i<cnt ; i++)
    {
        if(i == (cnt-1))
        {
            data[i] = I2C_ReadData_Nack(I2Cx);
        }
        else
        {
            data[i] = I2C_ReadData_Ack(I2Cx);
        }
    }
}

uint8_t I2C_Read_NoReg(I2C_TypeDef * I2Cx, uint8_t Addr)
{
    uint8_t received_data;
    I2C_Start(I2Cx, Addr, I2C_Direction_Receiver, ACK_ENABLE);
    received_data = I2C_ReadData_Ack(I2Cx);
    return received_data;
}

void I2C_MultiRead_NoReg(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t* data, uint16_t cnt)
{
    uint16_t i;
    I2C_Start(I2Cx, Addr, I2C_Direction_Receiver, ACK_ENABLE);
    for(i=0 ; i<cnt ; i++)
    {
        if(i == (cnt-1))
        {
            data[i] = I2C_ReadData_Nack(I2Cx);
        }
        else
        {
            data[i] = I2C_ReadData_Ack(I2Cx);
        }
    }
}

uint8_t I2C_IsDeviceConnected(I2C_TypeDef * I2Cx, uint8_t Addr)
{
    uint8_t connected = 0;
    if(!I2C_Start(I2Cx, Addr, I2C_Direction_Transmitter, ACK_ENABLE))
    {
        connected = 1;
    }

    I2C_Stop(I2Cx);

    return connected;
}

uint8_t buff[20];



int main(void)
{
    uint8_t dat;
	uint8_t data[100];
	

	
    I2C_configuration();
    UART_configuration();

		//while(1);
	
	//delay(0xffff);

	SendString_while(USART1,"I2C device EEPROM !\n\r");


///////////////////////////////////////
  	//I2C_GenerateSTOP(I2C1, ENABLE);

	
    if(I2C_IsDeviceConnected(I2C1, 0xa0))
    {
        SendString_while(USART1,"I2C device EEPROM is connected!\n\r");
    }

    I2C_MultiWrite(I2C1, 0xA0, 0x01, "hahaha22!!!hohoho!!!!", 12);

    delay(0xffff);

    I2C_MultiRead(I2C1, 0xa0, 0x01, buff, 12);

	SendString_while(USART1,buff);

    delay(0xffff);

    I2C_Write(I2C1, 0xa0, 0x80, '1');

    delay(0xffff);

    data[0] = I2C_Read(I2C1, 0xa0, 0x14);

	
	SendString_while(USART1,data);


#if 0

    dat = I2C_Read(I2C1, 0xa0, 0x01);

    USART_SendData(USART1, dat);
	
    while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
    USART_ClearFlag(USART1, USART_FLAG_TC);
    USART_SendData(USART1, '\n');
    while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));
    USART_ClearFlag(USART1, USART_FLAG_TC);
    SendString_while(USART1, (char *)buff);
  #endif
  
	while(1)
	{
	}
	
}
