#ifndef ETHERNET_H
#define ETHERNET_H

#include "Arduino.h"

#define INDEX_ETHERNET_DESTINATION_MAC_ADDR   0
#define INDEX_ETHERNET_SOURCE_MAC_ADDR        6
#define INDEX_ETHERNET_TYPE                   12

void print_ethernet_head(char *rx_buff);

#endif
