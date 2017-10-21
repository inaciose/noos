#include "config.h"
#include "block.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

// uart.c
int uart_putc(int);
void _puts(const char *);
void uart_init(void);

// printf.c
int printf(const char *format, ...);
// printf() expect this function
// configurable on printf.c (at top) 
void _putc(int c, void *stream)
{
   uart_putc(c);
}

// malloc.c
void init_heap(unsigned int, unsigned int);
void print_heap_range( void );

// emmc.c
int sd_card_init(struct block_device **dev);

// vfs.c
void vfs_init( void );

// mbr.c
int read_mbr(struct block_device *parent, struct block_device ***partitions, int *part_count);

// needed in mmio (dont know here it is init)
const unsigned int base_adjust = 0;

// mallooc heap start
extern unsigned int _end;
unsigned int *end = &_end;

int cmain(void) {
  uart_init();
  _puts("... hello\n");
  _puts("start sdcrp02\n");

  init_heap((unsigned int)end, 0x8000000);
  print_heap_range();  
  
  #ifdef ENABLE_SD
  struct block_device *sd_dev = NULL;
  if(sd_card_init(&sd_dev) == 0)
  {
    struct block_device *c_dev = sd_dev;
    _puts("sd_card_init\n");
  #ifdef ENABLE_BLOCK_CACHE
    uintptr_t cache_start = alloc_buf(BLOCK_CACHE_SIZE);
    if(cache_start != 0)
      cache_init(sd_dev, &c_dev, cache_start, BLOCK_CACHE_SIZE);
  #endif
  #ifdef ENABLE_MBR
    //vfs_init(); // not needed if -02 flag is removed from gcc
    read_mbr(c_dev, (void*)0, (void*)0);
    _puts("read_mbr\n");
  #endif
  }
  #endif

  // list devices (will be empty)
  _puts("MAIN: device list: ");
  vfs_list_devices();
  _puts("end\n");
  _puts("end\n");
    
  while(1) {}
  return -1;
}