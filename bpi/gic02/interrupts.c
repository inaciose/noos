#include "timer.h"
#include "gicv2.h"

// boot.S
extern void enable_irq( void );
extern void disable_irq( void );

//-------------------------------------------------------------------------

// global variable
int ledstate = 0;

void greenled_config( void ) {
  // Configure port PH24 (green led) for output
  asm ("\n"
    "mov r0,  #0x00000001 \n"
    "ldr r3, =0x01C20908 \n"
    "str r0, [r3] \n");
}

void greenled_on( void ) {
  
  // Turn led on PH24
  asm (" \n"
    "mov r0,  #0x01000000 \n"
    "ldr r3, =0x01C2090C \n"
    "str r0, [r3]");
}

void greenled_off( void ) {
  // Turn led off PH24
  asm (" \n"
    "mov r0,  #0x00000000 \n"
    "ldr r3, =0x01C2090C \n"
    "str r0, [r3]");
}

//-------------------------------------------------------------------------

void __attribute__((interrupt("UNDEF"))) undefined_instruction_vector(void)
{
  while( 1 ) { }
}

void __attribute__((interrupt("SWI"))) software_interrupt_vector(void)
{
  // blink green led fast
  
  // cpu disable interrupts
  //disable_irq();

  // timer1 init
  timer1_init(0x000F0000);

  // counter
  int n = 0;

  // loop checking timer
  while(n < 50) {
    // check timer1 pending status bit on TMR_IRQ_STA
    if(timer1_check()) {
      // increment counter
      n++;
      // change led status
      if(ledstate) {
        // led off
        ledstate = 0;
        greenled_off();
      } else {
        // led on
        ledstate = 1;
        greenled_on();
      }
      // clear timer1 pending status bit on TMR_IRQ_STA
      timer1_clear_irq();
    }
  }
  
  // cpu enable interrupts
  //enable_irq();

}

void __attribute__((interrupt("ABORT"))) prefetch_abort_vector(void)
{
  while( 1 ) { }
}

void __attribute__((interrupt("ABORT"))) data_abort_vector(void)
{
  while( 1 ) { }
}

void __attribute__((interrupt("IRQ"))) interrupt_vector(void)
{
  // get & ack the irqno from gic register
  unsigned int intno;
  intno = gic_getack();

  // process the interrupt
  switch(intno) {  
    // irqno for timer0 = 54 (0x36)
    case 0x36:
      // blink led
      if(ledstate) {
        // led off
        ledstate = 0;
        greenled_off();
      } else {
        // led on
        ledstate = 1;
        greenled_on();
      }
      timer0_clear_irq();
    break;
  }
  // clear the irqno from gic register
  gic_eoi(intno);
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_vector(void)
{
  while( 1 ) { }
}
