#ifndef ETHER_SHIELD_H
#define ETHER_SHIELD_H

#include "Arduino.h"

#define BUFFER_SIZE 250

void packet_send(uint16_t len, uint8_t* packet);
void eth_init(uint8_t *my_mac, uint8_t *my_ip);
uint16_t packet_receive(unsigned char *buff, uint16_t len);
void ether_shield_led();


#endif
