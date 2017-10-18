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

// malloc.c expect these functions
// bof functions
unsigned int memtest(unsigned int start, unsigned int end)
{
  return 0x01000000;
}

inline void cli( void )
{
  asm volatile("cpsid i");
}

inline void sti( void )
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
// eof functions

// bof test
struct test_struct1
{
    unsigned int    uia[2];
    unsigned int    uib;
    int             ic;
};

struct test_struct2
{
    unsigned int    uia;
    unsigned int    uib;
};
// eof test

int cmain(void) {
  
  struct MEMMAN *memman = (struct MEMMAN *) MEMMAN_ADDR;
  unsigned int memtotal;
  
  uart_init();
  uart_puts("... hello\n");
  uart_puts("... start malloc02 sample\n");
  
  // get max memory address
  memtotal = memtest(0x00400000, 0xbfffffff);
  printf("mem total: 0x%x\n", memtotal);

  // allocate memory to heap
  memman_init(memman);
  memman_free(memman, 0x00400000, memtotal - 0x00400000);

  // allocation feedback
  unsigned int mem1 = memman_total(memman);
  printf("mem available: 0x%x\n", mem1);  
  memman_monitor(memman);

  // bof alocation test
  uart_puts("malloc\n");

  struct test_struct1 *ret1;
  struct test_struct2 *ret2;
  char *ret3;
  struct test_struct1 *ret4;
  
  ret1 = (struct test_struct1 *) malloc(sizeof(struct test_struct1));
  ret2 = (struct test_struct2 *) malloc(sizeof(struct test_struct2));
  ret3 = (char *) malloc(sizeof(char));
  ret4 = (struct test_struct1 *) malloc(sizeof(struct test_struct1));

  hexstring((unsigned int) ret1, " ");
  hexstring((unsigned int) sizeof(struct test_struct1), "\n");
  hexstring((unsigned int) ret2, " ");
  hexstring((unsigned int) sizeof(struct test_struct2), "\n");
  hexstring((unsigned int) ret3, " ");
  hexstring((unsigned int) sizeof(char), "\n");
  hexstring((unsigned int) ret4, " ");
  hexstring((unsigned int) sizeof(struct test_struct1), "\n");

  mem1 = memman_total(memman);
  printf("mem available: 0x%x\n", mem1);
  memman_monitor(memman);
  
  uart_puts("free\n");
  free(ret1);
  free(ret2);
  free(ret3);
  free(ret4);  
  // eof alocation test

  mem1 = memman_total(memman);
  printf("mem available: 0x%x\n", mem1);
  memman_monitor(memman);

  uart_puts("... end sample\n");
  
  while(1) {}
  return -1;
}