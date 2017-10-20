.section .text.boot
.section .text
.globl _entry
_entry:
    # clear the bss section and stack
    LDR     r1, =__bss_start
    LDR     r2, =_end
    MOV     r3, #0x00

1:
    CMP     r1, r2
    STMLTIA r1!, {r3}
    BLT     1b
    LDR sp, =stktop

    BL cmain
    
hang: B hang
