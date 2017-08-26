#ifndef ARM_H
#define ARM_H


// trap frame: in ARM, there are seven modes. Among the 16 regular registers,
// r13 (sp), r14(lr), r15(pc) are banked in all modes.
// 1. In xv6_a, all kernel level activities (e.g., Syscall and IRQ) happens
// in the SVC mode. CPU is put in different modes by different events. We
// switch them to the SVC mode, by shoving the trapframe to the kernel stack.
// 2. during the context switched, the banked user space registers should also
// be saved/restored.
//
// Here is an example:
// 1. a user app issues a syscall (via SWI), its user-space registers are
// saved on its kernel stack, syscall is being served.
// 2. an interrupt happens, it preempted the syscall. the app's kernel-space
// registers are again saved on its stack.
// 3. interrupt service ended, and execution returns to the syscall.
// 4. kernel decides to reschedule (context switch), it saves the kernel states
// and switches to a new process (including user-space banked registers)
#ifndef __ASSEMBLER__
#include "types.h"
struct trapframe {
    uint    sp_usr;     // user mode sp
    uint    lr_usr;     // user mode lr
    uint    r14_svc;    // r14_svc (r14_svc == pc if SWI)
    uint    spsr;
    uint    r0;
    uint    r1;
    uint    r2;
    uint    r3;
    uint    r4;
    uint    r5;
    uint    r6;
    uint    r7;
    uint    r8;
    uint    r9;
    uint    r10;
    uint    r11;
    uint    r12;
    uint    pc;         // (lr on entry) instruction to resume execution
};
#endif

#define CPSR_IRQ_INHIBIT (1<<7)
#define CPSR_FIQ_INHIBIT (1<<6)
#define NO_INT  (CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT)
#define DIS_INT CPSR_IRQ_INHIBIT

// ARM has 7 modes and banked registers
#define MODE_MASK   0x1f
#define USR_MODE    0x10
#define FIQ_MODE    0x11
#define IRQ_MODE    0x12
#define SVC_MODE    0x13
#define ABT_MODE    0x17
#define UND_MODE    0x1b
#define SYS_MODE    0x1f


#endif
