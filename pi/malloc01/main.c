// uart.c
void uart_puts(const char *);
void uart_init(void);
void hexstring ( unsigned int, const char *);

// malloc.c
void* malloc(unsigned int);
void heap_init(unsigned int, unsigned int);
void free(void *);

// defined in linker.ld
extern unsigned int _end;
unsigned int *end = &_end;

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
  
  uart_init();
  uart_puts("... hello\n");
  uart_puts("... start malloc01 sample\n");
  
  heap_init((unsigned int)end, 0x8000000);
  
  // allocation feedback
  uart_puts("heap: ");
  hexstring((unsigned int)end, " ");
  uart_puts(" > ");
  hexstring(0x8000000, "\n");

  // bof test
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
  
  uart_puts("free\n");
  free(ret1);
  free(ret2);
  free(ret3);
  free(ret4);
  // eof test

  uart_puts("... end sample\n");
  
  while(1) {}
  return -1;
}