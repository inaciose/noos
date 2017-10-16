// define gpio register addresses
#define GPFSEL0           0x20200000
#define GPSET0            0x2020001C
#define GPPUD             0x20200094
#define GPPUDCLK0         0x20200098

// define mini uart register addresses
#define AUX_ENABLES       0x20215004
#define AUX_MU_IO_REG     0x20215040
#define AUX_MU_IER_REG    0x20215044
#define AUX_MU_IIR_REG    0x20215048
#define AUX_MU_LCR_REG    0x2021504C
#define AUX_MU_MCR_REG    0x20215050
#define AUX_MU_LSR_REG    0x20215054
#define AUX_MU_CNTL_REG   0x20215060
#define AUX_MU_BAUD_REG   0x20215068

// memory mapped i/o access macros
#define write32(addr, v)      (*((volatile unsigned long  *)(addr)) = (unsigned long)(v))
#define read32(addr)          (*((volatile unsigned long  *)(addr)))

static inline void uart_delay(unsigned int count)
{
  asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
     : "=r"(count): [count]"0"(count) : "cc");
}

void uart_setgpiofunc(unsigned int func, unsigned int alt)
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

void uart_putc(int c)
{
  if(c=='\n') {
    while(1) if(read32(AUX_MU_LSR_REG) & 0x20) break;
    write32(AUX_MU_IO_REG, 0x0d); // add CR before LF
  }
  while(1) if(read32(AUX_MU_LSR_REG) & 0x20) break;
  write32(AUX_MU_IO_REG, c);
}

int uart_getc(void)
{
  if(read32(AUX_MU_LSR_REG)&0x1) return read32(AUX_MU_IO_REG);
  else return -1;
}

void uart_puts(const char *s)
{
  while (*s) {
    if (*s == '\n')
      uart_putc('\r');
    uart_putc(*s++);
  }
}

void 
uart_init(void)
{
  
  write32(AUX_ENABLES, 1);
  write32(AUX_MU_CNTL_REG, 0);
  write32(AUX_MU_LCR_REG, 0x3);
  write32(AUX_MU_MCR_REG, 0);
  write32(AUX_MU_IER_REG, 0x1);
  write32(AUX_MU_IIR_REG, 0xC7);
  write32(AUX_MU_BAUD_REG, 270); // (250,000,000/(115200*8))-1 = 270
  
  uart_setgpiofunc(14, 2); // gpio 14, alt 5
  uart_setgpiofunc(15, 2); // gpio 15, alt 5

  write32(GPPUD, 0);
  uart_delay(10);
  write32(GPPUDCLK0, (1 << 14) | (1 << 15) );
  uart_delay(10);
  write32(GPPUDCLK0, 0);

  write32(AUX_MU_CNTL_REG, 3);
}
