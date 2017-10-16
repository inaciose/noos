void uart_init(void);
void uart_puts(const char *);
void uart_putc(int);
int uart_getc(void);
void hexstring(unsigned int, const char *);

int cmain(void) {
  int c;
  uart_init();
  uart_puts("...\n");
  uart_puts("Hello World\n");
  hexstring(0xdeadbeaf, "\n");
  uart_puts("Please type something...\n");
  while(1) {
    c = uart_getc();
    if(c > 0) {
      if (c == '\r')
        uart_putc('\n');
      uart_putc(c);
    }
  }
  return -1;
}