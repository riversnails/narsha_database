#ifndef ETHERNET_H
#define ETHERNET_H

#include "Arduino.h"

#define INDEX_ETHERNET_DESTINATION_MAC_ADDR   0
#define INDEX_ETHERNET_SOURCE_MAC_ADDR        6
#define INDEX_ETHERNET_TYPE                   12

#define ARP_PACKET 0x0806
#define IP_PACKET  0x0800

void print_ethernet_head(char *rx_buff);
int which_packet_type(char *rx_buff);
void parsing_ethernet_head(char *rx_buff);
void make_ethernet_head(char *tx_buff, uint8_t *my_mac, int ethernet_type);

#endif
