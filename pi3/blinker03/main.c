#include "macro.h"
#include "board.h"

//-------------------------------------------------------------------------

extern void enable_irq ( void );
extern void SetActLEDState( int );

void timer_init ( void );
void timer_start ( void );

void uart_init ( void );
void uart_putc ( unsigned int c );
void uart_puts(const char *s);
void miniuartintr( void );

//-------------------------------------------------------------------------

volatile unsigned int icount;

void handle_irq ( void )
{
  if(read32(IRQ_PENDING1)) {
    miniuartintr();
  }
  
  if(read32(IRQ_PENDING_BASIC)) {
    icount++;
    if(icount&1) {
      SetActLEDState(1);
      //uart_putc ('-');
    } else {
      SetActLEDState(0);
      //uart_putc ('.');
    }
    write32(ARM_TIMER_CLI,0);
  }
}

//-------------------------------------------------------------------------

int main ( void )
{
  uart_init();
  uart_puts("\n");
  uart_puts("uart_init\n");

  timer_init();
  uart_puts("timer_init\n");

  enable_irq();
  uart_puts("enable_irq\n");

  timer_start();
  uart_puts("timer_start\n");

  icount=0;
  while(1) { 
    // looping 
  }
  
  return(0);
}
//-------------------------------------------------------------------------
