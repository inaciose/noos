#ifndef MEMLAYOUT_H
#define MEMLAYOUT_H
// Memory layout

// Key addresses for address space layout (see kmap in vm.c for layout)
#define EXTMEM	  		0x20000
#define KERNBASE  		0x80000000         // First kernel virtual address
#define KERNLINK  		(KERNBASE+EXTMEM)  // Address where kernel is linked

#define DEVBASE         0x3f000000
#define DEV_MEM_SZ      0x400000
#define VEC_TBL         0xFFFF0000

// we first map 1MB low memory containing kernel code.
#define INIT_KERNMAP 	0x00100000 // 1MB
#define PHYSTOP         0x08000000 // 128MB

#define V2P(a) (((uint) (a)) - KERNBASE)
#define P2V(a) (((void *) (a)) + KERNBASE)

#define V2P_WO(x) ((x) - KERNBASE)    // same as V2P, but without casts
#define P2V_WO(x) ((x) + KERNBASE)    // same as V2P, but without casts

#ifndef __ASSEMBLER__
#include "types.h"

static inline uint v2p(void *a) { return ((uint) (a))  - KERNBASE; }
static inline void *p2v(uint a) { return (void *) ((a) + KERNBASE); }

enum
{
    // The GPIO registers base address.
    //GPIO_BASE = 0x20200000,
    GPIO_BASE = 0x3f200000,

    // The offsets for reach register.

    // Controls actuation of pull up/down to ALL GPIO pins.
    GPPUD = (GPIO_BASE + 0x94),

    // Controls actuation of pull up/down for specific GPIO pin.
    GPPUDCLK0 = (GPIO_BASE + 0x98),

    // The base address for UART.
    //UART0_BASE = 0x20201000,
    UART0_BASE = 0x3f201000,

    // The offsets for reach register for the UART.
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
};

static inline void mmio_write(uint32 reg, uint32 data) {
    asm volatile("str %[data], [%[reg]]" : : [reg]"r"(reg), [data]"r"(data));
}

static inline uint32 mmio_read(uint32 reg) {
    uint32 data;
    asm volatile("ldr %[data], [%[reg]]" : [data]"=r"(data) : [reg]"r"(reg));
    return data;
}
/* Loop <delay> times in a way that the compiler won't optimize away. */
static inline void delay(int count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
         : : [count]"r"(count) : "cc");
}
#endif
#endif
