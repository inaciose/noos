// To keep this in the first portion of the binary.
.section ".text.boot"
 
// Make _start global.
.global _start
_start:
    ldr pc, _reset_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_handler_h
    ldr pc, _interrupt_vector_h
    ldr pc, _fast_interrupt_vector_h

_reset_h:                           .word   _reset_
_undefined_instruction_vector_h:    .word   undefined_instruction_vector
_software_interrupt_vector_h:       .word   software_interrupt_vector
_prefetch_abort_vector_h:           .word   prefetch_abort_vector
_data_abort_vector_h:               .word   data_abort_vector
_unused_handler_h:                  .word   _reset_
_interrupt_vector_h:                .word   interrupt_vector
_fast_interrupt_vector_h:           .word   fast_interrupt_vector

_reset_:
    // We enter execution in supervisor mode. For more information on
    // processor modes see ARM Section A2.2 (Processor Modes)
    
    ;@ neeed to move vectors to base address
    ldr     r0, =_start
    mrc p15, 0, r1, c12, c0, 0 ;@ get vbar

    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}

    /* define stack pointer base */
    mov r4, #0x80000000

    /* SVC stack (for SWIs) at 0x2000 */
    cps #0x13    /* Change to supervisor (SVC) mode */
    add sp, r4, #0x2400

    /* ABORT stack at 0x2400 */
    cps #0x17    /* Change to Abort mode */
    add sp, r4, #0x2800

    /* IRQ stack at 0x2800 */
    cps #0x12    /* Change to IRQ mode */
    add sp, r4, #0x2c00

    /* System stack at 0x2c00 */
    cps #0x1f    /* Change to system mode */
    add sp, r4, #0x3c00

    /* Change to supervisor mode before go to main */
    /* some functions like mrc p15, 0, r0, c0, c0, 0 */
    /* just hangs if called with main without in svc mode */
    cps #0x13

    bl      main

    // If main does return for some reason, just catch it and stay here.
_inf_loop:
    b       _inf_loop

.globl enable_irq
enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

.globl disable_irq
disable_irq:
    mrs     r0, cpsr
    orr r0, r0, #0x80 /*( ARMV7_IRQ_MASK | ARMV7_FIQ_MASK )*/
    msr     cpsr_c, r0
    bx lr
    
.globl call_swi
call_swi:
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  swi 0
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  bx lr
    