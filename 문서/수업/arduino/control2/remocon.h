#ifndef REMOCON_H
#define REMOCON_H

#define REMOCON_PIN 3

#include "Arduino.h"

void remocon_init();
char c_remocon();
void set_remocon(char len);

#endif
