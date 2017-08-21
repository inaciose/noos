#include "timer.h"

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

  // init timer to start at specified value
  timer0_init(0x00F00000);
  
  // config PH24 to output
  greenled_config();
  
  // init led status
  int led_status = 0;
  
  // endless loop
  while(1) {
    // check (and clean it) for timer end status
    if(timer0_check()) {
      if(led_status) {
        // turn off
        led_status = 0;
        greenled_off();
      } else {
        // turn on
        led_status = 1;
        greenled_on();
      }
    }
  }
}
