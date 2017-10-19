#define MEMMAN_FREES      4090
#define MEMMAN_ADDR       0x003c0000

struct FREEINFO {
  unsigned int addr, size;
};

struct MEMMAN {
  unsigned int frees, maxfrees, lostsize, losts;
  struct FREEINFO free[MEMMAN_FREES];
};

