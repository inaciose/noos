sdcrp01

This is the sd card implementation found on jncronin/rpi-boot.

This sample is the first of the sdcrpXX series dedicated to reading and writing 
files on sd card using the code found on  rpi-boot.

It is not a modified version or wrapper, but the original John Cronin code
extracted and reassembled to a more or less minimal version of its sdcard access library.

The following post on raspberrypi forums talks about this implementation, its
errors, and the derivate works with some solutions.
https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=94133

None of the errors talked in the post is corrected in this version.
Almost all of the orginal code is preserved.

Asm files:
__aeabi_uidivmod.S
entry.S

C files:
__aeabi_uidiv.c
assert.c
emmc.c
main.c
malloc.c
mbox.c
mmio.c
printf.c
stdio.c
stdlib.c
timer.c
uart.c

H files:
assert.h
block.h
dirent.h
errno.h
fs.h
mbox.h
mmio.h
multiboot.h
output.h
stdio.h
stdlib.h
timer.h
uart.h
util.h
vfs.h

Highlight:
emmc.c, the sd card library

The sd card library requires the following helper code to work:
- malloc
- mailbox access
- stdio 
- stdlib
They are included from same work except malloc that its substitued by a simple
malloc sample.

Functions:

static void sd_power_off();
static uint32_t sd_get_base_clock_hz();
static int bcm_2708_power_on();
static int bcm_2708_power_cycle();
static uint32_t sd_get_clock_divider(uint32_t base_clock, uint32_t target_rate);
static int sd_switch_clock_rate(uint32_t base_clock, uint32_t target_rate);
static int sd_reset_cmd();
static int sd_reset_dat();
static void sd_issue_command_int(struct emmc_block_dev *dev, uint32_t cmd_reg, uint32_t argument, useconds_t timeout);
static void sd_handle_card_interrupt(struct emmc_block_dev *dev);
static void sd_handle_interrupts(struct emmc_block_dev *dev);
static void sd_issue_command(struct emmc_block_dev *dev, uint32_t command, uint32_t argument, useconds_t timeout);
int sd_card_init(struct block_device **dev);
static int sd_ensure_data_mode(struct emmc_block_dev *edev);
static int sd_suitable_for_dma(void *buf);
static int sd_do_data_command(struct emmc_block_dev *edev, int is_write, uint8_t *buf, size_t buf_size, uint32_t block_no);
int sd_read(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t block_no);
int sd_write(struct block_device *dev, uint8_t *buf, size_t buf_size, uint32_t block_no);
