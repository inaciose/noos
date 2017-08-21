#include "uart.h"
#include "timer.h"
#include "gicv2.h"

// boot.S
extern void enable_irq( void );
extern void disable_irq( void );
extern void call_swi( void );

// others
void greenled_config( void );

void main(unsigned int r0, unsigned int r1, unsigned int r2)
{
  // declare arguments as unused
  (void) r0;
  (void) r1;
  (void) r2;
  
  // cpu disable interrupts
  disable_irq();

  uart_init();
  uart_puts("uart_init: done\n");
  
  gic_v2_init();
  uart_puts("gic_v2_init: done\n");
  
  // uart_irq_enable on gic
  // uart0 intno (33) 0x21
  gic_irq_enable(33); 
  uart_puts("uart_irq_enable\n");

  // enable uart interrupts for gic
  // 0 = no int | 1 = rx int | 2 = rx/tx int
  uart_irq_rxtx_config(1); 
  uart_puts("uart_irq_rxtx enable for rx\n");

  // enable timer0 interrupts on gicv2
  gic_irq_enable(54); // timer0 intno (54) 0x36
  uart_puts("timer0_irq_enable\n");

  // timer0 init
  timer0_init(0x00F00000);
  uart_puts("timer0_init\n");

  // enable timer1 interrupts on gicv2
  gic_irq_enable(55); // timer1 intno (55) 0x37
  uart_puts("timer0_irq_enable\n");

  // timer1 init
  timer1_init(0x0F000000);
  uart_puts("timer0_init\n");

  // cpu enable interrupts
  enable_irq();

  // config ph24 to output
  greenled_config();
  
  // test swi interrupt
  call_swi();

  uart_puts("echo console test\n");
  while(1) {
   // keep loop here
  }
}
