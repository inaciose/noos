#include "defs.h"
#include "arm.h"
#include "memlayout.h"

#include "io.h"

void uart_putc ( unsigned char byte )
{
  while(1) {
    if(read32(AUX_MU_LSR_REG+KERNBASE)&0x20) break;
  }
  write32(AUX_MU_IO_REG+KERNBASE, byte);
}

void uart_puts(const char *s)
{
  while (*s) {
    if (*s == '\n')
      uart_putc('\r');
    uart_putc(*s++);
  }
}

unsigned char uart_getc ()
{
  while(1) {
    if(read32(AUX_MU_LSR_REG+KERNBASE)&0x01) break;
  }
  return(read32(AUX_MU_IO_REG+KERNBASE)&0xFF);
}

void print_hex(uint val) {
    char digit[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    char number[8] = {'0','0','0','0','0','0','0','0'};
    uint base = 16;
    int i = 7;
    uart_putc('0');
    uart_putc('x');

    while(val > 0) {
        number[i--] = digit[val % base];
        val /= base;

    }
    for(i=0;i<8;++i) {
        uart_putc(number[i]);
    }
    uart_putc('\r');
    uart_putc('\n');
}
