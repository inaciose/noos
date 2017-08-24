// memory mapped i/o
#define PBASE 0x3F000000

#define ARM_TIMER_LOD 0x3F00B400
#define ARM_TIMER_CTL 0x3F00B408
#define ARM_TIMER_CLI 0x3F00B40C
#define ARM_TIMER_RLD 0x3F00B418
#define ARM_TIMER_DIV 0x3F00B41C

#define INT_REGS_BASE     (PBASE+0xB200)//

#define IRQ_PENDING_BASIC (PBASE+0xB200)
#define IRQ_PENDING1      (PBASE+0xB204)
#define IRQ_PENDING2      (PBASE+0xB208)
#define IRQ_FIQ_CTL       (PBASE+0xB20C)
#define IRQ_ENABLE1       (PBASE+0xB210)
#define IRQ_ENABLE2       (PBASE+0xB214)
#define IRQ_ENABLE_BASIC  (PBASE+0xB218)//
#define IRQ_DISABLE1       (PBASE+0xB21C)
#define IRQ_DISABLE2       (PBASE+0xB220)
#define IRQ_DISABLE_BASIC (PBASE+0xB224)//

#define GPFSEL1         (PBASE+0x00200004)
#define GPSET0          (PBASE+0x0020001C)
#define GPCLR0          (PBASE+0x00200028)
#define GPPUD           (PBASE+0x00200094)
#define GPPUDCLK0       (PBASE+0x00200098)

#define AUX_IRQ         (PBASE+0x00215000) // Auxiliary Interrupt status
#define AUX_ENABLES     (PBASE+0x00215004)// // Auxiliary enables
// RESERVED             (reserved1[((0x40 - 0x04) / 4) - 1])
#define AUX_MU_IO_REG   (PBASE+0x00215040)// // Mini Uart I/O Data
#define AUX_MU_IER_REG  (PBASE+0x00215044)// // Mini Uart Interrupt Enable
#define AUX_MU_IIR_REG  (PBASE+0x00215048)// // Mini Uart Interrupt Identify 
#define AUX_MU_LCR_REG  (PBASE+0x0021504C)// // Mini Uart Line Control
#define AUX_MU_MCR_REG  (PBASE+0x00215050)// // Mini Uart Modem Control
#define AUX_MU_LSR_REG  (PBASE+0x00215054)// // Mini Uart Line Status
#define AUX_MU_MSR_REG  (PBASE+0x00215058)// // Mini Uart Modem Status
#define AUX_MU_SCRATCH  (PBASE+0x0021505C)// // Mini Uart Scratch
#define AUX_MU_CNTL_REG (PBASE+0x00215060)// // Mini Uart Extra Control
#define AUX_MU_STAT_REG (PBASE+0x00215064)// // Mini Uart Extra Status
#define AUX_MU_BAUD_REG (PBASE+0x00215068)// // Mini Uart Baudrate
// RESERVED             (reserved2[(0x80 - 0x68) / 4])
#define AUX_SPI0_CNTL0  (PBASE+0x00215080)
#define AUX_SPI0_CNTL1  (PBASE+0x00215084)
#define AUX_SPI0_STAT   (PBASE+0x00215088)
#define AUX_SPI0_IO     (PBASE+0x00215090)
#define AUX_SPI0_PEEK   (PBASE+0x00215094)
// RESERVED             (reserved3[(0xC0 - 0x94) / 4])
#define AUX_SPI1_CNTL0  (PBASE+0x002150C0)
#define AUX_SPI1_CNTL1  (PBASE+0x002150C4)
#define AUX_SPI1_STAT   (PBASE+0x002150C8)
#define AUX_SPI1_IO     (PBASE+0x002150D0)
#define AUX_SPI1_PEEK   (PBASE+0x002150D4)
