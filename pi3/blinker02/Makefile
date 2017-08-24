
ARMGNU ?= arm-none-eabi
#ARMGNU ?= arm-linux-gnueabi

COPS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding 

gcc : run.bin

all : gcc

clean :
	rm -f *.o
	rm -f *.bin
	rm -f *.hex
	rm -f *.elf
	rm -f *.list
	rm -f *.img

start.o : start.s
	$(ARMGNU)-as start.s -o start.o

greenled.o : greenled.s
	$(ARMGNU)-gcc $(COPS) -c greenled.s -o greenled.o

main.o : main.c
	$(ARMGNU)-gcc $(COPS) -c main.c -o main.o

run.bin : memmap start.o greenled.o main.o
	$(ARMGNU)-ld start.o greenled.o main.o -T memmap -o run.elf
	$(ARMGNU)-objdump -D run.elf > run.list
	$(ARMGNU)-objcopy run.elf -O binary run.bin

