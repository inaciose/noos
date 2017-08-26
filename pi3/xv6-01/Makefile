CROSS=arm-none-eabi-
CC=$(CROSS)gcc
LD=$(CROSS)ld
OBJCOPY=$(CROSS)objcopy
OBJDUMP=$(CROSS)objdump
CFLAGS=-mcpu=cortex-a7 -fno-pic -static -ffreestanding -std=gnu99 -O2 -Wall -Wextra -I.
ASFLAGS=-mcpu=cortex-a7 -fno-pic -ffreestanding -I.
LDFLAGS=-L.
BUILD_DIR=build

OBJS = \
	asm.o\
	cache.o\
	entry.o\
	trap_asm.o\
	swtch.o\
	\
	lib/string.o\
	\
	arm.o\
	picirq.o\
	proc.o\
	start.o\
	spinlock.o\
	syscall.o\
	trap.o\
	timer.o\
	uart.o\
	vm.o\
	greenled.o\
	main.o

quiet-command = $(if $(V),$1,$(if $(2),@echo $2 && $1, @$1))
build-directory = $(shell mkdir -p $(BUILD_DIR) $(BUILD_DIR)/lib)

LINK_BIN = $(call quiet-command,$(LD) $(LDFLAGS) \
    -T $(1) -o $(2) $(3) $(LIBS) -b binary $(4), "[LINK]    $(TARGET_DIR)$@")
AS_WITH = $(call quiet-command,$(CC) $(ASFLAGS) \
        $(1) -c -o $@ $<,"[AS]      $(TARGET_DIR)$@")
LINK_INIT = $(call quiet-command,$(LD) $(LDFLAGS) \
    $(1) -o $@.out $<, "[LINK]    $(TARGET_DIR)$@")
OBJCOPY_INIT = $(call quiet-command,$(OBJCOPY) \
    -S -O binary --prefix-symbols="_binary_$@" $@.out $@, "[OBJCOPY] $(TARGET_DIR)$@")

$(BUILD_DIR)/%.o: %.c
	$(call build-directory)
	$(call quiet-command,$(CC) $(CFLAGS) \
		-c -o $@ $<,"[CC]      $(TARGET_DIR)$@")

$(BUILD_DIR)/%.o: %.S
	$(call build-directory)
	$(call quiet-command,$(CC) $(ASFLAGS) \
		-c -o $@ $<,"[AS]      $(TARGET_DIR)$@")

kernel7.img: $(addprefix $(BUILD_DIR)/, $(OBJS)) kernel.ld build/initcode
	@cp -f build/initcode initcode
	$(call LINK_BIN, kernel.ld, $(BUILD_DIR)/kernel7.elf, \
		$(addprefix $(BUILD_DIR)/, $(OBJS)), \
		initcode)
	@rm initcode
	@$(OBJCOPY) $(BUILD_DIR)/kernel7.elf -O binary kernel7.img
	@echo kernel image has been built.
	@$(OBJDUMP) -d $(BUILD_DIR)/kernel7.elf > $(BUILD_DIR)/kernel7.asm
	#@cp kernel7.img ~/share/kernel7.img


INITCODE_OBJ = initcode.o
$(addprefix $(BUILD_DIR)/,$(INITCODE_OBJ)): initcode.S
	$(call build-directory)
	$(call AS_WITH, -nostdinc -I.)

sd:
	cp kernel7.img /media/removable/USB\ Drive/

build/initcode: $(addprefix $(BUILD_DIR)/,$(INITCODE_OBJ))
	$(call LINK_INIT, -N -e start -Ttext 0)
	$(call OBJCOPY_INIT)
	@$(OBJDUMP) -S $< > $(BUILD_DIR)/initcode.asm

clean:
	rm -rf build
	rm kernel7.img
