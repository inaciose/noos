// entry.S
unsigned int get_cpsr (void);

// uart.c
void uart_putc(int);
void uart_puts(const char *);
void uart_init(void);
void hexstring ( unsigned int, const char *);

//printf.c
void printf(const char *, ...);
// printf() expect this function
// configurable on printf.c (at top) 
void putc(int c, void *stream)
{
   uart_putc(c);
}

// malloc.c
#include "malloc.h"
void memman_init(struct MEMMAN *);
int memman_free(struct MEMMAN *, unsigned int, unsigned int);
void memman_monitor(struct MEMMAN *);
unsigned int memman_total(struct MEMMAN *);
void *malloc (unsigned);
void free (void *);


// bof expected functions
void cli( void )
{
  asm volatile("cpsid i");
}

void sti( void )
{
  asm volatile("cpsie i");
}

unsigned int hold_IRQ( void )
{
  unsigned int cpsr;
  cpsr = get_cpsr();
  if (!(cpsr & 0x00000080)) {
    //cli();
    return 1;
  } else {
    return 0;
  }
}

void unhold_IRQ(unsigned int irq_status)
{
  if (irq_status) {
    //sti();
  }
}
// eof expected functions

int cmain(void) {
  
  struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
  unsigned int memtotal;
  
  uart_init();
  uart_puts("... hello\n");
  uart_puts("start sdchb01 sample\n");
  
  // get max memory address
  memtotal = mailbox_getMemorySize();
  printf("mem total: 0x%x\n", memtotal);

  // allocate memory to heap
  memman_init(memman);
  memman_free(memman, 0x00400000, memtotal - 0x00400000);

  // allocation feedback
  unsigned int mem1 = memman_total(memman);
  printf("mem available: 0x%x\n", mem1);  
  memman_monitor(memman);
  
  init_filesystem(memman);

  uart_puts("end sample\n");
  
  while(1) {}
  return -1;
}