# osdev - Building and info

arm-none-eabi-gcc -fpic -ffreestanding -c boot.S -o boot.o

arm-none-eabi-gcc -fpic -ffreestanding -std=gnu99 -c kernel.c -o kernel.o -O2 -Wall -Wextra

arm-none-eabi-gcc -T linker.ld -o myos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o

arm-none-eabi-objcopy myos.elf -O binary myos.bin

Building in banana pi we can skip the -mcpu=

This code its recoded to banana pi from the following tutorial: http://wiki.osdev.org/ARM_RaspberryPi_Tutorial_C
