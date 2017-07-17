#!/bin/bash
arm-none-eabi-gcc -fpic -ffreestanding -std=gnu99 -c board.c -o board.o -O2 -Wall -Wextra
arm-none-eabi-gcc -fpic -ffreestanding -std=gnu99 -c main.c -o main.o -O2 -Wall -Wextra
arm-none-eabi-gcc -T linker.ld -o myos.elf -ffreestanding -O2 -nostdlib board.o main.o
arm-none-eabi-objcopy myos.elf -O binary myos.bin

