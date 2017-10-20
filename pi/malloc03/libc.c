// arm lib

/*
 * __aeabi_uidiv.c for 32-bit unsigned integer divide.
 */

extern unsigned int __udivmodsi4(unsigned int num, unsigned int den, unsigned int * rem_p);

/*
 * 32-bit unsigned integer divide.
 */
unsigned int __aeabi_uidiv(unsigned int num, unsigned int den)
{
  return __udivmodsi4(num, den, 0);
}

/*
 * __udivmodsi4.c
 *
 */

//extern void __div0(void);

/*
 * 32-bit. (internal funtion)
 */
unsigned int __udivmodsi4(unsigned int num, unsigned int den, unsigned int * rem_p)
{
  unsigned int quot = 0, qbit = 1;

  if (den == 0)
  {
    return 0;
  }

  /*
   * left-justify denominator and count shift
   */
  while ((signed int) den >= 0)
  {
    den <<= 1;
    qbit <<= 1;
  }

  while (qbit)
  {
    if (den <= num)
    {
      num -= den;
      quot += qbit;
    }
    den >>= 1;
    qbit >>= 1;
  }

  if (rem_p)
    *rem_p = num;

  return quot;
}

// stdlib
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "errno.h"

int errno;

void *memcpy(void *dest, const void *src, size_t n)
{
  char *s = (char *)src;
  char *d = (char *)dest;
  while(n > 0)
  {
    *d++ = *s++;
    n--;
  }
  return dest;
}

void *memset(void *s, int c, int n)
{
  char *dest = (char *)s;
  while(n > 0)
  {
    *dest++ = (char)c;
    n--;
  }
  return s;
}


void abort(void)
{
  //fputs("abort() called\n", stdout);
  //fputs("abort() called\n", stderr);

  while(1);
}
