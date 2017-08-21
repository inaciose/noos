#include "uart.h"
#include "timer.h"

//-------------------------------------------------------------------------

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

void main(unsigned int r0, unsigned int r1, unsigned int r2)
{
  // declare arguments as unused
  (void) r0;
  (void) r1;
  (void) r2;
  
  uart_init();
  uart_puts("uart_init: done\n");

  timer0_init(0x00F00000);
  uart_puts("timer0_init: done\n");
  
  // green led init
  int led_status = 0;
  greenled_config();
  
  uart_puts("echo console test\n");
  
  while(1) {
    
    // console echo
    uart_putc(uart_getc());

    // check timer 0 status bit
    if(timer0_check()) {
      if(led_status) {
        // led off
        led_status = 0;
        greenled_off();
      } else {
        // led on
        led_status = 1;
        greenled_on();
      }
    }
  }
}
