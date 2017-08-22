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
  unsigned int intno;
  
  /* TODO: int disable here? **/
  
  // get & ack the irqno from gic register
  // run the registered ISR for int 
  // clear the irqno from gic register  
  intno = gic_dispatch();
  
  // declare no use
  (void) intno;
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_vector(void)
{
  uart_puts("im where FIQ\n");
  while( 1 ) { }
}
