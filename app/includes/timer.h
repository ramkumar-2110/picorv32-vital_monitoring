/*
file name:timer.h
Descrition: Declaration of functions for timer
*/

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define TIMER_BASE       0x40000000u//base address

#define TIMER_CTRL       (*(volatile uint32_t *)(TIMER_BASE + 0x00u))
#define TIMER_PERIOD     (*(volatile uint32_t *)(TIMER_BASE + 0x04u))
#define TIMER_FLAG       (*(volatile uint32_t *)(TIMER_BASE + 0x08u))
#define TIMER_TICKS      (*(volatile uint32_t *)(TIMER_BASE + 0x0Cu))

#define TIMER_CTRL_ENABLE    (1u << 0)
#define TIMER_CTRL_IRQ_EN    (1u << 1)
#define TIMER_FLAG_TICK      (1u << 0)

void timer_init(uint32_t clk_hz, uint32_t tick_hz);
void timer_delay_ms(uint32_t ms);
void timer_enable(void);
void timer_disable(void);
void timer_clear_flag(void);
uint32_t timer_get_ticks(void);
uint8_t timer_tick_pending(void);

#endif
