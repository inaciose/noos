;@ useed in linker.ld to make put this code first
.section ".text.boot" 

.globl _start
_start:
    
    /*
    ;@ return from hypervisor
    mrs r0,cpsr
    bic r0,r0,#0x1F
    orr r0,r0,#0x13
    msr spsr_cxsf,r0
    add r0,pc,#4
    msr ELR_hyp,r0
    eret

    ;@ set stack pointer
    mov sp,#0x8000
    */
    
    bl main
    
hang: b hang
