#include "i2c.h"

#define I2C_WAIT_LIMIT 1000000u

static int i2c_wait_transfer(void)
{
    uint32_t timeout = I2C_WAIT_LIMIT;

    while ((I2C_SR & I2C_SR_TIP) != 0u) {
        if (--timeout == 0u)
            return -1;
    }

    if ((I2C_SR & I2C_SR_AL) != 0u)
        return -2;

    return 0;
}

static int i2c_send_command(uint32_t command)
{
    I2C_CR = command;

    if (i2c_wait_transfer() != 0)
        return -1;

    if ((I2C_SR & I2C_SR_IF) != 0u)
        I2C_CR = I2C_CR_IACK;

    return 0;
}

static int i2c_check_ack(void)
{
    return ((I2C_SR & I2C_SR_RXACK) == 0u) ? 0 : -1;
}

int i2c_init(uint32_t clk_hz, uint32_t scl_hz)
{
    if (clk_hz == 0u || scl_hz == 0u)
        return -1;

    if (scl_hz > clk_hz / 5u)
        return -1;

    uint32_t prescale = clk_hz / (5u * scl_hz);

    if (prescale == 0u)
        return -1;

    prescale--;

    if (prescale > 0xFFFFu)
        return -1;

    I2C_CTR = 0u;

    I2C_PRERLO = prescale & 0xFFu;
    I2C_PRERHI = (prescale >> 8) & 0xFFu;

    I2C_CTR = I2C_CTR_EN;

    return 0;
}

int i2c_write_register(
    uint8_t slave_address,
    uint8_t reg_address,
    uint8_t data)
{
    if (slave_address > 0x7Fu)
        return -1;

    I2C_TXR = ((uint32_t)slave_address << 1);

    if (i2c_send_command(I2C_CR_STA | I2C_CR_WR) != 0)
        goto error;

    if (i2c_check_ack() != 0)
        goto error;

    I2C_TXR = reg_address;

    if (i2c_send_command(I2C_CR_WR) != 0)
        goto error;

    if (i2c_check_ack() != 0)
        goto error;

    I2C_TXR = data;

    if (i2c_send_command(I2C_CR_WR) != 0)
        goto error;

    if (i2c_check_ack() != 0)
        goto error;

    if (i2c_send_command(I2C_CR_STO) != 0)
        return -2;

    return 0;

error:
    I2C_CR = I2C_CR_STO;
    return -2;
}

int i2c_read_register(
    uint8_t slave_address,
    uint8_t reg_address,
    uint8_t *data)
{
    if (slave_address > 0x7Fu || data == 0)
        return -1;

    /* Send slave address with write direction. */
    I2C_TXR = ((uint32_t)slave_address << 1);

    if (i2c_send_command(I2C_CR_STA | I2C_CR_WR) != 0)
        goto error;

    if (i2c_check_ack() != 0)
        goto error;

    /* Select the slave's register. */
    I2C_TXR = reg_address;

    if (i2c_send_command(I2C_CR_WR) != 0)
        goto error;

    if (i2c_check_ack() != 0)
        goto error;

    /* Repeated START, followed by the slave read address. */
    I2C_TXR = ((uint32_t)slave_address << 1) | 1u;

    if (i2c_send_command(I2C_CR_STA | I2C_CR_WR) != 0)
        goto error;

    if (i2c_check_ack() != 0)
        goto error;

    /* Read one byte, send NACK, and generate STOP. */
    if (i2c_send_command(I2C_CR_RD |
                         I2C_CR_ACK |
                         I2C_CR_STO) != 0)
        goto error;

    *data = (uint8_t)I2C_RXR;

    return 0;

error:
    I2C_CR = I2C_CR_STO;
    return -2;
}

int i2c_read_burst(
    uint8_t slave_address,
    uint8_t reg_address,
    uint8_t *data,
    uint32_t length)
{
    if (slave_address > 0x7Fu || data == 0 || length == 0u)
        return -1;

    /* Send the slave address in write mode. */
    I2C_TXR = ((uint32_t)slave_address << 1);

    if (i2c_send_command(I2C_CR_STA | I2C_CR_WR) != 0)
        goto error;

    if (i2c_check_ack() != 0)
        goto error;

    /* Select the register to read. */
    I2C_TXR = reg_address;

    if (i2c_send_command(I2C_CR_WR) != 0)
        goto error;

    if (i2c_check_ack() != 0)
        goto error;

    /* Repeated START followed by the slave read address. */
    I2C_TXR = ((uint32_t)slave_address << 1) | 1u;

    if (i2c_send_command(I2C_CR_STA | I2C_CR_WR) != 0)
        goto error;

    if (i2c_check_ack() != 0)
        goto error;

    /* Read each byte. NACK and STOP after the final byte. */
    for (uint32_t i = 0; i < length; i++) {
        uint32_t command = I2C_CR_RD;

        if (i == length - 1u)
            command |= I2C_CR_ACK | I2C_CR_STO;

        if (i2c_send_command(command) != 0)
            goto error;

        data[i] = (uint8_t)I2C_RXR;
    }

    return 0;

error:
    I2C_CR = I2C_CR_STO;
    return -2;
}
