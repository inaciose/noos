#define IO_BASE 0x3F000000

// BCM2835 ARM peripherals - Page 89
#define GPFSEL0         (IO_BASE + 0x00200000) // GPIO Function Select 0
#define GPPUD           (IO_BASE + 0x00200094) // GPIO Pin Pull-up/down Enable
#define GPPUDCLK0       (IO_BASE + 0x00200098) // GPIO Pin Pull-up/down Enable Clock 0

// BCM2835 ARM peripherals - Page 8-20
#define AUX_ENABLES     (IO_BASE + 0x00215004) // Auxiliary enables
#define AUX_MU_IO_REG   (IO_BASE + 0x00215040) // Mini Uart I/O Data
#define AUX_MU_IER_REG  (IO_BASE + 0x00215044) // Mini Uart Interrupt Enable
#define AUX_MU_IIR_REG  (IO_BASE + 0x00215048) // Mini Uart Interrupt Identify 
#define AUX_MU_LCR_REG  (IO_BASE + 0x0021504C) // Mini Uart Line Control
#define AUX_MU_MCR_REG  (IO_BASE + 0x00215050) // Mini Uart Modem Control
#define AUX_MU_LSR_REG  (IO_BASE + 0x00215054) // Mini Uart Line Status
//#define AUX_MU_MSR_REG  (IO_BASE + 0x00215058) // Mini Uart Modem Status
//#define AUX_MU_SCRATCH  (IO_BASE + 0x0021505C) // Mini Uart Scratch
#define AUX_MU_CNTL_REG (IO_BASE + 0x00215060) // Mini Uart Extra Control
//#define AUX_MU_STAT_REG (IO_BASE + 0x00215064) // Mini Uart Extra Status
#define AUX_MU_BAUD_REG (IO_BASE + 0x00215068) // Mini Uart Baudrate

// memory mapped i/o access macros
#define write32(addr, v)      (*((volatile unsigned long  *)(addr)) = (unsigned long)(v))
#define read32(addr)          (*((volatile unsigned long  *)(addr)))
