#include "macro.h"
#include "uart.h"
#include "timer.h"
#include "timer_io.h"

//-------------------------------------------------------------------------

void hexstrings ( unsigned int d )
{
  unsigned int rb;
  unsigned int rc;

  rb=32;
  while(1)
  {
    rb-=4;
    rc=(d>>rb)&0xF;
    if(rc>9) rc+=0x37; else rc+=0x30;
    uart_putc(rc);
    if(rb==0) break;
  }
  uart_putc(0x20);
}

void hexstring ( unsigned int d )
{
  hexstrings(d);
  uart_putc(0x0D);
  uart_putc(0x0A);
}

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
  
  timer1_init(0x0F000000);
  uart_puts("timer1_init: done\n");
  
  timer2_init(0xF0000000);
  uart_puts("timer2_init: done\n");
  
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
    
    // check timer 1 status bit
    if(timer1_check()) {
      uart_puts("\nTIMER 1\n");
    }

    // check timer 2 status bit
    if(timer2_check()) {
      uart_puts("\nTIMER 2 ");
      hexstrings(mmio_read32(TMR_IRQ_EN));
      hexstrings(mmio_read32(TMR_IRQ_STA));

      hexstrings(mmio_read32(TMR_0_CTRL));
      hexstrings(mmio_read32(TMR_0_INTR_VAL));
      hexstrings(mmio_read32(TMR_0_CUR_VAL));

      hexstrings(mmio_read32(TMR_1_CTRL));
      hexstrings(mmio_read32(TMR_1_INTR_VAL));
      hexstrings(mmio_read32(TMR_1_CUR_VAL));

      hexstrings(mmio_read32(TMR_2_CTRL));
      hexstrings(mmio_read32(TMR_2_INTR_VAL));
      hexstring(mmio_read32(TMR_2_CUR_VAL));
    }
  }
}
