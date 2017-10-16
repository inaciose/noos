#ifndef NULL
#define NULL ((void*)0)
#endif

// the heap start is page aligned
#define ALIGN_PAGE  4096
// the allocation is 32 bits aligned
#define ALIGN_BYTES 4
// align macros
#define ALIGN(x,a)              __ALIGN_MASK(x,(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))
// generic void macro
#define UNUSED(x) (void)(x)

unsigned int heap_end = 0;
unsigned int heap_start = 0;

void heap_init(unsigned int start, unsigned int end) {
    heap_start = ALIGN(start, ALIGN_PAGE);
    heap_end = end;
}

void* malloc(unsigned int nbytes) {
    if (heap_start + nbytes >= heap_end)
        return NULL;
    unsigned int temp = heap_start;
    heap_start += ALIGN(nbytes, ALIGN_BYTES);
    return (void*)temp;
}

void free(void *ptr) {
    UNUSED(ptr);
}

