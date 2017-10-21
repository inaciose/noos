#include <stdlib.h>

// printf.c
int printf(const char *format, ...);

// stdlib.c
void *memset(void *, int, size_t);

#ifndef NULL
#define NULL ((void*)0)
#endif

#define ARR_LEN(a) (sizeof(a) / sizeof(a[0]))
#define UNUSED(x) (void)(x)

#define ALIGN(x,a)              __ALIGN_MASK(x,(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)    (((x)+(mask))&~(mask))

unsigned int end_of_heap = 0;
unsigned int start_of_heap = 0;

void init_heap(unsigned int start, unsigned int end) {
    start_of_heap = start;
    end_of_heap = end;
    memset((void *)start_of_heap, 0, end_of_heap - start_of_heap);
}

void* malloc(unsigned int nbytes) {
    if (start_of_heap + nbytes >= end_of_heap)
        return NULL;
    unsigned int temp = start_of_heap;
    start_of_heap += ALIGN(nbytes,4);
    return (void*)temp;
}

void free(void *ptr) {
    // just void the pontier
    UNUSED(ptr);
}

void print_heap_range(void) {
  printf("heap start: %x, end %x\n", start_of_heap, end_of_heap);
}
