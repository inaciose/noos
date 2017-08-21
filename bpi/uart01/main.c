#include "uart.h"

void main(unsigned int r0, unsigned int r1, unsigned int r2)
{
  // declare arguments as unused
  (void) r0;
  (void) r1;
  (void) r2;
  
  uart_init();
  uart_puts("uart_init done\n");
  
  uart_puts("echo console test\n");
  while(1) {
    uart_putc(uart_getc());
  }
}

