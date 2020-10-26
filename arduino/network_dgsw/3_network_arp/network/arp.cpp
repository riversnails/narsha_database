#include "arp.h"

char arp_rx_dst_mac[6];
char arp_rx_dst_ip[4];

void print_arp_head(char *rx_buff)
{
  Serial.printf("Hardware Type :           ");
  for (int i = 0; i < 2; i++)
    Serial.printf("%02x ", rx_buff[i + INDEX_ARP_HARDWARE_TYPE]);
  Serial.printf("\n");

  Serial.printf("Protocol Type :           ");
  for (int i = 0; i < 2; i++)
    Serial.printf("%02x ", rx_buff[i + INDEX_ARP_PROTOCOL_TYPE]);
  Serial.printf("\n");

  Serial.printf("Hardware Size :           ");
  Serial.printf("%02x ", rx_buff[INDEX_ARP_HARDWARE_LENGTH]);
  Serial.printf("\n");

  Serial.printf("Protocol Size :           ");
  Serial.printf("%02x ", rx_buff[INDEX_ARP_PROTOCOL_LENGTH]);
  Serial.printf("\n");

  Serial.printf("opcode :                  ");
  for (int i = 0; i < 2; i++)
    Serial.printf("%02x ", rx_buff[i + INDEX_ARP_OPCODE]);
  Serial.printf("\n");

  Serial.printf("Sender mac address :      ");
  for (int i = 0; i < 6; i++)
    Serial.printf("%02x ", rx_buff[i + INDEX_ARP_SENDER_MAC_ADDR] & 0xff);
  Serial.printf("\n");

  Serial.printf("Sender ip address :       ");
  for (int i = 0; i < 4; i++)
    Serial.printf("%02x ", rx_buff[i + INDEX_ARP_SENDER_IP_ADDR] & 0xff);
  Serial.printf("\n");

  Serial.printf("Target mac address :      ");
  for (int i = 0; i < 6; i++)
    Serial.printf("%02x ", rx_buff[i + INDEX_ARP_TARGET_MAC_ADDR] & 0xff);
  Serial.printf("\n");

  Serial.printf("Target ip address :       ");
  for (int i = 0; i < 4; i++)
    Serial.printf("%02x ", rx_buff[i + INDEX_ARP_TARGET_IP_ADDR] & 0xff);
  Serial.printf("\n");
  Serial.printf("==========================================\n");
}

bool is_my_ip(char *rx_buff, uint8_t *my_ip) // uint8_t == unsigned char
{
  char ip_count = 0;

  for (int i = 0; i < 4; i++)
  {
    if ((rx_buff[i + INDEX_ARP_TARGET_IP_ADDR] & 0xff) == my_ip[i]) ip_count++;
  }

  if (ip_count == 4) return true;
  else return false;
}

void parsing_arp_head(char *rx_buff)
{
  for (int i = 0; i < 6; i++) arp_rx_dst_mac[i] = rx_buff[i + INDEX_ARP_SENDER_MAC_ADDR];
  for (int i = 0; i < 4; i++) arp_rx_dst_ip[i] = rx_buff[i + INDEX_ARP_SENDER_IP_ADDR];
}


void make_arp_head(char *tx_buff, uint8_t *my_ip, uint8_t *my_mac)
{
  tx_buff[0 + INDEX_ARP_HARDWARE_TYPE] = 0x00;
  tx_buff[1 + INDEX_ARP_HARDWARE_TYPE] = 0x01;

  tx_buff[0 + INDEX_ARP_PROTOCOL_TYPE] = 0x08;
  tx_buff[1 + INDEX_ARP_PROTOCOL_TYPE] = 0x00;

  tx_buff[0 + INDEX_ARP_HARDWARE_LENGTH] = 0x06;
  tx_buff[0 + INDEX_ARP_PROTOCOL_LENGTH] = 0x04;

  tx_buff[0 + INDEX_ARP_OPCODE] = 0x00;
  tx_buff[1 + INDEX_ARP_OPCODE] = 0x02;

  for (int i = 0; i < 6; i++) tx_buff[i + INDEX_ARP_SENDER_MAC_ADDR] = my_mac[i];
  for (int i = 0; i < 4; i++) tx_buff[i + INDEX_ARP_SENDER_IP_ADDR] = my_ip[i];
  for (int i = 0; i < 6; i++) tx_buff[i + INDEX_ARP_TARGET_MAC_ADDR] = arp_rx_dst_mac[i];
  for (int i = 0; i < 4; i++) tx_buff[i + INDEX_ARP_TARGET_IP_ADDR] = arp_rx_dst_ip[i];
}
