#include "I2C.h"

/******************************************************************************
* Function Name  : I2C_configuration
* Description    : Setting GPIO & I2C
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
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

/******************************************************************************
* Function Name  : I2C_Start
* Description    : I2C start (START signal generate & send slave Address)
* Input          : I2Cx, Addr, I2C_Direction, ACK_state
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
uint8_t I2C_Start(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t I2C_Direction, FunctionalState ACK_state)
{
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

/******************************************************************************
* Function Name  : I2C_Stop
* Description    : I2C stop(STOP signal generate)
* Input          : I2Cx
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
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

/******************************************************************************
* Function Name  : I2C_WriteData
* Description    : I2C data write
* Input          : I2Cx, data
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void I2C_WriteData(I2C_TypeDef * I2Cx, uint8_t data)
{
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE));
    I2C_SendData(I2Cx, data);
}

/******************************************************************************
* Function Name  : I2C_ReadData_Ack
* Description    : I2C data read & ack enable
* Input          : I2Cx
* Output         : None
* Return         : data
* Attention		 : None
*******************************************************************************/	 
uint8_t I2C_ReadData_Ack(I2C_TypeDef * I2Cx)
{
    uint8_t data;

    I2C_AcknowledgeConfig(I2Cx, ENABLE);

    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

    data = I2C_ReceiveData(I2Cx);

    return data;
}

/******************************************************************************
* Function Name  : I2C_ReadData_Ack
* Description    : I2C data read & ack disable
* Input          : I2Cx
* Output         : None
* Return         : data
* Attention		 : None
*******************************************************************************/	 
uint8_t I2C_ReadData_Nack(I2C_TypeDef * I2Cx)
{
    uint8_t data;

    I2C_AcknowledgeConfig(I2Cx, DISABLE);

    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));

    data = I2C_ReceiveData(I2Cx);

    return data;
}

/******************************************************************************
* Function Name  : I2C_Write
* Description    : I2C Address, Register, Data(1 byte) write
* Input          : I2Cx, Addr, Reg, data
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void I2C_Write(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t Reg, uint8_t data)
{
    I2C_Start(I2Cx, Addr, I2C_Direction_Transmitter, ACK_DISABLE);
    I2C_WriteData(I2Cx, Reg);
    I2C_WriteData(I2Cx, data);
    I2C_Stop(I2Cx);
}

/******************************************************************************
* Function Name  : I2C_MultiWrite
* Description    : I2C Address, Register, Data(more 2 bytes) write
* Input          : I2Cx, Addr, Reg, *data, cnt
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
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

/******************************************************************************
* Function Name  : I2C_Write_NoReg
* Description    : I2C Address, Data(1 byte) write
* Input          : I2Cx, Addr, data
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
void I2C_Write_NoReg(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t data)
{
    I2C_Start(I2Cx, Addr, I2C_Direction_Transmitter, ACK_DISABLE);
    I2C_WriteData(I2Cx, data);
    I2C_Stop(I2Cx);
}

/******************************************************************************
* Function Name  : I2C_MultiWrite_NoReg
* Description    : I2C Address, Data(more 2 bytes) write
* Input          : I2Cx, Addr, *data, cnt
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/	 
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

/******************************************************************************
* Function Name  : I2C_Read
* Description    : I2C slave data(1 byte) read
* Input          : I2Cx, Addr, Reg
* Output         : None
* Return         : received_data
* Attention		 : None
*******************************************************************************/	 
uint8_t I2C_Read(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t Reg)
{
    uint8_t received_data;
    I2C_Start(I2Cx, Addr, I2C_Direction_Transmitter, ACK_DISABLE);
    I2C_WriteData(I2Cx, Reg);
    I2C_Stop(I2Cx);
    received_data = I2C_Start(I2Cx, Addr, I2C_Direction_Receiver, ACK_DISABLE);
    received_data = I2C_ReadData_Ack(I2Cx);
    return received_data;
}

/******************************************************************************
* Function Name  : I2C_MultiRead
* Description    : I2C slave data(more 2 bytes) read
* Input          : I2Cx, Addr, Reg, *data, cnt
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
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

/******************************************************************************
* Function Name  : I2C_Read_NoReg
* Description    : I2C slave data(1 byte) read
* Input          : I2Cx, Addr
* Output         : None
* Return         : received_data
* Attention		 : None
*******************************************************************************/	 
uint8_t I2C_Read_NoReg(I2C_TypeDef * I2Cx, uint8_t Addr)
{
    uint8_t received_data;
    I2C_Start(I2Cx, Addr, I2C_Direction_Receiver, ACK_ENABLE);
    received_data = I2C_ReadData_Ack(I2Cx);
    return received_data;
}

/******************************************************************************
* Function Name  : I2C_MultiRead_NoReg
* Description    : I2C slave data(more 2 bytes) read
* Input          : I2Cx, Addr, *data, cnt
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
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

/******************************************************************************
* Function Name  : I2C_IsDeviceConnected
* Description    : I2C Slave Device check connect
* Input          : I2Cx, Addr
* Output         : None
* Return         : connected(0: Not connect, 1: Connect)
* Attention		 : None
*******************************************************************************/
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

