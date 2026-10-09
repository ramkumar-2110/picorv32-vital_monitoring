#include <stdint.h>
#include "max30102.h"
#include "uart.h"

static void uart_print_uint(uint32_t value)
{
    char buf[10];
    uint32_t i = 0;

    if (value == 0u) {
        uart_putc('0');
        return;
    }

    while (value != 0u) {
        buf[i++] = (char)('0' + value % 10u);
        value /= 10u;
    }

    while (i > 0u)
        uart_putc(buf[--i]);
}

int main(void)
{
    uint32_t red;
    uint32_t ir;
    int status;

    uart_puts("\r\n");
    uart_puts("========================\r\n");
    uart_puts(" MAX30102 SENSOR DISPLAY\r\n");
    uart_puts("========================\r\n");

    status = max30102_init();

    if (status != 0) {
        uart_puts("Sensor initialization FAILED\r\n");
        uart_puts("Error code: ");
        uart_print_uint((uint32_t)(-status));
        uart_puts("\r\n");

        while (1)
            ;
    }

    uart_puts("Sensor initialization PASS\r\n");
    uart_puts("Waiting for sensor data...\r\n\r\n");

    while (1) {
        status = max30102_read(&red, &ir);

        if (status == 0) {
            uart_puts("RED: ");
            uart_print_uint(red);

            uart_puts(" | IR: ");
            uart_print_uint(ir);

            uart_puts("\r\n");
        } else if (status < 0) {
            uart_puts("I2C read error\r\n");
        }

        /* Temporary software delay. */
        for (volatile uint32_t i = 0; i < 50000u; i++)
            ;
    }

    return 0;
}
