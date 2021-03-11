#ifndef __SPISOFT_H
#define __SPISOFT_H

#define SPI_SOFT_CS_L					GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define SPI_SOFT_CS_H					GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define SW_SPI_MOSI_L					GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define SW_SPI_MOSI_H					GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define SW_SPI_CLK_L					GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define SW_SPI_CLK_H					GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define SW_SPI_ReadVal_MISO		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)

#define FLASH_ID 0XEF14

#define W25X_WriteEnable				0x06 
#define W25X_WriteDisable				0x04 
#define W25X_ReadStatusReg			0x05 
#define W25X_WriteStatusReg			0x01 
#define W25X_ReadData					0x03 
#define W25X_FastReadData			0x0B 
#define W25X_FastReadDual			0x3B 
#define W25X_PageProgram			0x02 
#define W25X_BlockErase				0xD8 
#define W25X_SectorErase				0x20 
#define W25X_ChipErase					0xC7 
#define W25X_PowerDown				0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID					0xAB 
#define W25X_ManufactDeviceID		0x90 
#define W25X_JedecDeviceID			0x9F 

void SPISoft_Init(void);
u16 SPISoft_Flash_ReadID(void);
u8 SPISoft_ReadWriteByte(u8 TxData);
void SW_SPI_MOSI_OUT(u8 out);
void SW_SPI_CLK_Init(void);
void SW_SPI_CLK_Toggle(void);
u8 SPISoft_Flash_ReadSR(void);
void SPISoft_FLASH_Write_SR(u8 sr);
void SPISoft_FLASH_Write_Enable(void);
void SPISoft_FLASH_Write_Disable(void);
void SPISoft_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
void SPISoft_Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void SPISoft_Flash_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void SPISoft_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void SPISoft_Flash_Erase_Chip(void);
void SPISoft_Flash_Erase_Sector(u32 Dst_Addr);
void SPISoft_Flash_Wait_Busy(void);
void SPISoft_Flash_PowerDown(void);
void SPISoft_Flash_WAKEUP(void);
#endif
