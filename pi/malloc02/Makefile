CROSSGNU = arm-none-eabi-
CC = $(CROSSGNU)gcc
AS = $(CROSSGNU)as
LD = $(CROSSGNU)ld
OBJCOPY = $(CROSSGNU)objcopy
OBJDUMP = $(CROSSGNU)objdump

LIBS = 

COPS = -O2 -nostdlib -nostartfiles -ffreestanding -fno-builtin -march=armv6zk -mtune=arm1176jzf-s -I. -std=gnu99 

all : kernel.img

entry.o : entry.S
	$(CC) $(COPS) -c entry.S -o entry.o

__aeabi_uidiv.o : __aeabi_uidiv.c
	$(CC) $(COPS) -c __aeabi_uidiv.c -o __aeabi_uidiv.o

__aeabi_uidivmod.o : __aeabi_uidivmod.S
	$(CC) $(COPS) -c __aeabi_uidivmod.S -o __aeabi_uidivmod.o

printf.o : printf.c
	$(CC) $(COPS) -c printf.c -o printf.o

malloc.o : malloc.c
	$(CC) $(COPS) -c malloc.c -o malloc.o

uart.o : uart.c
	$(CC) $(COPS) -c uart.c -o uart.o

main.o : main.c
	$(CC) $(COPS) -c main.c -o main.o

kernel.img : linker.ld entry.o __aeabi_uidiv.o __aeabi_uidivmod.o uart.o printf.o malloc.o main.o
	$(LD) entry.o __aeabi_uidiv.o __aeabi_uidivmod.o uart.o printf.o malloc.o main.o $(LIBS) -T linker.ld -o kernel.elf
	$(OBJDUMP) -D kernel.elf > kernel.list
	$(OBJCOPY) kernel.elf -O binary kernel.img

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.img

