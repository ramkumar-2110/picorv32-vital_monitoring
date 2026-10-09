#ifndef FIR_H
#define FIR_H

#include <stdint.h>

#define FIR_BASE          0x50000000u

#define FIR_CTRL          (*(volatile uint32_t *)(FIR_BASE + 0x00u))
#define FIR_STATUS        (*(volatile uint32_t *)(FIR_BASE + 0x04u))
#define FIR_DATA_IN       (*(volatile uint32_t *)(FIR_BASE + 0x08u))
#define FIR_DATA_OUT      (*(volatile uint32_t *)(FIR_BASE + 0x0Cu))
#define FIR_COEF_ADDR     (*(volatile uint32_t *)(FIR_BASE + 0x10u))
#define FIR_COEF_DATA     (*(volatile uint32_t *)(FIR_BASE + 0x14u))

#define FIR_CTRL_START    (1u << 0)
#define FIR_CTRL_IRQ_EN   (1u << 1)
#define FIR_CTRL_CHANNEL_SHIFT  8u

#define FIR_STATUS_BUSY   (1u << 0)
#define FIR_STATUS_DONE   (1u << 1)

#define FIR_CHANNELS      4u
#define FIR_TAPS          64u

int fir_step(uint8_t channel, int16_t input, int16_t *output);

int fir_load_coefficients(
    const int16_t coefficients[FIR_CHANNELS][FIR_TAPS]);

#endif
