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

emmc.o : emmc.c
	$(CC) $(COPS) -c emmc.c -o emmc.o

mmio.o : mmio.c
	$(CC) $(COPS) -c mmio.c -o mmio.o

malloc.o : malloc.c
	$(CC) $(COPS) -c malloc.c -o malloc.o

stdio.o : stdio.c
	$(CC) $(COPS) -c stdio.c -o stdio.o

printf.o : printf.c
	$(CC) $(COPS) -c  printf.c -o  printf.o

mbox.o : mbox.c
	$(CC) $(COPS) -c mbox.c -o mbox.o

timer.o : timer.c
	$(CC) $(COPS) -c timer.c -o timer.o

uart.o : uart.c
	$(CC) $(COPS) -c uart.c -o uart.o

stdlib.o : stdlib.c
	$(CC) $(COPS) -c stdlib.c -o stdlib.o

assert.o : assert.c
	$(CC) $(COPS) -c assert.c -o assert.o

main.o : main.c
	$(CC) $(COPS) -c main.c -o main.o

kernel.img : linker.ld entry.o __aeabi_uidiv.o __aeabi_uidivmod.o assert.o uart.o malloc.o stdio.o stdlib.o printf.o timer.o mbox.o mmio.o emmc.o main.o
	$(LD) entry.o __aeabi_uidiv.o __aeabi_uidivmod.o assert.o uart.o malloc.o stdio.o stdlib.o printf.o timer.o mbox.o mmio.o emmc.o main.o $(LIBS) -T linker.ld -o kernel.elf
	$(OBJDUMP) -D kernel.elf > kernel.list
	$(OBJCOPY) kernel.elf -O binary kernel.img

.PHONY: clean

clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.img

