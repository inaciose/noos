#include "uart.h"
#include "gicv2.h"

void __attribute__((interrupt("UNDEF"))) undefined_instruction_vector(void)
{
  uart_puts("im where UNDEF\n");
  while( 1 ) { }
}

void __attribute__((interrupt("SWI"))) software_interrupt_vector(void)
{
  uart_puts("im where SWI\n");
}

void __attribute__((interrupt("ABORT"))) prefetch_abort_vector(void)
{
  uart_puts("im where ABORT1\n");
  while( 1 ) { }
}

void __attribute__((interrupt("ABORT"))) data_abort_vector(void)
{
  uart_puts("im where ABORT2\n");
  while( 1 ) { }
}

void __attribute__((interrupt("IRQ"))) interrupt_vector(void)
{
  // get & ack the irqno from gic register
  unsigned int intno;
  intno = gic_getack();
  
  // process the interrupt
  switch(intno) {
    case 0x21:
      // irqno for uart0 = 33 (0x21)
      uart_putc(uart_getc()); 
    break;
  }
  // clear the irqno from gic register
  gic_eoi(intno);
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_vector(void)
{
  uart_puts("im where FIQ\n");
  while( 1 ) { }
}
