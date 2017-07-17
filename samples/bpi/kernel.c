#include <stddef.h>
#include <stdint.h>
 
// Memory-Mapped I/O output
#define writel(v, addr)    (*((volatile unsigned long  *)(addr)) = (unsigned long)(v))
#define set_wbit(addr, v)  (*((volatile unsigned long  *)(addr)) |= (unsigned long)(v))
 
// Memory-Mapped I/O input
#define readl(addr)    (*((volatile unsigned long  *)(addr)))

#define SUNXI_UART0_BASE  0x01C28000
#define AW_CCM_BASE    0x01c20000

#define CONFIG_CONS_INDEX  1
//#define APB2_CFG    (AW_CCM_BASE + 0x058)
#define APB2_GATE    (AW_CCM_BASE + 0x06C)
#define APB2_RESET    (AW_CCM_BASE + 0x2D8)
#define APB2_GATE_UART_SHIFT  (16)
#define APB2_RESET_UART_SHIFT  (16)

#define UART0_RBR (SUNXI_UART0_BASE + 0x0)    /* receive buffer register */
#define UART0_THR (SUNXI_UART0_BASE + 0x0)    /* transmit holding register */
#define UART0_DLL (SUNXI_UART0_BASE + 0x0)    /* divisor latch low register */

#define UART0_DLH (SUNXI_UART0_BASE + 0x4)    /* divisor latch high register */
//#define UART0_IER (SUNXI_UART0_BASE + 0x4)    /* interrupt enable reigster */

//#define UART0_IIR (SUNXI_UART0_BASE + 0x8)    /* interrupt identity register */
//#define UART0_FCR (SUNXI_UART0_BASE + 0x8)    /* fifo control register */

#define UART0_LCR (SUNXI_UART0_BASE + 0xc)    /* line control register */

#define UART0_LSR (SUNXI_UART0_BASE + 0x14)   /* line status register */

#define BAUD_115200    (0xD) /* 24 * 1000 * 1000 / 16 / 115200 = 13 */
#define NO_PARITY      (0)
#define ONE_STOP_BIT   (0)
#define DAT_LEN_8_BITS (3)
#define LC_8_N_1       (NO_PARITY << 3 | ONE_STOP_BIT << 2 | DAT_LEN_8_BITS)


// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count)
{
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		 : "=r"(count): [count]"0"(count) : "cc");
}

void clock_init_uart(void)
{
  /* Open the clock gate for UART0 */
  set_wbit(APB2_GATE, 1 << (APB2_GATE_UART_SHIFT + CONFIG_CONS_INDEX - 1));
  /* Deassert UART0 reset (only needed on A31/A64/H3) */
  set_wbit(APB2_RESET, 1 << (APB2_RESET_UART_SHIFT + CONFIG_CONS_INDEX - 1));
}

void uart_init(void)
{
  clock_init_uart();

  /* select dll dlh */
  writel(0x80, UART0_LCR);
  /* set baudrate */
  writel(0, UART0_DLH);
  writel(BAUD_115200, UART0_DLL);
  /* set line control */
  writel(LC_8_N_1, UART0_LCR);
}

 
void uart_putc(char c)
{
  while ((readl(UART0_LSR) & 0x20) == 0) continue;
  writel(c, UART0_THR);
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
  if ((readl(UART0_LSR) & 0x01) == 0) {
    return 0;
  } else {
    return readl(UART0_RBR);
  }
}

void uart_fifo_flush ( void )
{
  while (( readl(UART0_LSR) & 0x40) == 0) continue;
}
 
#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags)
{
	// Declare as unused
	(void) r0;
	(void) r1;
	(void) atags;
 
	uart_init();
	uart_puts("Hello, kernel World!\r\n");
 
	while (1)
		uart_putc(uart_getc());
}

