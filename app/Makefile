TOOLCHAIN_PREFIX = riscv-none-elf-

CFLAGS  = -march=rv32i -mabi=ilp32 -mno-relax -O2 -ffreestanding -Wall -Iincludes
LDFLAGS = -T sections.ld -nostdlib -Wl,--no-relax 

all: app.bin

app.elf: start.s main.c sections.ld src/fir.c src/i2c.c src/timer.c src/max30102.c src/gpio.c src/uart.c includes/timer.h includes/i2c.h includes/fir.h includes/max30102.h includes/gpio.h includes/uart.h
	$(TOOLCHAIN_PREFIX)gcc $(CFLAGS) $(LDFLAGS) -o app.elf start.s main.c src/fir.c src/i2c.c src/timer.c src/max30102.c src/gpio.c src/uart.c -lgcc


app.bin: app.elf
	$(TOOLCHAIN_PREFIX)objcopy -O binary app.elf app.bin

clean:
	rm -f app.elf app.bin

.PHONY: all clean
