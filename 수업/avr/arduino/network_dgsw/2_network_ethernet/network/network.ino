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
  print_ethernet_head(rx_buff);
  
  //  if (pkt_len != 0)
  //  {
  //    //    Serial.printf("pkt_len = %d\n", pkt_len);
  //    for (int i = 0; i < 60; i++)
  //    {
  //      Serial.printf("%02x ", rx_buff[i]);
  //
  //      if (i % 10 == 0) Serial.printf("\n");
  //    }
  //  Serial.printf("\n\n");
  //  }

  //  if (pkt_len != 0) {
  //    Serial.printf("Destination MAC Address : ");
  //    for(int i=0;i<6;i++)
  //      Serial.printf("%02x ", rx_buff[i]);
  //    Serial.printf("\n");
  //
  //    Serial.printf("Source MAC Address : ");
  //    for(int i=0;i<6;i++)
  //      Serial.printf("%02x ", rx_buff[i+6]);
  //    Serial.printf("\n");
  //
  //    Serial.printf("Ethernet Type : ");
  //    for(int i=0;i<2;i++)
  //      Serial.printf("%02x ", rx_buff[i+12]);
  //    Serial.printf("\n");
  //  }
}
