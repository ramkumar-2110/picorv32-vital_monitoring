#include "fir.h"

#define FIR_WAIT_LIMIT  10000000u

int fir_step(uint8_t channel, int16_t input, int16_t *output)
{
    if (channel >= FIR_CHANNELS || output == 0)
        return -1;

    uint32_t timeout = FIR_WAIT_LIMIT;

    while ((FIR_STATUS & FIR_STATUS_BUSY) != 0u) {
        if (--timeout == 0u)
            return -2;
    }

    FIR_DATA_IN = (uint16_t)input;

    FIR_CTRL = ((uint32_t)channel << FIR_CTRL_CHANNEL_SHIFT)
             | FIR_CTRL_START;

    timeout = FIR_WAIT_LIMIT;

    while ((FIR_STATUS & FIR_STATUS_DONE) == 0u) {
        if (--timeout == 0u)
            return -3;
    }

    *output = (int16_t)(FIR_DATA_OUT & 0xFFFFu);

    FIR_STATUS = FIR_STATUS_DONE;

    return 0;
}

int fir_load_coefficients(
    const int16_t coefficients[FIR_CHANNELS][FIR_TAPS])
{
    if (coefficients == 0)
        return -1;

    for (uint32_t channel = 0; channel < FIR_CHANNELS; channel++) {
        for (uint32_t tap = 0; tap < FIR_TAPS; tap++) {
            FIR_COEF_ADDR = (channel << 6) | tap;
            FIR_COEF_DATA = (uint16_t)coefficients[channel][tap];
        }
    }

    /*
     * Clear previous filter history by processing 64 zero
     * samples through each channel.
     */
    for (uint32_t channel = 0; channel < FIR_CHANNELS; channel++) {
        for (uint32_t tap = 0; tap < FIR_TAPS; tap++) {
            int16_t output;

            if (fir_step((uint8_t)channel, 0, &output) != 0)
                return -2;
        }
    }

    return 0;
}
