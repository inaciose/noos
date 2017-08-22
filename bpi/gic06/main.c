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

  // init uart0 
  uart_init();
  uart_puts("uart_init: done\n");
  
  // init gic: gicd & gicc
  gic_v2_init();
  uart_puts("gic_v2_init: done\n");

  // uart0 irq_enable on gic and ISR function
  // uart0 intno (33) 0x21
  uart_gic_enable();
  uart_puts("uart_gic_enable: done\n");

  // timers irq_enable on gic and ISR functions
  // timer0 intno (54) 0x36
  timer0_gic_enable();
  uart_puts("timer0_pic_enable: done\n");
  // timer1 intno (55) 0x37
  timer1_gic_enable();
  uart_puts("timer1_pic_enable: done\n");
  // timer2 intno (56) 0x38
  timer2_gic_enable();
  uart_puts("timer2_pic_enable: done\n");

  // init timers
  timer0_init(0x00F00000);
  uart_puts("timer0_init: done\n");

  timer1_init(0x0F000000);
  uart_puts("timer1_init: done\n");
  
  timer2_init(0xF0000000);
  uart_puts("timer2_init: done\n");

  // cpu enable interrupts
  enable_irq();
  uart_puts("enable_cpu_irq: done\n");

  // config ph24 to output
  greenled_config();
  uart_puts("greenled_config: done\n");
  
  // test swi interrupt
  call_swi();
  uart_puts("test_swi: done\n");

  uart_puts("echo console test\n");
  
  while(1) {
   // keep loop here
  }
}
