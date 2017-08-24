#include "macro.h"
#include "board.h"

void timer_init ( void )
{
  write32(IRQ_DISABLE_BASIC,1);
  write32(ARM_TIMER_CTL,0x003E0000);
  write32(ARM_TIMER_LOD,400000-1); // initial period
  write32(ARM_TIMER_RLD,400000-1); // reload period
  write32(ARM_TIMER_DIV,0x000000F9);
  write32(ARM_TIMER_CLI,0);
  write32(IRQ_ENABLE_BASIC,1);
}

void timer_start ( void )
{
  write32(ARM_TIMER_CTL,0x003E00A2);
  write32(ARM_TIMER_CLI,0);
}
