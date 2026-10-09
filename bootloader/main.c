// VitalSoC resident UART bootloader
// Target: PicoRV32 on Boolean FPGA
// UART: 115200 baud, 8N1
//
// Upload protocol:
//   1. Host sends 4-byte little-endian image length.
//   2. Host sends the raw application binary.
//   3. Bootloader sends 'K' after receiving the image.
//   4. Bootloader jumps to the application entry point.

#include <stdint.h>

#define UART_DATA       (*(volatile uint32_t *)0x20000000u)
#define UART_STATUS     (*(volatile uint32_t *)0x20000004u)
#define LED_REG         (*(volatile uint32_t *)0x30000000u)

#define APP_BASE        0x10000000u
#define APP_SIZE        0x00010000u   // Current RTL: 8 KiB
#define APP_MAX_SIZE    0x0000F000u   // Reserve 2 KiB for the stack

#define TX_BUSY         (1u << 0)
#define RX_VALID        (1u << 1)

typedef void (*app_entry_t)(void);


//Wait for one received byte.
//RX_VALID is bit 1 of UART_STATUS in the uploaded top.v.
//Reading UART_DATA acknowledges the received byte.
static uint8_t uart_getc(void)
{
    while ((UART_STATUS & RX_VALID) == 0u)
        ;

    return (uint8_t)UART_DATA;
}


// Transmit one byte.
//Wait until the transmitter is idle before writing UART_DATA.
static void uart_putc(uint8_t c)
{
    while ((UART_STATUS & TX_BUSY) != 0u)
        ;

    UART_DATA = c;
}

int main(void)
{
    // LED pattern: waiting for an application upload.
    LED_REG = 0x0001u;

    // Receive the 32-bit image length, little-endian.
    uint32_t len = 0;

    len |= (uint32_t)uart_getc();
    len |= (uint32_t)uart_getc() << 8;
    len |= (uint32_t)uart_getc() << 16;
    len |= (uint32_t)uart_getc() << 24;

    // Reject empty or oversized images before writing application RAM.
    if (len == 0u || len > APP_MAX_SIZE) {
        LED_REG = 0x00FFu;

        // Invalid upload: remain in the bootloader.
        while (1)
            ;
    }

    // LED pattern: receiving the application.
    LED_REG = 0x0003u;

    volatile uint8_t *dst = (volatile uint8_t *)APP_BASE;

    for (uint32_t i = 0; i < len; i++)
        dst[i] = uart_getc();

    // LED pattern: upload completed.
    LED_REG = 0xFFFFu;

    // Acknowledge successful reception.
    uart_putc('K');

    // Transfer execution to the uploaded application's entry point.
    ((app_entry_t)APP_BASE)();

    // The application should not return to the bootloader.
    while (1)
        ;
}
