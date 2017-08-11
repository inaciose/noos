#include "uart.h"

int main ( void )
{
  uart_init();
  uart_puts("\n");
  uart_puts("uart_init\n");

  while(1) {
    uart_putc(uart_getc());
  }
  return(0);
}
