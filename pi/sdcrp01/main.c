#ifndef NULL
#define NULL ((void*)0)
#endif

#include "block.h"

// uart.c
int uart_putc(int);
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

// needed in mmio
const unsigned int base_adjust = 0;

// malloc heap start
extern unsigned int _end;
unsigned int *end = &_end;

int cmain(void) {
  uart_init();
  printf("... hello\n");
  printf("start sdcrp01\n");

  // malloc
  init_heap((unsigned int)end, 0x8000000);
  print_heap_range();  
  
  // sd init
  struct block_device *sd_dev = NULL;
  if(sd_card_init(&sd_dev) == 0)
  {
    struct block_device *c_dev = sd_dev;
  }
  
  printf("sd_card_init done\n");
  printf("end\n");
  
  while(1) {}
  return -1;
}
