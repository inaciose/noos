#ifndef PROC_H
#define PROC_H

#include "types.h"
#include "arm.h"

#define NCPU 4

struct cpu {
    unsigned char           id;
    struct context* scheduler;    // swtch() here to enter scheduler
    volatile uint   started;        // Has the CPU started?

    int             ncli;           // Depth of pushcli nesting.
    int             intena;         // Were interrupts enabled before pushcli?
};

extern struct cpu cpus[NCPU];
extern struct cpu* cpu;
extern struct proc* curproc;

struct context {
    uint r4;
    uint r5;
    uint r6;
    uint r7;
    uint r8;
    uint r9;
    uint r10;
    uint r11;
    uint r12;
    uint lr;
};

enum procstate {
    UNUSED,
    EMBRYO,
    SLEEPING,
    RUNNABLE,
    RUNNING,
    ZOMBIE
};

struct proc {
    volatile int      pid;
    char              name[16];
    pde_t*            pgdir;  // Page table
    uint              size;   // size of process memory (bytes)
    int               killed; 
    char*             kstack; // Bottom of kernel stack for this process
    void*             chan;   // If non-zero, sleeping on chan
    enum procstate    state;
    struct proc*      parent;
    struct trapframe* tf;
    struct context*   context;
};

#endif
