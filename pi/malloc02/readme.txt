malloc02

This is the malloc implementation I found on RPiHaribote/haribote.

Functions:

void memman_init(struct MEMMAN *man);
void *malloc (unsigned nSize);
void free (void *pBlock);
void memman_monitor(struct MEMMAN *man);
unsigned int memman_total(struct MEMMAN *man);
unsigned int memman_alloc(struct MEMMAN *man, unsigned int size);
unsigned int memman_alloc_4k(struct MEMMAN *man, unsigned int size);
int memman_free(struct MEMMAN *man, unsigned int addr, unsigned int size);
int memman_free_4k(struct MEMMAN *man, unsigned int addr, unsigned int size);

The following functions are not used in the sample:

memman_alloc_4k
memman_free_4k
