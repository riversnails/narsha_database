#include"ether_shield.h"



uint8_t my_mac[6] = {0x54,0x55,0x58,0x10,0x00,0x24}; 
uint8_t everyone_mac[6] = {0xff,0xff,0xff,0xff,0xff,0xff}; 
uint8_t my_ip[4] = {192,168,1,17};
uint8_t target_ip[4] = {192,168,1,10};
unsigned char rx_buff[BUFFER_SIZE+1];
unsigned char tx_buff[BUFFER_SIZE+1];


void setup()
{
	Serial.begin(115200);

	//------------------------------
	for(int i=0;i<BUFFER_SIZE+1;i++)
	{
		rx_buff[i] = 0;
		tx_buff[i] = 0;
	}
 
  eth_init(my_mac, my_ip);
  ether_shield_led();

}

void loop(){

}
