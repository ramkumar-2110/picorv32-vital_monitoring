#include "uart.h"

#define UART_BASE       0x20000000U
#define UART_DATA       (*(volatile uint32_t *)(UART_BASE + 0x00U))
#define UART_STATUS     (*(volatile uint32_t *)(UART_BASE + 0x04U))

#define UART_TX_BUSY    (1U << 0)
#define UART_RX_VALID   (1U << 1)

void uart_putc(char c)
{
    if (c == '\n')
        uart_putc('\r');

    while (UART_STATUS & UART_TX_BUSY)
    {
    }

    UART_DATA = (uint8_t)c;
}

void uart_puts(const char *str)
{
    while (*str)
        uart_putc(*str++);
}

int uart_getc(void)
{
    while (!(UART_STATUS & UART_RX_VALID))
    {
    }

    return (int)(UART_DATA & 0xFFU);
}

int uart_getc_timeout(uint32_t timeout)
{
    while (timeout--)
    {
        if (UART_STATUS & UART_RX_VALID)
            return (int)(UART_DATA & 0xFFU);
    }

    return -1;
}

int uart_gets(char *buffer, uint32_t size)
{
    uint32_t i = 0;

    if (buffer == 0 || size == 0)
        return -1;

    while (i < size - 1U)
    {
        int c = uart_getc();

        if (c == '\r' || c == '\n')
        {
            uart_putc('\n');
            break;
        }

        if (c == 8 || c == 127)
        {
            if (i > 0)
            {
                i--;
                uart_puts("\b \b");
            }
            continue;
        }

        buffer[i++] = (char)c;
        uart_putc((char)c);
    }

    buffer[i] = '\0';
    return (int)i;
}
