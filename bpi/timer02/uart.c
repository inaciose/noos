/*****************************************************************************
 * UART is mostly the same on A10/A13/A20/A31/H3/A64, except that newer SoCs *
 * have changed the APB numbering scheme (A10/A13/A20 used to have APB0 and  *
 * APB1 names, but newer SoCs just have renamed them into APB1 and APB2).    *
 * The constants below are using the new APB numbering convention.           *
 * Also the newer SoCs have introduced the APB2_RESET register, but writing  *
 * to it effectively goes nowhere on older SoCs and is harmless.             *
 *****************************************************************************/

#include "macro.h"
#include "uart_io.h"

void clock_init_uart(void)
{
  // Open the clock gate for UART0
  //set_wbit(APB2_GATE, 1 << (APB2_GATE_UART_SHIFT + CONFIG_CONS_INDEX - 1));
  mmio_write32or(APB2_GATE, 1 << (APB2_GATE_UART_SHIFT + CONFIG_CONS_INDEX - 1));
  // Deassert UART0 reset (only needed on A31/A64/H3)
  mmio_write32or(APB2_RESET, 1 << (APB2_RESET_UART_SHIFT + CONFIG_CONS_INDEX - 1));
}

void uart_init(void)
{
  clock_init_uart();
  
  // select dll dlh
  mmio_write32(UART0_LCR, 0x80);
  // disable uart0 interrupts
  mmio_write32(UART0_IER, 0);
  // set baudrate
  mmio_write32(UART0_DLL, BAUD_115200);
  // set line control
  mmio_write32(UART0_LCR, LC_8_N_1);
  // enable uart0 interrupts
  //writel(0x3, UART0_IER);
}
 
void uart_putc(char c)
{
  while ((mmio_read32(UART0_LSR) & 0x20) == 0) continue;
  mmio_write32(UART0_THR, c);
}

void uart_puts(const char *s)
{
  while (*s) {
    if (*s == '\n')
      uart_putc('\r');
    uart_putc(*s++);
  }
}

char uart_getc()
{
  if ((mmio_read32(UART0_LSR) & 0x01) == 0) {
    return 0;
  } else {
    return mmio_read32(UART0_RBR);
  }
}
