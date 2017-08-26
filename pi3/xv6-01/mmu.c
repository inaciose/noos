#include "defs.h"
#include "mmu.h"

static unsigned int gen_l1_pte(unsigned int paddr) {
    return (paddr & 0xfff00000);
}

static unsigned int gen_l1_pte_addr(unsigned int baddr, unsigned int vaddr) {
    return (baddr & 0xffffc000) | VIRT_TO_PTE_L1_INDEX(vaddr);
}

void gen_mapping(unsigned int paddr,
                 unsigned int vaddr,
                 unsigned int memsize,
                 unsigned int device) {
    unsigned int pte;
    unsigned int pte_addr;
    unsigned int i;

    // map 1GB virt to phy one to one
    for(i=0;i<memsize>>20;++i) {
        pte = gen_l1_pte(paddr+(i<<20));
        if (device == 0) {
            pte |= 0xDE2;
        } else {
            pte |= 0xDE2;
        }
        pte_addr = gen_l1_pte_addr(L1_PTR_BASE_ADDR, vaddr+(i<<20));
        *(volatile unsigned int *)pte_addr = pte;
    }
}

void mmu_init() {
    uart_puts("mmu_init()...");
    //unsigned int ttb = L1_PTR_BASE_ADDR;
    gen_mapping(PHYSICAL_MEM_ADDR, VIRTUAL_MEM_ADDR, MEM_MAP_SIZE, 0);
    gen_mapping(PHYSICAL_IO_ADDR, VIRTUAL_IO_ADDR, IO_MAP_SIZE, 1);
    asm volatile(
        // disable data cache
        "mrc p15, #0, r0, c1, c0, #0;"
        "bic r0, r0, #0x00000004;"
        "mcr p15, #0, r0, c1, c0, #0;"
        // disable instruction cache
        "mrc p15, #0, r0, c1, c0, #0;"
        "bic r0, r0, #0x00001000;"
        "mcr p15, #0, r0, c1, c0, #0;"
        // disable MMU
        "mrc p15, #0, r0, c1, c0, #0;"
        "bic r0, r0, #0x1;"
        "mcr p15, #0, r0, c1, c0, #0;"
        // invalidate tlb
        "mcr p15, #0, r0, c8, c7, #0;"
        // setup domain ttbr0
        "mov r0, #0xffffffff;"
        "mcr p15, 0, r0, c3, c0, 0;"
        "ldr r0, =0x30700000;"
        "mcr p15, 0, r0, c2, c0, 0;"

        // enable mmu
        "mrc p15, #0, r0, c1, c0, #0;"
        "orr r0, r0, #0x1;"
        "mcr p15, #0, r0, c1, c0, #0;"
        // enable instruction cache
        "mrc p15, #0, r0, c1, c0, #0;"
        "orr r0, r0, #0x00001000;"
        "mcr p15, #0, r0, c1, c0, #0;"
        // enable data cache
        "mrc p15, #0, r0, c1, c0, #0;"
        "orr r0, r0, #0x00000004;"
        "mcr p15, #0, r0, c1, c0, #0;"
    );
    uart_puts("done\r\n");
}
