#include <stm32f10x_i2c.h>
#include <stm32f10x_gpio.h>

void I2C_configuration(void);
uint8_t I2C_Start(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t I2C_Direction, FunctionalState ACK_state);
uint8_t I2C_Stop(I2C_TypeDef * I2Cx);
void I2C_WriteData(I2C_TypeDef * I2Cx, uint8_t data);
uint8_t I2C_ReadData_Ack(I2C_TypeDef * I2Cx);
uint8_t I2C_ReadData_Nack(I2C_TypeDef * I2Cx);
void I2C_Write(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t Reg, uint8_t data);
void I2C_MultiWrite(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t Reg, uint8_t* data, uint16_t cnt);
void I2C_Write_NoReg(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t data);
void I2C_MultiWrite_NoReg(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t* data, uint16_t cnt);
uint8_t I2C_Read(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t Reg);
void I2C_MultiRead(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t Reg, uint8_t* data, uint16_t cnt);
uint8_t I2C_Read_NoReg(I2C_TypeDef * I2Cx, uint8_t Addr);
void I2C_MultiRead_NoReg(I2C_TypeDef * I2Cx, uint8_t Addr, uint8_t* data, uint16_t cnt);
uint8_t I2C_IsDeviceConnected(I2C_TypeDef * I2Cx, uint8_t Addr);

