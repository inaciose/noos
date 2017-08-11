#include "io.h"

// Loop <delay> times
static inline void delay(unsigned int count)
{
  asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
     : "=r"(count): [count]"0"(count) : "cc");
}

unsigned int uart_getc ( void )
{
  while(1) {
    if(read32(AUX_MU_LSR_REG)&0x01) break;
  }
  return(read32(AUX_MU_IO_REG)&0xFF);
}

void uart_putc ( unsigned int c )
{
  while(1) {
    if(read32(AUX_MU_LSR_REG)&0x20) break;
  }
  write32(AUX_MU_IO_REG,c);
}

void uart_puts(const char *s)
{
  while (*s) {
    if (*s == '\n')
      uart_putc('\r');
    uart_putc(*s++);
  }
}

void setgpiofunc(unsigned int func, unsigned int alt)
{
  unsigned int sel, data, shift;

  if(func > 53) return;
  
  sel = 0;
  while (func > 10) {
    func = func - 10;
    sel++;
  }
  sel = (sel << 2) + GPFSEL0;

  data = read32(sel);
  shift = func + (func << 1);
  data &= ~(7 << shift);
  data |= alt << shift;
  
  write32(sel, data);
}

void uart_init ( void )
{
  // enable Mini UART
  write32(AUX_ENABLES,1);
  
  // Mini UART Interrupt
  // bit0: 0/1 = Disable/Enable transmit interrupt
  // bit1: 0/1 = Disable/Enable receive interrupt
  // disable receive & transmit UART interrupt
  write32(AUX_MU_IER_REG,0);
  
  // Mini UART Extra Control
  // bit0: 0/1 = mini UART receiver Disable/Enable
  // bit1: 0/1 = mini UART transmitter Disable/Enable
  // disable UART receive & transmit
  write32(AUX_MU_CNTL_REG,0);
  
  // Mini UART Line Control
  // bit0: 0=7bit mode | 1=8bit mode # bit1: unk
  write32(AUX_MU_LCR_REG,3);

  // Mini UART Modem Control
  // bit1: 0=UART1_RTS line is high | 1=UART1_RTS line is low
  write32(AUX_MU_MCR_REG,0);
  
  // Mini UART Interrupt
  // disable receive & transmit interrupt
  write32(AUX_MU_IER_REG,0);

  // Mini UART Interrupt Identify
  // bit0:R 0/1 Interrupt pending / no interrupt pending
  // bit1-2: 2+4 = Clear receive FIFO + Clear transmit FIFO
  write32(AUX_MU_IIR_REG,0xC6);
  
  // Mini UART Baudrate
  // bit0-15: baud rate counter
  write32(AUX_MU_BAUD_REG,270);

  // Setup the GPIO pin 14 && 15
  setgpiofunc(14, 2); // gpio 14, alt 5
  setgpiofunc(15, 2); // gpio 15, alt 5
  
  // Disable pull up/down for all GPIO pins & delay for 150 cycles
  write32(GPPUD,0);
  delay(150);
  
  // Disable pull up/down for pin 14,15 & delay for 150 cycles
  write32(GPPUDCLK0,(1<<14)|(1<<15));
  delay(150);
  
  // Write 0 to GPPUDCLK0 to make it take effect.
  write32(GPPUDCLK0,0);

  // Mini UART Extra Control
  // enable UART receive & transmit
  write32(AUX_MU_CNTL_REG,3);
}
