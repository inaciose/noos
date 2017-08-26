// The ARM UART, a memory mapped device
#include "types.h"
#include "defs.h"
#include "arm.h"
#include "memlayout.h"
#include "proc.h"

void cprintf(char *fmt, ...) {
    uart_puts("cprintf not implement yet...");
    uart_puts(fmt);
}

// trap routine
void swi_handler (struct trapframe *r)
{
    curproc->tf = r;
    syscall();
}

// trap routine
void irq_handler (struct trapframe *r)
{
    (void)r;
    // proc points to the current process. If the kernel is
    // running scheduler, proc is NULL.
    /*
    if (proc != NULL) {
        proc->tf = r;
    }
    */
    pic_dispatch(r);
}

// trap routine
void reset_handler (struct trapframe *r)
{
    cli();
    cprintf ("reset at: 0x%x \r\n", r->pc);
}

// trap routine
void und_handler (struct trapframe *r)
{
    cli();
    cprintf ("und at: 0x%x \r\n", r->pc);
}

// trap routine
void dabort_handler (struct trapframe *r)
{
    uint dfs, fa;

    cli();

    // read data fault status register
    asm("MRC p15, 0, %[r], c5, c0, 0": [r]"=r" (dfs)::);

    // read the fault address register
    asm("MRC p15, 0, %[r], c6, c0, 0": [r]"=r" (fa)::);
    uart_puts("data abort: instruction ");
    print_hex(r->pc);
    uart_puts(", fault addr ");
    print_hex(fa);
    uart_puts(", reason ");
    print_hex(dfs);

    dump_trapframe (r);
}

// trap routine
void iabort_handler (struct trapframe *r)
{
    uint ifs;

    // read fault status register
    asm("MRC p15, 0, %[r], c5, c0, 0": [r]"=r" (ifs)::);

    cli();
    uart_puts("prefetch abort at:");
    print_hex(r->pc);
    uart_puts(" (reason: ");
    print_hex(ifs);
    uart_puts(")\r\n");
    dump_trapframe (r);
}

// trap routine
void na_handler (struct trapframe *r)
{
    cli();
    cprintf ("n/a at: 0x%x \r\n", r->pc);
}

// trap routine
void fiq_handler (struct trapframe *r)
{
    cli();
    cprintf ("fiq at: 0x%x \r\n", r->pc);
}

// low-level init code: in real hardware, lower memory is usually mapped
// to flash during startup, we need to remap it to SDRAM
void trap_init()
{
    volatile uint32 *ram_start;
    char *stk;
    uint i;
    uint modes[] = {FIQ_MODE, IRQ_MODE, ABT_MODE, UND_MODE};

    // the opcode of PC relative load (to PC) instruction LDR pc, [pc,...]
    static uint32 const LDR_PCPC = 0xE59FF000U;

    // create the excpetion vectors
    ram_start = (uint32*)VEC_TBL;

    ram_start[0] = LDR_PCPC | 0x18; // Reset (SVC)
    ram_start[1] = LDR_PCPC | 0x18; // Undefine Instruction (UND)
    ram_start[2] = LDR_PCPC | 0x18; // Software interrupt (SVC)
    ram_start[3] = LDR_PCPC | 0x18; // Prefetch abort (ABT)
    ram_start[4] = LDR_PCPC | 0x18; // Data abort (ABT)
    ram_start[5] = LDR_PCPC | 0x18; // Not assigned (-)
    ram_start[6] = LDR_PCPC | 0x18; // IRQ (IRQ)
    ram_start[7] = LDR_PCPC | 0x18; // FIQ (FIQ)

    ram_start[8]  = (uint32)trap_reset;
    ram_start[9]  = (uint32)trap_und;
    ram_start[10] = (uint32)trap_swi;
    ram_start[11] = (uint32)trap_iabort;
    ram_start[12] = (uint32)trap_dabort;
    ram_start[13] = (uint32)trap_na;
    ram_start[14] = (uint32)trap_irq;
    ram_start[15] = (uint32)trap_fiq;

    // initialize the stacks for different mode
    for (i = 0; i < sizeof(modes)/sizeof(uint); i++) {
        stk = kpt_alloc();

        if (stk == NULL) {
            panic("failed to alloc memory for irq stack");
        }

        set_stk (modes[i], (uint)stk);
    }
}

void test_cprintf(char *prefix, uint val) {
    uart_puts(prefix);
    print_hex(val);
}

void dump_trapframe (struct trapframe *tf)
{
    test_cprintf("r14_svc: ", tf->r14_svc);
    test_cprintf("   spsr: ", tf->spsr);
    test_cprintf("     r0: ", tf->r0);
    test_cprintf("     r1: ", tf->r1);
    test_cprintf("     r2: ", tf->r2);
    test_cprintf("     r3: ", tf->r3);
    test_cprintf("     r4: ", tf->r4);
    test_cprintf("     r5: ", tf->r5);
    test_cprintf("     r6: ", tf->r6);
    test_cprintf("     r7: ", tf->r7);
    test_cprintf("     r8: ", tf->r8);
    test_cprintf("     r9: ", tf->r9);
    test_cprintf("    r10: ", tf->r10);
    test_cprintf("    r11: ", tf->r11);
    test_cprintf("    r12: ", tf->r12);
    test_cprintf("     pc: ", tf->pc);
}
