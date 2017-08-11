#define IO_BASE 0x3F000000
#define AUX_UART_BASE     (IO_BASE + 0x00215000)

// BCM2835 ARM peripherals - Page 89
#define GPFSEL0         (IO_BASE + 0x00200000) // GPIO Function Select 0
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

// -----------------------------------------------------------------------------

// BCM2835 ARM peripherals - Page 89
/*
typedef struct {
   volatile unsigned int GPFSEL0;               // 00 GPIO Function Select 0
   volatile unsigned int GPFSEL1;               // 04
   volatile unsigned int GPFSEL2;               // 08
   volatile unsigned int GPFSEL3;               // 0C
   volatile unsigned int GPFSEL4;               // 10
   volatile unsigned int GPFSEL5;               // 14
   
   volatile unsigned int RESERVED1;             // 18 
   
   volatile unsigned int GPSET0;                // 1C
   volatile unsigned int GPSET1;                // 20
   
   volatile unsigned int GPCLR0;                // 28
   volatile unsigned int GPCLR1;                // 2C
   
   volatile unsigned int RESERVED2;             // 30
   
   volatile unsigned int GPLEV0;                // 34
   volatile unsigned int GPLEV1;                // 38
   
   volatile unsigned int RESERVED3;             // 3C
   
   volatile unsigned int GPEDS0;                // 40
   volatile unsigned int GPEDS1;                // 44
   
   volatile unsigned int RESERVED4;             // 48
   
   volatile unsigned int GPREN0;                // 4C
   volatile unsigned int GPREN1;                // 50
   
   volatile unsigned int RESERVED5;             // 54
   
   volatile unsigned int GPFEN0;                // 58
   volatile unsigned int GPFEN1;                // 5C
   
   volatile unsigned int RESERVED6;             // 60
   
   volatile unsigned int GPHEN0;                // 64
   volatile unsigned int GPHEN1;                // 68
   
   volatile unsigned int RESERVED7;             // 6C
   
   volatile unsigned int GPLEN0;                // 70
   volatile unsigned int GPLEN1;                // 74
   
   volatile unsigned int RESERVED8;             // 78

   volatile unsigned int GPAREN0;               // 7C
   volatile unsigned int GPAREN1;               // 80

   volatile unsigned int RESERVED9;             // 84
   
   volatile unsigned int GPAFEN0;               // 88
   volatile unsigned int GPAFEN1;               // 8C
   
   volatile unsigned int RESERVED10;            // 90
   
   volatile unsigned int GPPUD;                 // 94
   volatile unsigned int GPPUDCLK0;             // 98
   volatile unsigned int GPPUDCLK1;             // 9C

} struct_gpio_regs_t;
*/
