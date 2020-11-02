#include"ether_shield.h"

uint8_t my_mac[6] = {0x54, 0x55, 0x58, 0x10, 0x00, 0x24};
uint8_t everyone_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t my_ip[4] = {192, 168, 1, 17};
uint8_t target_ip[4] = {192, 168, 1, 10};
unsigned char rx_buff[BUFFER_SIZE + 1];
unsigned char tx_buff[BUFFER_SIZE + 1];

uint16_t pkt_len;

void setup()
{
  Serial.begin(115200);

  //  int b;
  //  int *a = &b;
  //  *a = 0x1234;
  //
  //  Serial.printf("%02x\n", (char)a[1]);
  //  while(1);
  //------------------------------
  for (int i = 0; i < BUFFER_SIZE + 1; i++)
  {
    rx_buff[i] = 0;
    tx_buff[i] = 0;
  }

  eth_init(my_mac, my_ip);
}

void loop()
{
  pkt_len = packet_receive(rx_buff, BUFFER_SIZE);

  if (pkt_len != 0)
  {

    if (which_packet_type(rx_buff) == ARP_PACKET)
    {
      print_ethernet_head(rx_buff);
      print_arp_head(rx_buff);
      if (is_my_ip(rx_buff, my_ip) == true)
      {
        Serial.printf("my arp packet\n");
        parsing_ethernet_head(rx_buff);
        make_ethernet_head(tx_buff, my_mac, ARP_PACKET);

        parsing_arp_head(rx_buff);
        make_arp_head(tx_buff, my_ip, my_mac);
        packet_send(60, tx_buff);
      }
    }
  }
}
