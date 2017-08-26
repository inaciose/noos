#include "types.h"

void* memset(void *dst, int v, int n)
{
    uint8	*p;
    uint8	c;
    uint32	val;
    uint32	*p4;

    p   = dst;
    c   = v & 0xff;
    val = (c << 24) | (c << 16) | (c << 8) | c;

    // set bytes before whole uint32
    for (; (n > 0) && ((uint)p % 4); n--, p++){
        *p = c;
    }

    // set memory 4 bytes a time
    p4 = (uint*)p;

    for (; n >= 4; n -= 4, p4++) {
        *p4 = val;
    }

    // set leftover one byte a time
    p = (uint8*)p4;

    for (; n > 0; n--, p++) {
        *p = c;
    }

    return dst;
}

uint32 strlen(const char* str) {
    uint32 ret = 0;
    while(str[ret] != 0) {
        ret++;
    }
    return ret;
}

void strncpy(char* buf, char* str, int size) {
    int i;
    for(i=0;i<size;++i) {
        if (str[i] != '\0') {
            buf[i] = str[i];
        }
    }
}

void memmove(char* dest, char* src, int size) {
    int i;
    for(i=0;i<size;++i) {
        dest[i] = src[i];
    }
}
