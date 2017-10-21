/* Copyright (C) 2013 by John Cronin <jncronin@tysos.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <stdio.h>
#include "mmio.h"
#include "uart.h"
#include "timer.h"

// define Register addresses
#define GPFSEL0           0x20200000
#define GPFSEL1           0x20200004
#define GPFSEL2           0x20200008
#define GPFSEL3           0x2020000C
#define GPFSEL4           0x20200010
#define GPFSEL5           0x20200014
#define GPSET0            0x2020001C
#define GPSET1            0x20200020
#define GPCLR0            0x20200028
#define GPCLR1            0x2020002C
#define GPPUD             0x20200094
#define GPPUDCLK0         0x20200098
#define GPPUDCLK1         0x2020009C

#define AUX_IRQ           0x20215000
#define AUX_ENABLES       0x20215004
#define AUX_MU_IO_REG     0x20215040
#define AUX_MU_IER_REG    0x20215044
#define AUX_MU_IIR_REG    0x20215048
#define AUX_MU_LCR_REG    0x2021504C
#define AUX_MU_MCR_REG    0x20215050
#define AUX_MU_LSR_REG    0x20215054
#define AUX_MU_MSR_REG    0x20215058
#define AUX_MU_SCRATCH    0x2021505C
#define AUX_MU_CNTL_REG   0x20215060
#define AUX_MU_STAT_REG   0x20215064
#define AUX_MU_BAUD_REG   0x20215068


// bof keep from original
#define GPIO_BASE       0x20200000
#define UART0_BASE      0x20215000 // adapted

static uint32_t uart_base = UART0_BASE;
static uint32_t gpio_base = GPIO_BASE;

void uart_set_base(uint32_t base)
{
  uart_base = base;
}

void gpio_set_base(uint32_t base)
{
  gpio_base = base;
}
// eof

// memory mapped i/o access macros
#define write32(addr, v)      (*((volatile unsigned long  *)(addr)) = (unsigned long)(v))
#define read32(addr)          (*((volatile unsigned long  *)(addr)))

static inline void uart_delay(unsigned int count)
{
  asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
     : "=r"(count): [count]"0"(count) : "cc");
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

int uart_putc(int c)
{
  if(c=='\n') {
    while(1) if(read32(AUX_MU_LSR_REG) & 0x20) break;
    write32(AUX_MU_IO_REG, 0x0d); // add CR before LF
  }
  while(1) if(read32(AUX_MU_LSR_REG) & 0x20) break;
  write32(AUX_MU_IO_REG, c);
  return c;
}

int uart_getc(void)
{
  if(read32(AUX_MU_LSR_REG)&0x1) return read32(AUX_MU_IO_REG);
  else return -1;
}

void _puts(const char *s)
{
  while (*s) {
    if (*s == '\n')
      uart_putc('\r');
    uart_putc(*s++);
  }
}

void uart_puts(const char *s)
{
  while (*s) {
    if (*s == '\n')
      uart_putc('\r');
    uart_putc(*s++);
  }
}

void uart_init(void)
{
  write32(AUX_ENABLES, 1);
  write32(AUX_MU_CNTL_REG, 0);
  write32(AUX_MU_LCR_REG, 0x3);
  write32(AUX_MU_MCR_REG, 0);
  write32(AUX_MU_IER_REG, 0x1);
  write32(AUX_MU_IIR_REG, 0xC7);
  write32(AUX_MU_BAUD_REG, 270); // (250,000,000/(115200*8))-1 = 270
  
  setgpiofunc(14, 2); // gpio 14, alt 5
  setgpiofunc(15, 2); // gpio 15, alt 5

  write32(GPPUD, 0);
  uart_delay(10);
  write32(GPPUDCLK0, (1 << 14) | (1 << 15) );
  uart_delay(10);
  write32(GPPUDCLK0, 0);

  write32(AUX_MU_CNTL_REG, 3);
  
}
