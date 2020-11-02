#ifndef UART_PRINTF_H
#define UART_PRINTF_H

#include <stdarg.h>

    #define PRINTF_BUF 80 // define the tmp buffer size (change if desired)

       void _printf(const char *format, ...)

       {

       char buf[PRINTF_BUF];

       va_list ap;

            va_start(ap, format);

            vsnprintf(buf, sizeof(buf), format, ap);

            for(char *p = &buf[0]; *p; p++) // emulate cooked mode for newlines

            {

                    if(*p == '\n')
                    	{
				while(!(UCSR0A & 0x20));
				UDR0 = '\r';
                    	}

                    while(!(UCSR0A & 0x20));
		    UDR0 = *p;

            }

            va_end(ap);

       }

    #ifdef F // check to see if F() macro is available

       void _printf(const __FlashStringHelper *format, ...)

       {

       char buf[PRINTF_BUF];

       va_list ap;

            va_start(ap, format);

    #ifdef __AVR__

            vsnprintf_P(buf, sizeof(buf), (const char *)format, ap); // progmem for AVR

    #else

            vsnprintf(buf, sizeof(buf), (const char *)format, ap); // for the rest of the world

    #endif

            for(char *p = &buf[0]; *p; p++) // emulate cooked mode for newlines

            {

                    if(*p == '\n')
                    	{
				while(!(UCSR0A & 0x20));
				UDR0 = '\r';
                    	}
                    while(!(UCSR0A & 0x20));
		    UDR0 = *p;

            }

            va_end(ap);

       }

    #endif

#endif
