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
  
  // init the array of interrupt handlers
  // interrupt service routine sources
  isrs_init();
  uart_puts("isrs_init: done\n");
  
  // uart_irq_enable on gic
  // uart0 intno (33) 0x21
  gic_irq_enable(33); 
  uart_puts("uart_irq_enable: done\n");

  // enable uart interrupts for gic
  // 0 = no int | 1 = rx int | 2 = rx/tx int
  uart_irq_rxtx_config(1); 
  uart_puts("uart_irq_rxtx enable for rx: done\n");

  // add uart0 function handler to ISRS
  uart_pic_enable();
  uart_puts("uart_pic_enable: done\n");
  
  // enable timer0 interrupts on gicv2
  gic_irq_enable(54); // timer0 intno (54) 0x36
  uart_puts("timer0_irq_enable: done\n");

  // enable timer1 interrupts on gicv2
  gic_irq_enable(55); // timer1 intno (55) 0x37
  uart_puts("timer1_irq_enable: done\n");

  // enable timer2 interrupts on gicv2
  gic_irq_enable(56); // timer2 intno (56) 0x38
  uart_puts("timer2_irq_enable: done\n");

  // add timers function handlers to ISRS
  timer0_pic_enable();
  uart_puts("timer0_pic_enable: done\n");
  timer1_pic_enable();
  uart_puts("timer1_pic_enable: done\n");
  timer2_pic_enable();
  uart_puts("timer2_pic_enable: done\n");

  // timer0 init
  timer0_init(0x00F00000);
  uart_puts("timer0_init: done\n");

  // timer1 init
  timer1_init(0x0F000000);
  uart_puts("timer1_init: done\n");
  
  // timer2 init
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
