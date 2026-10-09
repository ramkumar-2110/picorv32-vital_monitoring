/*
file name : timer.c
Descrition : Definition of the functions for timer
 */
#include "timer.h"

//init the timer with system clock frequency 
void timer_init(uint32_t clk_hz, uint32_t tick_hz)
{
    if (clk_hz == 0u || tick_hz == 0u || tick_hz > clk_hz)
        return;

    timer_disable();

    TIMER_PERIOD = (clk_hz / tick_hz) - 1u;
    TIMER_FLAG = TIMER_FLAG_TICK;

    TIMER_CTRL = 0u;
}

void timer_enable(void)
{
    TIMER_CTRL |= TIMER_CTRL_ENABLE;
}

void timer_disable(void)
{
    TIMER_CTRL &= ~TIMER_CTRL_ENABLE;
}

void timer_clear_flag(void)
{
    TIMER_FLAG = TIMER_FLAG_TICK;
}

uint32_t timer_get_ticks(void)
{
    return TIMER_TICKS;
}

uint8_t timer_tick_pending(void)
{
    return (TIMER_FLAG & TIMER_FLAG_TICK) != 0u;
}
void timer_delay_ms(uint32_t ms)
{
    while (ms--)
    {
        /* Wait until the timer sets its tick flag */
        while ((TIMER_FLAG & 1U) == 0U)
        {
        }

        /* Clear the tick flag (write-one-to-clear) */
        TIMER_FLAG = 1U;
    }
}
