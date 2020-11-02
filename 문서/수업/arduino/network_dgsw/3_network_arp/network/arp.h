#ifndef ARP_H
#define ARP_H

#include "Arduino.h"

#define INDEX_ARP_HARDWARE_TYPE     14
#define INDEX_ARP_PROTOCOL_TYPE     16
#define INDEX_ARP_HARDWARE_LENGTH   18
#define INDEX_ARP_PROTOCOL_LENGTH   19
#define INDEX_ARP_OPCODE            20
#define INDEX_ARP_SENDER_MAC_ADDR   22
#define INDEX_ARP_SENDER_IP_ADDR    28
#define INDEX_ARP_TARGET_MAC_ADDR   32
#define INDEX_ARP_TARGET_IP_ADDR    38

void print_arp_head(char *rx_buff);
bool is_my_ip(char *rx_buff, uint8_t *my_ip);
void parsing_arp_head(char *rx_buff);
void make_arp_head(char *tx_buff, uint8_t *my_ip, uint8_t *my_mac);

#endif
