#ifndef I2C_H
#define I2C_H

#include <stdint.h>

#define I2C_BASE       0x60000000u

#define I2C_PRERLO     (*(volatile uint32_t *)(I2C_BASE + 0x00u))
#define I2C_PRERHI     (*(volatile uint32_t *)(I2C_BASE + 0x04u))
#define I2C_CTR        (*(volatile uint32_t *)(I2C_BASE + 0x08u))
#define I2C_TXR        (*(volatile uint32_t *)(I2C_BASE + 0x0Cu))
#define I2C_RXR        (*(volatile uint32_t *)(I2C_BASE + 0x0Cu))
#define I2C_CR         (*(volatile uint32_t *)(I2C_BASE + 0x10u))
#define I2C_SR         (*(volatile uint32_t *)(I2C_BASE + 0x10u))

#define I2C_CTR_EN     (1u << 7)
#define I2C_CTR_IEN    (1u << 6)

#define I2C_CR_STA     (1u << 7)
#define I2C_CR_STO     (1u << 6)
#define I2C_CR_RD      (1u << 5)
#define I2C_CR_WR      (1u << 4)
#define I2C_CR_ACK     (1u << 3)
#define I2C_CR_IACK    (1u << 0)

#define I2C_SR_RXACK   (1u << 7)
#define I2C_SR_BUSY    (1u << 6)
#define I2C_SR_AL      (1u << 5)
#define I2C_SR_TIP     (1u << 1)
#define I2C_SR_IF      (1u << 0)

int i2c_init(uint32_t clk_hz, uint32_t scl_hz);

int i2c_write_register(
    uint8_t slave_address,
    uint8_t reg_address,
    uint8_t data);

int i2c_read_register(
    uint8_t slave_address,
    uint8_t reg_address,
    uint8_t *data);

int i2c_read_burst(
    uint8_t slave_address,
    uint8_t reg_address,
    uint8_t *data,
    uint32_t length);

#endif
