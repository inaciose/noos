.section .text.boot
.section .text
.globl _start
_start:
    mov sp,#0x8000
    bl cmain

hang: b hang
