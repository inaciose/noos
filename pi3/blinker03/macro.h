// memory mapped i/o access
#define write32(addr, v)      (*((volatile unsigned long  *)(addr)) = (unsigned long)(v))
#define read32(addr)          (*((volatile unsigned long  *)(addr)))
#define writebit32(addr, v)   (*((volatile unsigned long  *)(addr)) |= (unsigned long)(v))