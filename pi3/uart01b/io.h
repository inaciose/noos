#define IO_BASE 0x3F000000
#define AUX_UART_BASE     (IO_BASE + 0x00215000)

// BCM2835 ARM peripherals - Page 89
#define GPFSEL1         (IO_BASE + 0x00200004) // GPIO Function Select 1
#define GPPUD           (IO_BASE + 0x00200094) // GPIO Pin Pull-up/down Enable
#define GPPUDCLK0       (IO_BASE + 0x00200098) // GPIO Pin Pull-up/down Enable Clock 0

// memory mapped i/o access macros
#define write32(addr, v)      (*((volatile unsigned long  *)(addr)) = (unsigned long)(v))
#define read32(addr)          (*((volatile unsigned long  *)(addr)))

// -----------------------------------------------------------------------------

// BCM2835 ARM peripherals - Page 8-20
typedef struct {
  volatile unsigned int AUX_IRQ;               // 00 Auxiliary interrupts
  volatile unsigned int AUX_ENABLES;           // 04 Auxiliary enables

  volatile unsigned int RESERVED1[((0x40 - 0x04) / 4) - 1];

  volatile unsigned int AUX_MU_IO_REG;         // 40 Mini Uart I/O Data
  volatile unsigned int AUX_MU_IER_REG;        // 44 Mini Uart Interrupt Enable
  volatile unsigned int AUX_MU_IIR_REG;        // 48 Mini Uart Interrupt Identify
  volatile unsigned int AUX_MU_LCR_REG;        // 4C Mini Uart Line Control
  volatile unsigned int AUX_MU_MCR_REG;        // 50 Mini Uart Modem Control
  volatile unsigned int AUX_MU_LSR_REG;        // 54 Mini Uart Line Status
  volatile unsigned int AUX_MU_MSR_REG;        // 58 Mini Uart Modem Status
  volatile unsigned int AUX_MU_SCRATCH;        // 5C Mini Uart Scratch
  volatile unsigned int AUX_MU_CNTL_REG;       // 60 Mini Uart Extra Control
  volatile unsigned int AUX_MU_STAT_REG;       // 64 Mini Uart Extra Status
  volatile unsigned int AUX_MU_BAUD_REG;       // 68 Mini Uart Baudrate

  /*
  volatile unsigned int RESERVED2[(0x80 - 0x68) / 4];

  volatile unsigned int AUX_SPI0_CNTL0_REG;
  volatile unsigned int AUX_SPI0_CNTL1_REG;
  volatile unsigned int AUX_SPI0_STAT_REG;
  volatile unsigned int AUX_SPI0_IO_REG;
  volatile unsigned int AUX_SPI0_PEEK_REG;

  volatile unsigned int RESERVED3[(0xC0 - 0x94) / 4];

  volatile unsigned int AUX_SPI1_CNTL0_REG;
  volatile unsigned int AUX_SPI1_CNTL1_REG;
  volatile unsigned int AUX_SPI1_STAT_REG;
  volatile unsigned int AUX_SPI1_IO_REG;
  volatile unsigned int AUX_SPI1_PEEK_REG;
  */
} struct_aux_regs_t;

