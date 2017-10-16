// uart.c
void uart_putc(int);
void uart_puts(const char *);
void uart_init(void);
//printf
void printf(const char *, ...);

// printf() expect this function
// configurable on printf.c (at top) 
void putc(int c, void *stream)
{
   uart_putc(c);
}

int cmain(void) 
{
  uart_init();
  uart_puts("... hello\n");
  uart_puts("... start printf01 sample\n");
  
  // eof test
  printf("this is printf format output\n");
  printf("this is a string printf output: %s\n", "string");
  printf("this is an int printf output: %i \n", 0xf);
  printf("this is an hex printf output: 0x%x \n", 61440);
  // eof test

  uart_puts("... end sample\n");
  
  while(1) {}
  return -1;
}
