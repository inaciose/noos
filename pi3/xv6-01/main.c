#include "types.h"
#include "memlayout.h"
#include "arm.h"
#include "defs.h"
#include "proc.h"
#include "mmu.h"
#include "rpi2.h"

extern void* __end;

struct cpu cpus[NCPU];
struct cpu * cpu;

void panic(char* str)
{
    cli(); // disable interrupt
    uart_puts("kernel panic: ");
    if (str != NULL) {
        uart_puts(str);
    }
    while (1);
}

void kmain1(void) {
  kmain();
}

void kmain(void)
{
    uint vectbl;
    uart_puts("enter kmain\r\n");
    unsigned char c;

    cpu = &cpus[0];
    vectbl = P2V_WO(VEC_TBL & PDE_MASK); //  =  0x000F 0000
    //         we mapped      0xFFF0 0000 to    0x0000 0000  1MB aligned
    // so when we want to use 0xFFFF 0000 means 0x000F 0000
    init_vmm();
    kpt_freerange(align_up(&__end, PT_SZ), vectbl);
    kpt_freerange(vectbl + PT_SZ, P2V_WO(INIT_KERNMAP));
    uart_puts("init memory for page table.\r\n");

    // interrupt related
    trap_init();
    uart_puts("trap init done.\r\n");
    pic_init(P2V(VIC_BASE));
    timer_init();
    uart_puts("timer init done.\r\n");

    // map reset of physical memory for allocation
    paging_init(INIT_KERNMAP, PHYSTOP);
    kmem_freerange(P2V_WO(INIT_KERNMAP), P2V_WO(PHYSTOP));
    uart_puts("two level paging done.\r\n");

    // file system init

    // init process
    pinit();
    userinit();
    uart_puts("userinit done.\r\n");
    scheduler();

    // if remove these lines kernel will crash...
    // why~~~
    // uart_puts("> ");
    // while(1) {
    //     c = uart_getc();
    //     if (c == '0') {
    //         sti();
    //     } else {
    //         uart_putc(c);
    //     }
    // }
}
