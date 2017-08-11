
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : run.bin

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
	rm -f *.list

start.o : start.s
	$(ARMGNU)-as start.s -o start.o

uart.o : uart.c
	$(ARMGNU)-gcc $(COPS) -c uart.c -o uart.o

main.o : main.c
	$(ARMGNU)-gcc $(COPS) -c main.c -o main.o

run.bin : linker.ld start.o uart.o main.o
	$(ARMGNU)-ld start.o uart.o main.o -T linker.ld -o run.elf
	$(ARMGNU)-objdump -D run.elf > run.list
	$(ARMGNU)-objcopy run.elf -O binary run.bin

