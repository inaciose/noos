CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
CFLAGS = -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra -mcpu=cortex-a7 \
	-Wl,-Tlinker.ld -nostartfiles \

TARGET = myos.bin
all: $(TARGET)

$(TARGET): main.c interrupts.c gicv2.c timer.c uart.c boot.S
	$(CC) $(CFLAGS) $^ -o myos.elf
	$(CROSS_COMPILE)objcopy -Obinary myos.elf myos.bin
	$(CROSS_COMPILE)objdump -S myos.elf > myos.list

clean:
	rm -f *.o *.elf *.bin *.list

