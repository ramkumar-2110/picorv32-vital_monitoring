#include "gpio.h"

#define GPIO_BASE  0x30000000U

#define GPIO_LED   (*(volatile uint32_t *)(GPIO_BASE + 0x00U))
#define GPIO_SW    (*(volatile uint32_t *)(GPIO_BASE + 0x04U))
#define GPIO_BTN   (*(volatile uint32_t *)(GPIO_BASE + 0x08U))

void gpio_led_write(uint16_t value)
{
    GPIO_LED = value;
}

uint16_t gpio_switch_read(void)
{
    return (uint16_t)GPIO_SW;
}

uint8_t gpio_button_read(void)
{
    return (uint8_t)GPIO_BTN;
}
