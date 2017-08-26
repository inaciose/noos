#ifndef TYPES_H
#define TYPES_H
typedef int bool;
typedef unsigned int   uint;
typedef unsigned int   uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;

typedef uint32 pte_t;
typedef uint32 pde_t;

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#endif
