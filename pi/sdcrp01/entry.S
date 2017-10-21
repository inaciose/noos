.section .text.boot
.section .text
.globl _start
_start:
    mov sp,#0x8000
    bl cmain
hang: b hang

.globl memory_barrier
memory_barrier:
  mov  r0, #0
  mcr  p15, #0, r0, c7, c10, #5
  mov  pc, lr
