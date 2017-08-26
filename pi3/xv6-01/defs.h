#ifndef DEFS_H
#define DEFS_H

// number of elements in fixed-size array
#define NELEM(x) (sizeof(x)/sizeof((x)[0]))

#include "arm.h"
#include "types.h"

struct context;
struct spinlock;
struct trapframe;
struct proc;

typedef void (*ISR) (struct trapframe *tf, int n);

// arm.c
uint   spsr_usr(void);
bool   int_enabled(void);
void   set_stk(uint mode, uint addr);
void*  get_fp (void);
void   cli(void);
void   sti(void);
void   pushcli(void);
void   popcli(void);

// picirq.c
void   pic_init(void *base);
void   pic_enable(int n, ISR isr);
void   pic_disable(int n);
void   pic_dispatch (struct trapframe *tp);

// proc.c
void   pinit(void);
void   error_init(void);
void   userinit(void);
void   scheduler(void);
void   forkret(void);
void   yield(void);

// string.c
void   memmove(char* dest, char* src, int size);
void   strncpy(char* buf, char* str, int size);
void*  memset(void *dst, int v, int n);
uint32 strlen(const char* str);

// spinlock.c
void   acquire(struct spinlock*);
int    holding(struct spinlock*);
void   initlock(struct spinlock*, char*);
void   release(struct spinlock*);

// syscall.c
void   syscall(void);

// timer.c
void   timer_init(void);
void   isr_timer(struct trapframe *tp, int irq_idx);

// trap.c
void   trap_init(void);
void   dump_trapframe(struct trapframe *);

// trap_asm.S
void   trap_reset(void);
void   trap_und(void);
void   trap_swi(void);
void   trap_iabort(void);
void   trap_dabort(void);
void   trap_na(void);
void   trap_irq(void);
void   trap_fiq(void);

// uart.c
void uart_putc(unsigned char byte);
void uart_puts(const char* str);
unsigned char uart_getc();
void print_hex(uint val);

// vm.c
void init_vmm(void);
void kpt_freerange(uint32 low, uint32 high);
void kmem_freerange(uint32 low, uint32 high);
void* alloc_page(void);
void* kpt_alloc(void);
void inituvm(pde_t *pgdir, char *init, uint sz);
void switchuvm(struct proc* p);

void paging_init(uint phy_low, uint phy_hi);

// main.c
void panic(char* str);

// swtch.S
void swtch(struct context**, struct context*);

#endif
