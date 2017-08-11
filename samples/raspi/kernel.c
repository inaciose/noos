#include <stddef.h>
#include <stdint.h>
 
// Memory-Mapped I/O output
static inline void mmio_write(uint32_t reg, uint32_t data)
{
	*(volatile uint32_t*)reg = data;
}
 
// Memory-Mapped I/O input
static inline uint32_t mmio_read(uint32_t reg)
{
	return *(volatile uint32_t*)reg;
}
 
// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count)
{
	asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		 : "=r"(count): [count]"0"(count) : "cc");
}
 
enum
{
    // The GPIO registers base address.
    
    GPIO_BASE = 0x3F200000, // for raspi2 & 3, 0x20200000 for raspi1
 
    // The offsets for reach register.
 
    GPFSEL1 = (GPIO_BASE + 0x4), // added
    
    // Controls actuation of pull up/down to ALL GPIO pins.
    GPPUD = (GPIO_BASE + 0x94),
 
    // Controls actuation of pull up/down for specific GPIO pin.
    GPPUDCLK0 = (GPIO_BASE + 0x98),
 
    // The base address for UART.
    UART0_BASE = 0x3F215000, // for raspi2 & 3, 0x20215000 for raspi1
 
    // The offsets for reach register for the UART.
    AUX_IRQ     = (UART0_BASE + 0x00),
    AUX_ENABLES = (UART0_BASE + 0x04),
    AUX_MU_IO_REG     = (UART0_BASE + 0x40),
    AUX_MU_IER_REG   = (UART0_BASE + 0x44),
    AUX_MU_IIR_REG   = (UART0_BASE + 0x48),
    AUX_MU_LCR_REG   = (UART0_BASE + 0x4C),
    AUX_MU_MCR_REG   = (UART0_BASE + 0x50),
    AUX_MU_LSR_REG     = (UART0_BASE + 0x54),
    AUX_MU_MSR_REG   = (UART0_BASE + 0x58),
    AUX_MU_SCRATCH   = (UART0_BASE + 0x5C),
    AUX_MU_CNTL_REG    = (UART0_BASE + 0x60),
    AUX_MU_STAT_REG    = (UART0_BASE + 0x64),
    AUX_MU_BAUD_REG    = (UART0_BASE + 0x68),

};
 
void uart_init()
{
	// enable Mini UART
	mmio_write(AUX_ENABLES, 0x1);
	
  // disable Mini UART Interrupt
  // bit0: 0/1 = Disable/Enable transmit interrupt
  // bit1: 0/1 = Disable/Enable receive interrupt
  mmio_write(AUX_MU_IER_REG, 0x0);
  
  // Mini UART Extra Control
  // bit0: 0/1 = mini UART receiver Disable/Enable
  // bit1: 0/1 = mini UART transmitter Disable/Enable
  mmio_write(AUX_MU_CNTL_REG, 0x0);
  
  // Mini UART Line Control
  // bit0: 0=7bit mode | 1=8bit mode # bit1: unk
  mmio_write(AUX_MU_LCR_REG, 0x3); 
  
  // Mini UART Modem Control
  // bit1: 0=UART1_RTS line is high | 1=UART1_RTS line is low
  mmio_write(AUX_MU_MCR_REG, 0x0);
  
  // disable Mini UART Interrupt
  mmio_write(AUX_MU_IER_REG,0);
  
  // Mini UART Interrupt Identify
  mmio_write(AUX_MU_IIR_REG,0xC6);
  
  // Mini UART Baudrate
  mmio_write(AUX_MU_BAUD_REG,270);

  // Setup the GPIO pin 14 && 15. 
  unsigned int ra;
  ra=mmio_read(GPFSEL1);
  
  ra&=~(7<<12); //gpio14
  ra|=2<<12;    //alt5
  ra&=~(7<<15); //gpio15
  ra|=2<<15;    //alt5
  mmio_write(GPFSEL1,ra);
  
  // Setup the GPIO pin 14 && 15.
 
	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	mmio_write(GPPUD, 0x00000000);
	delay(150);
 
	// Disable pull up/down for pin 14,15 & delay for 150 cycles.
	mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);
 
	// Write 0 to GPPUDCLK0 to make it take effect.
	mmio_write(GPPUDCLK0, 0x00000000);
  
  mmio_write(AUX_MU_CNTL_REG, 0x3);
   
	// Clear pending interrupts.
	//mmio_write(UART0_ICR, 0x7FF);
 
	// Set integer & fractional part of baud rate.
	// Divider = UART_CLOCK/(16 * Baud)
	// Fraction part register = (Fractional part * 64) + 0.5
	// UART_CLOCK = 3000000; Baud = 115200.
 
	// Divider = 3000000 / (16 * 115200) = 1.627 = ~1.
	//mmio_write(UART0_IBRD, 1);
	// Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
	//mmio_write(UART0_FBRD, 40);
 
	// Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
	//mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
 
	// Mask all interrupts.
	//mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |
	//                       (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
 
	// Enable UART0, receive & transfer part of UART.
	//mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}
 
void uart_putc(unsigned char c)
{
	// Wait for UART to become ready to transmit.
	while ( mmio_read(AUX_MU_LSR_REG) & (1 << 5) ) { }
	mmio_write(AUX_MU_IO_REG, c);
}
 
unsigned char uart_getc()
{
    // Wait for UART to have received something.
    while ( mmio_read(AUX_MU_LSR_REG) & (1 << 4) ) { }
    return mmio_read(AUX_MU_IO_REG);
}
 
void uart_puts(const char* str)
{
	size_t i;
  for (i = 0; str[i] != '\0'; i ++)
		uart_putc((unsigned char)str[i]);
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

