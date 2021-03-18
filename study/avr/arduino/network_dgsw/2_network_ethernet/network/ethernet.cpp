#include "ethernet.h"
#include "ether_shield.h"

void print_ethernet_head(char *rx_buff)
{
  uint16_t pkt_len = packet_receive(rx_buff, BUFFER_SIZE);

  if (pkt_len != 0) {
    Serial.printf("==========================================\n");
    Serial.printf("Destination MAC Address : ");
    for (int i = 0; i < 6; i++)
      Serial.printf("%02x ", rx_buff[i + INDEX_ETHERNET_DESTINATION_MAC_ADDR]);
    Serial.printf("\n");

    Serial.printf("Source MAC Address :      ");
    for (int i = 0; i < 6; i++)
      Serial.printf("%02x ", rx_buff[i + INDEX_ETHERNET_SOURCE_MAC_ADDR]);
    Serial.printf("\n");

    Serial.printf("Ethernet Type :           ");
    for (int i = 0; i < 2; i++)
      Serial.printf("%02x ", rx_buff[i + INDEX_ETHERNET_TYPE]);
    Serial.printf("\n");

    Serial.printf("Hardware Type :           ");
    for (int i = 0; i < 2; i++)
      Serial.printf("%02x ", rx_buff[i + 14]);
    Serial.printf("\n");

    Serial.printf("Protocol Type :           ");
    for (int i = 0; i < 2; i++)
      Serial.printf("%02x ", rx_buff[i + 16]);
    Serial.printf("\n");

    Serial.printf("Hardware Size :           ");
    Serial.printf("%02x ", rx_buff[18]);
    Serial.printf("\n");

    Serial.printf("Hardware Size :           ");
    Serial.printf("%02x ", rx_buff[18]);
    Serial.printf("\n");

    Serial.printf("\n");
  }
}
