
//#include "stm32f1xx_hal.h"
#include "stm32f072b_discovery.h"
#include "main.h"
#include "pn532_stm32f1.h"

#define _SPI_STATREAD                   0x02
#define _SPI_DATAWRITE                  0x01
#define _SPI_DATAREAD                   0x03
#define _SPI_READY                      0x01

#define _SPI_TIMEOUT                    10
// This indicates if the bits read/write should be reversed
#define _SPI_HARDWARE_LSB

extern SPI_HandleTypeDef hspi1;

//	
uint8_t reverse_bit(uint8_t num) {
    uint8_t result = 0;
    for (uint8_t i = 0; i < 8; i++) {
        result <<= 1;
        result += (num & 1);
        num >>= 1;
    }
    return result;
}

//	#define SS_Pin GPIO_PIN_7
//	#define SS_GPIO_Port GPIOB


#if 0
	
void spi_rw(uint8_t* data, uint8_t count) {
    HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
#ifndef _SPI_HARDWARE_LSB
    for (uint8_t i = 0; i < count; i++) {
        data[i] = reverse_bit(data[i]);
    }
    HAL_SPI_TransmitReceive(&hspi1, data, data, count, _SPI_TIMEOUT);
    for (uint8_t i = 0; i < count; i++) {
        data[i] = reverse_bit(data[i]);
    }
#else
    HAL_SPI_TransmitReceive(&hspi1, data, data, count, _SPI_TIMEOUT);
#endif
    HAL_Delay(1);
    HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_SET);
}


//======================================================
#else

//	    PB3     ------> SPI1_SCK
//	    PB4     ------> SPI1_MISO
//	    PB5     ------> SPI1_MOSI 

//	#define clk_high()		SET_BIT(GPIOA, (0x01 << 5))
//	#define clk_low()		CLEAR_BIT(GPIOA, (0x01 << 5))
//	#define mosi_high()		SET_BIT(GPIOA, (0x01 << 7))
//	#define mosi_low()		CLEAR_BIT(GPIOA, (0x01 << 7))
//	#define get_miso()		READ_BIT(GPIOA, (0x01 << 6))


void clk_high()
{	
	GPIOB->ODR |= (0x01 << 3);
}

void clk_low()
{
	GPIOB->ODR &= ~(0x01 << 3);
}

void mosi_high()
{
	GPIOB->ODR |= (0x01 << 5);
}

void mosi_low()
{
	GPIOB->ODR &= ~(0x01 << 5);
}

int get_miso()
{
	return (GPIOB->IDR & (0x01 << 4));
}


void delay_for()
{
	for(int i=0;i<1000;i++);
}


void spi_rw(uint8_t* data, uint8_t count) {

	unsigned char tmp_buf = 0x00;
	
	HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);

	for(int j=0;j<count;j++)
	{
		tmp_buf = 0x00;
		
		for(int i=0;i<8 ; i++)
		{
			if( *(data+j) &  (0x01 << i) ) mosi_high();
			else mosi_low();

			clk_high();
			//HAL_Delay(1);
			tmp_buf >>= 1;
			if(get_miso()) tmp_buf |= 0x80;
		
			clk_low();
			//HAL_Delay(1);
		}

		*(data+j)  = tmp_buf;
		//printf("%02x \n\r", tmp_buf);
		
	}


    HAL_Delay(1);
    HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_SET);
}

#endif
//======================================================





int PN532_Reset(void) {
    // TODO: in most cases, reset pin is no need for SPI
    /*
    HAL_GPIO_WritePin(RSTPD_N_GPIO_Port, RSTPD_N_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(RSTPD_N_GPIO_Port, RSTPD_N_Pin, GPIO_PIN_RESET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(RSTPD_N_GPIO_Port, RSTPD_N_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    */
   return PN532_STATUS_OK;
}

int PN532_ReadData(uint8_t* data, uint16_t count) {
    uint8_t frame[count + 1];
    frame[0] = _SPI_DATAREAD;
    HAL_Delay(5);
    spi_rw(frame, count + 1);
    for (uint8_t i = 0; i < count; i++) {
        data[i] = frame[i + 1];
    }
    return PN532_STATUS_OK;
}

int PN532_WriteData(uint8_t *data, uint16_t count) {
    uint8_t frame[count + 1];
    frame[0] = _SPI_DATAWRITE;
    for (uint8_t i = 0; i < count; i++) {
        frame[i + 1] = data[i];
    }
    spi_rw(frame, count + 1);
    return PN532_STATUS_OK;
}

bool PN532_WaitReady(uint32_t timeout) {
    uint8_t status[] = {_SPI_STATREAD, 0x00};
    uint32_t tickstart = HAL_GetTick();
    while (HAL_GetTick() - tickstart < timeout) {
        HAL_Delay(10);
        spi_rw(status, sizeof(status));
        if (status[1] == _SPI_READY) {
            return true;
        } else {
            HAL_Delay(5);
        }
    }
    return false;
}

#define SS_Pin GPIO_PIN_7
#define SS_GPIO_Port GPIOB
int PN532_Wakeup(void) {
    // Send any special commands/data to wake up PN532
    uint8_t data[] = {0x00};
    HAL_Delay(1000);
    HAL_GPIO_WritePin(SS_GPIO_Port, SS_Pin, GPIO_PIN_RESET);
    HAL_Delay(2); // T_osc_start
    spi_rw(data, 1);
    HAL_Delay(1000);
    return PN532_STATUS_OK;
}

void PN532_Log(const char* log) {
    printf(log);
    printf("\r\n");
}

void PN532_Init(PN532* pn532) {
    // init the pn532 functions
    pn532->reset =  PN532_Reset;
    pn532->read_data = PN532_ReadData;
    pn532->write_data = PN532_WriteData;
    pn532->wait_ready = PN532_WaitReady;
    pn532->wakeup = PN532_Wakeup;
    pn532->log = PN532_Log;

    // hardware wakeup
    pn532->wakeup();
}
