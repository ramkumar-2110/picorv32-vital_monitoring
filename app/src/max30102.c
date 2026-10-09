#include "max30102.h"
#include "i2c.h"

#define MAX30102_ADDR       0x57u

#define REG_INT_STATUS_1    0x00u
#define REG_FIFO_WR_PTR     0x04u
#define REG_OVF_COUNTER     0x05u
#define REG_FIFO_RD_PTR     0x06u
#define REG_FIFO_DATA       0x07u
#define REG_FIFO_CONFIG     0x08u
#define REG_MODE_CONFIG     0x09u
#define REG_SPO2_CONFIG     0x0Au
#define REG_LED1_PA         0x0Cu
#define REG_LED2_PA         0x0Du
#define REG_PART_ID         0xFFu

#define MAX30102_PART_ID    0x15u

int max30102_init(void)
{
    uint8_t id;

    /* Configure a 100 kHz I2C bus with a 100 MHz system clock. */
    if (i2c_init(100000000u, 100000u) != 0)
        return -1;

    /* Verify the connected sensor. */
    if (i2c_read_register(
            MAX30102_ADDR, REG_PART_ID, &id) != 0)
        return -2;

    if (id != MAX30102_PART_ID)
        return -3;

    /* Reset the sensor. */
    if (i2c_write_register(
            MAX30102_ADDR, REG_MODE_CONFIG, 0x40u) != 0)
        return -4;

    /* Allow the reset to complete. */
    for (volatile uint32_t i = 0; i < 100000u; i++)
        ;

    /* Clear the FIFO pointers and overflow counter. */
    if (i2c_write_register(
            MAX30102_ADDR, REG_FIFO_WR_PTR, 0u) != 0)
        return -5;

    if (i2c_write_register(
            MAX30102_ADDR, REG_OVF_COUNTER, 0u) != 0)
        return -6;

    if (i2c_write_register(
            MAX30102_ADDR, REG_FIFO_RD_PTR, 0u) != 0)
        return -7;

    /* FIFO configuration: averaging and rollover settings. */
    if (i2c_write_register(
            MAX30102_ADDR, REG_FIFO_CONFIG, 0x4Fu) != 0)
        return -8;

    /* SpO2 mode enables red and IR measurements. */
    if (i2c_write_register(
            MAX30102_ADDR, REG_MODE_CONFIG, 0x03u) != 0)
        return -9;

    /* Configure ADC range, sample rate and pulse width. */
    if (i2c_write_register(
            MAX30102_ADDR, REG_SPO2_CONFIG, 0x27u) != 0)
        return -10;

    /* Set red and IR LED current. */
    if (i2c_write_register(
            MAX30102_ADDR, REG_LED1_PA, 0x24u) != 0)
        return -11;

    if (i2c_write_register(
            MAX30102_ADDR, REG_LED2_PA, 0x24u) != 0)
        return -12;

    /* Clear pending sensor status. */
    if (i2c_read_register(
            MAX30102_ADDR, REG_INT_STATUS_1, &id) != 0)
        return -13;

    return 0;
}

int max30102_samples_available(uint8_t *count)
{
    uint8_t wr;
    uint8_t rd;

    if (count == 0)
        return -1;

    if (i2c_read_register(
            MAX30102_ADDR, REG_FIFO_WR_PTR, &wr) != 0)
        return -2;

    if (i2c_read_register(
            MAX30102_ADDR, REG_FIFO_RD_PTR, &rd) != 0)
        return -3;

    /* FIFO pointers wrap around at 32 entries. */
    *count = (uint8_t)((wr - rd) & 0x1Fu);

    return 0;
}

int max30102_read(uint32_t *red, uint32_t *ir)
{
    uint8_t count;
    uint8_t sample[6];

    if (red == 0 || ir == 0)
        return -1;

    if (max30102_samples_available(&count) != 0)
        return -2;

    /* Return 1 when no complete sample is available. */
    if (count == 0u)
        return 1;

    /* Read red (3 bytes) followed by IR (3 bytes). */
    if (i2c_read_burst(
            MAX30102_ADDR,
            REG_FIFO_DATA,
            sample,
            6u) != 0)
        return -3;

    /* Combine the three red bytes into an 18-bit value. */
    *red = (((uint32_t)sample[0] << 16) |
            ((uint32_t)sample[1] << 8)  |
            (uint32_t)sample[2]) & 0x3FFFFu;

    /* Combine the three IR bytes into an 18-bit value. */
    *ir = (((uint32_t)sample[3] << 16) |
           ((uint32_t)sample[4] << 8)  |
           (uint32_t)sample[5]) & 0x3FFFFu;

    return 0;
}
