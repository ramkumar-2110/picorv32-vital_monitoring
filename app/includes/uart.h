#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_putc(char c);
void uart_puts(const char *str);

int uart_getc(void);
int uart_getc_timeout(uint32_t timeout);
int uart_gets(char *buffer, uint32_t size);

#endif
