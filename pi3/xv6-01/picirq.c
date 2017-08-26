#include "arm.h"
#include "defs.h"
#include "rpi2.h"

static volatile uint *vic_base;

#define VIC_IRQPENDING 0
#define VIC_INTENABLE  6
#define VIC_INTCLEAR   9

#define NUM_INTSRC 32
static ISR isrs[NUM_INTSRC];

static void default_isr(struct trapframe *tf, int n) {
    (void)tf;
    uart_puts("unhandled interrupt: ");
    print_hex(n);
}

void pic_init(void *base) {
    int i;

    vic_base = base;
    vic_base[VIC_INTCLEAR] = 0xFFFFFFFF;

    for(i=0;i<NUM_INTSRC;++i) {
        isrs[i] = default_isr;
    }
}

void pic_enable(int n, ISR isr) {
    if ((n<0) || (n>=NUM_INTSRC)) {
        panic("invalid interrupt source");
    }
    isrs[n] = isr;
    vic_base[VIC_INTENABLE] = (1<<n);
}

void pic_disable(int n) {
    if ((n<0) || (n>=NUM_INTSRC)) {
        panic("invalid interrupt source");
    }
    vic_base[VIC_INTCLEAR] = (1<<n);
    isrs[n] = default_isr;
}

// dispatch the interrupt
void pic_dispatch (struct trapframe *tf) {
    uint intstatus;
    int i;
    intstatus = vic_base[VIC_IRQPENDING];
    for(i=0;i<NUM_INTSRC;++i) {
        if (intstatus & (1<<i)) {
            isrs[i](tf, i);
        }
    }
}
