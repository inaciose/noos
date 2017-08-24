//-------------------------------------------------------------------------

// memory mapped i/o
#define write32(addr, v)      (*((volatile unsigned long  *)(addr)) = (unsigned long)(v))
#define read32(addr)          (*((volatile unsigned long  *)(addr)))

#define ARM_TIMER_LOD 0x3F00B400
#define ARM_TIMER_CTL 0x3F00B408
#define ARM_TIMER_CLI 0x3F00B40C
#define ARM_TIMER_RLD 0x3F00B418
#define ARM_TIMER_DIV 0x3F00B41C

#define IRQ_ENABLE_BASIC 0x3F00B218
#define IRQ_DISABLE_BASIC 0x3F00B224

#define AUX_MU_IO_REG   0x3F215040
#define AUX_MU_LSR_REG  0x3F215054

//-------------------------------------------------------------------------

extern void enable_irq ( void );
extern void SetActLEDState( int );

//-------------------------------------------------------------------------

void uart_send ( unsigned int c )
{
  while(1) {
    if(read32(AUX_MU_LSR_REG)&0x20) break;
  }
  write32(AUX_MU_IO_REG,c);
}

//-------------------------------------------------------------------------

volatile unsigned int icount;

void handle_irq ( void )
{
  icount++;
  if(icount&1) {
    SetActLEDState(1);
    uart_send ('-');
  } else {
    SetActLEDState(0);
    uart_send ('.');
  }
  write32(ARM_TIMER_CLI,0);
}

void timer_init ( void )
{
  write32(IRQ_DISABLE_BASIC,1);
  write32(ARM_TIMER_CTL,0x003E0000);
  write32(ARM_TIMER_LOD,4000000-1); // initial period
  write32(ARM_TIMER_RLD,4000000-1); // reload period
  write32(ARM_TIMER_DIV,0x000000F9);
  write32(ARM_TIMER_CLI,0);
  write32(IRQ_ENABLE_BASIC,1);
}

void timer_start ( void )
{
  write32(ARM_TIMER_CTL,0x003E00A2);
  write32(ARM_TIMER_CLI,0);
}
//-------------------------------------------------------------------------

int main ( void )
{
  timer_init();
  icount=0;
  enable_irq();
  timer_start();
  while(1) continue;
  return(0);
}
//-------------------------------------------------------------------------
