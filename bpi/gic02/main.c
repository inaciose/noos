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

  gic_v2_init();

  // enable timer0 interrupts on gicv2
  gic_irq_enable(54); // timer0 intno (54) 0x36

  // timer0 init
  timer0_init(0x00F00000);

  // cpu enable interrupts
  enable_irq();

  // config ph24 to output
  greenled_config();
  
  // test swi interrupt
  call_swi();

  while(1) {
   // keep loop here
  }
}
