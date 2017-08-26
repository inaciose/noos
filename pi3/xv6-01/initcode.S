# Initial process execs /init.

#include "syscall.h"


.globl start

# exec(init, argv)
start:
    ldr r1, =init
    ldr r2, =argv
    mov r0, #SYS_hello
    swi 0x00

halt:
    b halt

# char init[] = "/init\0";
init:
    .string "/hello init world.\0"

# char *argv[] = { init, 0 };
.p2align 2
argv:
    .word init
    .word 0
