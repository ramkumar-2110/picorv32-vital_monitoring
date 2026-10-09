#ifndef MAX30102_H
#define MAX30102_H

#include <stdint.h>

int max30102_init(void);
int max30102_samples_available(uint8_t *count);
int max30102_read(uint32_t *red, uint32_t *ir);

#endif
