#include"ether_shield.h"
#include "enc28j60.h"


void packet_send(uint16_t len, uint8_t* packet)
{
 	enc28j60PacketSend(len, packet);
}


void eth_init(uint8_t *my_mac, uint8_t *my_ip)
{
	enc28j60Init(my_mac);
	enc28j60clkout(2);// change clkout from 6.25MHz to 12.5MHz

	delay(10);

	// 0x476 is PHLCON LEDA=links status, LEDB=receive/transmit
	// enc28j60PhyWrite(PHLCON,0b0000 0100 0111 01 10);
	enc28j60PhyWrite(PHLCON,  0x476);

	delay(100);

}

uint16_t packet_receive(unsigned char *buff, uint16_t len)
{
	return enc28j60PacketReceive(len, buff);
}

void ether_shield_led()
{
  enc28j60clkout(2); // change clkout from 6.25MHz to 12.5MHz
  delay(10);
  
  for(int i=0;i<5;i++)
  {
    enc28j60PhyWrite(PHLCON,0x880);
    delay(1000);
    enc28j60PhyWrite(PHLCON,0x990);
    delay(1000);
  }
  
  enc28j60PhyWrite(PHLCON,0x476);
  delay(100);
}
