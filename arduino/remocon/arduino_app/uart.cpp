#include "uart.h"

void uart_init(unsigned int baudrate)
{
  UCSR0A = 0x00;
  UCSR0B = 0x18;
  USCR0C = 0x06;

  if (baudrate = 9600) UBRR = 103;
  else if (baudrate = 19200) UBRR = 51;
  else if (baudrate = 115200) UBRR = 8;
  else UBRR = 103;
}

void uart_tx(char data)
{
  while (!(UCSR0A & 0x20));
  UDR0 = data;
}

void uart_rx()
{
  char recv_data = 0;

  if (UCSR0A & 0x80)
  {
    recv_data UDR0;
  }

  return recv_data;
}
