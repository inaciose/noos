#ifndef SPINLOCK_H
#define SPINLOCK_H

#include "proc.h"

struct spinlock {
    uint        locked;

    // For debugging:
    char        *name;      // Name of lock.
    uint        pcs[10];    // The call stack (an array of program counters)
    struct cpu  *cpu;
};

#endif
