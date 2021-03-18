#include "uart.h"

void uart_init(unsigned int baudrate)
{
  UCSR0A = 0x00;
  UCSR0B = 0x18;
  UCSR0C = 0x06;

  if (baudrate = 9600) UBRR0 = 103;
  else if (baudrate = 19200) UBRR0 = 51;
  else if (baudrate = 115200) UBRR0 = 8;
  else UBRR0 = 103;
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
    recv_data = UDR0;
  }

  return recv_data;
}

void uart_tx_string(char *string)
{
  int str_len = strlen(string);

  for (int i = 0; i < str_len; i++)
  {
    uart_tx(*(string + i));
  }
}

void printh(char *format, ...)
{
  char buf[128];
  va_list arglist;

  va_start(arglist, format);
  vsprintf(buf, format, arglist);
  va_end(arglist);

  uart_tx_string(buf);
}
