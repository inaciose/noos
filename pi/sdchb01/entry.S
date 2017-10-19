.section .text.boot
.section .text
.globl _entry
_entry:
    mov sp,#0x8000
    bl cmain
hang: b hang

.globl get_cpsr
get_cpsr:  
    mrs    r0, cpsr
    bx     lr

.globl set_cpsr
set_cpsr:
    msr   cpsr_c, r0
    bx    lr
    