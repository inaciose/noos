#include "types.h"
#include "defs.h"
#include "arm.h"
#include "memlayout.h"
#include "spinlock.h"

void initlock(struct spinlock *lk, char *name)
{
    lk->name = name;
    lk->locked = 0;
    lk->cpu = 0;
}

// Acquire the lock.
// Loops (spins) until the lock is acquired.
// Holding a lock for a long time may cause
// other CPUs to waste time spinning to acquire it.
void acquire(struct spinlock *lk)
{
    uart_puts("acquire: ");
    print_hex(lk);
    pushcli();		// disable interrupts to avoid deadlock.
    uint32 tmp;
    asm volatile(
    "1:  ldrex %0, [%1];"
        "teq %0, #0;"
        "strexeq %0, %2, [%1];"
        "teqeq %0, #0;"
        "bne 1b;"
        :"=&r"(tmp)
        :"r"(&lk->locked), "r" (1)
        :"cc"
    );
    lk->cpu = cpu;
}

// Release the lock.
void release(struct spinlock *lk)
{
    uart_puts("release: ");
    print_hex(lk);
    lk->locked = 0;
    popcli();
}

// Check whether this cpu is holding the lock.
int holding(struct spinlock *lk)
{
    uart_puts("holding: ");
    print_hex(lk);
    return lk->locked; // && lock->cpu == cpus;
}
