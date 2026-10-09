#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void gpio_led_write(uint16_t value);
uint16_t gpio_switch_read(void);
uint8_t gpio_button_read(void);

#endif
