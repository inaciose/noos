#include "defs.h"
#include "mmu.h"
#include "memlayout.h"
#include "spinlock.h"

extern uint _kernel_pgtbl;
void flush_dcache_all(void); // implemented in cache.S

struct run {
    struct run *next;
};

struct {
    struct spinlock lock;
    struct run *freelist;
} kpt_mem;

struct {
    struct spinlock lock;
    struct run *freelist;
} kmem;

void init_vmm(void) {
    initlock(&kpt_mem.lock, "vm");
    kpt_mem.freelist = NULL;

    initlock(&kmem.lock, "kmem");
    kmem.freelist = NULL;
}

static void _kpt_free(char *v)
{
    struct run *r;

    r = (struct run*) v;
    r->next = kpt_mem.freelist;
    kpt_mem.freelist = r;
}

// add some memory used for page tables (initialization code)
void kpt_freerange(uint32 low, uint32 hi)
{
    while (low < hi) {
        _kpt_free ((char*)low);
        low += PT_SZ;
    }
}

void* kpt_alloc(void)
{
    struct run *r;

    acquire(&kpt_mem.lock);
    if ((r = kpt_mem.freelist) != NULL ) {
        kpt_mem.freelist = r->next;
    }
    release(&kpt_mem.lock);

    memset(r, 0, PT_SZ);
    return r;
}

static void _kmem_free(char *v)
{
    struct run *r;

    r = (struct run*) v;
    r->next = kmem.freelist;
    kmem.freelist = r;
}

// add pages
void kmem_freerange(uint32 low, uint32 hi)
{
    (void)hi;
    // trace mapping
    while (low < hi) {
        _kmem_free ((char*)low);
        low += PTE_SZ;
    }
}

void* alloc_page(void)
{
    struct run *r;

    acquire(&kmem.lock);
    if ((r = kmem.freelist) != NULL ) {
        kmem.freelist = r->next;
    }
    release(&kmem.lock);

    memset(r, 0, PTE_SZ);
    return r;
}

static pte_t* walkpgdir(pde_t *pgdir, const void *va, int alloc) {
    pde_t *pde;
    pte_t *pgtab;
    int i;

    pde = &pgdir[PDE_IDX(va)];

    if ((*pde) & PE_TYPES) {
        pgtab = (pte_t*)P2V(PT_ADDR(*pde));
    } else {
        if (alloc) {
            if ((pgtab = (pte_t*)kpt_alloc()) == 0) {
                return 0;
            }
        } else {
            return 0;
        }
        //memset(pgtab, 0, PT_SZ);
        char *zero = (char*)pgtab;
        for(i=0;i<PT_SZ;++i) {
            zero[i] = 0;
        }
        *pde = V2P(pgtab) | UPDE_TYPE;
    }

    return &pgtab[PTE_IDX(va)];
}

static int mappages(pde_t *pgdir, void* va, uint size, uint pa, uint ap) {
    char *a, *last;
    pte_t *pte;

    a    = (char*)align_dn(va             , PTE_SZ);
    last = (char*)align_dn((uint)va+size-1, PTE_SZ);

    for(;;) {
        if ((pte = walkpgdir(pgdir, a, 1)) == 0) {
            uart_puts("walk error\r\n");
            return -1;
        }

        if (*pte & PE_TYPES) {
            panic("remap");
        }

        *pte = pa | ((ap & 0x3) << 4) | PE_CACHE | PE_BUF | PTE_TYPE;

        if (a == last) {
            break;
        }
        a  += PTE_SZ;
        pa += PTE_SZ;
    }

    return 0;
}

static void flush_tlb(void) {
    uint val = 0;
    // invalidate tlb
    asm volatile("mcr p15, 0, %[r], c8, c7, 0" : :[r]"r" (val):);
    // flush icache all
    asm volatile ("mcr p15, 0, %0, c7, c5,  0" : : "r" (0) : "memory");
    // flush dcache
    flush_dcache_all();
}

void paging_init(uint phy_low, uint phy_hi) {
    // _kernel_pgtbl is preserved in kernel.ld
    // AP_KU means full access
    mappages(P2V(&_kernel_pgtbl), P2V(phy_low), phy_hi-phy_low, phy_low, AP_KU);
    flush_tlb();
}

// Load the initcode into address 0 of pgdir. sz must be less than a page.
void inituvm(pde_t *pgdir, char *init, uint sz)
{
    char *mem;
    uart_puts("inituvm pgdir: ");
    print_hex((uint)pgdir);

    if (sz >= PTE_SZ) {
        panic("inituvm: more than a page");
    }

    mem = alloc_page();
    memset(mem, 0, PTE_SZ);
    mappages(pgdir, 0, PTE_SZ, v2p(mem), AP_KU);
    memmove(mem, init, sz);
}

void switchuvm(struct proc* p) {
    uint val;
    uart_puts("switchuvm to : ");
    uart_puts(p->name);
    uart_puts("\r\np->pgdir: ");
    print_hex((uint)p->pgdir);

    pushcli();

    if (p->pgdir == 0) {
        panic("switchuvm: no pgdir");
    }

    val = (uint) V2P(p->pgdir) | 0x00;

    asm("MCR p15, 0, %[v], c2, c0, 0": :[v]"r" (val):);
    flush_tlb();

    popcli();
}
