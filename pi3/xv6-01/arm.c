#include "types.h"
#include "defs.h"
#include "arm.h"
#include "proc.h"

// disable interrupt
void cli(void) {
    uint32 val;

    asm("mrs %[v], cpsr":[v]"=r"(val)::);
    val |= DIS_INT;
    asm("msr cpsr_c, %[v]"::[v]"r"(val):);
}

// enable interrupt
void sti(void) {
    uint32 val;

    asm("mrs %[v], cpsr":[v]"=r"(val)::);
    val &= ~DIS_INT;
    asm("msr cpsr_c, %[v]"::[v]"r"(val):);
}

// return the cpsr used for user program
uint spsr_usr(void)
{
    uint val;

    asm("mrs %[v], cpsr": [v]"=r" (val)::);
    val &= ~MODE_MASK;
    val |= USR_MODE;
    val &= ~DIS_INT; // enable interrupt

    return val;
}

// return if interrupt is enabled
bool int_enabled(void) {
    uint32 val;

    asm("mrs %[v], cpsr":[v]"=r"(val)::);

    // if DIS_INT bit is set(1) then interrupt is disabled
    // so we add a not(!) here
    return !(val & DIS_INT);
}

void pushcli(void) {
    bool enabled;
    enabled = int_enabled();

    cli();

    if (cpu->ncli == 0) {
        cpu->intena = enabled;
    }
    cpu->ncli++;
}

void popcli(void) {
    if (int_enabled()) {
        panic("popcli() - interruptible");
    }

    cpu->ncli--;
    if (cpu->ncli < 0) {
        panic("popcli() - ncli < 0");
    }

    if ((cpu->ncli == 0) && cpu->intena) {
        sti();
    }
}
